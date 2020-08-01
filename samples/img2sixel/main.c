#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define CIXEL_IMPLEMENTATION
#include "cixel.h"

cixel_u32* convert(int width, int height, int channels, unsigned char* src)
{
    int size = width * height;
    cixel_u32* dst = CIXEL_REINTERPRET_CAST(cixel_u32*)(malloc(size * sizeof(cixel_u32)));
    for(int i = 0; i < size; ++i){
        unsigned char* pixel = src + channels * i;
        unsigned int r = pixel[0];
        unsigned int g = pixel[1];
        unsigned int b = pixel[2];
        dst[i] = (0xFF000000U) | (r << 0) | (g << 8) | (b << 16);
    }
    return dst;
}

void quantize(const Cixel* cixel, cixel_u32* pixels, cixel_s32 size, const cixel_u8* indices)
{
    for(int i = 0; i < size; ++i){
        Color c = cixelGetPalletColor(cixel, indices[i]);
        pixels[i] = c.color_;
    }
}

int main(int argc, char** argv)
{
    if(argc<2){
        fprintf(stderr, "Usage: img2sixel <path-to-image>\n");
        return 0;
    }

    int width, height, channels;
    unsigned char* data = stbi_load(argv[1], &width, &height, &channels, 0);
    if(NULL == data){
        fprintf(stderr, "Error: failed to open an image\n");
        return 0;
    }
    fprintf(stderr, "Loaded\n");
    cixel_u32* pixels = convert(width, height, channels, data);
    stbi_image_free(data);

    Cixel* cixel = cixelCreate(width, height, CIXEL_NULL, CIXEL_NULL);
    cixel_s32 size = width * height;
    cixel_u8* indices = CIXEL_REINTERPRET_CAST(cixel_u8*)(malloc(size * sizeof(cixel_u8)));

    cixelQuantize(cixel, indices, pixels, false);
    cixelPrint(cixel, stdout, indices);

    free(indices);
    free(pixels);
    cixelDestroy(cixel);
    return 0;
}

