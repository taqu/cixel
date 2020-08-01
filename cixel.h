#ifndef INC_CIXEL_H_
#define INC_CIXEL_H_
/**
@file cppsixel.h
@author t-sakai

# License
This software is distributed under two licenses, choose whichever you like.

## MIT License
Copyright 2020 Takuro Sakai

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

## Public Domain
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <http://unlicense.org>
```

# Usage
Put '#define CIXEL_IMPLEMENTATION' before including this file to create the implementation.
*/
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/stat.h>

#ifndef __cplusplus
#include <stdbool.h>
#endif

#if defined(__AVX__) || defined(__AVX2__)
#    define CIXEL_SSE (1)
#endif
#if defined(__ARM_NEON) || defined(__ARM_NEON__)
#    define CIXEL_NEON (1)
#endif

#if defined(CIXEL_SSE)
#    include <immintrin.h>
#endif

#ifdef __cplusplus
#define CIXEL_NAMESPACE_BEGIN(name) namespace name {
#define CIXEL_NAMESPACE_END(name) }
#define CIXEL_NAMESPACE_EMPTY_BEGIN namespace {
#define CIXEL_NAMESPACE_EMPTY_END }
#define CIXEL_STATIC
#define CIXEL_REINTERPRET_CAST(type) reinterpret_cast<type>
#define CIXEL_STATIC_CAST(type) static_cast<type>
#else
#define CIXEL_NAMESPACE_BEGIN(name)
#define CIXEL_NAMESPACE_END(name)
#define CIXEL_NAMESPACE_EMPTY_BEGIN
#define CIXEL_NAMESPACE_EMPTY_END
#define CIXEL_STATIC static
#define CIXEL_REINTERPRET_CAST(type) (type)
#define CIXEL_STATIC_CAST(type) (type)
#endif

CIXEL_NAMESPACE_BEGIN(cixel)
#ifndef CIXEL_NULL
#    ifdef __cplusplus
#        if 201103L <= __cplusplus || 1700 <= _MSC_VER
#            define CIXEL_NULL nullptr
#        else
#            define CIXEL_NULL 0
#        endif
#    else
#        define CIXEL_NULL (void*)0
#    endif
#endif

#ifdef __clang__
#    define CIXEL_ALIGN(n) __attribute__((aligned(n)))
#    define CIXEL_RESTRICT __restrict__

#elif defined(_MSC_VER)
#    define CIXEL_ALIGN(n) __declspec(align(n))
#    define CIXEL_RESTRICT __restrict
#else
#    define CIXEL_ALIGN(n) __attribute__((aligned(n)))
#    define CIXEL_RESTRICT __restrict__
#endif

#ifndef CIXEL_TYPES
#    define CIXEL_TYPES
typedef int8_t cixel_s8;
typedef int16_t cixel_s16;
typedef int32_t cixel_s32;
typedef int64_t cixel_s64;

typedef uint8_t cixel_u8;
typedef uint16_t cixel_u16;
typedef uint32_t cixel_u32;
typedef uint64_t cixel_u64;

typedef float cixel_f32;
typedef double cixel_f64;

typedef size_t cixel_size_t;
#endif // CIXEL_TYPES

#ifndef CIXEL_ASSERT
#    define CIXEL_ASSERT(exp) assert(exp)
#endif

#ifdef __cplusplus
constexpr cixel_s32 MAX_COLORS = 256;

constexpr cixel_u32 ALIGN_SIZE = 16; //< alignment of inner memory allocation
constexpr uintptr_t ALIGN_OFFSET = ALIGN_SIZE - 1;
constexpr uintptr_t ALIGN_MASK = ~ALIGN_OFFSET;

#else
#define MAX_COLORS (256)

// alignment of inner memory allocation
#define ALIGN_SIZE (16)
#define ALIGN_OFFSET (15)
#define ALIGN_MASK (~15)
#endif

typedef void* (*AllocFunc)(cixel_size_t);
typedef void (*FreeFunc)(void*);

//--- Utility functions
//-----------------------------------------------------------
#ifdef __cplusplus
template<class T>
T maximum(T x0, T x1)
{
    return (x0 < x1) ? x1 : x0;
}

template<class T>
T minimum(T x0, T x1)
{
    return (x0 < x1) ? x0 : x1;
}

#else
#define maximum(x0, x1) (x0<x1)? x1 : x0
#define minimum(x0, x1) (x0<x1)? x0 : x1
#endif

//--- Utility structures
//-----------------------------------------------------------
struct RGBA_t
{
    cixel_u8 r_;
    cixel_u8 g_;
    cixel_u8 b_;
    cixel_u8 a_;
};
typedef struct RGBA_t RGBA;

struct Color_t
{
    union
    {
        cixel_u32 color_;
        RGBA rgba_;
        cixel_u8 bytes_[4];
    };
};

typedef struct Color_t Color;

struct Color32_t
{
    cixel_u32 r_;
    cixel_u32 g_;
    cixel_u32 b_;
    cixel_u32 a_;
};

typedef struct Color32_t Color32;

struct ColorS32_t
{
    cixel_s32 r_;
    cixel_s32 g_;
    cixel_s32 b_;
    cixel_s32 a_;
};

typedef struct ColorS32_t ColorS32;

struct PointU8_t
{
    cixel_u8 x_;
    cixel_u8 y_;
    cixel_u8 z_;
    cixel_u8 w_;
};

typedef struct PointU8_t PointU8;

struct BoxU8_t
{
    PointU8 start_;
    PointU8 end_;
};

typedef struct BoxU8_t BoxU8;

//-----------------------------------------------------------
//---
//--- Sixel
//---
//-----------------------------------------------------------
struct Cixel_t;
typedef struct Cixel_t Cixel;

/**
@brief 
@param [in] width ... width of image
@param [in] height ... height of image
@param [in] allocFunc ... custom memory allocation
@param [in] freeFunc ... custom memory deallocation
@note allocFunc and freeFunc should be assigned appropriately
*/
Cixel* cixelCreate(cixel_s32 width, cixel_s32 height, AllocFunc allocFunc, FreeFunc freeFunc);
void cixelDestroy(Cixel* cixel);

void cixelQuantize(Cixel* cixel, cixel_u8* CIXEL_RESTRICT indices, const cixel_u32* CIXEL_RESTRICT pixels, bool flipVertical);
void cixelPrint(Cixel* cixel, FILE* file, const cixel_u8* CIXEL_RESTRICT indices);

Color cixelGetPalletColor(const Cixel* cixel, cixel_s32 index);

#if 0
struct Sixel
{
public:
    static const cixel_s32 RESOLUTION_Y = 32;//128;
    static const cixel_s32 RESOLUTION_U = 32;
    static const cixel_s32 RESOLUTION_V = 32;
    static const cixel_s32 SHIFT_Y = 3;//1;
    static const cixel_s32 SHIFT_U = 3;
    static const cixel_s32 SHIFT_V = 3;
    static const cixel_s32 FREQUENCY_SIZE = (RESOLUTION_Y + 1) * (RESOLUTION_U + 1) * (RESOLUTION_V + 1);
    static const cixel_s32 UV_PLANE_SIZE = (RESOLUTION_V + 1) * (RESOLUTION_V + 1);
    static const cixel_s32 V_SIZE = (RESOLUTION_V + 1);

    static const cixel_s32 GRID_SIZE = RESOLUTION_Y * RESOLUTION_U * RESOLUTION_V;
    static const cixel_s32 GRID_UV_SIZE = RESOLUTION_U * RESOLUTION_V;
    static const cixel_s32 GRID_V_SIZE = RESOLUTION_V;
    static const cixel_s32 GRID_SHIFT_Y = (8 - SHIFT_U) + (8 - SHIFT_V);
    static const cixel_s32 GRID_SHIFT_U = (8 - SHIFT_V);

    Cixel();

    /**
     @brief 
     @param [in] width ... width of image
     @param [in] height ... height of image
     @param [in] allocFunc ... custom memory allocation
     @param [in] freeFunc ... custom memory deallocation
     @note allocFunc and freeFunc should be assigned appropriately
     */
    Sixel(cixel_s32 width, cixel_s32 height, AllocFunc allocFunc, FreeFunc freeFunc);

    ~Sixel();
    Sixel(Sixel&& rhs);
    Sixel& operator=(Sixel&& rhs);

    void swap(Sixel& rhs);

    /**
     @brief get the number of quantized colors
     @return the number of colors
     */
    cixel_s32 size() const;

    /**
     @brief get color in the pallet
     @param [in] index ... index in the pallet
     @return color
     */
    Color operator[](cixel_s32 index) const;

    /**
     @brief 
     @param indices
     @param pixels
     */
    void quantize(cixel_u8* CIXEL_RESTRICT indices, const cixel_u32* CIXEL_RESTRICT pixels);

    /**
     @brief 
     @param file
     @param indices
     */
    void print(FILE* file, const cixel_u8* CIXEL_RESTRICT indices) const;

private:
    Sixel(const Sixel&) = delete;
    Sixel& operator=(const Sixel&) = delete;

    void calcPrefixSum();
    cixel_u32 getSum(const BoxU8& box) const;
    void getSumRGB(cixel_u32& count, Color32& rgb, const BoxU8& box) const;

    static cixel_u32 calcSquaredDistance(
        cixel_u32 count0, const Color32& rgb0,
        cixel_u32 count1, const Color32& rgb1,
        const Color32& rgb);

    void sortToUpper(cixel_s32 index, cixel_s32 size, Bucket* buckets);
    void sortToLower(cixel_s32 start, cixel_s32 index, Bucket* buckets);
    bool medianCut(Bucket& bucket0, Bucket& bucket1, const Bucket& src);

    bool calcCenterColor(Color& color, const BoxU8& box) const;
    void add(const Color& color, const BoxU8& box);

#if defined(CIXEL_SSE)
    void diffuseRight(cixel_u8* CIXEL_RESTRICT indices, cixel_s32 width2, cixel_s32 y,
                      const __m128i& zero, const __m128i& c255,
                      const __m128i& cK12YUV655, const __m128i& cK20YUV655,
                      const __m128i& cK21YUV655, const __m128i& cK22YUV655);
    void diffuseLeft(cixel_u8* CIXEL_RESTRICT indices, cixel_s32 width2, cixel_s32 y,
                     const __m128i& zero, const __m128i& c255,
                     const __m128i& cK12YUV655, const __m128i& cK20YUV655,
                     const __m128i& cK21YUV655, const __m128i& cK22YUV655);
#else
    void diffuseRight(cixel_u8* CIXEL_RESTRICT indices, cixel_s32 width2, cixel_s32 y);
    void diffuseLeft(cixel_u8* CIXEL_RESTRICT indices, cixel_s32 width2, cixel_s32 y);
#endif
    void errorDiffusion(cixel_u8* CIXEL_RESTRICT indices);

    AllocFunc allocFunc_;
    FreeFunc freeFunc_;

    cixel_s32 width_;
    cixel_s32 height_;
    cixel_s32 size_;

    void* work_;

    Color* colors_;
    cixel_s16* grid_;

    Color* yuv_;

    cixel_u32* frequencies_;
    Color32* accColors_;
    Bucket* boxes_;

    ColorS32* errors_;

    cixel_u8* writeBuffer_;
    cixel_u8* indicesFlags_;
    cixel_u32* colorFlags_;
    cixel_u8* palletIndices_;
};
#endif

cixel_u32 cixelRGB2YUV(cixel_u32 rgba);
cixel_u32 cixelYUV2RGB(cixel_u32 yuva);

CIXEL_NAMESPACE_END(cixel)
#endif // INC_CIXEL_H_

#ifdef CIXEL_IMPLEMENTATION
CIXEL_NAMESPACE_BEGIN(cixel)
CIXEL_NAMESPACE_EMPTY_BEGIN

#ifdef _DEBUG
CIXEL_STATIC void validate(const BoxU8 box)
{
    CIXEL_ASSERT(box.start_.x_ <= box.end_.x_);
    CIXEL_ASSERT(box.start_.y_ <= box.end_.y_);
    CIXEL_ASSERT(box.start_.z_ <= box.end_.z_);
}

CIXEL_STATIC void overlap(const BoxU8 x0, const BoxU8 x1)
{
    CIXEL_ASSERT(x0.end_.x_ < x1.start_.x_
                    || x0.end_.y_ < x1.start_.y_
                    || x0.end_.z_ < x1.start_.z_
                    || x1.end_.x_ < x0.start_.x_
                    || x1.end_.y_ < x0.start_.y_
                    || x1.end_.z_ < x0.start_.z_);
}
#endif

CIXEL_STATIC inline void addColor32(Color32* c0, const Color32* c1)
{
    c0->r_ += c1->r_;
    c0->g_ += c1->g_;
    c0->b_ += c1->b_;
}

CIXEL_STATIC inline void subColor32(Color32* c0, const Color32* c1)
{
    c0->r_ -= c1->r_;
    c0->g_ -= c1->g_;
    c0->b_ -= c1->b_;
}

#ifdef __cplusplus
    template<class T>
    inline T align(T x)
    {
        return (x + ALIGN_OFFSET) & ALIGN_MASK;
    }

    template<class T>
    inline T absolute(T x)
    {
        return 0 <= x ? x : -x;
    }

    template<class T>
    T clamp(T x, T minx, T maxx)
    {
        return minx <= x ? x <= maxx ? x : maxx : minx;
    }

    template<class T>
    void swap(T& x0, T& x1)
    {
        T t = x0;
        x0 = x1;
        x1 = t;
    }

    cixel_u8 toU8(cixel_u32 x)
    {
        return (x < 256) ? CIXEL_STATIC_CAST(cixel_u8)(x) : 255;
    }

#if defined(CIXEL_SSE)
    constexpr cixel_u32 align16(cixel_u32 size)
    {
        return (size + 15U) & ~15U;
    }

    template<cixel_u32 SIZE>
    void setZero16(void* CIXEL_RESTRICT ptr)
    {
        constexpr cixel_u32 total = (SIZE >> 4);
        constexpr cixel_u32 count = total >> 2;
        constexpr cixel_u32 remain = total - (count << 2);

        __m128* p = reinterpret_cast<__m128*>(ptr);
        __m128 zero = _mm_setzero_ps();
        for(cixel_u32 i = 0; i < count; ++i) {
            _mm_store_ps((cixel_f32*)(p + 0), zero);
            _mm_store_ps((cixel_f32*)(p + 1), zero);
            _mm_store_ps((cixel_f32*)(p + 2), zero);
            _mm_store_ps((cixel_f32*)(p + 3), zero);
            p += 4;
        }
        for(cixel_u32 i = 0; i < remain; ++i, ++p) {
            _mm_store_ps((cixel_f32*)p, zero);
        }
    }

    template<cixel_u32 SIZE>
    void setMinusOne16(void* CIXEL_RESTRICT ptr)
    {
        constexpr cixel_u32 total = (SIZE >> 4);
        constexpr cixel_u32 count = total >> 2;
        constexpr cixel_u32 remain = total - (count << 2);

        __m128i* p = reinterpret_cast<__m128i*>(ptr);
        __m128i v = _mm_set1_epi32(-1);
        for(cixel_u32 i = 0; i < count; ++i) {
            _mm_store_si128((p + 0), v);
            _mm_store_si128((p + 1), v);
            _mm_store_si128((p + 2), v);
            _mm_store_si128((p + 3), v);
            p += 4;
        }
        for(cixel_u32 i = 0; i < remain; ++i, ++p) {
            _mm_store_si128(p, v);
        }
    }

    void setZero16(void* CIXEL_RESTRICT ptr, cixel_u32 size)
    {
        const cixel_u32 total = (size >> 4);
        const cixel_u32 count = total >> 2;
        const cixel_u32 remain = total - (count << 2);

        __m128* p = reinterpret_cast<__m128*>(ptr);
        __m128 zero = _mm_setzero_ps();
        for(cixel_u32 i = 0; i < count; ++i) {
            _mm_store_ps((cixel_f32*)(p + 0), zero);
            _mm_store_ps((cixel_f32*)(p + 1), zero);
            _mm_store_ps((cixel_f32*)(p + 2), zero);
            _mm_store_ps((cixel_f32*)(p + 3), zero);
            p += 4;
        }
        for(cixel_u32 i = 0; i < remain; ++i, ++p) {
            _mm_store_ps((cixel_f32*)p, zero);
        }
    }
#endif

#else //__cplusplus

#define  align(x) ((x + ALIGN_OFFSET) & ALIGN_MASK)
#define absolute(x) (0<=x? x : -x)
#define clamp(x, minx, maxx) (minx<=x? x<=maxx? x : maxx : minx)
#define swap(type, x0, x1) {type tmp = x0; x0 = x1; x1 = tmp;}

    CIXEL_STATIC cixel_u8 toU8(cixel_u32 x)
    {
        return (x < 256) ? (cixel_u8)(x) : 255;
    }

