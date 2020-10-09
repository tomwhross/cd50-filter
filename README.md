# cs50-filter
[A CS50 Project](https://cs50.harvard.edu/x/2020/psets/4/filter/less/) - Image manipulation and filtering


## Getting started

Clone the repository, or download it as a zip file and unzip it

```
$ git clone git@github.com:tomwhross/cd50-filter.git
```

## Running the program

The program takes three arguments

1. The filter switch

```
-g: apply greyscale (black and white) filter
-s: apply sepia filter
-r: relect the image horizontally
-b: blur the image with a box-blur filter
```

2. The input image file
3. The output image file

Example applying grey scale:

```
$ ./filter -g images/yard.bmp out.bmp
```


## Compiling

Run the following to compile a new version:

```
$ make -B filter
```

Note the `-B` flag (for `always-make`) may or may not be required depending on your setup
