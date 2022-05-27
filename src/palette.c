#include "palette.h"
#include <string.h>

void DestroyPalette(Palette **palette)
{
    free((*palette)->palette);
    free(*palette);
    *palette = NULL;
}

Palette *CreatePaletteFromImageRandom(Image *image, uint32_t size)
{
    Palette *palette = (Palette *)malloc(sizeof(Palette));
    palette->palette = (Pixel *)malloc(sizeof(Pixel) * size);
    palette->size = size;
    for (int i = 0; i < size; i++)
    {
        size_t index = rand() % (image->height * image->width);
        palette->palette[i] = image->pixels[index];
    }

    return palette;
}

Palette *CreatePalette(Pixel* pixels, uint32_t size)
{
    Palette *palette = (Palette *)malloc(sizeof(Palette));
    palette->palette = (Pixel *)malloc(sizeof(Pixel) * size);
    palette->size = size;
    memcpy(palette->palette, pixels, sizeof(Pixel) * size);
    return palette;
}