#if defined(CIXEL_SSE)
    #define align16(x) ((x+15U)&(~15U))

    CIXEL_STATIC void setZero16(void* CIXEL_RESTRICT ptr, cixel_u32 size)
    {
        const cixel_u32 total = (size >> 4);
        const cixel_u32 count = total >> 2;
        const cixel_u32 remain = total - (count << 2);

        __m128* p = CIXEL_REINTERPRET_CAST(__m128*)(ptr);
        __m128 zero = _mm_setzero_ps();
        for(cixel_u32 i = 0; i < count; ++i) {
            _mm_store_ps((cixel_f32*)(p + 0), zero);
            _mm_store_ps((cixel_f32*)(p + 1), zero);
            _mm_store_ps((cixel_f32*)(p + 2), zero);
            _mm_store_ps((cixel_f32*)(p + 3), zero);
            p += 4;
        }
        for(cixel_u32 i = 0; i < remain; ++i, ++p) {
            _mm_store_ps((cixel_f32*)p, zero);
        }
    }

    CIXEL_STATIC void setMinusOne16(void* CIXEL_RESTRICT ptr, cixel_u32 size)
    {
        const cixel_u32 total = (size >> 4);
        const cixel_u32 count = total >> 2;
        const cixel_u32 remain = total - (count << 2);

        __m128i* p = CIXEL_REINTERPRET_CAST(__m128i*)(ptr);
        __m128i v = _mm_set1_epi32(-1);
        for(cixel_u32 i = 0; i < count; ++i) {
            _mm_store_si128((p + 0), v);
            _mm_store_si128((p + 1), v);
            _mm_store_si128((p + 2), v);
            _mm_store_si128((p + 3), v);
            p += 4;
        }
        for(cixel_u32 i = 0; i < remain; ++i, ++p) {
            _mm_store_si128(p, v);
        }
    }

#endif
#endif

    static const cixel_s32 K12YUV655 = 7; // 7.0f / 16.0f;
    static const cixel_s32 K20YUV655 = 3; // 3.0f / 16.0f;
    static const cixel_s32 K21YUV655 = 5; // 5.0f / 16.0f;
    static const cixel_s32 K22YUV655 = 1; // 1.0f / 16.0f;
#ifdef __cplusplus
#    if defined(CIXEL_SSE)
    void muladdDiffustion(ColorS32* c, const __m128i* ratio, const __m128i* error)
    {
        __m128i c0 = _mm_load_si128((const __m128i*)c);
        c0 = _mm_add_epi32(c0, _mm_mul_epi32(*ratio, *error));
        _mm_store_si128((__m128i*)c, c0);
    }
#    else
    void muladdDiffusion(ColorS32* c, cixel_s32 ratio, const cixel_s32 error[4])
    {
        c->r_ += ratio * error[0];
        c->g_ += ratio * error[1];
        c->b_ += ratio * error[2];
    }
#    endif
#else
#    if defined(CIXEL_SSE)
    void muladdDiffustion(ColorS32* c, const __m128i* ratio, const __m128i* error)
    {
        __m128i c0 = _mm_load_si128((const __m128i*)c);
        c0 = _mm_add_epi32(c0, _mm_mul_epi32(*ratio, *error));
        _mm_store_si128((__m128i*)c, c0);
    }
#    else
    void muladdDiffusion(ColorS32* c, cixel_s32 ratio, const cixel_s32 error[4])
    {
        c->r_ += ratio * error[0];
        c->g_ += ratio * error[1];
        c->b_ += ratio * error[2];
    }
#    endif
#endif

    static const char header[] = {
        0x1BU, // ESC
        0x50U, // P
        0x30U, // 0
        0x3BU, //;
        0x30U, // 0
        0x3BU, //;
        0x38U, // 8
        0x71U, // q

        // raster attributes
        0x22U, //"
        0x31U, // 1
        0x3BU, //;
        0x31U, // 1
    };

    static const char footer[] = {
        0x1BU, // ESC
        0x5CU, //
    };

#ifdef __cplusplus
    static const cixel_s32 RESOLUTION_Y = 32; //128;
    static const cixel_s32 RESOLUTION_U = 32;
    static const cixel_s32 RESOLUTION_V = 32;
    static const cixel_s32 SHIFT_Y = 3; //1;
    static const cixel_s32 SHIFT_U = 3;
    static const cixel_s32 SHIFT_V = 3;
    static const cixel_s32 FREQUENCY_SIZE = (RESOLUTION_Y + 1) * (RESOLUTION_U + 1) * (RESOLUTION_V + 1);
    static const cixel_s32 UV_PLANE_SIZE = (RESOLUTION_V + 1) * (RESOLUTION_V + 1);
    static const cixel_s32 V_SIZE = (RESOLUTION_V + 1);

    static const cixel_s32 GRID_SIZE = RESOLUTION_Y * RESOLUTION_U * RESOLUTION_V;
    static const cixel_s32 GRID_UV_SIZE = RESOLUTION_U * RESOLUTION_V;
    static const cixel_s32 GRID_V_SIZE = RESOLUTION_V;
    static const cixel_s32 GRID_SHIFT_Y = (8 - SHIFT_U) + (8 - SHIFT_V);
    static const cixel_s32 GRID_SHIFT_U = (8 - SHIFT_V);

#else
#    define RESOLUTION_Y (32)
#    define RESOLUTION_U (32)
#    define RESOLUTION_V (32)
#    define SHIFT_Y (3)
#    define SHIFT_U (3)
#    define SHIFT_V (3)
#    define FREQUENCY_SIZE ((RESOLUTION_Y + 1) * (RESOLUTION_U + 1) * (RESOLUTION_V + 1))
#    define UV_PLANE_SIZE ((RESOLUTION_V + 1) * (RESOLUTION_V + 1))
#    define V_SIZE (RESOLUTION_V + 1)

#    define GRID_SIZE (RESOLUTION_Y * RESOLUTION_U * RESOLUTION_V)
#    define GRID_UV_SIZE (RESOLUTION_U * RESOLUTION_V)
#    define GRID_V_SIZE RESOLUTION_V
#    define GRID_SHIFT_Y ((8 - SHIFT_U) + (8 - SHIFT_V))
#    define GRID_SHIFT_U (8 - SHIFT_V)
#endif

    CIXEL_STATIC void YUV2RGBPercent(cixel_s32 rgba[4], cixel_u32 yuva)
    {
#ifdef __cplusplus
        static const cixel_s32 Base = (1 << 10);
        static const cixel_s32 Half = (Base>>1);
        static const cixel_s32 s00 = CIXEL_STATIC_CAST(cixel_s32)(1.000f * Base);
        static const cixel_s32 s02 = CIXEL_STATIC_CAST(cixel_s32)(1.402f * Base);

        static const cixel_s32 s10 = CIXEL_STATIC_CAST(cixel_s32)(1.000f * Base);
        static const cixel_s32 s11 = CIXEL_STATIC_CAST(cixel_s32)(-0.344f * Base);
        static const cixel_s32 s12 = CIXEL_STATIC_CAST(cixel_s32)(-0.714f * Base);

        static const cixel_s32 s20 = CIXEL_STATIC_CAST(cixel_s32)(1.000f * Base);
        static const cixel_s32 s21 = CIXEL_STATIC_CAST(cixel_s32)(1.772f * Base);
#else
        static const cixel_s32 Half = (512);
        static const cixel_s32 s00 = CIXEL_STATIC_CAST(cixel_s32)(1.000f * 1024);
        static const cixel_s32 s02 = CIXEL_STATIC_CAST(cixel_s32)(1.402f * 1024);

        static const cixel_s32 s10 = CIXEL_STATIC_CAST(cixel_s32)(1.000f * 1024);
        static const cixel_s32 s11 = CIXEL_STATIC_CAST(cixel_s32)(-0.344f * 1024);
        static const cixel_s32 s12 = CIXEL_STATIC_CAST(cixel_s32)(-0.714f * 1024);

        static const cixel_s32 s20 = CIXEL_STATIC_CAST(cixel_s32)(1.000f * 1024);
        static const cixel_s32 s21 = CIXEL_STATIC_CAST(cixel_s32)(1.772f * 1024);
#endif

        cixel_u8 y = CIXEL_STATIC_CAST(cixel_u8)(yuva);
        cixel_u8 u = CIXEL_STATIC_CAST(cixel_u8)(yuva >> 8);
        cixel_u8 v = CIXEL_STATIC_CAST(cixel_u8)(yuva >> 16);

        cixel_s32 su = CIXEL_STATIC_CAST(cixel_s32)(u) - 128;
        cixel_s32 sv = CIXEL_STATIC_CAST(cixel_s32)(v) - 128;
        rgba[0] = (s00 * y + s02 * sv + Half) >> 10;
        rgba[1] = (s10 * y + s11 * su + s12 * sv + Half) >> 10;
        rgba[2] = (s20 * y + s21 * su + Half) >> 10;
        rgba[0] = clamp(rgba[0], 0, 255);
        rgba[1] = clamp(rgba[1], 0, 255);
        rgba[2] = clamp(rgba[2], 0, 255);

        rgba[0] *= 100 * (1 << (10 - 8));
        rgba[1] *= 100 * (1 << (10 - 8));
        rgba[2] *= 100 * (1 << (10 - 8));

        rgba[0] >>= 10;
        rgba[1] >>= 10;
        rgba[2] >>= 10;
        CIXEL_ASSERT(0 <= rgba[0] && rgba[0] <= 100);
        CIXEL_ASSERT(0 <= rgba[1] && rgba[1] <= 100);
        CIXEL_ASSERT(0 <= rgba[2] && rgba[2] <= 100);
    }

CIXEL_NAMESPACE_EMPTY_END

cixel_u32 cixelRGB2YUV(cixel_u32 rgba)
{
#if defined(CIXEL_SSE)
    static CIXEL_ALIGN(16) const cixel_f32 sr[4] = {0.299f, -0.169f, 0.500f, 1.0f};
    static CIXEL_ALIGN(16) const cixel_f32 sg[4] = {0.587f, -0.331f, -0.419f, 0.0f};
    static CIXEL_ALIGN(16) const cixel_f32 sb[4] = {0.114f, 0.500f, -0.081f, 0.0f};
    static CIXEL_ALIGN(16) const cixel_f32 so[4] = {0.0f, 128.0f, 128.0f, 0.0f};

    __m128i zero = _mm_setzero_si128();
    __m128i i0 = _mm_cvtsi32_si128(*((cixel_s32*)&rgba));
    i0 = _mm_unpacklo_epi8(i0, zero);
    i0 = _mm_unpacklo_epi16(i0, zero);

    __m128 f0 = _mm_cvtepi32_ps(i0);
    __m128 fr0 = _mm_shuffle_ps(f0, f0, _MM_SHUFFLE(3, 0, 0, 0));
    __m128 fg0 = _mm_shuffle_ps(f0, f0, _MM_SHUFFLE(3, 1, 1, 1));
    __m128 fb0 = _mm_shuffle_ps(f0, f0, _MM_SHUFFLE(3, 2, 2, 2));

    fr0 = _mm_mul_ps(fr0, _mm_load_ps(sr));
    fg0 = _mm_mul_ps(fg0, _mm_load_ps(sg));
    fb0 = _mm_mul_ps(fb0, _mm_load_ps(sb));
    f0 = _mm_add_ps(_mm_add_ps(fr0, fg0), _mm_add_ps(fb0, _mm_load_ps(so)));

    i0 = _mm_cvttps_epi32(f0);
    i0 = _mm_packs_epi32(i0, i0);
    i0 = _mm_packus_epi16(i0, i0);

    *((cixel_s32*)&rgba) = _mm_cvtsi128_si32(i0);
    return rgba;
#else
#    ifdef __cplusplus
    static const cixel_s32 Base = 1 << 10;
    static const cixel_s32 Half = Base >> 1;
    static const cixel_s32 s00 = CIXEL_STATIC_CAST(cixel_s32)(0.299f * Base);
    static const cixel_s32 s01 = CIXEL_STATIC_CAST(cixel_s32)(0.587f * Base);
    static const cixel_s32 s02 = CIXEL_STATIC_CAST(cixel_s32)(0.114f * Base);

    static const cixel_s32 s10 = CIXEL_STATIC_CAST(cixel_s32)(-0.169f * Base);
    static const cixel_s32 s11 = CIXEL_STATIC_CAST(cixel_s32)(-0.331f * Base);
    static const cixel_s32 s12 = CIXEL_STATIC_CAST(cixel_s32)(0.500f * Base);

    static const cixel_s32 s20 = CIXEL_STATIC_CAST(cixel_s32)(0.500f * Base);
    static const cixel_s32 s21 = CIXEL_STATIC_CAST(cixel_s32)(-0.419f * Base);
    static const cixel_s32 s22 = CIXEL_STATIC_CAST(cixel_s32)(-0.081f * Base);
#    else
    static const cixel_s32 Base = 1024;
    static const cixel_s32 Half = 512;
    static const cixel_s32 s00 = CIXEL_STATIC_CAST(cixel_s32)(0.299f * 1024);
    static const cixel_s32 s01 = CIXEL_STATIC_CAST(cixel_s32)(0.587f * 1024);
    static const cixel_s32 s02 = CIXEL_STATIC_CAST(cixel_s32)(0.114f * 1024);

    static const cixel_s32 s10 = CIXEL_STATIC_CAST(cixel_s32)(-0.169f * 1024);
    static const cixel_s32 s11 = CIXEL_STATIC_CAST(cixel_s32)(-0.331f * 1024);
    static const cixel_s32 s12 = CIXEL_STATIC_CAST(cixel_s32)(0.500f * 1024);

    static const cixel_s32 s20 = CIXEL_STATIC_CAST(cixel_s32)(0.500f * 1024);
    static const cixel_s32 s21 = CIXEL_STATIC_CAST(cixel_s32)(-0.419f * 1024);
    static const cixel_s32 s22 = CIXEL_STATIC_CAST(cixel_s32)(-0.081f * 1024);
#    endif

    cixel_u8 r = CIXEL_STATIC_CAST(cixel_u8)(rgba);
    cixel_u8 g = CIXEL_STATIC_CAST(cixel_u8)(rgba >> 8);
    cixel_u8 b = CIXEL_STATIC_CAST(cixel_u8)(rgba >> 16);
    cixel_s32 y = (s00 * r + s01 * g + s02 * b + Half) >> 10;
    cixel_s32 u = ((s10 * r + s11 * g + s12 * b + Half) >> 10) + 128;
    cixel_s32 v = ((s20 * r + s21 * g + s22 * b + Half) >> 10) + 128;

    rgba &= 0xFF000000U;
    rgba |= minimum(y, 255);
    rgba |= minimum(u, 255) << 8;
    rgba |= minimum(v, 255) << 16;
    return rgba;
#endif
}

