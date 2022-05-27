// const palette = [
//     [103, 51, 11],
//     [22, 22, 19],
//     [53, 57, 45],
//     [0, 103, 2],
//     [100, 89, 16]
// ]
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "png_wrapper.h"
#include "palette.h"

Pixel FindColorInPalette(Pixel p, Palette *palette)
{
    double min_distance = Distance(p, palette->palette[0]);
    size_t min_index = 0;
    for (size_t i = 1; i < palette->size; i++)
    {
        double d = Distance(p, palette->palette[i]);
        if (d < min_distance)
        {
            min_distance = d;
            min_index = i;
        }
    }
    return palette->palette[min_index];
}

void Quantize(Image *image, Palette *palette)
{
    #pragma omp parallel for
    for (size_t i = 0; i < image->height * image->width; i++)
    {
        image->pixels[i] = FindColorInPalette(image->pixels[i], palette);
    }
}

void Dither(Image *image, Palette* palette)
{
    for (size_t y = 0; y < image->height; y++)
    {
        for (size_t x = 0; x < image->width; x++)
        {
            Pixel old = image->pixels[y * image->width + x];
            Pixel new = FindColorInPalette(old, palette);
            image->pixels[y * image->width + x] = new;
            Pixel error = PixelSub(old, new);
            
            if(InImage(image->width, image->height, x + 1, y))
            {
                image->pixels[y * image->width + x + 1] = PixelAdd(image->pixels[y * image->width + x + 1], PixelMulByNum(error, 7.0 / 16.0));
            }
            if(InImage(image->width, image->height, x - 1, y + 1))
            {
                image->pixels[(y + 1) * image->width + x - 1] = PixelAdd(image->pixels[(y + 1) * image->width + x - 1], PixelMulByNum(error, 3.0 / 16.0));
            }
            if(InImage(image->width, image->height, x, y + 1))
            {
                image->pixels[(y + 1) * image->width + x] = PixelAdd(image->pixels[(y + 1) * image->width + x], PixelMulByNum(error, 5.0 / 16.0));
            }
            if(InImage(image->width, image->height, x + 1, y + 1))
            {
                image->pixels[(y + 1) * image->width + x + 1] = PixelAdd(image->pixels[(y + 1) * image->width + x + 1], PixelMulByNum(error, 1.0 / 16.0));
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    srand(time(NULL));
    Image *image = ReadPNG("car.png");

    Pixel colors[] = {
        {22, 23, 23},
        {5, 105, 3},
        {108, 106, 106},
        {122, 15, 14},
        {188, 190, 57}
    };
        

    Palette *palette = CreatePalette(colors, sizeof(colors) / sizeof(Pixel));
    //Quantize(image, palette);
    Dither(image, palette);
    WritePNG("out.png", image);
    DestroyPalette(&palette);
    DestroyImage(&image);
    return 0;
}
