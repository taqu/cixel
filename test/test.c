#include <stdio.h>

#include "utest.h"

#include "stb_image.h"
#include "stb_image_write.h"

#include "../cixel.h"

#ifdef _WIN32
#    define SPRINTF(BUFF, FORMAT, VAR0, VAR1) sprintf_s((BUFF), sizeof(BUFF), (FORMAT), (VAR0), (VAR1))
#else
#    define SPRINTF(BUFF, FORMAT, VAR0, VAR1) sprintf((BUFF), (FORMAT), (VAR0), (VAR1))
#endif
cixel_u32* convert(int width, int height, int channels, unsigned char* src)
{
    int size = width * height;
    cixel_u32* dst = CIXEL_REINTERPRET_CAST(cixel_u32*)(malloc(size * sizeof(cixel_u32)));
    for(int i = 0; i < size; ++i) {
        unsigned char* pixel = src + channels * i;
        unsigned int r = pixel[0];
        unsigned int g = pixel[1];
        unsigned int b = pixel[2];
        dst[i] = (0xFF000000U) | (r << 0) | (g << 8) | (b << 16);
    }
    return dst;
}

void quantizeYUV655(cixel_u32* pixels, cixel_s32 size, const cixel_u8* indices, const Cixel* cixel)
{
    for(int i = 0; i < size; ++i) {
        Color color = cixelGetPalletColor(cixel, indices[i]);
        color.color_ = cixelYUV2RGB(color.color_);

        cixel_u8 y = CIXEL_STATIC_CAST(cixel_u8)(color.rgba_.r_);
        cixel_u8 u = CIXEL_STATIC_CAST(cixel_u8)(color.rgba_.g_);
        cixel_u8 v = CIXEL_STATIC_CAST(cixel_u8)(color.rgba_.b_);
        Color c;
        c.rgba_.a_ = 0xFFU;
        c.rgba_.r_ = y;
        c.rgba_.g_ = u;
        c.rgba_.b_ = v;
        pixels[i] = c.color_;
    }
}

bool test(const char* src, const char* dst0, const char* dst1, const char* directory)
{
    char buffer[128];
    SPRINTF(buffer, "%s%s", directory, src);

    int width, height, channels;
    unsigned char* data = stbi_load(buffer, &width, &height, &channels, 0);
    if(NULL == data) {
        return false;
    }

    Cixel* cixel = cixelCreate(width, height, CIXEL_NULL, CIXEL_NULL);

    cixel_u32* pixels = convert(width, height, channels, data);
    stbi_image_free(data);

    cixel_s32 size = width * height;
    cixel_u8* indices = CIXEL_REINTERPRET_CAST(cixel_u8*)(malloc(size * sizeof(cixel_u8)));
    cixel_u32* pixels0 = CIXEL_REINTERPRET_CAST(cixel_u32*)(malloc(size * sizeof(cixel_u32)));

    cixelQuantize(cixel, indices, pixels, false);
    printf("quantize\n");

    quantizeYUV655(pixels0, size, indices, cixel);

    SPRINTF(buffer, "%s%s", directory, dst0);
    stbi_write_jpg(buffer, width, height, 4, CIXEL_REINTERPRET_CAST(unsigned char*)(pixels0), 100);

    {
        SPRINTF(buffer, "%s%s", directory, dst1);
        FILE* file = fopen(buffer, "wb");
        if(NULL != file) {
            cixelPrint(cixel, file, indices);
            printf("write sixels\n");
            fclose(file);
        }
    }

    free(pixels0);
    free(indices);
    free(pixels);
    cixelDestroy(cixel);
    return true;
}

UTEST(Quantize_Encode, grad)
{
    EXPECT_TRUE(test("grad.png", "grad_out.jpg", "grad.txt", "../data/"));
}

#if 0
UTEST(Quantize_Encode, snake)
{
    EXPECT_TRUE(test("snake.png", "snake_out.jpg", "snake.txt", "../data/"));
}

UTEST(Quantize_Encode, baboon)
{
    EXPECT_TRUE(test("baboon.png", "baboon_out.jpg", "baboon.txt", "../data/"));
}

UTEST(Quantize_Encode, girl)
{
    EXPECT_TRUE(test("girl.jpg", "girl_out.jpg", "girl.txt", "../data/"));
}

UTEST(Quantize_Encode, lena)
{
    EXPECT_TRUE(test("lena.png", "lena_out.jpg", "lena.txt", "../data/"));
}

UTEST(Quantize_Encode, person)
{
    EXPECT_TRUE(test("person.jpg", "person_out.jpg", "person.txt", "../data/"));
}
#endif