cixel_u32 cixelYUV2RGB(cixel_u32 yuva)
{
#if defined(CIXEL_SSE)
    static CIXEL_ALIGN(16) const cixel_f32 sy[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    static CIXEL_ALIGN(16) const cixel_f32 su[4] = {0.0f, -0.344f, 1.772f, 0.0f};
    static CIXEL_ALIGN(16) const cixel_f32 sv[4] = {1.402f, -0.714f, 0.0f, 0.0f};
    static CIXEL_ALIGN(16) const cixel_f32 so[4] = {0.0f, -128.0f, -128.0f, 0.0f};

    __m128i zero = _mm_setzero_si128();
    __m128i i0 = _mm_cvtsi32_si128(*((cixel_s32*)&yuva));
    i0 = _mm_unpacklo_epi8(i0, zero);
    i0 = _mm_unpacklo_epi16(i0, zero);

    __m128 f0 = _mm_cvtepi32_ps(i0);
    f0 = _mm_add_ps(f0, _mm_load_ps(so));
    __m128 fy0 = _mm_shuffle_ps(f0, f0, _MM_SHUFFLE(3, 0, 0, 0));
    __m128 fu0 = _mm_shuffle_ps(f0, f0, _MM_SHUFFLE(3, 1, 1, 1));
    __m128 fv0 = _mm_shuffle_ps(f0, f0, _MM_SHUFFLE(3, 2, 2, 2));

    fy0 = _mm_mul_ps(fy0, _mm_load_ps(sy));
    fu0 = _mm_mul_ps(fu0, _mm_load_ps(su));
    fv0 = _mm_mul_ps(fv0, _mm_load_ps(sv));
    f0 = _mm_add_ps(fy0, _mm_add_ps(fu0, fv0));

    i0 = _mm_cvttps_epi32(f0);
    i0 = _mm_packs_epi32(i0, i0);
    i0 = _mm_packus_epi16(i0, i0);

    *((cixel_s32*)&yuva) = _mm_cvtsi128_si32(i0);
    return yuva;

#else
#    ifdef __cplusplus
    static const cixel_s32 Base = 1 << 10;
    static const cixel_s32 Half = Base >> 1;
    static const cixel_s32 s00 = CIXEL_STATIC_CAST(cixel_s32)(1.000f * Base);
    static const cixel_s32 s02 = CIXEL_STATIC_CAST(cixel_s32)(1.402f * Base);

    static const cixel_s32 s10 = CIXEL_STATIC_CAST(cixel_s32)(1.000f * Base);
    static const cixel_s32 s11 = CIXEL_STATIC_CAST(cixel_s32)(-0.344f * Base);
    static const cixel_s32 s12 = CIXEL_STATIC_CAST(cixel_s32)(-0.714f * Base);

    static const cixel_s32 s20 = CIXEL_STATIC_CAST(cixel_s32)(1.000f * Base);
    static const cixel_s32 s21 = CIXEL_STATIC_CAST(cixel_s32)(1.772f * Base);
#    else
    static const cixel_s32 Base = 1024;
    static const cixel_s32 Half = 512;
    static const cixel_s32 s00 = CIXEL_STATIC_CAST(cixel_s32)(1.000f * 1024);
    static const cixel_s32 s02 = CIXEL_STATIC_CAST(cixel_s32)(1.402f * 1024);

    static const cixel_s32 s10 = CIXEL_STATIC_CAST(cixel_s32)(1.000f * 1024);
    static const cixel_s32 s11 = CIXEL_STATIC_CAST(cixel_s32)(-0.344f * 1024);
    static const cixel_s32 s12 = CIXEL_STATIC_CAST(cixel_s32)(-0.714f * 1024);

    static const cixel_s32 s20 = CIXEL_STATIC_CAST(cixel_s32)(1.000f * 1024);
    static const cixel_s32 s21 = CIXEL_STATIC_CAST(cixel_s32)(1.772f * 1024);
#    endif

    cixel_u8 y = CIXEL_STATIC_CAST(cixel_u8)(yuva);
    cixel_u8 u = CIXEL_STATIC_CAST(cixel_u8)(yuva >> 8);
    cixel_u8 v = CIXEL_STATIC_CAST(cixel_u8)(yuva >> 16);

    cixel_s32 su = CIXEL_STATIC_CAST(cixel_s32)(u) - 128;
    cixel_s32 sv = CIXEL_STATIC_CAST(cixel_s32)(v) - 128;
    cixel_s32 r = (s00 * y + s02 * sv + Half) >> 10;
    cixel_s32 g = (s10 * y + s11 * su + s12 * sv + Half) >> 10;
    cixel_s32 b = (s20 * y + s21 * su + Half) >> 10;
    r = clamp(r, 0, 255);
    g = clamp(g, 0, 255);
    b = clamp(b, 0, 255);

    yuva &= 0xFF000000U;
    yuva |= r;
    yuva |= g << 8;
    yuva |= b << 16;
    return yuva;
#endif
}

//-----------------------------------------------------------
//---
//--- Cixel
//---
//-----------------------------------------------------------
struct Bucket_t
{
    BoxU8 box_;
    cixel_u32 frequency_;
};

typedef struct Bucket_t Bucket;

struct Cixel_t
{
    AllocFunc allocFunc_;
    FreeFunc freeFunc_;

    cixel_s32 width_;
    cixel_s32 height_;
    cixel_s32 size_;

    Color* colors_;
    cixel_s16* grid_;

    Color* yuv_;

    cixel_u32* frequencies_;
    Color32* accColors_;
    Bucket* boxes_;

    ColorS32* errors_;

    cixel_u8* writeBuffer_;
    cixel_u8* indicesFlags_;
    cixel_u32* colorFlags_;
    cixel_u8* palletIndices_;
};

CIXEL_NAMESPACE_EMPTY_BEGIN
    //-----------------------------------------------------------
    //---
    //--- Cixel functions
    //---
    //-----------------------------------------------------------
    CIXEL_STATIC void calcPrefixSum(Cixel* cixel)
    {
        cixel_u32* frequencies = cixel->frequencies_;
        Color32* accColors = cixel->accColors_;

        cixel_s32 row0 = 0;
        for(cixel_s32 i = 1; i <= RESOLUTION_Y; ++i) {
            cixel_s32 row1 = row0 + UV_PLANE_SIZE;
            cixel_s32 col0 = 0;
            for(cixel_s32 j = 1; j <= RESOLUTION_U; ++j) {
                cixel_s32 col1 = col0 + V_SIZE;
                cixel_s32 dep0 = 0;
                for(cixel_s32 k = 1; k <= RESOLUTION_V; ++k) {
                    cixel_s32 dep1 = k;

                    cixel_s32 index = row1 + col1 + dep1;
                    CIXEL_ASSERT(0 <= index && index < FREQUENCY_SIZE);
                    CIXEL_ASSERT(0 <= (row0 + col0 + dep0) && (row0 + col0 + dep0) < FREQUENCY_SIZE);

                    frequencies[index] += frequencies[row0 + col0 + dep0];

                    frequencies[index] += frequencies[row0 + col1 + dep1];
                    frequencies[index] += frequencies[row1 + col0 + dep1];
                    frequencies[index] += frequencies[row1 + col1 + dep0];

                    frequencies[index] -= frequencies[row0 + col0 + dep1];
                    frequencies[index] -= frequencies[row0 + col1 + dep0];
                    frequencies[index] -= frequencies[row1 + col0 + dep0];

                    addColor32(&accColors[index], &accColors[row0 + col0 + dep0]);

                    addColor32(&accColors[index], &accColors[row0 + col1 + dep1]);
                    addColor32(&accColors[index], &accColors[row1 + col0 + dep1]);
                    addColor32(&accColors[index], &accColors[row1 + col1 + dep0]);

                    subColor32(&accColors[index], &accColors[row0 + col0 + dep1]);
                    subColor32(&accColors[index], &accColors[row0 + col1 + dep0]);
                    subColor32(&accColors[index], &accColors[row1 + col0 + dep0]);

                    dep0 = dep1;
                }
                col0 = col1;
            }
            row0 = row1;
        }
    }
    CIXEL_STATIC cixel_u32 getSum(const Cixel* cixel, const BoxU8* box)
    {
        CIXEL_ASSERT(box->start_.x_ <= box->end_.x_);
        CIXEL_ASSERT(box->start_.y_ <= box->end_.y_);
        CIXEL_ASSERT(box->start_.z_ <= box->end_.z_);

        cixel_s32 r0 = box->start_.x_ * UV_PLANE_SIZE;
        cixel_s32 r1 = (box->end_.x_ + 1) * UV_PLANE_SIZE;

        cixel_s32 g0 = box->start_.y_ * V_SIZE;
        cixel_s32 g1 = (box->end_.y_ + 1) * V_SIZE;

        cixel_s32 b0 = box->start_.z_;
        cixel_s32 b1 = box->end_.z_ + 1;

        const cixel_u32* frequencies = cixel->frequencies_;
        cixel_u32 c = frequencies[r1 + g1 + b1];
        c += frequencies[r0 + g0 + b1];
        c += frequencies[r0 + g1 + b0];
        c += frequencies[r1 + g0 + b0];

        c -= frequencies[r0 + g1 + b1];
        c -= frequencies[r1 + g0 + b1];
        c -= frequencies[r1 + g1 + b0];
        c -= frequencies[r0 + g0 + b0];
        return c;
    }

    CIXEL_STATIC void getSumRGB(const Cixel* cixel, cixel_u32* count, Color32* rgb, const BoxU8* box)
    {
        CIXEL_ASSERT(box->start_.x_ <= box->end_.x_);
        CIXEL_ASSERT(box->start_.y_ <= box->end_.y_);
        CIXEL_ASSERT(box->start_.z_ <= box->end_.z_);

        cixel_s32 r0 = box->start_.x_ * UV_PLANE_SIZE;
        cixel_s32 r1 = (box->end_.x_ + 1) * UV_PLANE_SIZE;

        cixel_s32 g0 = box->start_.y_ * V_SIZE;
        cixel_s32 g1 = (box->end_.y_ + 1) * V_SIZE;

        cixel_s32 b0 = box->start_.z_;
        cixel_s32 b1 = box->end_.z_ + 1;

        const cixel_u32* frequencies = cixel->frequencies_;
        *count = frequencies[r1 + g1 + b1];
        *count += frequencies[r0 + g0 + b1];
        *count += frequencies[r0 + g1 + b0];
        *count += frequencies[r1 + g0 + b0];

        *count -= frequencies[r0 + g1 + b1];
        *count -= frequencies[r1 + g0 + b1];
        *count -= frequencies[r1 + g1 + b0];
        *count -= frequencies[r0 + g0 + b0];

        const Color32* accColors = cixel->accColors_;
        *rgb = accColors[r1 + g1 + b1];
        addColor32(rgb, &accColors[r0 + g0 + b1]);
        addColor32(rgb, &accColors[r0 + g1 + b0]);
        addColor32(rgb, &accColors[r1 + g0 + b0]);

        subColor32(rgb, &accColors[r0 + g1 + b1]);
        subColor32(rgb, &accColors[r1 + g0 + b1]);
        subColor32(rgb, &accColors[r1 + g1 + b0]);
        subColor32(rgb, &accColors[r0 + g0 + b0]);
    }

    CIXEL_STATIC void calcCentroid(cixel_u32 count, Color32* rgb)
    {
        if(0 < count) {
            rgb->r_ /= count;
            rgb->g_ /= count;
            rgb->b_ /= count;
        }
    }

    CIXEL_STATIC cixel_u32 calcSquaredDistance(
        cixel_u32 count0, const Color32* rgb0,
        cixel_u32 count1, const Color32* rgb1,
        const Color32* rgb)
    {
        if(count0<=0 && count1<=0){
            return 0;
        }

#ifdef CIXEL_SSE
        __m128i i0 = _mm_loadu_si128(CIXEL_REINTERPRET_CAST(const __m128i*)(rgb0));
        __m128i i1 = _mm_loadu_si128(CIXEL_REINTERPRET_CAST(const __m128i*)(rgb1));
        __m128i ic = _mm_loadu_si128(CIXEL_REINTERPRET_CAST(const __m128i*)(rgb));

        i0 = _mm_sub_epi32(i0, ic);
        i1 = _mm_sub_epi32(i1, ic);

        i0 = _mm_mullo_epi32(i0, i0);
        i1 = _mm_mullo_epi32(i1, i1);

        __m128i ic0 = _mm_set1_epi32(count0);
        __m128i ic1 = _mm_set1_epi32(count1);
        i0 = _mm_mullo_epi32(i0, ic0);
        i1 = _mm_mullo_epi32(i1, ic1);

        __m128 t0 = _mm_cvtepi32_ps(i0);
        __m128 t1 = _mm_cvtepi32_ps(i1);
        __m128 ttotal = _mm_cvtepi32_ps(_mm_add_epi32(ic0, ic1));
        t0 = _mm_div_ps(t0, ttotal);
        t1 = _mm_div_ps(t1, ttotal);
        t0 = _mm_add_ps(t0, t1);

        i0 = _mm_cvttps_epi32(t0);
        i0 = _mm_hadd_epi32(i0, i0);
        i0 = _mm_hadd_epi32(i0, i0);
        return CIXEL_STATIC_CAST(cixel_u32)(_mm_cvtsi128_si32(i0));
#else
        cixel_u32 total = count0 + count1;
        cixel_s32 r0 = CIXEL_STATIC_CAST(cixel_s32)(rgb0->r_);
        cixel_s32 g0 = CIXEL_STATIC_CAST(cixel_s32)(rgb0->g_);
        cixel_s32 b0 = CIXEL_STATIC_CAST(cixel_s32)(rgb0->b_);
        r0 = r0 - rgb->r_;
        g0 = g0 - rgb->g_;
        b0 = b0 - rgb->b_;

        cixel_s32 r1 = CIXEL_STATIC_CAST(cixel_s32)(rgb1->r_);
        cixel_s32 g1 = CIXEL_STATIC_CAST(cixel_s32)(rgb1->g_);
        cixel_s32 b1 = CIXEL_STATIC_CAST(cixel_s32)(rgb1->b_);
        r1 = r1 - rgb->r_;
        g1 = g1 - rgb->g_;
        b1 = b1 - rgb->b_;

        cixel_u32 d0 = (r0*r0 + g0*g0 + b0*b0)*count0/total;
        cixel_u32 d1 = (r1*r1 + g1*g1 + b1*b1)*count1/total;
        return d0+d1;
#endif
    }


    CIXEL_STATIC bool medianCut(const Cixel* cixel, Bucket* bucket0, Bucket* bucket1, const Bucket* src)
    {
        BoxU8 box = src->box_;
        CIXEL_ASSERT(box.start_.x_ <= box.end_.x_);
        CIXEL_ASSERT(box.start_.y_ <= box.end_.y_);
        CIXEL_ASSERT(box.start_.z_ <= box.end_.z_);

        cixel_u32 count;
        Color32 rgb;
        getSumRGB(cixel, &count, &rgb, &box);
        calcCentroid(count, &rgb);

        cixel_u8 mid[3];
        mid[0] = CIXEL_STATIC_CAST(cixel_u8)((CIXEL_STATIC_CAST(cixel_u16)(box.end_.x_) + box.start_.x_) >> 1);
        mid[1] = CIXEL_STATIC_CAST(cixel_u8)((CIXEL_STATIC_CAST(cixel_u16)(box.end_.y_) + box.start_.y_) >> 1);
        mid[2] = CIXEL_STATIC_CAST(cixel_u8)((CIXEL_STATIC_CAST(cixel_u16)(box.end_.z_) + box.start_.z_) >> 1);
        cixel_s32 axis;
        cixel_u32 sqrDistances;
        cixel_u16 bstart;
        cixel_u16 bend;
        Color32 rgb0;
        Color32 rgb1;

        BoxU8 b0;
        cixel_u32 count0;
        BoxU8 b1;
        cixel_u32 count1;
        {
            b0 = box;
            b0.start_.x_ = box.start_.x_;
            b0.end_.x_ = mid[0];
            getSumRGB(cixel, &count0, &rgb0, &b0);
            calcCentroid(count0, &rgb0);

            b1 = box;
            b1.start_.x_ = mid[0];
            b1.end_.x_ = b1.end_.x_;
            getSumRGB(cixel, &count1, &rgb1, &b1);
            calcCentroid(count1, &rgb1);

            axis = 0;
            sqrDistances = calcSquaredDistance(count0, &rgb0, count1, &rgb1, &rgb);

            bstart = box.start_.x_;
            bend = box.end_.x_;
        }

        {
            b0 = box;
            b0.start_.y_ = box.start_.y_;
            b0.end_.y_ = mid[1];
            getSumRGB(cixel, &count0, &rgb0, &b0);
            calcCentroid(count0, &rgb0);

            b1 = box;
            b1.start_.y_ = mid[1];
            b1.end_.y_ = box.end_.y_;
            getSumRGB(cixel, &count1, &rgb1, &b1);
            calcCentroid(count1, &rgb1);

            cixel_u32 tsqrDistances = calcSquaredDistance(count0, &rgb0, count1, &rgb1, &rgb);
            if(sqrDistances < tsqrDistances) {
                sqrDistances = tsqrDistances;
                axis = 1;
                bstart = box.start_.y_;
                bend = box.end_.y_;
            }
        }

        {
            b0 = box;
            b0.start_.z_ = box.start_.z_;
            b0.end_.z_ = mid[2];
            getSumRGB(cixel, &count0, &rgb0, &b0);
            calcCentroid(count0, &rgb0);

            b1 = box;
            b1.start_.z_ = mid[2];
            b1.end_.z_ = box.end_.z_;
            getSumRGB(cixel, &count1, &rgb1, &b1);
            calcCentroid(count1, &rgb1);

            cixel_u32 tsqrDistances = calcSquaredDistance(count0, &rgb0, count1, &rgb1, &rgb);
            if(sqrDistances < tsqrDistances) {
                sqrDistances = tsqrDistances;
                axis = 2;
                bstart = box.start_.z_;
                bend = box.end_.z_;
            }
        }
        if(bstart == bend) {
            return false;
        }
        cixel_u8 split0 = CIXEL_STATIC_CAST(cixel_u8)((bstart + bend) >> 1);
        cixel_u8 split1 = split0 + 1;
#ifdef __cplusplus
        switch(axis) {
        case 0:
            bucket0->box_ = {{box.start_.x_, box.start_.y_, box.start_.z_, 0}, {split0, box.end_.y_, box.end_.z_, 0}};
            bucket1->box_ = {{split1, box.start_.y_, box.start_.z_, 0}, {box.end_.x_, box.end_.y_, box.end_.z_, 0}};
            break;
        case 1:
            bucket0->box_ = {{box.start_.x_, box.start_.y_, box.start_.z_, 0}, {box.end_.x_, split0, box.end_.z_, 0}};
            bucket1->box_ = {{box.start_.x_, split1, box.start_.z_, 0}, {box.end_.x_, box.end_.y_, box.end_.z_, 0}};
            break;
        case 2:
            bucket0->box_ = {{box.start_.x_, box.start_.y_, box.start_.z_, 0}, {box.end_.x_, box.end_.y_, split0, 0}};
            bucket1->box_ = {{box.start_.x_, box.start_.y_, split1, 0}, {box.end_.x_, box.end_.y_, box.end_.z_, 0}};
            break;
        default:
            CIXEL_ASSERT(false);
            break;
        }
#else
        switch(axis) {
        case 0:
            bucket0->box_ = (BoxU8){{box.start_.x_, box.start_.y_, box.start_.z_, 0}, {split0, box.end_.y_, box.end_.z_, 0}};
            bucket1->box_ = (BoxU8){{split1, box.start_.y_, box.start_.z_, 0}, {box.end_.x_, box.end_.y_, box.end_.z_, 0}};
            break;
        case 1:
            bucket0->box_ = (BoxU8){{box.start_.x_, box.start_.y_, box.start_.z_, 0}, {box.end_.x_, split0, box.end_.z_, 0}};
            bucket1->box_ = (BoxU8){{box.start_.x_, split1, box.start_.z_, 0}, {box.end_.x_, box.end_.y_, box.end_.z_, 0}};
            break;
        case 2:
            bucket0->box_ = (BoxU8){{box.start_.x_, box.start_.y_, box.start_.z_, 0}, {box.end_.x_, box.end_.y_, split0, 0}};
            bucket1->box_ = (BoxU8){{box.start_.x_, box.start_.y_, split1, 0}, {box.end_.x_, box.end_.y_, box.end_.z_, 0}};
            break;
        default:
            CIXEL_ASSERT(false);
            break;
        }
#endif
        bucket0->frequency_ = getSum(cixel, &bucket0->box_);
        bucket1->frequency_ = getSum(cixel, &bucket1->box_);

        return true;
    }

    CIXEL_STATIC void sortToUpper(cixel_s32 index, cixel_s32 size, Bucket* buckets)
    {
        for(cixel_s32 i = index + 1; i < size; ++i) {
            if(buckets[i].frequency_ <= buckets[i-1].frequency_) {
                return;
            }
            Bucket tmp = buckets[i];
            buckets[i] = buckets[i-1];
            buckets[i-1] = tmp;
        }
    }

    CIXEL_STATIC void sortToLower(cixel_s32 start, cixel_s32 index, Bucket* buckets)
    {
        for(cixel_s32 i = index; start < i; --i) {
            if(buckets[i].frequency_ <= buckets[i-1].frequency_) {
                return;
            }
            Bucket tmp = buckets[i];
            buckets[i] = buckets[i-1];
            buckets[i-1] = tmp;
        }
    }

    CIXEL_STATIC bool calcCenterColor(const Cixel* cixel, Color* color, const BoxU8* box)
    {
        cixel_u32 count;
        Color32 rgb;
        getSumRGB(cixel, &count, &rgb, box);

        if(0 < count) {
            cixel_u32 r = ((rgb.r_ << 1) / count + 1) >> 1;
            cixel_u32 g = ((rgb.g_ << 1) / count + 1) >> 1;
            cixel_u32 b = ((rgb.b_ << 1) / count + 1) >> 1;

            color->rgba_.r_ = toU8(r);
            color->rgba_.g_ = toU8(g);
            color->rgba_.b_ = toU8(b);
            color->rgba_.a_ = 0xFFU;
            return true;
        } else {
            return false;
        }
    }

    CIXEL_STATIC void add(Cixel* cixel, Color color, const BoxU8* box)
    {
        CIXEL_ASSERT(cixel->size_ < MAX_COLORS);
        cixel->colors_[cixel->size_] = color;

        cixel_u8 us = CIXEL_STATIC_CAST(cixel_u8)(cixel->size_);
        for(cixel_s32 r = box->start_.x_; r <= box->end_.x_; ++r) {
            cixel_s32 tr = r << GRID_SHIFT_Y;
            for(cixel_s32 g = box->start_.y_; g <= box->end_.y_; ++g) {
                cixel_s32 tg = tr + (g << GRID_SHIFT_U);
                for(cixel_s32 b = box->start_.z_; b <= box->end_.z_; ++b) {
                    cixel->grid_[tg + b] = us;
                }
            }
        }
        ++cixel->size_;
    }

#if defined(CIXEL_SSE)
    //-----------------------------------------------------------
    CIXEL_STATIC void diffuseRight(Cixel* cixel, cixel_u8* CIXEL_RESTRICT indices, cixel_s32 width2, cixel_s32 y,
        const __m128i* zero, const __m128i* c255,
        const __m128i* cK12YUV655, const __m128i* cK20YUV655,
        const __m128i* cK21YUV655, const __m128i* cK22YUV655)
    {
        cixel_s32 width = cixel->width_;

        const Color* yuv = cixel->yuv_;
        const Color* colors = cixel->colors_;
        const cixel_s16* grid = cixel->grid_;
        ColorS32* errors = cixel->errors_;

        CIXEL_ALIGN(16) cixel_s32 tmp[4];

        cixel_s32 index0 = y * width;
        cixel_s32 index1 = y * width2 + 1;
        for(cixel_s32 j = 0; j < width; ++j, ++index0, ++index1) {
            __m128i i0 = _mm_cvtsi32_si128(*((cixel_s32*)&yuv[index0]));
            i0 = _mm_unpacklo_epi8(i0, *zero);
            i0 = _mm_unpacklo_epi16(i0, *zero);

            __m128i error = _mm_load_si128((const __m128i*)&errors[index1]);
            error = _mm_add_epi32(error, _mm_slli_epi32(i0, 4));
            error = _mm_srai_epi32(error, 4);
            error = _mm_max_epi32(error, *zero);
            error = _mm_min_epi32(error, *c255);
            _mm_store_si128((__m128i*)tmp, error);

            cixel_s32 index = ((tmp[0] >> SHIFT_Y) << GRID_SHIFT_Y) + ((tmp[1] >> SHIFT_U) << GRID_SHIFT_U) + (tmp[2] >> SHIFT_V);
            if(0 <= grid[index]) {
                indices[index0] = CIXEL_STATIC_CAST(cixel_u8)(grid[index]);

                __m128i t0 = _mm_cvtsi32_si128(*((cixel_s32*)&colors[indices[index0]]));
                t0 = _mm_unpacklo_epi8(t0, *zero);
                t0 = _mm_unpacklo_epi16(t0, *zero);

                error = _mm_sub_epi32(i0, t0);

                muladdDiffustion(&errors[index1 - 1], cK12YUV655, &error);
                muladdDiffustion(&errors[index1 + width2 - 1], cK22YUV655, &error);
                muladdDiffustion(&errors[index1 + width2], cK21YUV655, &error);
                muladdDiffustion(&errors[index1 + width2 + 1], cK20YUV655, &error);

            } else {
                _mm_store_si128((__m128i*)tmp, i0);
                index = ((tmp[0] >> SHIFT_Y) << GRID_SHIFT_Y) + ((tmp[1] >> SHIFT_U) << GRID_SHIFT_U) + (tmp[2] >> SHIFT_V);
                indices[index0] = CIXEL_STATIC_CAST(cixel_u8)(grid[index]);
                CIXEL_ASSERT(0 <= grid[index]);
            }
        } // for(cixel_s32 j=0
    }

    //-----------------------------------------------------------
    CIXEL_STATIC void diffuseLeft(Cixel* cixel, cixel_u8* CIXEL_RESTRICT indices, cixel_s32 width2, cixel_s32 y,
        const __m128i* zero, const __m128i* c255,
        const __m128i* cK12YUV655, const __m128i* cK20YUV655,
        const __m128i* cK21YUV655, const __m128i* cK22YUV655)
    {
        cixel_s32 width = cixel->width_;

        const Color* yuv = cixel->yuv_;
        const Color* colors = cixel->colors_;
        const cixel_s16* grid = cixel->grid_;
        ColorS32* errors = cixel->errors_;

        CIXEL_ALIGN(16) cixel_s32 tmp[4];

        cixel_s32 index0 = y * width + width - 1;
        cixel_s32 index1 = y * width2 + width;
        for(cixel_s32 j = width; 1 <= j; --j, --index0, --index1) {
            __m128i i0 = _mm_cvtsi32_si128(*((cixel_s32*)&yuv[index0]));
            i0 = _mm_unpacklo_epi8(i0, *zero);
            i0 = _mm_unpacklo_epi16(i0, *zero);

            __m128i error = _mm_load_si128((const __m128i*)&errors[index1]);
            error = _mm_add_epi32(error, _mm_slli_epi32(i0, 4));
            error = _mm_srai_epi32(error, 4);
            error = _mm_max_epi32(error, *zero);
            error = _mm_min_epi32(error, *c255);
            _mm_store_si128((__m128i*)tmp, error);

            cixel_s32 index = ((tmp[0] >> SHIFT_Y) << GRID_SHIFT_Y) + ((tmp[1] >> SHIFT_U) << GRID_SHIFT_U) + (tmp[2] >> SHIFT_V);
            if(0 <= grid[index]) {
                indices[index0] = CIXEL_STATIC_CAST(cixel_u8)(grid[index]);

                __m128i t0 = _mm_cvtsi32_si128(*((cixel_s32*)&colors[indices[index0]]));
                t0 = _mm_unpacklo_epi8(t0, *zero);
                t0 = _mm_unpacklo_epi16(t0, *zero);

                error = _mm_sub_epi32(i0, t0);

                muladdDiffustion(&errors[index1 - 1], cK12YUV655, &error);
                muladdDiffustion(&errors[index1 + width2 - 1], cK20YUV655, &error);
                muladdDiffustion(&errors[index1 + width2], cK21YUV655, &error);
                muladdDiffustion(&errors[index1 + width2 + 1], cK22YUV655, &error);

            } else {
                _mm_store_si128((__m128i*)tmp, i0);
                index = ((tmp[0] >> SHIFT_Y) << GRID_SHIFT_Y) + ((tmp[1] >> SHIFT_U) << GRID_SHIFT_U) + (tmp[2] >> SHIFT_V);
                indices[index0] = CIXEL_STATIC_CAST(cixel_u8)(grid[index]);
                CIXEL_ASSERT(0 <= grid[index]);
            }
        } // for(cixel_s32 j=width
    }

#else
    //-----------------------------------------------------------
    CIXEL_STATIC void diffuseRight(Cixel* cixel, cixel_u8* CIXEL_RESTRICT indices, cixel_s32 width2, cixel_s32 y)
    {
        cixel_s32 width = cixel->width_;

        const Color* yuv = cixel->yuv_;
        const Color* colors = cixel->colors_;
        const cixel_s16* grid = cixel->grid_;
        ColorS32* errors = cixel->errors_;

        cixel_s32 index0 = y * width;
        cixel_s32 index1 = y * width2 + 1;
        for(cixel_s32 j = 0; j < width; ++j, ++index0, ++index1) {
            cixel_u8 r = yuv[index0].rgba_.r_;
            cixel_u8 g = yuv[index0].rgba_.g_;
            cixel_u8 b = yuv[index0].rgba_.b_;
            cixel_s32 sr = CIXEL_STATIC_CAST(cixel_s32)(r);
            cixel_s32 sg = CIXEL_STATIC_CAST(cixel_s32)(g);
            cixel_s32 sb = CIXEL_STATIC_CAST(cixel_s32)(b);
            cixel_s32 ey = errors[index1].r_ + (sr << 4);
            cixel_s32 eu = errors[index1].g_ + (sg << 4);
            cixel_s32 ev = errors[index1].b_ + (sb << 4);
            cixel_s32 ty = clamp(ey >> 4, 0, 255);
            cixel_s32 tu = clamp(eu >> 4, 0, 255);
            cixel_s32 tv = clamp(ev >> 4, 0, 255);

            cixel_s32 index = ((ty >> SHIFT_Y) << GRID_SHIFT_Y) + ((tu >> SHIFT_U) << GRID_SHIFT_U) + (tv >> SHIFT_V);
            if(0 <= grid[index]) {
                indices[index0] = CIXEL_STATIC_CAST(cixel_u8)(grid[index]);
                ty = colors[indices[index0]].rgba_.r_;
                tu = colors[indices[index0]].rgba_.g_;
                tv = colors[indices[index0]].rgba_.b_;

                cixel_s32 error[4];
                error[0] = sr - ty;
                error[1] = sg - tu;
                error[2] = sb - tv;

                muladdDiffusion(&errors[index1 - 1], K12YUV655, error);
                muladdDiffusion(&errors[index1 + width2 - 1], K22YUV655, error);
                muladdDiffusion(&errors[index1 + width2], K21YUV655, error);
                muladdDiffusion(&errors[index1 + width2 + 1], K20YUV655, error);

            } else {
                sr >>= SHIFT_Y;
                sg >>= SHIFT_U;
                sb >>= SHIFT_V;
                index = (sr << GRID_SHIFT_Y) + (sg << GRID_SHIFT_U) + sb;
                indices[index0] = CIXEL_STATIC_CAST(cixel_u8)(grid[index]);
                CIXEL_ASSERT(0 <= grid[index]);
            }
        } // for(cixel_s32 j=0
    }

    //-----------------------------------------------------------
    CIXEL_STATIC void diffuseLeft(Cixel* cixel, cixel_u8* CIXEL_RESTRICT indices, cixel_s32 width2, cixel_s32 y)
    {
        cixel_s32 width = cixel->width_;

        const Color* yuv = cixel->yuv_;
        const Color* colors = cixel->colors_;
        const cixel_s16* grid = cixel->grid_;
        ColorS32* errors = cixel->errors_;

        cixel_s32 index0 = y * width + width - 1;
        cixel_s32 index1 = y * width2 + width;
        for(cixel_s32 j = width; 1 <= j; --j, --index0, --index1) {
            cixel_u8 r = yuv[index0].rgba_.r_;
            cixel_u8 g = yuv[index0].rgba_.g_;
            cixel_u8 b = yuv[index0].rgba_.b_;
            cixel_s32 sr = CIXEL_STATIC_CAST(cixel_s32)(r);
            cixel_s32 sg = CIXEL_STATIC_CAST(cixel_s32)(g);
            cixel_s32 sb = CIXEL_STATIC_CAST(cixel_s32)(b);
            cixel_s32 ey = errors[index1].r_ + (sr << 4);
            cixel_s32 eu = errors[index1].g_ + (sg << 4);
            cixel_s32 ev = errors[index1].b_ + (sb << 4);
            cixel_s32 ty = clamp(ey >> 4, 0, 255);
            cixel_s32 tu = clamp(eu >> 4, 0, 255);
            cixel_s32 tv = clamp(ev >> 4, 0, 255);

            cixel_s32 index = ((ty >> SHIFT_Y) << GRID_SHIFT_Y) + ((tu >> SHIFT_U) << GRID_SHIFT_U) + (tv >> SHIFT_V);
            if(0 <= grid[index]) {
                indices[index0] = CIXEL_STATIC_CAST(cixel_u8)(grid[index]);
                ty = colors[indices[index0]].rgba_.r_;
                tu = colors[indices[index0]].rgba_.g_;
                tv = colors[indices[index0]].rgba_.b_;
                cixel_s32 error[4];
                error[0] = sr - ty;
                error[1] = sg - tu;
                error[2] = sb - tv;

                muladdDiffusion(&errors[index1 - 1], K12YUV655, error);
                muladdDiffusion(&errors[index1 + width2 - 1], K20YUV655, error);
                muladdDiffusion(&errors[index1 + width2], K21YUV655, error);
                muladdDiffusion(&errors[index1 + width2 + 1], K22YUV655, error);

            } else {
                sr >>= SHIFT_Y;
                sg >>= SHIFT_U;
                sb >>= SHIFT_V;
                index = (sr << GRID_SHIFT_Y) + (sg << GRID_SHIFT_U) + sb;
                indices[index0] = CIXEL_STATIC_CAST(cixel_u8)(grid[index]);
                CIXEL_ASSERT(0 <= grid[index]);
            }
        } // for(cixel_s32 j=width
    }
#endif

    CIXEL_STATIC void errorDiffusion(Cixel* cixel, cixel_u8* CIXEL_RESTRICT indices)
    {
        CIXEL_ASSERT(CIXEL_NULL != indices);
        cixel_s32 width2 = cixel->width_ + 2;
        cixel_s32 size = width2 * (cixel->height_ + 1);

#if defined(CIXEL_SSE)
        setZero16(cixel->errors_, sizeof(ColorS32) * size);
        __m128i zero = _mm_setzero_si128();
        __m128i c255 = _mm_set1_epi32(255);

        __m128i cK12YUV655 = _mm_set1_epi32(K12YUV655);
        __m128i cK20YUV655 = _mm_set1_epi32(K20YUV655);
        __m128i cK21YUV655 = _mm_set1_epi32(K21YUV655);
        __m128i cK22YUV655 = _mm_set1_epi32(K22YUV655);

        for(cixel_s32 i = 0; i < cixel->height_; ++i) {
            if(0 == (i & 0x01U)) {
                diffuseRight(cixel, indices, width2, i,
                    &zero, &c255, &cK12YUV655, &cK20YUV655, &cK21YUV655, &cK22YUV655);
            } else {
                diffuseLeft(cixel, indices, width2, i,
                    &zero, &c255, &cK12YUV655, &cK20YUV655, &cK21YUV655, &cK22YUV655);
            }
        }
#else
        memset(cixel->errors_, 0, sizeof(ColorS32) * size);

        for(cixel_s32 i = 0; i < cixel->height_; ++i) {
            if(0 == (i & 0x01U)) {
                diffuseRight(cixel, indices, width2, i);
            } else {
                diffuseLeft(cixel, indices, width2, i);
            }
        }
#endif
    }

    CIXEL_STATIC cixel_s32 writeNumber(cixel_s32 pos, cixel_u8* str, cixel_s32 number)
    {
        CIXEL_ASSERT(0 <= number && number < 1000);
        if(100 <= number) {
            cixel_s32 hundreds = number / 100;
            number -= 100 * hundreds;
            cixel_s32 tens = number / 10;
            number -= 10 * tens;
            cixel_s32 ones = number;
            str[pos + 0] = CIXEL_STATIC_CAST(cixel_u8)(hundreds + '0');
            str[pos + 1] = CIXEL_STATIC_CAST(cixel_u8)(tens + '0');
            str[pos + 2] = CIXEL_STATIC_CAST(cixel_u8)(ones + '0');
            return pos + 3;

        } else if(10 <= number) {
            cixel_s32 tens = number / 10;
            number -= 10 * tens;
            cixel_s32 ones = number;
            str[pos + 0] = CIXEL_STATIC_CAST(cixel_u8)(tens + '0');
            str[pos + 1] = CIXEL_STATIC_CAST(cixel_u8)(ones + '0');
            return pos + 2;

        } else {
            str[pos + 0] = CIXEL_STATIC_CAST(cixel_u8)(number + '0');
            return pos + 1;
        }
    }

    CIXEL_STATIC cixel_s32 cixelWrite(cixel_s32 pos, cixel_u8* str, cixel_s32 n, const char* data)
    {
        for(cixel_s32 i = 0; i < n; ++i, ++pos) {
            str[pos] = data[i];
        }
        return pos;
    }

    CIXEL_STATIC inline cixel_s32 put(cixel_s32 pos, cixel_u8* str, char c)
    {
        str[pos] = CIXEL_STATIC_CAST(cixel_u8)(c);
        return pos + 1;
    }

    CIXEL_STATIC cixel_s32 writeColorIndex(cixel_s32 pos, cixel_u8* str, cixel_u8 index)
    {
        pos = put(pos, str, '#');
        pos = writeNumber(pos, str, index);
        return pos;
    }

    CIXEL_STATIC cixel_s32 writeBits(cixel_s32 pos, cixel_u8* str, cixel_s32 run, cixel_u8 bits)
    {
        CIXEL_ASSERT(0 < run && run <= 255);
        CIXEL_ASSERT(bits <= 63);
        bits += 63;
        if(3 < run) {
            pos = put(pos, str, '!'); // repeat
            pos = writeNumber(pos, str, run);
            pos = put(pos, str, bits);
            return pos;

        } else {
            for(cixel_s32 i = 0; i < run; ++i) {
                pos = put(pos, str, bits);
            }
            CIXEL_ASSERT(run <= 8);
            return pos;
        }
    }

    CIXEL_STATIC cixel_s32 writePalletColor(cixel_s32 pos, cixel_u8* str, cixel_s32 index, cixel_s32 r, cixel_s32 g, cixel_s32 b)
    {
        pos = put(pos, str, '#');
        pos = writeNumber(pos, str, index);
        pos = cixelWrite(pos, str, 3, ";2;");
        pos = writeNumber(pos, str, r);
        pos = put(pos, str, ';');
        pos = writeNumber(pos, str, g);
        pos = put(pos, str, ';');
        pos = writeNumber(pos, str, b);
        return pos;
    }

CIXEL_NAMESPACE_EMPTY_END

Cixel* cixelCreate(cixel_s32 width, cixel_s32 height, AllocFunc allocFunc, FreeFunc freeFunc)
{
    CIXEL_ASSERT(0 <= width);
    CIXEL_ASSERT(0 <= height);
    if(CIXEL_NULL == allocFunc) {
        allocFunc = malloc;
    }
    if(CIXEL_NULL == freeFunc) {
        freeFunc = free;
    }

    // Always needs
    cixel_size_t colorSize = align(sizeof(Color) * MAX_COLORS);
    cixel_size_t gridSize = align(sizeof(cixel_s16) * GRID_SIZE);

    // Buffer for quantization
    cixel_size_t yuvSize = align(sizeof(Color) * width * height);

    // Buffer for only quantization
    cixel_size_t freqSize = align(sizeof(cixel_u32) * FREQUENCY_SIZE);
    cixel_size_t accSize = align(sizeof(Color32) * FREQUENCY_SIZE);
    cixel_size_t bucketSize = align(sizeof(Bucket) * (MAX_COLORS * 2));

    // Buffer for only error diffution
    cixel_size_t errorSize = align(sizeof(ColorS32) * (width + 2) * (height + 1));

    // Buffer for writing sixel
    cixel_s32 sixelHeight = ((height + 5) / 6);
    cixel_size_t writeBufferSize = align((MAX_COLORS * 18 + 1) + (width + 5) * MAX_COLORS * sixelHeight + sizeof(header) + sizeof(footer));
    cixel_size_t indicesFlagsSize = align(sizeof(cixel_u8) * width * MAX_COLORS);
    cixel_size_t colorUsedSize = align(MAX_COLORS);
    cixel_size_t palletIndicesSize = align(MAX_COLORS);

    cixel_size_t palletSize = colorSize + gridSize;
    cixel_size_t quantizationSize = palletSize + yuvSize + freqSize + accSize + bucketSize;
    cixel_size_t diffusionSize = palletSize + yuvSize + errorSize;
    cixel_size_t writingSixelSize = palletSize + writeBufferSize + indicesFlagsSize + colorUsedSize + palletIndicesSize;

    cixel_size_t cixelSize = align(sizeof(Cixel));
    cixel_size_t totalSize = cixelSize + maximum(maximum(quantizationSize, diffusionSize), writingSixelSize);

    Cixel* cixel = CIXEL_REINTERPRET_CAST(Cixel*)(allocFunc(totalSize + ALIGN_SIZE));
    cixel->allocFunc_ = allocFunc;
    cixel->freeFunc_ = freeFunc;
    cixel->width_ = width;
    cixel->height_ = height;

    uintptr_t ptr = (CIXEL_REINTERPRET_CAST(uintptr_t)(cixel) + cixelSize + ALIGN_OFFSET) & ALIGN_MASK;
    cixel_u8* work = CIXEL_REINTERPRET_CAST(cixel_u8*)(ptr);

    cixel->colors_ = CIXEL_REINTERPRET_CAST(Color*)(work);
    cixel->grid_ = CIXEL_REINTERPRET_CAST(cixel_s16*)(work + colorSize);

    cixel->yuv_ = CIXEL_REINTERPRET_CAST(Color*)(work + palletSize);

    cixel->frequencies_ = CIXEL_REINTERPRET_CAST(cixel_u32*)(work + palletSize + yuvSize);
    cixel->accColors_ = CIXEL_REINTERPRET_CAST(Color32*)(work + palletSize + yuvSize + freqSize);
    cixel->boxes_ = CIXEL_REINTERPRET_CAST(Bucket*)(work + palletSize + yuvSize + freqSize + accSize);

    cixel->errors_ = CIXEL_REINTERPRET_CAST(ColorS32*)(work + palletSize + yuvSize);

    cixel->writeBuffer_ = CIXEL_REINTERPRET_CAST(cixel_u8*)(work + palletSize);
    cixel->indicesFlags_ = CIXEL_REINTERPRET_CAST(cixel_u8*)(work + palletSize + writeBufferSize);
    cixel->colorFlags_ = CIXEL_REINTERPRET_CAST(cixel_u32*)(work + palletSize + writeBufferSize + indicesFlagsSize);
    cixel->palletIndices_ = CIXEL_REINTERPRET_CAST(cixel_u8*)(work + palletSize + writeBufferSize + indicesFlagsSize + colorUsedSize);

    return cixel;
}

void cixelDestroy(Cixel* cixel)
{
    if(CIXEL_NULL == cixel){
        return;
    }
    cixel->freeFunc_(cixel);
}

void cixelQuantize(Cixel* cixel, cixel_u8* CIXEL_RESTRICT indices, const cixel_u32* CIXEL_RESTRICT pixels, bool flipVertical)
{
    CIXEL_ASSERT(CIXEL_NULL != cixel);
    CIXEL_ASSERT(CIXEL_NULL != pixels);
    CIXEL_ASSERT(0 <= cixel->width_);
    CIXEL_ASSERT(0 <= cixel->height_);
#if defined(CIXEL_SSE)
#ifdef __cplusplus
    setZero16<align16(sizeof(cixel_u32) * FREQUENCY_SIZE)>(cixel->frequencies_);
    setZero16<align16(sizeof(Color32) * FREQUENCY_SIZE)>(cixel->accColors_);
    setMinusOne16<align16(sizeof(cixel_s16) * GRID_SIZE)>(cixel->grid_);
#else
    setZero16(cixel->frequencies_, align16(sizeof(cixel_u32) * FREQUENCY_SIZE));
    setZero16(cixel->accColors_, align16(sizeof(Color32) * FREQUENCY_SIZE));
    setMinusOne16(cixel->grid_, align16(sizeof(cixel_s16) * GRID_SIZE));
#endif

#else
    memset(cixel->frequencies_, 0, sizeof(cixel_u32) * FREQUENCY_SIZE);
    memset(cixel->accColors_, 0, sizeof(Color32) * FREQUENCY_SIZE);
    memset(cixel->grid_, -1, sizeof(cixel_s16) * GRID_SIZE);
#endif
    Bucket* buckets = cixel->boxes_;
#ifdef __cplusplus
    buckets[0] = {
        {
            {CIXEL_STATIC_CAST(cixel_u8)(RESOLUTION_Y - 1), CIXEL_STATIC_CAST(cixel_u8)(RESOLUTION_U - 1), CIXEL_STATIC_CAST(cixel_u8)(RESOLUTION_V - 1), 0},
            {}
        },
        0
    };
#else
    buckets[0] = (Bucket){
        {
            {CIXEL_STATIC_CAST(cixel_u8)(RESOLUTION_Y - 1), CIXEL_STATIC_CAST(cixel_u8)(RESOLUTION_U - 1), CIXEL_STATIC_CAST(cixel_u8)(RESOLUTION_V - 1), 0},
            {0, 0, 0, 0},
        },
        0
    };
#endif

    cixel_s32 size = cixel->width_ * cixel->height_;
    if(flipVertical){
        cixel_s32 d = 0;
        cixel_s32 s = (cixel->height_-1)*cixel->width_;
        for(cixel_s32 i = 0; i<cixel->height_; ++i) {
            for(cixel_s32 j = 0; j < cixel->width_; ++j) {
                BoxU8* box = &buckets[0].box_;
                cixel_s32 d0 = d+j;
                cixel->yuv_[d0].color_ = cixelRGB2YUV(pixels[s+j]);

                cixel_s32 qr = cixel->yuv_[d0].rgba_.r_ >> SHIFT_Y;
                cixel_s32 qg = cixel->yuv_[d0].rgba_.g_ >> SHIFT_U;
                cixel_s32 qb = cixel->yuv_[d0].rgba_.b_ >> SHIFT_V;
                cixel_s32 index = (qr + 1) * UV_PLANE_SIZE + (qg + 1) * V_SIZE + qb + 1;
                cixel->frequencies_[index] += 1;

                cixel->accColors_[index].r_ += cixel->yuv_[d0].rgba_.r_;
                cixel->accColors_[index].g_ += cixel->yuv_[d0].rgba_.g_;
                cixel->accColors_[index].b_ += cixel->yuv_[d0].rgba_.b_;

                cixel_u8 r8 = CIXEL_STATIC_CAST(cixel_u8)(qr);
                cixel_u8 g8 = CIXEL_STATIC_CAST(cixel_u8)(qg);
                cixel_u8 b8 = CIXEL_STATIC_CAST(cixel_u8)(qb);

                box->start_.x_ = minimum(box->start_.x_, r8);
                box->start_.y_ = minimum(box->start_.y_, g8);
                box->start_.z_ = minimum(box->start_.z_, b8);

                box->end_.x_ = maximum(box->end_.x_, r8);
                box->end_.y_ = maximum(box->end_.y_, g8);
                box->end_.z_ = maximum(box->end_.z_, b8);
            }
            d += cixel->width_;
            s -= cixel->width_;
        }
    } else {
        for(cixel_s32 i = 0; i < size; ++i) {
            BoxU8* box = &buckets[0].box_;
            cixel->yuv_[i].color_ = cixelRGB2YUV(pixels[i]);

            cixel_s32 qr = cixel->yuv_[i].rgba_.r_ >> SHIFT_Y;
            cixel_s32 qg = cixel->yuv_[i].rgba_.g_ >> SHIFT_U;
            cixel_s32 qb = cixel->yuv_[i].rgba_.b_ >> SHIFT_V;
            cixel_s32 index = (qr + 1) * UV_PLANE_SIZE + (qg + 1) * V_SIZE + qb + 1;
            cixel->frequencies_[index] += 1;

            cixel->accColors_[index].r_ += cixel->yuv_[i].rgba_.r_;
            cixel->accColors_[index].g_ += cixel->yuv_[i].rgba_.g_;
            cixel->accColors_[index].b_ += cixel->yuv_[i].rgba_.b_;

            cixel_u8 r8 = CIXEL_STATIC_CAST(cixel_u8)(qr);
            cixel_u8 g8 = CIXEL_STATIC_CAST(cixel_u8)(qg);
            cixel_u8 b8 = CIXEL_STATIC_CAST(cixel_u8)(qb);

            box->start_.x_ = minimum(box->start_.x_, r8);
            box->start_.y_ = minimum(box->start_.y_, g8);
            box->start_.z_ = minimum(box->start_.z_, b8);

            box->end_.x_ = maximum(box->end_.x_, r8);
            box->end_.y_ = maximum(box->end_.y_, g8);
            box->end_.z_ = maximum(box->end_.z_, b8);
        }
    }

    calcPrefixSum(cixel);
    cixel->boxes_[0].frequency_ = getSum(cixel, &(cixel->boxes_[0].box_));

    cixel_s32 ncolors = 256;
    cixel_s32 candidate = 0;
    cixel_s32 numBoxes = 1;
    for(; candidate < numBoxes && numBoxes < ncolors;) {
        bool success = medianCut(cixel, &buckets[candidate], &buckets[numBoxes], &buckets[candidate]);
        if(success) {
            sortToUpper(candidate, numBoxes + 1, cixel->boxes_);
            sortToLower(candidate, numBoxes, cixel->boxes_);
            ++numBoxes;
        } else {
            ++candidate;
        }
    }

    cixel->size_ = 0;
    Color color;
    for(cixel_s32 i = 0; i < numBoxes; ++i) {
        if(!calcCenterColor(cixel, &color, &buckets[i].box_)) {
            continue;
        }
        add(cixel, color, &buckets[i].box_);
    }

#ifdef _DEBUG
    // validate
    for(cixel_s32 i = 0; i < numBoxes; ++i) {
        validate(cixel->boxes_[i].box_);
    }

    // check overlaps
    for(cixel_s32 i = 0; i < cixel->size_; ++i) {
        for(cixel_s32 j = i + 1; j < cixel->size_; ++j) {
            overlap(cixel->boxes_[i].box_, cixel->boxes_[j].box_);
        }
    }
#endif
    errorDiffusion(cixel, indices);
}

void cixelPrint(Cixel* cixel, FILE* file, const cixel_u8* CIXEL_RESTRICT indices)
{
    CIXEL_ASSERT(CIXEL_NULL != cixel);
    CIXEL_ASSERT(CIXEL_NULL != file);
    CIXEL_ASSERT(CIXEL_NULL != indices);

    cixel_s32 width = cixel->width_;
    cixel_s32 height = cixel->height_;
    cixel_s32 size = cixel->size_;

    const Color* colors = cixel->colors_;
    cixel_u8* writeBuffer = cixel->writeBuffer_;
    cixel_u8* indicesFlags = cixel->indicesFlags_;
    cixel_u32* colorFlags = cixel->colorFlags_;
    cixel_u8* palletIndices = cixel->palletIndices_;

    cixel_s32 outHeight = ((height + 5) / 6) * 6;
    cixel_s32 pos = 0;
    pos = cixelWrite(pos, writeBuffer, sizeof(header), header);
    // Write a pallet
    for(cixel_s32 i = 0; i < size; ++i) {
        cixel_s32 rgba[4];
        YUV2RGBPercent(rgba, colors[i].color_);
        pos = writePalletColor(pos, writeBuffer, i, rgba[0], rgba[1], rgba[2]);
    }

#if defined(CIXEL_SSE)
    setZero16(indicesFlags, sizeof(cixel_u8) * width * MAX_COLORS);
#else
    memset(indicesFlags, 0, sizeof(cixel_u8) * width * MAX_COLORS);
#endif

    cixel_s32 block = width * 6;
    cixel_s32 row = 0;
    for(cixel_s32 i = 0; i < outHeight; i += 6, row += block) {
        for(cixel_s32 j=0; j<(MAX_COLORS/32); ++j){
            colorFlags[j] = 0;
        }
        cixel_s32 hblock = minimum(6, height - i);
        cixel_s32 colorCount = 0;
        for(cixel_s32 j = 0, trow0 = row; j < hblock; ++j, trow0 += width) {
            for(cixel_s32 k = 0; k < width; ++k) {
                cixel_u8 color = indices[trow0 + k];
                cixel_u8 flagBlock = color>>5;
                cixel_u32 flag = 0x01U<<(color&31U);
                if(0 == (colorFlags[flagBlock]&flag)){
                    colorFlags[flagBlock] |= flag;
                    palletIndices[colorCount] = color;
                    ++colorCount;
                }
                indicesFlags[width * color + k] |= (0x01U << j);
            }
        }

        for(cixel_s32 j = 0; j < colorCount; ++j) {
            if(0 < j) {
                pos = put(pos, writeBuffer, '$');
            }
            cixel_u8 color = palletIndices[j];
            cixel_s32 colorWidth = width * color;
            pos = writeColorIndex(pos, writeBuffer, color);

            cixel_s32 run = 0;
            cixel_u8 prevBits = 0xFFU;
            for(cixel_s32 k = 0; k < width; ++k) {
                cixel_u8 bits = indicesFlags[colorWidth + k];
                indicesFlags[colorWidth + k] = 0;
                CIXEL_ASSERT(bits <= 0x3FU);
                if(prevBits != bits) {
                    if(0 < run) {
                        pos = writeBits(pos, writeBuffer, run, prevBits);
                        run = 0;
                    }
                }
                prevBits = bits;
                ++run;
                if(256 <= run) {
                    pos = writeBits(pos, writeBuffer, 255, bits);
                    run -= 255;
                }
            }
            if(0 < run) {
                pos = writeBits(pos, writeBuffer, run, prevBits);
            }
        }

        pos = put(pos, writeBuffer, '-'); // graphics new line '-'
    }
    pos = cixelWrite(pos, writeBuffer, sizeof(footer), footer);
#ifdef _DEBUG
    {
        cixel_s32 sixelHeight = ((height + 5) / 6);
        cixel_size_t writeBufferSize =
            align((256 * 18 + 1) + (width + 5) * 256 * sixelHeight + sizeof(header) + sizeof(footer));
        CIXEL_ASSERT(pos <= writeBufferSize);
    }
#endif
    fwrite(writeBuffer, pos, 1, file);
}

Color cixelGetPalletColor(const Cixel* cixel, cixel_s32 index)
{
    return cixel->colors_[index];
}

#if 0
void Sixel::calcPrefixSum()
{
    cixel_s32 row0 = 0;
    for(cixel_s32 i = 1; i <= RESOLUTION_Y; ++i) {
        cixel_s32 row1 = row0 + UV_PLANE_SIZE;
        cixel_s32 col0 = 0;
        for(cixel_s32 j = 1; j <= RESOLUTION_U; ++j) {
            cixel_s32 col1 = col0 + V_SIZE;
            cixel_s32 dep0 = 0;
            for(cixel_s32 k = 1; k <= RESOLUTION_V; ++k) {
                cixel_s32 dep1 = k;

                cixel_s32 index = row1 + col1 + dep1;
                CIXEL_ASSERT(0 <= index && index < FREQUENCY_SIZE);
                CIXEL_ASSERT(0 <= (row0 + col0 + dep0) && (row0 + col0 + dep0) < FREQUENCY_SIZE);

                frequencies_[index] += frequencies_[row0 + col0 + dep0];

                frequencies_[index] += frequencies_[row0 + col1 + dep1];
                frequencies_[index] += frequencies_[row1 + col0 + dep1];
                frequencies_[index] += frequencies_[row1 + col1 + dep0];

                frequencies_[index] -= frequencies_[row0 + col0 + dep1];
                frequencies_[index] -= frequencies_[row0 + col1 + dep0];
                frequencies_[index] -= frequencies_[row1 + col0 + dep0];

                accColors_[index] += accColors_[row0 + col0 + dep0];

                accColors_[index] += accColors_[row0 + col1 + dep1];
                accColors_[index] += accColors_[row1 + col0 + dep1];
                accColors_[index] += accColors_[row1 + col1 + dep0];

                accColors_[index] -= accColors_[row0 + col0 + dep1];
                accColors_[index] -= accColors_[row0 + col1 + dep0];
                accColors_[index] -= accColors_[row1 + col0 + dep0];

                dep0 = dep1;
            }
            col0 = col1;
        }
        row0 = row1;
    }
}

cixel_u32 Sixel::getSum(const BoxU8& box) const
{
    CIXEL_ASSERT(box.start_.x_ <= box.end_.x_);
    CIXEL_ASSERT(box.start_.y_ <= box.end_.y_);
    CIXEL_ASSERT(box.start_.z_ <= box.end_.z_);

    cixel_s32 r0 = box.start_.x_ * UV_PLANE_SIZE;
    cixel_s32 r1 = (box.end_.x_ + 1) * UV_PLANE_SIZE;

    cixel_s32 g0 = box.start_.y_ * V_SIZE;
    cixel_s32 g1 = (box.end_.y_ + 1) * V_SIZE;

    cixel_s32 b0 = box.start_.z_;
    cixel_s32 b1 = box.end_.z_ + 1;

    cixel_u32 c = frequencies_[r1 + g1 + b1];
    c += frequencies_[r0 + g0 + b1];
    c += frequencies_[r0 + g1 + b0];
    c += frequencies_[r1 + g0 + b0];

    c -= frequencies_[r0 + g1 + b1];
    c -= frequencies_[r1 + g0 + b1];
    c -= frequencies_[r1 + g1 + b0];
    c -= frequencies_[r0 + g0 + b0];
    return c;
}

void Sixel::getSumRGB(cixel_u32& count, Color32& rgb, const BoxU8& box) const
{
    CIXEL_ASSERT(box.start_.x_ <= box.end_.x_);
    CIXEL_ASSERT(box.start_.y_ <= box.end_.y_);
    CIXEL_ASSERT(box.start_.z_ <= box.end_.z_);

    cixel_s32 r0 = box.start_.x_ * UV_PLANE_SIZE;
    cixel_s32 r1 = (box.end_.x_ + 1) * UV_PLANE_SIZE;

    cixel_s32 g0 = box.start_.y_ * V_SIZE;
    cixel_s32 g1 = (box.end_.y_ + 1) * V_SIZE;

    cixel_s32 b0 = box.start_.z_;
    cixel_s32 b1 = box.end_.z_ + 1;

    count = frequencies_[r1 + g1 + b1];
    count += frequencies_[r0 + g0 + b1];
    count += frequencies_[r0 + g1 + b0];
    count += frequencies_[r1 + g0 + b0];

    count -= frequencies_[r0 + g1 + b1];
    count -= frequencies_[r1 + g0 + b1];
    count -= frequencies_[r1 + g1 + b0];
    count -= frequencies_[r0 + g0 + b0];

    rgb = accColors_[r1 + g1 + b1];
    rgb += accColors_[r0 + g0 + b1];
    rgb += accColors_[r0 + g1 + b0];
    rgb += accColors_[r1 + g0 + b0];

    rgb -= accColors_[r0 + g1 + b1];
    rgb -= accColors_[r1 + g0 + b1];
    rgb -= accColors_[r1 + g1 + b0];
    rgb -= accColors_[r0 + g0 + b0];
}

cixel_u32 Sixel::calcSquaredDistance(
    cixel_u32 count0, const Color32& rgb0,
    cixel_u32 count1, const Color32& rgb1, const Color32& rgb)
{
    cixel_s64 total = count0 + count1;

    cixel_s64 r0 = CIXEL_STATIC_CAST(cixel_s64)(rgb0.r_) - rgb.r_*count0;
    cixel_s64 g0 = CIXEL_STATIC_CAST(cixel_s64)(rgb0.g_) - rgb.g_*count0;
    cixel_s64 b0 = CIXEL_STATIC_CAST(cixel_s64)(rgb0.b_) - rgb.b_*count0;

    cixel_s64 r1 = CIXEL_STATIC_CAST(cixel_s64)(rgb1.r_) - rgb.r_*count1;
    cixel_s64 g1 = CIXEL_STATIC_CAST(cixel_s64)(rgb1.g_) - rgb.g_*count1;
    cixel_s64 b1 = CIXEL_STATIC_CAST(cixel_s64)(rgb1.b_) - rgb.b_*count1;

    cixel_s64 c0 = total*count0;
    cixel_s64 c1 = total*count1;

    cixel_s64 d0 = (r0*r0 + g0*g0 + b0*b0)<<4;
    cixel_s64 d1 = (r1*r1 + g1*g1 + b1*b1)<<4;
    if(0<c1){
        d0 /= c1;
    }
    if(0<c0){
        d1 /= c0;
    }
    return static_cast<cixel_u32>(d0+d1);
}

void Sixel::sortToUpper(cixel_s32 index, cixel_s32 size, Bucket* buckets)
{
    for(cixel_s32 i = index + 1; i < size; ++i) {
        if(buckets[i].frequency_ <= buckets[i-1].frequency_) {
            return;
        }
        Bucket tmp = buckets[i];
        buckets[i] = buckets[i-1];
        buckets[i-1] = tmp;
    }
}

void Sixel::sortToLower(cixel_s32 start, cixel_s32 index, Bucket* buckets)
{
    for(cixel_s32 i = index; start < i; --i) {
        if(buckets[i].frequency_ <= buckets[i-1].frequency_) {
            return;
        }
        Bucket tmp = buckets[i];
        buckets[i] = buckets[i-1];
        buckets[i-1] = tmp;
    }
}

bool Sixel::medianCut(Bucket& bucket0, Bucket& bucket1, const Bucket& src)
{
    BoxU8 box = src.box_;
    CIXEL_ASSERT(box.start_.x_ <= box.end_.x_);
    CIXEL_ASSERT(box.start_.y_ <= box.end_.y_);
    CIXEL_ASSERT(box.start_.z_ <= box.end_.z_);

    cixel_u32 count;
    Color32 rgb;
    getSumRGB(count, rgb, box);
    if(0 < count) {
        rgb.r_ /= count;
        rgb.g_ /= count;
        rgb.b_ /= count;
    }

    cixel_u16 mid[3];
    mid[0] = (static_cast<cixel_u16>(box.end_.x_) + box.start_.x_) >> 1;
    mid[1] = (static_cast<cixel_u16>(box.end_.y_) + box.start_.y_) >> 1;
    mid[2] = (static_cast<cixel_u16>(box.end_.z_) + box.start_.z_) >> 1;
    cixel_s32 axis;
    cixel_u32 sqrDistances;

    Color32 rgb0;
    Color32 rgb1;

    BoxU8 b0;
    cixel_u32 count0;
    BoxU8 b1;
    cixel_u32 count1;
    {
        b0 = box;
        b0.start_.x_ = box.start_.x_;
        b0.end_.x_ = mid[0];
        getSumRGB(count0, rgb0, b0);

        b1 = box;
        b1.start_.x_ = mid[0];
        b1.end_.x_ = b1.end_.x_;
        getSumRGB(count1, rgb1, b1);

        axis = 0;
        sqrDistances = calcSquaredDistance(count0, rgb0, count1, rgb1, rgb);
    }

    {
        b0 = box;
        b0.start_.y_ = box.start_.y_;
        b0.end_.y_ = mid[1];
        getSumRGB(count0, rgb0, b0);

        b1 = box;
        b1.start_.y_ = mid[1];
        b1.end_.y_ = box.end_.y_;
        getSumRGB(count1, rgb1, b1);

        cixel_u32 tsqrDistances = calcSquaredDistance(count0, rgb0, count1, rgb1, rgb);
        if(sqrDistances < tsqrDistances) {
            sqrDistances = tsqrDistances;
            axis = 1;
        }
    }

    {
        b0 = box;
        b0.start_.z_ = box.start_.z_;
        b0.end_.z_ = mid[2];
        getSumRGB(count0, rgb0, b0);

        b1 = box;
        b1.start_.z_ = mid[2];
        b1.end_.z_ = box.end_.z_;
        getSumRGB(count1, rgb1, b1);

        cixel_u32 tsqrDistances = calcSquaredDistance(count0, rgb0, count1, rgb1, rgb);
        if(sqrDistances < tsqrDistances) {
            sqrDistances = tsqrDistances;
            axis = 2;
        }
    }
    cixel_u16 bstart = box.start_[axis];
    cixel_u16 bend = box.end_[axis];
    if(bstart == bend) {
        return false;
    }
    cixel_u8 split0 = CIXEL_STATIC_CAST(cixel_u8)((bstart + bend) >> 1);
    cixel_u8 split1 = split0 + 1;
    switch(axis) {
    case 0:
        bucket0.box_ = {{box.start_.x_, box.start_.y_, box.start_.z_, 0}, {split0, box.end_.y_, box.end_.z_, 0}};
        bucket1.box_ = {{split1, box.start_.y_, box.start_.z_, 0}, {box.end_.x_, box.end_.y_, box.end_.z_, 0}};
        break;
    case 1:
        bucket0.box_ = {{box.start_.x_, box.start_.y_, box.start_.z_, 0}, {box.end_.x_, split0, box.end_.z_, 0}};
        bucket1.box_ = {{box.start_.x_, split1, box.start_.z_, 0}, {box.end_.x_, box.end_.y_, box.end_.z_, 0}};
        break;
    case 2:
        bucket0.box_ = {{box.start_.x_, box.start_.y_, box.start_.z_, 0}, {box.end_.x_, box.end_.y_, split0, 0}};
        bucket1.box_ = {{box.start_.x_, box.start_.y_, split1, 0}, {box.end_.x_, box.end_.y_, box.end_.z_, 0}};
        break;
    default:
        CIXEL_ASSERT(false);
        break;
    }
    bucket0.frequency_ = getSum(bucket0.box_);
    bucket1.frequency_ = getSum(bucket1.box_);

    return true;
} // medianCut

bool Sixel::calcCenterColor(Color& color, const BoxU8& box) const
{
    cixel_u32 count;
    Color32 rgb;
    getSumRGB(count, rgb, box);

    if(0 < count) {
        cixel_u32 r = ((rgb.r_ << 1) / count + 1) >> 1;
        cixel_u32 g = ((rgb.g_ << 1) / count + 1) >> 1;
        cixel_u32 b = ((rgb.b_ << 1) / count + 1) >> 1;

        color.rgba_.r_ = toU8(r);
        color.rgba_.g_ = toU8(g);
        color.rgba_.b_ = toU8(b);
        color.rgba_.a_ = 0xFFU;
        return true;
    } else {
        return false;
    }
}

cixel_s32 Sixel::size() const
{
    return size_;
}

Color Sixel::operator[](cixel_s32 index) const
{
    CIXEL_ASSERT(0 <= index && index < size_);
    return colors_[index];
}

void Sixel::quantize(cixel_u8* CIXEL_RESTRICT indices, const cixel_u32* CIXEL_RESTRICT pixels)
{
    CIXEL_ASSERT(CIXEL_NULL != pixels);
    CIXEL_ASSERT(0 <= width_);
    CIXEL_ASSERT(0 <= height_);
#if defined(CIXEL_SSE)
    setZero16<align16(sizeof(cixel_u32) * FREQUENCY_SIZE)>(frequencies_);
    setZero16<align16(sizeof(Color32) * FREQUENCY_SIZE)>(accColors_);
    setMinusOne16<align16(sizeof(cixel_s16) * GRID_SIZE)>(grid_);

#else
    memset(frequencies_, 0, sizeof(cixel_u32) * FREQUENCY_SIZE);
    memset(accColors_, 0, sizeof(Color32) * FREQUENCY_SIZE);
    memset(grid_, -1, sizeof(cixel_s16) * GRID_SIZE);
#endif
    boxes_[0] = {
        {{CIXEL_STATIC_CAST(cixel_u8)(RESOLUTION_Y - 1), CIXEL_STATIC_CAST(cixel_u8)(RESOLUTION_U - 1),
          CIXEL_STATIC_CAST(cixel_u8)(RESOLUTION_V - 1), 0},
         {}}};
    cixel_s32 size = width_ * height_;
    for(cixel_s32 i = 0; i < size; ++i) {
        BoxU8& box = boxes_[0].box_;
        yuv_[i].color_ = cixelRGB2YUV(pixels[i]);

        cixel_s32 qr = yuv_[i].rgba_.r_ >> SHIFT_Y;
        cixel_s32 qg = yuv_[i].rgba_.g_ >> SHIFT_U;
        cixel_s32 qb = yuv_[i].rgba_.b_ >> SHIFT_V;
        cixel_s32 index = (qr + 1) * UV_PLANE_SIZE + (qg + 1) * V_SIZE + qb + 1;
        frequencies_[index] += 1;

        accColors_[index].r_ += yuv_[i].rgba_.r_;
        accColors_[index].g_ += yuv_[i].rgba_.g_;
        accColors_[index].b_ += yuv_[i].rgba_.b_;

        cixel_u8 r8 = CIXEL_STATIC_CAST(cixel_u8)(qr);
        cixel_u8 g8 = CIXEL_STATIC_CAST(cixel_u8)(qg);
        cixel_u8 b8 = CIXEL_STATIC_CAST(cixel_u8)(qb);

        box.start_.x_ = minimum(box.start_.x_, r8);
        box.start_.y_ = minimum(box.start_.y_, g8);
        box.start_.z_ = minimum(box.start_.z_, b8);

        box.end_.x_ = maximum(box.end_.x_, r8);
        box.end_.y_ = maximum(box.end_.y_, g8);
        box.end_.z_ = maximum(box.end_.z_, b8);
    }

    calcPrefixSum();
    boxes_[0].frequency_ = getSum(boxes_[0].box_);

    cixel_s32 ncolors = 256;
    cixel_s32 candidate = 0;
    cixel_s32 numBoxes = 1;
    for(; candidate < numBoxes && numBoxes < ncolors;) {
        bool success = medianCut(boxes_[candidate], boxes_[numBoxes], boxes_[candidate]);
        if(success) {
            sortToUpper(candidate, numBoxes + 1, boxes_);
            sortToLower(candidate, numBoxes, boxes_);
            ++numBoxes;
        } else {
            ++candidate;
        }
    }

    size_ = 0;
    Color color;
    for(cixel_s32 i = 0; i < numBoxes; ++i) {
        if(!calcCenterColor(color, boxes_[i].box_)) {
            continue;
        }
        add(color, boxes_[i].box_);
    }

#ifdef _DEBUG
    // validate
    for(cixel_s32 i = 0; i < numBoxes; ++i) {
        validate(boxes_[i].box_);
    }

    // check overlaps
    for(cixel_s32 i = 0; i < size_; ++i) {
        for(cixel_s32 j = i + 1; j < size_; ++j) {
            overlap(boxes_[i].box_, boxes_[j].box_);
        }
    }
#endif
    errorDiffusion(indices);
}

void Sixel::add(const Color& color, const BoxU8& box)
{
    CIXEL_ASSERT(size_ < MAX_COLORS);
    colors_[size_] = color;

    cixel_u8 us = CIXEL_STATIC_CAST(cixel_u8)(size_);
    for(cixel_s32 r = box.start_.x_; r <= box.end_.x_; ++r) {
        cixel_s32 tr = r << GRID_SHIFT_Y;
        for(cixel_s32 g = box.start_.y_; g <= box.end_.y_; ++g) {
            cixel_s32 tg = tr + (g << GRID_SHIFT_U);
            for(cixel_s32 b = box.start_.z_; b <= box.end_.z_; ++b) {
                grid_[tg + b] = us;
            }
        }
    }
    ++size_;
}

#if defined(CIXEL_SSE)
//-----------------------------------------------------------
void Sixel::diffuseRight(cixel_u8* CIXEL_RESTRICT indices, cixel_s32 width2, cixel_s32 y,
                         const __m128i& zero, const __m128i& c255,
                         const __m128i& cK12YUV655, const __m128i& cK20YUV655,
                         const __m128i& cK21YUV655, const __m128i& cK22YUV655)
{
    CIXEL_ALIGN(16)
    cixel_s32 tmp[4];

    cixel_s32 index0 = y * width_;
    cixel_s32 index1 = y * width2 + 1;
    for(cixel_s32 j = 0; j < width_; ++j, ++index0, ++index1) {
        __m128i i0 = _mm_cvtsi32_si128(*((cixel_s32*)&yuv_[index0]));
        i0 = _mm_unpacklo_epi8(i0, zero);
        i0 = _mm_unpacklo_epi16(i0, zero);

        __m128i error = _mm_load_si128((const __m128i*)&errors_[index1]);
        error = _mm_add_epi32(error, _mm_slli_epi32(i0, 4));
        error = _mm_srai_epi32(error, 4);
        error = _mm_max_epi32(error, zero);
        error = _mm_min_epi32(error, c255);
        _mm_store_si128((__m128i*)tmp, error);

        cixel_s32 index = ((tmp[0] >> SHIFT_Y) << GRID_SHIFT_Y) + ((tmp[1] >> SHIFT_U) << GRID_SHIFT_U) + (tmp[2] >> SHIFT_V);
        if(0 <= grid_[index]) {
            indices[index0] = CIXEL_STATIC_CAST(cixel_u8)(grid_[index]);

            __m128i t0 = _mm_cvtsi32_si128(*((cixel_s32*)&colors_[indices[index0]]));
            t0 = _mm_unpacklo_epi8(t0, zero);
            t0 = _mm_unpacklo_epi16(t0, zero);

            error = _mm_sub_epi32(i0, t0);

            muladdDiffustion(errors_[index1 - 1], cK12YUV655, error);
            muladdDiffustion(errors_[index1 + width2 - 1], cK22YUV655, error);
            muladdDiffustion(errors_[index1 + width2], cK21YUV655, error);
            muladdDiffustion(errors_[index1 + width2 + 1], cK20YUV655, error);

        } else {
            _mm_store_si128((__m128i*)tmp, i0);
            index = ((tmp[0] >> SHIFT_Y) << GRID_SHIFT_Y) + ((tmp[1] >> SHIFT_U) << GRID_SHIFT_U) + (tmp[2] >> SHIFT_V);
            indices[index0] = CIXEL_STATIC_CAST(cixel_u8)(grid_[index]);
            CIXEL_ASSERT(0 <= grid_[index]);
        }
    } // for(cixel_s32 j=0
}

//-----------------------------------------------------------
void Sixel::diffuseLeft(cixel_u8* CIXEL_RESTRICT indices, cixel_s32 width2, cixel_s32 y,
                        const __m128i& zero, const __m128i& c255,
                        const __m128i& cK12YUV655, const __m128i& cK20YUV655,
                        const __m128i& cK21YUV655, const __m128i& cK22YUV655)
{
    CIXEL_ALIGN(16)
    cixel_s32 tmp[4];

    cixel_s32 index0 = y * width_ + width_ - 1;
    cixel_s32 index1 = y * width2 + width_;
    for(cixel_s32 j = width_; 1 <= j; --j, --index0, --index1) {
        __m128i i0 = _mm_cvtsi32_si128(*((cixel_s32*)&yuv_[index0]));
        i0 = _mm_unpacklo_epi8(i0, zero);
        i0 = _mm_unpacklo_epi16(i0, zero);

        __m128i error = _mm_load_si128((const __m128i*)&errors_[index1]);
        error = _mm_add_epi32(error, _mm_slli_epi32(i0, 4));
        error = _mm_srai_epi32(error, 4);
        error = _mm_max_epi32(error, zero);
        error = _mm_min_epi32(error, c255);
        _mm_store_si128((__m128i*)tmp, error);

        cixel_s32 index = ((tmp[0] >> SHIFT_Y) << GRID_SHIFT_Y) + ((tmp[1] >> SHIFT_U) << GRID_SHIFT_U) + (tmp[2] >> SHIFT_V);
        if(0 <= grid_[index]) {
            indices[index0] = CIXEL_STATIC_CAST(cixel_u8)(grid_[index]);

            __m128i t0 = _mm_cvtsi32_si128(*((cixel_s32*)&colors_[indices[index0]]));
            t0 = _mm_unpacklo_epi8(t0, zero);
            t0 = _mm_unpacklo_epi16(t0, zero);

            error = _mm_sub_epi32(i0, t0);

            muladdDiffustion(errors_[index1 - 1], cK12YUV655, error);
            muladdDiffustion(errors_[index1 + width2 - 1], cK20YUV655, error);
            muladdDiffustion(errors_[index1 + width2], cK21YUV655, error);
            muladdDiffustion(errors_[index1 + width2 + 1], cK22YUV655, error);

        } else {
            _mm_store_si128((__m128i*)tmp, i0);
            index = ((tmp[0] >> SHIFT_Y) << GRID_SHIFT_Y) + ((tmp[1] >> SHIFT_U) << GRID_SHIFT_U) + (tmp[2] >> SHIFT_V);
            indices[index0] = CIXEL_STATIC_CAST(cixel_u8)(grid_[index]);
            CIXEL_ASSERT(0 <= grid_[index]);
        }
    } // for(cixel_s32 j=width
}

#else
//-----------------------------------------------------------
void Sixel::diffuseRight(cixel_u8* CIXEL_RESTRICT indices, cixel_s32 width2, cixel_s32 y)
{
    cixel_s32 index0 = y * width_;
    cixel_s32 index1 = y * width2 + 1;
    for(cixel_s32 j = 0; j < width_; ++j, ++index0, ++index1) {
        cixel_u8 r = yuv_[index0].rgba_.r_;
        cixel_u8 g = yuv_[index0].rgba_.g_;
        cixel_u8 b = yuv_[index0].rgba_.b_;
        cixel_s32 sr = CIXEL_STATIC_CAST(cixel_s32)(r);
        cixel_s32 sg = CIXEL_STATIC_CAST(cixel_s32)(g);
        cixel_s32 sb = CIXEL_STATIC_CAST(cixel_s32)(b);
        cixel_s32 ey = errors_[index1].r_ + (sr << 4);
        cixel_s32 eu = errors_[index1].g_ + (sg << 4);
        cixel_s32 ev = errors_[index1].b_ + (sb << 4);
        cixel_s32 ty = clamp(ey >> 4, 0, 255);
        cixel_s32 tu = clamp(eu >> 4, 0, 255);
        cixel_s32 tv = clamp(ev >> 4, 0, 255);

        cixel_s32 index = ((ty >> SHIFT_Y) << GRID_SHIFT_Y) + ((tu >> SHIFT_U) << GRID_SHIFT_U) + (tv >> SHIFT_V);
        if(0 <= grid_[index]) {
            indices[index0] = CIXEL_STATIC_CAST(cixel_u8)(grid_[index]);
            ty = colors_[indices[index0]].rgba_.r_;
            tu = colors_[indices[index0]].rgba_.g_;
            tv = colors_[indices[index0]].rgba_.b_;

            cixel_s32 error[4];
            error[0] = sr - ty;
            error[1] = sg - tu;
            error[2] = sb - tv;

            muladdDiffusion(errors_[index1 - 1], K12YUV655, error);
            muladdDiffusion(errors_[index1 + width2 - 1], K22YUV655, error);
            muladdDiffusion(errors_[index1 + width2], K21YUV655, error);
            muladdDiffusion(errors_[index1 + width2 + 1], K20YUV655, error);

        } else {
            sr >>= SHIFT_Y;
            sg >>= SHIFT_U;
            sb >>= SHIFT_V;
            index = (sr << GRID_SHIFT_Y) + (sg << GRID_SHIFT_U) + sb;
            indices[index0] = CIXEL_STATIC_CAST(cixel_u8)(grid_[index]);
            CIXEL_ASSERT(0 <= grid_[index]);
        }
    } // for(cixel_s32 j=0
}

//-----------------------------------------------------------
void Sixel::diffuseLeft(cixel_u8* CIXEL_RESTRICT indices, cixel_s32 width2, cixel_s32 y)
{
    cixel_s32 index0 = y * width_ + width_ - 1;
    cixel_s32 index1 = y * width2 + width_;
    for(cixel_s32 j = width_; 1 <= j; --j, --index0, --index1) {
        cixel_u8 r = yuv_[index0].rgba_.r_;
        cixel_u8 g = yuv_[index0].rgba_.g_;
        cixel_u8 b = yuv_[index0].rgba_.b_;
        cixel_s32 sr = CIXEL_STATIC_CAST(cixel_s32)(r);
        cixel_s32 sg = CIXEL_STATIC_CAST(cixel_s32)(g);
        cixel_s32 sb = CIXEL_STATIC_CAST(cixel_s32)(b);
        cixel_s32 ey = errors_[index1].r_ + (sr << 4);
        cixel_s32 eu = errors_[index1].g_ + (sg << 4);
        cixel_s32 ev = errors_[index1].b_ + (sb << 4);
        cixel_s32 ty = clamp(ey >> 4, 0, 255);
        cixel_s32 tu = clamp(eu >> 4, 0, 255);
        cixel_s32 tv = clamp(ev >> 4, 0, 255);

        cixel_s32 index = ((ty >> SHIFT_Y) << GRID_SHIFT_Y) + ((tu >> SHIFT_U) << GRID_SHIFT_U) + (tv >> SHIFT_V);
        if(0 <= grid_[index]) {
            indices[index0] = CIXEL_STATIC_CAST(cixel_u8)(grid_[index]);
            ty = colors_[indices[index0]].rgba_.r_;
            tu = colors_[indices[index0]].rgba_.g_;
            tv = colors_[indices[index0]].rgba_.b_;
            cixel_s32 error[4];
            error[0] = sr - ty;
            error[1] = sg - tu;
            error[2] = sb - tv;

            muladdDiffusion(errors_[index1 - 1], K12YUV655, error);
            muladdDiffusion(errors_[index1 + width2 - 1], K20YUV655, error);
            muladdDiffusion(errors_[index1 + width2], K21YUV655, error);
            muladdDiffusion(errors_[index1 + width2 + 1], K22YUV655, error);

        } else {
            sr >>= SHIFT_Y;
            sg >>= SHIFT_U;
            sb >>= SHIFT_V;
            index = (sr << GRID_SHIFT_Y) + (sg << GRID_SHIFT_U) + sb;
            indices[index0] = CIXEL_STATIC_CAST(cixel_u8)(grid_[index]);
            CIXEL_ASSERT(0 <= grid_[index]);
        }
    } // for(cixel_s32 j=width
}
#endif

//-----------------------------------------------------------
void Sixel::errorDiffusion(cixel_u8* CIXEL_RESTRICT indices)
{
    CIXEL_ASSERT(CIXEL_NULL != indices);
    cixel_s32 width2 = width_ + 2;
    cixel_s32 size = width2 * (height_ + 1);

#if defined(CIXEL_SSE)
    setZero16(errors_, sizeof(ColorS32) * size);
    __m128i zero = _mm_setzero_si128();
    __m128i c255 = _mm_set1_epi32(255);

    __m128i cK12YUV655 = _mm_set1_epi32(K12YUV655);
    __m128i cK20YUV655 = _mm_set1_epi32(K20YUV655);
    __m128i cK21YUV655 = _mm_set1_epi32(K21YUV655);
    __m128i cK22YUV655 = _mm_set1_epi32(K22YUV655);

    for(cixel_s32 i = 0; i < height_; ++i) {
        if(0 == (i & 0x01U)) {
            diffuseRight(indices, width2, i, zero, c255, cK12YUV655, cK20YUV655,
                         cK21YUV655, cK22YUV655);
        } else {
            diffuseLeft(indices, width2, i, zero, c255, cK12YUV655, cK20YUV655,
                        cK21YUV655, cK22YUV655);
        }
    }
#else
    memset(errors_, 0, sizeof(ColorS32) * size);

    for(cixel_s32 i = 0; i < height_; ++i) {
        if(0 == (i & 0x01U)) {
            diffuseRight(indices, width2, i);
        } else {
            diffuseLeft(indices, width2, i);
        }
    }
#endif
}

cixel_u32 cixelRGB2YUV(cixel_u32 rgba)
{
#if defined(CIXEL_SSE)
    static CIXEL_ALIGN(16) const cixel_f32 sr[4] = {0.299f, -0.169f, 0.500f, 1.0f};
    static CIXEL_ALIGN(16) const cixel_f32 sg[4] = {0.587f, -0.331f, -0.419f, 0.0f};
    static CIXEL_ALIGN(16) const cixel_f32 sb[4] = {0.114f, 0.500f, -0.081f, 0.0f};
    static CIXEL_ALIGN(16) const cixel_f32 so[4] = {0.0f, 128.0f, 128.0f, 0.0f};

    __m128i zero = _mm_setzero_si128();
    __m128i i0 = _mm_cvtsi32_si128(*((cixel_s32*)&rgba));
    i0 = _mm_unpacklo_epi8(i0, zero);
    i0 = _mm_unpacklo_epi16(i0, zero);

    __m128 f0 = _mm_cvtepi32_ps(i0);
    __m128 fr0 = _mm_shuffle_ps(f0, f0, _MM_SHUFFLE(3, 0, 0, 0));
    __m128 fg0 = _mm_shuffle_ps(f0, f0, _MM_SHUFFLE(3, 1, 1, 1));
    __m128 fb0 = _mm_shuffle_ps(f0, f0, _MM_SHUFFLE(3, 2, 2, 2));

    fr0 = _mm_mul_ps(fr0, _mm_load_ps(sr));
    fg0 = _mm_mul_ps(fg0, _mm_load_ps(sg));
    fb0 = _mm_mul_ps(fb0, _mm_load_ps(sb));
    f0 = _mm_add_ps(_mm_add_ps(fr0, fg0), _mm_add_ps(fb0, _mm_load_ps(so)));

    i0 = _mm_cvttps_epi32(f0);
    i0 = _mm_packs_epi32(i0, i0);
    i0 = _mm_packus_epi16(i0, i0);

    *((cixel_s32*)&rgba) = _mm_cvtsi128_si32(i0);
    return rgba;
#else
    static const cixel_s32 Base = 1 << 10;
    static const cixel_s32 Half = Base >> 1;
    static const cixel_s32 s00 = CIXEL_STATIC_CAST(cixel_s32)(0.299f * Base);
    static const cixel_s32 s01 = CIXEL_STATIC_CAST(cixel_s32)(0.587f * Base);
    static const cixel_s32 s02 = CIXEL_STATIC_CAST(cixel_s32)(0.114f * Base);

    static const cixel_s32 s10 = CIXEL_STATIC_CAST(cixel_s32)(-0.169f * Base);
    static const cixel_s32 s11 = CIXEL_STATIC_CAST(cixel_s32)(-0.331f * Base);
    static const cixel_s32 s12 = CIXEL_STATIC_CAST(cixel_s32)(0.500f * Base);

    static const cixel_s32 s20 = CIXEL_STATIC_CAST(cixel_s32)(0.500f * Base);
    static const cixel_s32 s21 = CIXEL_STATIC_CAST(cixel_s32)(-0.419f * Base);
    static const cixel_s32 s22 = CIXEL_STATIC_CAST(cixel_s32)(-0.081f * Base);

    cixel_u8 r = CIXEL_STATIC_CAST(cixel_u8)(rgba);
    cixel_u8 g = CIXEL_STATIC_CAST(cixel_u8)(rgba >> 8);
    cixel_u8 b = CIXEL_STATIC_CAST(cixel_u8)(rgba >> 16);
    cixel_s32 y = (s00 * r + s01 * g + s02 * b + Half) >> 10;
    cixel_s32 u = ((s10 * r + s11 * g + s12 * b + Half) >> 10) + 128;
    cixel_s32 v = ((s20 * r + s21 * g + s22 * b + Half) >> 10) + 128;

    rgba &= 0xFF000000U;
    rgba |= minimum(y, 255);
    rgba |= minimum(u, 255) << 8;
    rgba |= minimum(v, 255) << 16;
    return rgba;
#endif
}

cixel_u32 cixelYUV2RGB(cixel_u32 yuva)
{
#if defined(CIXEL_SSE)
    static CIXEL_ALIGN(16) const cixel_f32 sy[4] = {1.0f, 1.0f, 1.0f, 1.0f};
    static CIXEL_ALIGN(16) const cixel_f32 su[4] = {0.0f, -0.344f, 1.772f, 0.0f};
    static CIXEL_ALIGN(16) const cixel_f32 sv[4] = {1.402f, -0.714f, 0.0f, 0.0f};
    static CIXEL_ALIGN(16) const cixel_f32 so[4] = {0.0f, -128.0f, -128.0f, 0.0f};

    __m128i zero = _mm_setzero_si128();
    __m128i i0 = _mm_cvtsi32_si128(*((cixel_s32*)&yuva));
    i0 = _mm_unpacklo_epi8(i0, zero);
    i0 = _mm_unpacklo_epi16(i0, zero);

    __m128 f0 = _mm_cvtepi32_ps(i0);
    f0 = _mm_add_ps(f0, _mm_load_ps(so));
    __m128 fy0 = _mm_shuffle_ps(f0, f0, _MM_SHUFFLE(3, 0, 0, 0));
    __m128 fu0 = _mm_shuffle_ps(f0, f0, _MM_SHUFFLE(3, 1, 1, 1));
    __m128 fv0 = _mm_shuffle_ps(f0, f0, _MM_SHUFFLE(3, 2, 2, 2));

    fy0 = _mm_mul_ps(fy0, _mm_load_ps(sy));
    fu0 = _mm_mul_ps(fu0, _mm_load_ps(su));
    fv0 = _mm_mul_ps(fv0, _mm_load_ps(sv));
    f0 = _mm_add_ps(fy0, _mm_add_ps(fu0, fv0));

    i0 = _mm_cvttps_epi32(f0);
    i0 = _mm_packs_epi32(i0, i0);
    i0 = _mm_packus_epi16(i0, i0);

    *((cixel_s32*)&yuva) = _mm_cvtsi128_si32(i0);
    return yuva;

#else
    static const cixel_s32 Base = 1 << 10;
    static const cixel_s32 Half = Base >> 1;
    static const cixel_s32 s00 = CIXEL_STATIC_CAST(cixel_s32)(1.000f * Base);
    static const cixel_s32 s02 = CIXEL_STATIC_CAST(cixel_s32)(1.402f * Base);

    static const cixel_s32 s10 = CIXEL_STATIC_CAST(cixel_s32)(1.000f * Base);
    static const cixel_s32 s11 = CIXEL_STATIC_CAST(cixel_s32)(-0.344f * Base);
    static const cixel_s32 s12 = CIXEL_STATIC_CAST(cixel_s32)(-0.714f * Base);

    static const cixel_s32 s20 = CIXEL_STATIC_CAST(cixel_s32)(1.000f * Base);
    static const cixel_s32 s21 = CIXEL_STATIC_CAST(cixel_s32)(1.772f * Base);

    cixel_u8 y = CIXEL_STATIC_CAST(cixel_u8)(yuva);
    cixel_u8 u = CIXEL_STATIC_CAST(cixel_u8)(yuva >> 8);
    cixel_u8 v = CIXEL_STATIC_CAST(cixel_u8)(yuva >> 16);

    cixel_s32 su = CIXEL_STATIC_CAST(cixel_s32)(u) - 128;
    cixel_s32 sv = CIXEL_STATIC_CAST(cixel_s32)(v) - 128;
    cixel_s32 r = (s00 * y + s02 * sv + Half) >> 10;
    cixel_s32 g = (s10 * y + s11 * su + s12 * sv + Half) >> 10;
    cixel_s32 b = (s20 * y + s21 * su + Half) >> 10;
    r = clamp(r, 0, 255);
    g = clamp(g, 0, 255);
    b = clamp(b, 0, 255);

    yuva &= 0xFF000000U;
    yuva |= r;
    yuva |= g << 8;
    yuva |= b << 16;
    return yuva;
#endif
}

void YUV2RGBPercent(cixel_s32 rgba[4], cixel_u32 yuva)
{
    static const cixel_s32 Base = 1 << 10;
    static const cixel_s32 Half = Base >> 1;
    static const cixel_s32 s00 = CIXEL_STATIC_CAST(cixel_s32)(1.000f * Base);
    static const cixel_s32 s02 = CIXEL_STATIC_CAST(cixel_s32)(1.402f * Base);

    static const cixel_s32 s10 = CIXEL_STATIC_CAST(cixel_s32)(1.000f * Base);
    static const cixel_s32 s11 = CIXEL_STATIC_CAST(cixel_s32)(-0.344f * Base);
    static const cixel_s32 s12 = CIXEL_STATIC_CAST(cixel_s32)(-0.714f * Base);

    static const cixel_s32 s20 = CIXEL_STATIC_CAST(cixel_s32)(1.000f * Base);
    static const cixel_s32 s21 = CIXEL_STATIC_CAST(cixel_s32)(1.772f * Base);

    cixel_u8 y = CIXEL_STATIC_CAST(cixel_u8)(yuva);
    cixel_u8 u = CIXEL_STATIC_CAST(cixel_u8)(yuva >> 8);
    cixel_u8 v = CIXEL_STATIC_CAST(cixel_u8)(yuva >> 16);

    cixel_s32 su = CIXEL_STATIC_CAST(cixel_s32)(u) - 128;
    cixel_s32 sv = CIXEL_STATIC_CAST(cixel_s32)(v) - 128;
    cixel_s32& r = rgba[0];
    cixel_s32& g = rgba[1];
    cixel_s32& b = rgba[2];
    r = (s00 * y + s02 * sv + Half) >> 10;
    g = (s10 * y + s11 * su + s12 * sv + Half) >> 10;
    b = (s20 * y + s21 * su + Half) >> 10;
    r = clamp(r, 0, 255);
    g = clamp(g, 0, 255);
    b = clamp(b, 0, 255);

    r *= 100 * (1 << (10 - 8));
    g *= 100 * (1 << (10 - 8));
    b *= 100 * (1 << (10 - 8));

    r >>= 10;
    g >>= 10;
    b >>= 10;
    CIXEL_ASSERT(0 <= rgba[0] && rgba[0] <= 100);
    CIXEL_ASSERT(0 <= rgba[1] && rgba[1] <= 100);
    CIXEL_ASSERT(0 <= rgba[2] && rgba[2] <= 100);
}

namespace
{
    cixel_s32 writeNumber(cixel_s32 pos, cixel_u8* str, cixel_s32 number)
    {
        CIXEL_ASSERT(0 <= number && number < 1000);
        if(100 <= number) {
            cixel_s32 hundreds = number / 100;
            number -= 100 * hundreds;
            cixel_s32 tens = number / 10;
            number -= 10 * tens;
            cixel_s32 ones = number;
            str[pos + 0] = CIXEL_STATIC_CAST(cixel_u8)(hundreds + '0');
            str[pos + 1] = CIXEL_STATIC_CAST(cixel_u8)(tens + '0');
            str[pos + 2] = CIXEL_STATIC_CAST(cixel_u8)(ones + '0');
            return pos + 3;

        } else if(10 <= number) {
            cixel_s32 tens = number / 10;
            number -= 10 * tens;
            cixel_s32 ones = number;
            str[pos + 0] = CIXEL_STATIC_CAST(cixel_u8)(tens + '0');
            str[pos + 1] = CIXEL_STATIC_CAST(cixel_u8)(ones + '0');
            return pos + 2;

        } else {
            str[pos + 0] = CIXEL_STATIC_CAST(cixel_u8)(number + '0');
            return pos + 1;
        }
    }

    cixel_s32 write(cixel_s32 pos, cixel_u8* str, cixel_s32 n, const char* data)
    {
        for(cixel_s32 i = 0; i < n; ++i, ++pos) {
            str[pos] = data[i];
        }
        return pos;
    }

    inline cixel_s32 put(cixel_s32 pos, cixel_u8* str, char c)
    {
        str[pos] = CIXEL_STATIC_CAST(cixel_u8)(c);
        return pos + 1;
    }

    cixel_s32 writeColorIndex(cixel_s32 pos, cixel_u8* str, cixel_u8 index)
    {
        pos = put(pos, str, '#');
        pos = writeNumber(pos, str, index);
        return pos;
    }

    cixel_s32 writeBits(cixel_s32 pos, cixel_u8* str, cixel_s32 run, cixel_u8 bits)
    {
        CIXEL_ASSERT(0 < run && run <= 255);
        CIXEL_ASSERT(bits <= 63);
        bits += 63;
        if(3 < run) {
            pos = put(pos, str, '!'); // repeat
            pos = writeNumber(pos, str, run);
            pos = put(pos, str, bits);
            return pos;

        } else {
            for(cixel_s32 i = 0; i < run; ++i) {
                pos = put(pos, str, bits);
            }
            CIXEL_ASSERT(run <= 8);
            return pos;
        }
    }

    cixel_s32 writePalletColor(cixel_s32 pos, cixel_u8* str, cixel_s32 index, cixel_s32 r, cixel_s32 g, cixel_s32 b)
    {
        pos = put(pos, str, '#');
        pos = writeNumber(pos, str, index);
        pos = write(pos, str, 3, ";2;");
        pos = writeNumber(pos, str, r);
        pos = put(pos, str, ';');
        pos = writeNumber(pos, str, g);
        pos = put(pos, str, ';');
        pos = writeNumber(pos, str, b);
        return pos;
    }
} // namespace

void Sixel::print(FILE* file, const cixel_u8* CIXEL_RESTRICT indices) const
{
    CIXEL_ASSERT(CIXEL_NULL != file);
    CIXEL_ASSERT(CIXEL_NULL != indices);

    cixel_s32 outHeight = ((height_ + 5) / 6) * 6;
    cixel_s32 pos = 0;
    pos = write(pos, writeBuffer_, sizeof(header), header);
    // Write a pallet
    for(cixel_s32 i = 0; i < size_; ++i) {
        cixel_s32 rgba[4];
        YUV2RGBPercent(rgba, colors_[i].color_);
        pos = writePalletColor(pos, writeBuffer_, i, rgba[0], rgba[1], rgba[2]);
    }
#ifdef _DEBUG
    pos = put(pos, writeBuffer_, '\n');
#endif

#if defined(CIXEL_SSE)
    setZero16(indicesFlags_, sizeof(cixel_u8) * width_ * MAX_COLORS);
#else
    memset(indicesFlags_, 0, sizeof(cixel_u8) * width_ * MAX_COLORS);
#endif

    cixel_s32 block = width_ * 6;
    cixel_s32 row = 0;
    for(cixel_s32 i = 0; i < outHeight; i += 6, row += block) {
        for(cixel_s32 j=0; j<(MAX_COLORS/32); ++j){
            colorFlags_[j] = 0;
        }
        cixel_s32 hblock = minimum(6, height_ - i);
        cixel_s32 colorCount = 0;
        for(cixel_s32 j = 0, trow0 = row; j < hblock; ++j, trow0 += width_) {
            for(cixel_s32 k = 0; k < width_; ++k) {
                cixel_u8 color = indices[trow0 + k];
                cixel_u8 flagBlock = color>>5;
                cixel_u32 flag = 0x01U<<(color&31U);
                if(0 == (colorFlags_[flagBlock]&flag)){
                    colorFlags_[flagBlock] |= flag;
                    palletIndices_[colorCount] = color;
                    ++colorCount;
                }
                indicesFlags_[width_ * color + k] |= (0x01U << j);
            }
        }

        for(cixel_s32 j = 0; j < colorCount; ++j) {
            if(0 < j) {
                pos = put(pos, writeBuffer_, '$');
#ifdef _DEBUG
                pos = put(pos, writeBuffer_, '\n');
#endif
            }
            cixel_u8 color = palletIndices_[j];
            cixel_s32 width = width_ * color;
            pos = writeColorIndex(pos, writeBuffer_, color);

            cixel_s32 run = 0;
            cixel_u8 prevBits = 0xFFU;
            for(cixel_s32 k = 0; k < width_; ++k) {
                cixel_u8 bits = indicesFlags_[width + k];
                indicesFlags_[width + k] = 0;
                CIXEL_ASSERT(bits <= 0x3FU);
                if(prevBits != bits) {
                    if(0 < run) {
                        pos = writeBits(pos, writeBuffer_, run, prevBits);
                        run = 0;
                    }
                }
                prevBits = bits;
                ++run;
                if(256 <= run) {
                    pos = writeBits(pos, writeBuffer_, 255, bits);
                    run -= 255;
                }
            }
            if(0 < run) {
                pos = writeBits(pos, writeBuffer_, run, prevBits);
            }
        }

        pos = put(pos, writeBuffer_, '-'); // graphics new line '-'
#ifdef _DEBUG
        pos = put(pos, writeBuffer_, '\n');
#endif
    }
    pos = write(pos, writeBuffer_, sizeof(footer), footer);
#ifdef _DEBUG
    {
        cixel_s32 sixelHeight = ((height_ + 5) / 6);
        cixel_size_t writeBufferSize =
            align((256 * 18 + 1) + (width_ + 5) * 256 * sixelHeight + sizeof(header) + sizeof(footer));
        CIXEL_ASSERT(pos <= writeBufferSize);
    }
#endif
    fwrite(writeBuffer_, pos, 1, file);
}
#endif

CIXEL_NAMESPACE_END(cixel)
#endif // CIXEL_IMPLEMENTATION
