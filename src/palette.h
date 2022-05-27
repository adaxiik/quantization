#pragma once
#include "png_wrapper.h"
#include <math.h>
typedef struct
{
    Pixel *palette;
    uint32_t size;
} Palette;

inline double Distance(Pixel p1, Pixel p2)
{
    return sqrt(pow(p1.red - p2.red, 2) + pow(p1.green - p2.green, 2) + pow(p1.blue - p2.blue, 2));
}

Palette* CreatePaletteFromImageRandom(Image* image, uint32_t size);
Palette *CreatePalette(Pixel* pixels, uint32_t size);
void DestroyPalette(Palette **palette);