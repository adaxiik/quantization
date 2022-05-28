#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Pixel;

typedef struct
{
    Pixel *pixels;
    uint32_t width;
    uint32_t height;
} Image;

void WritePNG(const char *filename, Image *image);

Image *ReadPNG(const char *filename);

Image* Scale(Image* image, float scale);


void DestroyImage(Image **image);




inline uint8_t Clamp(double x)
{
    return x > 255 ? 255 : (x < 0 ? 0 : x);
}

inline Pixel PixelAdd(Pixel a, Pixel b)
{
    return (Pixel){Clamp(a.red + b.red), Clamp(a.green + b.green), Clamp(a.blue + b.blue)};
}

inline Pixel PixelSub(Pixel a, Pixel b)
{
    return (Pixel){Clamp(a.red - b.red), Clamp(a.green - b.green), Clamp(a.blue - b.blue)};
}

inline Pixel PixelMulByNum(Pixel a, double b)
{
    return (Pixel){Clamp(a.red * b), Clamp(a.green * b), Clamp(a.blue * b)};
}

inline bool InImage(uint32_t width, uint32_t height, uint32_t x, uint32_t y)
{
    return x < width && y < height && x >= 0 && y >= 0;
}