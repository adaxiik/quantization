#include "png_wrapper.h"
#include <png.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

void WritePNG(const char *filename, Image *image)
{
    // http://zarb.org/~gc/html/libpng.html
    FILE *fp = fopen(filename, "wb");
    if (!fp)
    {
        fprintf(stderr, "Could not open file %s for writing: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png)
    {
        fprintf(stderr, "Could not allocate write struct\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    png_infop info = png_create_info_struct(png);
    if (!info)
    {
        fprintf(stderr, "Could not allocate info struct\n");
        fclose(fp);
        png_destroy_write_struct(&png, NULL);
        exit(EXIT_FAILURE);
    }

    if (setjmp(png_jmpbuf(png)))
    {
        fprintf(stderr, "Error during png creation\n");
        fclose(fp);
        png_destroy_write_struct(&png, &info);
        exit(EXIT_FAILURE);
    }

    png_init_io(png, fp);
    png_set_IHDR(png, info, image->width, image->height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);
    png_byte *row = (png_byte *)malloc(3 * image->width * sizeof(png_byte));
    if (!row)
    {
        fprintf(stderr, "Could not allocate row\n");
        fclose(fp);
        png_destroy_write_struct(&png, &info);
        return;
    }
    for (int y = 0; y < image->height; y++)
    {
        for (int x = 0; x < image->width; x++)
        {
            row[3 * x + 0] = image->pixels[y * image->width + x].red;
            row[3 * x + 1] = image->pixels[y * image->width + x].green;
            row[3 * x + 2] = image->pixels[y * image->width + x].blue;
        }
        png_write_row(png, row);
    }
    free(row);
    png_write_end(png, NULL);
    png_destroy_write_struct(&png, &info);
    fclose(fp);
    printf("Image %s successfully created\n", filename);
}

Image *ReadPNG(const char *filename)
{
    Image *image = (Image *)malloc(sizeof(Image));
    FILE *fp = fopen(filename, "rb");
    if (!fp)
    {
        fprintf(stderr, "Could not open file %s for reading: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png)
    {
        fprintf(stderr, "Could not allocate read struct\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    png_infop info = png_create_info_struct(png);
    if (!info)
    {
        fprintf(stderr, "Could not allocate info struct\n");
        fclose(fp);
        png_destroy_read_struct(&png, NULL, NULL);
        exit(EXIT_FAILURE);
    }

    if (setjmp(png_jmpbuf(png)))
    {
        fprintf(stderr, "Error during png creation\n");
        fclose(fp);
        png_destroy_read_struct(&png, &info, NULL);
        exit(EXIT_FAILURE);
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    int bit_depth, color_type, interlace_type;
    png_get_IHDR(png, info, &image->width, &image->height, &bit_depth, &color_type, &interlace_type, NULL, NULL);
    int step;
    if (color_type == PNG_COLOR_TYPE_RGB)
    {
        step = 3;
    }
    else if (color_type == PNG_COLOR_TYPE_RGBA)
    {
        step = 4;
    }
    else
    {
        fprintf(stderr, "Unsupported color type\n");
        fclose(fp);
        png_destroy_read_struct(&png, &info, NULL);
        exit(EXIT_FAILURE);
    }

    image->pixels = malloc((image->width) * (image->height) * sizeof(Pixel));
    Pixel *pixels = image->pixels;
    png_read_update_info(png, info);
    png_byte *row = (png_byte *)malloc(step * (image->width) * sizeof(png_byte));
    if (!row)
    {
        fprintf(stderr, "Could not allocate row\n");
        fclose(fp);
        png_destroy_read_struct(&png, &info, NULL);
        exit(EXIT_FAILURE);
    }
    for (int y = 0; y < image->height; y++)
    {
        png_read_row(png, row, NULL);
        for (int x = 0; x < image->width; x++)
        {
            pixels[y * (image->width) + x].red = row[step * x + 0];
            pixels[y * (image->width) + x].green = row[step * x + 1];
            pixels[y * (image->width) + x].blue = row[step * x + 2];
        }
    }
    free(row);
    png_read_end(png, NULL);
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);
    return image;
}

void DestroyImage(Image **image)
{
    free((*image)->pixels);
    free(*image);
    *image = NULL;
}

