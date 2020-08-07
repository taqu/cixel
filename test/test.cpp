#include <chrono>
#include <stdio.h>

#include "utest.h"

#include "stb_image.h"
#include "stb_image_write.h"

#include "../cixel.h"

namespace
{
#ifdef _WIN32
#    define SPRINTF(BUFF, FORMAT, VAR0, VAR1) sprintf_s((BUFF), (FORMAT), (VAR0), (VAR1))
#else
#    define SPRINTF(BUFF, FORMAT, VAR0, VAR1) sprintf((BUFF), (FORMAT), (VAR0), (VAR1))
#endif
    cixel::cixel_u32* convert(int width, int height, int channels, unsigned char* src)
    {
        int size = width * height;
        cixel::cixel_u32* dst = reinterpret_cast<cixel::cixel_u32*>(malloc(size * sizeof(cixel::cixel_u32)));
        for(int i = 0; i < size; ++i) {
            unsigned char* pixel = src + channels * i;
            unsigned int r = pixel[0];
            unsigned int g = pixel[1];
            unsigned int b = pixel[2];
            dst[i] = (0xFF000000U) | (r << 0) | (g << 8) | (b << 16);
        }
        return dst;
    }

    void quantizeYUV655(cixel::cixel_u32* pixels, cixel::cixel_s32 size, const cixel::cixel_u8* indices, const cixel::Cixel* cixel)
    {
        for(int i = 0; i < size; ++i) {
            cixel::Color color = cixel::cixelGetPalletColor(cixel, indices[i]);
            color.color_ = cixel::cixelYUV2RGB(color.color_);

            cixel::cixel_s8 y = static_cast<cixel::cixel_s8>(color.rgba_.r_);
            cixel::cixel_s8 u = static_cast<cixel::cixel_s8>(color.rgba_.g_);
            cixel::cixel_s8 v = static_cast<cixel::cixel_s8>(color.rgba_.b_);
            cixel::Color c;
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

        cixel::Cixel* cixel = cixel::cixelCreate(width, height, CIXEL_NULL, CIXEL_NULL);

        cixel::cixel_u32* pixels = convert(width, height, channels, data);
        stbi_image_free(data);

        cixel::cixel_s32 size = width * height;
        cixel::cixel_u8* indices = reinterpret_cast<cixel::cixel_u8*>(malloc(size * sizeof(cixel::cixel_u8)));
        cixel::cixel_u32* pixels0 = reinterpret_cast<cixel::cixel_u32*>(malloc(size * sizeof(cixel::cixel_u32)));

        std::chrono::high_resolution_clock::time_point start;
        start = std::chrono::high_resolution_clock::now();
        cixel::cixelQuantize(cixel, indices, pixels, false);
        long long timeQuantize = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
        printf("quantize: %lld microseconds\n", timeQuantize);

        quantizeYUV655(pixels0, size, indices, cixel);

        SPRINTF(buffer, "%s%s", directory, dst0);
        stbi_write_jpg(buffer, width, height, 4, reinterpret_cast<unsigned char*>(pixels0), 100);

        {
            SPRINTF(buffer, "%s%s", directory, dst1);
            FILE* file = fopen(buffer, "wb");
            if(NULL != file) {
                start = std::chrono::high_resolution_clock::now();

                cixel::cixelPrint(cixel, file, indices);

                long long timeWrite = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();
                printf("write sixels: %lld microseconds\n", timeWrite);

                fclose(file);
            }
        }

        free(pixels0);
        free(indices);
        free(pixels);
        cixel::cixelDestroy(cixel);
        return true;
    }
} // namespace

UTEST(Quantize_Encode, grad)
{
    EXPECT_TRUE(test("grad.png", "grad_out.jpg", "grad.txt", "../data/"));
    EXPECT_TRUE(test("test.bmp", "test_out.jpg", "test.txt", "../data/"));
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

