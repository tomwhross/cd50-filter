#include <math.h>

#include <stdio.h>

#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    /*
     * For the height and width of the image file
     * Get the BGR (BMP reversed-RGB) value
     * Take the average of the three values
     * Set the RGB values to the average, same number
     */

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int blue = image[i][j].rgbtBlue;
            int green = image[i][j].rgbtGreen;
            int red = image[i][j].rgbtRed;

            // first convert to float so the average isn't immediately truncated back to an int
            // then round to the nearest whole number (up or down)
            int average = round((float)(blue + green + red) / 3);

            // set the new values
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    /*
     * For the height and width of the image file
     * Get the BGR (BMP reversed-RGB) value
     * Apply sepia generator to each and reset them
     */

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int original_blue = image[i][j].rgbtBlue;
            int original_green = image[i][j].rgbtGreen;
            int original_red = image[i][j].rgbtRed;

            // apply sephia calculations
            int sepia_red = round(0.393 * original_red + 0.769 * original_green + 0.189 * original_blue);
            int sepia_green = round(0.349 * original_red + 0.686 * original_green + 0.168 * original_blue);
            int sepia_blue = round(0.272 * original_red + 0.534 * original_green + 0.131 * original_blue);

            // catch any values greater than 255
            if (sepia_red > 255)
            {
                sepia_red = 255;
            }
            if (sepia_green > 255)
            {
                sepia_green = 255;
            }
            if (sepia_blue > 255)
            {
                sepia_blue = 255;
            }

            // set the values in the image
            image[i][j].rgbtBlue = sepia_blue;
            image[i][j].rgbtGreen = sepia_green;
            image[i][j].rgbtRed = sepia_red;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{

    // create an array to store the original image
    int line[height][width][3];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int original_blue = image[i][j].rgbtBlue;
            int original_green = image[i][j].rgbtGreen;
            int original_red = image[i][j].rgbtRed;

            line[i][j][0] = original_blue;
            line[i][j][1] = original_green;
            line[i][j][2] = original_red;
        }
    }

    for (int x = 0; x < height; x++)
    {
        // for the width of the image
        // traverse it backwards
        for (int y = width; y > 0; y = y - 1)
        {
            // because y is the width, when using the y value to refer
            // to a position in array, must subtract 1
            image[x][y - 1].rgbtBlue = line[x][width - y][0];
            image[x][y - 1].rgbtGreen = line[x][width - y][1];
            image[x][y - 1].rgbtRed = line[x][width - y][2];
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // create an array to store the original image
    int line[height][width][3];

    // read and store the original image into the array copy
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int original_blue = image[i][j].rgbtBlue;
            int original_green = image[i][j].rgbtGreen;
            int original_red = image[i][j].rgbtRed;

            line[i][j][0] = original_blue;
            line[i][j][1] = original_green;
            line[i][j][2] = original_red;
        }
    }

    /*
        There are 3 scenarios split into 9 conditions here:
            1. Top-left pixel (corner)
            2. Top-right pixel (corner)
            3. Bottom-left pixel (corner)
            4. Bottom-right pixel (corner)
            5. Top edge (edge)
            6. Bottom edge (edge)
            7. Left edge (edge)
            8. Right edge (edge)
            9. Everthing else (middle)
    */
    for (int x = 0; x < height; x++)
    {
        for (int y = 0; y < width; y++)
        {
            // handle middle pixels
            if ((x > 0 && x < height - 1) && (y > 0 && y < width - 1))
            {
                int average_blue = round((float)(line[x + 1][y - 1][0] + line[x + 1][y - 0][0] + line[x + 1][y + 1][0] + line[x][y - 1][0] +
                                                 line[x][y - 0][0] + line[x][y + 1][0] + line[x - 1][y - 1][0] + line[x - 1][y - 0][0] + line[x - 1][y + 1][0]) / 9);

                int average_green = round((float)(line[x + 1][y - 1][1] + line[x + 1][y - 0][1] + line[x + 1][y + 1][1] + line[x][y - 1][1] +
                                                  line[x][y - 0][1] + line[x][y + 1][1] + line[x - 1][y - 1][1] + line[x - 1][y - 0][1] + line[x - 1][y + 1][1]) / 9);

                int average_red = round((float)(line[x + 1][y - 1][2] + line[x + 1][y - 0][2] + line[x + 1][y + 1][2] + line[x][y - 1][2] +
                                                line[x][y - 0][2] + line[x][y + 1][2] + line[x - 1][y - 1][2] + line[x - 1][y - 0][2] + line[x - 1][y + 1][2]) / 9);

                image[x][y].rgbtBlue = average_blue;
                image[x][y].rgbtGreen = average_green;
                image[x][y].rgbtRed = average_red;
            }
            // handle top edge
            else if (x == 0 && y > 0 && y < width - 1)
            {
                int average_blue = round((float)(
                                             line[x + 1][y - 1][0] + line[x + 1][y - 0][0] + line[x + 1][y + 1][0] +
                                             line[x][y - 1][0] + line[x][y - 0][0] + line[x][y + 1][0]) / 6
                                        );

                int average_green = round((float)(
                                              line[x + 1][y - 1][1] + line[x + 1][y - 0][1] + line[x + 1][y + 1][1] +
                                              line[x][y - 1][1] + line[x][y - 0][1] + line[x][y + 1][1]) / 6
                                         );

                int average_red = round((float)(
                                            line[x + 1][y - 1][2] + line[x + 1][y - 0][2] + line[x + 1][y + 1][2] +
                                            line[x][y - 1][2] + line[x][y - 0][2] + line[x][y + 1][2]) / 6
                                       );

                image[x][y].rgbtBlue = average_blue;
                image[x][y].rgbtGreen = average_green;
                image[x][y].rgbtRed = average_red;
            }
            // handle bottom edge
            else if (x == height - 1 && y > 0 && y < width - 1)
            {
                int average_blue = round((float)(
                                             line[x][y - 1][0] + line[x][y - 0][0] + line[x][y + 1][0] +
                                             line[x - 1][y - 1][0] + line[x - 1][y - 0][0] + line[x - 1][y + 1][0]) / 6);

                int average_green = round((float)(
                                              line[x][y - 1][1] + line[x][y - 0][1] + line[x][y + 1][1] +
                                              line[x - 1][y - 1][1] + line[x - 1][y - 0][1] + line[x - 1][y + 1][1]) / 6);

                int average_red = round((float)(
                                            line[x][y - 1][2] + line[x][y - 0][2] + line[x][y + 1][2] +
                                            line[x - 1][y - 1][2] + line[x - 1][y - 0][2] + line[x - 1][y + 1][2]) / 6);

                image[x][y].rgbtBlue = average_blue;
                image[x][y].rgbtGreen = average_green;
                image[x][y].rgbtRed = average_red;
            }
            // handle left edge
            else if (y == 0 && x > 0 && x < height - 1)
            {
                int average_blue = round((float)(
                                             line[x - 1][y - 0][0] + line[x - 1][y + 1][0] + line[x][y][0] +
                                             line[x + 0][y + 1][0] + line[x + 1][y + 0][0] + line[x + 1][y + 1][0]) / 6);

                int average_green = round((float)(
                                              line[x - 1][y - 0][1] + line[x - 1][y + 1][1] + line[x][y][1] +
                                              line[x + 0][y + 1][1] + line[x + 1][y + 0][1] + line[x + 1][y + 1][1]) / 6);

                int average_red = round((float)(
                                            line[x - 1][y - 0][2] + line[x - 1][y + 1][2] + line[x][y][2] +
                                            line[x + 0][y + 1][2] + line[x + 1][y + 0][2] + line[x + 1][y + 1][2]) / 6);

                image[x][y].rgbtBlue = average_blue;
                image[x][y].rgbtGreen = average_green;
                image[x][y].rgbtRed = average_red;
            }
            // handle right edge
            else if (y == width - 1 && x > 0 && x < height - 1)
            {
                int average_blue = round((float)(
                                             line[x + 1][y - 0][0] + line[x + 1][y - 1][0] + line[x][y][0] +
                                             line[x + 0][y - 1][0] + line[x - 1][y + 0][0] + line[x - 1][y - 1][0]) / 6);

                int average_green = round((float)(
                                              line[x + 1][y - 0][1] + line[x + 1][y - 1][1] + line[x][y][1] +
                                              line[x + 0][y - 1][1] + line[x - 1][y + 0][1] + line[x - 1][y - 1][1]) / 6);

                int average_red = round((float)(
                                            line[x + 1][y - 0][2] + line[x + 1][y - 1][2] + line[x][y][2] +
                                            line[x + 0][y - 1][2] + line[x - 1][y + 0][2] + line[x - 1][y - 1][2]) / 6);

                image[x][y].rgbtBlue = average_blue;
                image[x][y].rgbtGreen = average_green;
                image[x][y].rgbtRed = average_red;
            }
            // handle top left corner
            else if (x == 0 && y == 0)
            {
                int average_blue = round((float)(
                                             line[x][y][0] + line[x][y + 1][0] +
                                             line[x + 1][y][0] + line[x + 1][y + 1][0]) / 4);

                int average_green = round((float)(
                                              line[x][y][1] + line[x][y + 1][1] +
                                              line[x + 1][y][1] + line[x + 1][y + 1][1]) / 4);

                int average_red = round((float)(
                                            line[x][y][2] + line[x][y + 1][2] +
                                            line[x + 1][y][2] + line[x + 1][y + 1][2]) / 4);

                image[x][y].rgbtBlue = average_blue;
                image[x][y].rgbtGreen = average_green;
                image[x][y].rgbtRed = average_red;
            }
            // handle top right corner
            else if (x == 0 && y == width - 1)
            {
                int average_blue = round((float)(
                                             line[x][y][0] + line[x][y - 1][0] +
                                             line[x + 1][y][0] + line[x + 1][y - 1][0]) / 4);

                int average_green = round((float)(
                                              line[x][y][1] + line[x][y - 1][1] +
                                              line[x + 1][y][1] + line[x + 1][y - 1][1]) / 4);

                int average_red = round((float)(
                                            line[x][y][2] + line[x][y - 1][2] +
                                            line[x + 1][y][2] + line[x + 1][y - 1][2]) / 4);

                image[x][y].rgbtBlue = average_blue;
                image[x][y].rgbtGreen = average_green;
                image[x][y].rgbtRed = average_red;
            }
            // handle bottom left corner
            else if (x == height - 1 && y == 0)
            {
                int average_blue = round((float)(
                                             line[x - 1][y][0] + line[x - 1][y + 1][0] +
                                             line[x][y][0] + line[x][y + 1][0]) / 4);

                int average_green = round((float)(
                                              line[x - 1][y][1] + line[x - 1][y + 1][1] +
                                              line[x][y][1] + line[x][y + 1][1]) / 4);

                int average_red = round((float)(
                                            line[x - 1][y][2] + line[x - 1][y + 1][2] +
                                            line[x][y][2] + line[x][y + 1][2]) / 4);

                image[x][y].rgbtBlue = average_blue;
                image[x][y].rgbtGreen = average_green;
                image[x][y].rgbtRed = average_red;
            }
            // handle bottom right corner
            else if (x == height - 1 && y == width - 1)
            {
                int average_blue = round((float)(
                                             line[x - 1][y - 1][0] + line[x - 1][y][0] +
                                             line[x][y][0] + line[x][y - 1][0]) / 4);

                int average_green = round((float)(
                                              line[x - 1][y - 1][1] + line[x - 1][y][1] +
                                              line[x][y][1] + line[x][y - 1][1]) / 4);

                int average_red = round((float)(
                                            line[x - 1][y - 1][2] + line[x - 1][y][2] +
                                            line[x][y][2] + line[x][y - 1][2]) / 4);

                image[x][y].rgbtBlue = average_blue;
                image[x][y].rgbtGreen = average_green;
                image[x][y].rgbtRed = average_red;
            }

        }
    }

    return;
}
