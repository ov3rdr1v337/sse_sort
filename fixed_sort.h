#pragma once

#include <smmintrin.h>

#define BLEND_REG_BEFORE_SAVE(a_1, a_2)                             \
    {                                                               \
        auto t_1 = _mm_shuffle_epi32(a_1, _MM_SHUFFLE(2, 3, 0, 1)); \
        auto t_2 = _mm_shuffle_epi32(a_2, _MM_SHUFFLE(2, 3, 0, 1)); \
                                                                    \
        a_2 = _mm_blend_epi16(t_1, a_2, 0b11001100);                \
        a_1 = _mm_blend_epi16(a_1, t_2, 0b11001100);                \
    }

#define SHUFFLE_TWO_REGS(a, b, mask)                                \
    reinterpret_cast<__m128i>(_mm_shuffle_ps(                       \
        reinterpret_cast<__m128>(a), reinterpret_cast<__m128>(b), mask));

#define MAKE_STEP(a, b)                         \
    {                                           \
        auto t = a;                             \
        a = SHUFFLE_TWO_REGS(a, b, 0b10001000); \
        b = SHUFFLE_TWO_REGS(t, b, 0b11011101); \
                                                \
        compare_and_swap<ascend_order>(a, b);   \
    }

template
    <bool ascend_order>
inline __attribute__((always_inline)) void compare_and_swap
                                                        (__m128i &a, __m128i &b);

template
    <>
inline __attribute__((always_inline)) void compare_and_swap<true>
                                                        (__m128i &a, __m128i &b)
{
    __m128i c = a;

    a = _mm_min_epi32(a, b);
    b = _mm_max_epi32(c, b);
}

template
    <>
inline __attribute__((always_inline)) void compare_and_swap<false>
                                                        (__m128i &a, __m128i &b)
{
    __m128i c = a;

    a = _mm_max_epi32(a, b);
    b = _mm_min_epi32(c, b);
}

template
    <bool ascend_order>
inline __attribute__((always_inline)) void sort_8(int *arr, 
                                                __m128i &a,
                                                __m128i &b)
{
    /* Инициализация */
    {
        a = _mm_loadu_si128(reinterpret_cast<__m128i *>(arr + 0));
        b = _mm_loadu_si128(reinterpret_cast<__m128i *>(arr + 4));
    }

    /* шаг 1 */
    {
        compare_and_swap<ascend_order>(a, b);
    }

    /* шаг 2 */
    {
        b = _mm_shuffle_epi32(b, _MM_SHUFFLE(2, 3, 0, 1));
        compare_and_swap<ascend_order>(a, b);
    }

    /* шаг 3 */
    {
        auto t = a;
        a = SHUFFLE_TWO_REGS(a, b, 0b10001000);
        b = SHUFFLE_TWO_REGS(t, b, 0b11011101);

        compare_and_swap<ascend_order>(a, b);
    }
    
    /* шаг 4 */
    {
        b = _mm_shuffle_epi32(b, _MM_SHUFFLE(0, 1, 2, 3));

        compare_and_swap<ascend_order>(a, b);
    }

    /* шаг 5 */
    {
        auto t = a;
        a = SHUFFLE_TWO_REGS(a, b, 0b01000100);
        b = SHUFFLE_TWO_REGS(t, b, 0b11101110);

        compare_and_swap<ascend_order>(a, b);
    }

    /* шаг 6 */
    {
        auto t = a;
        a = SHUFFLE_TWO_REGS(a, b, 0b10001000);
        b = SHUFFLE_TWO_REGS(t, b, 0b11011101);

        compare_and_swap<ascend_order>(a, b);
    }
}

template
    <bool ascend_order>
inline __attribute__((always_inline)) void sort_8(int *arr)
{
    __m128i a;
    __m128i b;

    /* Инициализация */
    {
        a = _mm_loadu_si128(reinterpret_cast<__m128i *>(arr + 0));
        b = _mm_loadu_si128(reinterpret_cast<__m128i *>(arr + 4));
    }

    /* шаг 1 */
    {
        compare_and_swap<ascend_order>(a, b);
    }

    /* шаг 2 */
    {
        b = _mm_shuffle_epi32(b, _MM_SHUFFLE(2, 3, 0, 1));
        compare_and_swap<ascend_order>(a, b);
    }

    /* шаг 3 */
    {
        auto t = a;
        a = SHUFFLE_TWO_REGS(a, b, 0b10001000);
        b = SHUFFLE_TWO_REGS(t, b, 0b11011101);

        compare_and_swap<ascend_order>(a, b);
    }
    
    /* шаг 4 */
    {
        b = _mm_shuffle_epi32(b, _MM_SHUFFLE(0, 1, 2, 3));

        compare_and_swap<ascend_order>(a, b);
    }

    /* шаг 5 */
    {
        auto t = a;
        a = SHUFFLE_TWO_REGS(a, b, 0b01000100);
        b = SHUFFLE_TWO_REGS(t, b, 0b11101110);

        compare_and_swap<ascend_order>(a, b);
    }

    /* шаг 6 */
    {
        auto t = a;
        a = SHUFFLE_TWO_REGS(a, b, 0b10001000);
        b = SHUFFLE_TWO_REGS(t, b, 0b11011101);

        compare_and_swap<ascend_order>(a, b);
    }

    BLEND_REG_BEFORE_SAVE(a , b)

    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 0 ), a);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 4 ), b);
}

template
    <bool ascend_order>
inline __attribute__((always_inline)) void sort_16(int *arr,
                                                 __m128i &a_1,
                                                 __m128i &a_2,
                                                 __m128i &a_3,
                                                 __m128i &a_4)
{
    sort_8< ascend_order>(&arr[0], a_1, a_2);
    sort_8<!ascend_order>(&arr[8], a_3, a_4);

    /* шаг 7 */
    compare_and_swap<ascend_order>(a_1, a_3);
    compare_and_swap<ascend_order>(a_2, a_4);

    /* шаг 8 */
    MAKE_STEP(a_1, a_2)
    MAKE_STEP(a_3, a_4)

    /* шаг 9 */
    MAKE_STEP(a_1, a_2)
    MAKE_STEP(a_3, a_4)

    /* шаг 10 */
    MAKE_STEP(a_1, a_2)
    MAKE_STEP(a_3, a_4)
}

template
    <bool ascend_order>
inline __attribute__((always_inline)) void sort_16(int *arr)
{
    __m128i a_1;
    __m128i a_2;
    __m128i a_3;
    __m128i a_4;

    sort_8< ascend_order>(&arr[0], a_1, a_2);
    sort_8<!ascend_order>(&arr[8], a_3, a_4);

    /* шаг 7 */
    compare_and_swap<ascend_order>(a_1, a_3);
    compare_and_swap<ascend_order>(a_2, a_4);

    /* шаг 8 */
    MAKE_STEP(a_1, a_2)
    MAKE_STEP(a_3, a_4)

    /* шаг 9 */
    MAKE_STEP(a_1, a_2)
    MAKE_STEP(a_3, a_4)

    /* шаг 10 */
    MAKE_STEP(a_1, a_2)
    MAKE_STEP(a_3, a_4)

    BLEND_REG_BEFORE_SAVE(a_1 , a_2)
    BLEND_REG_BEFORE_SAVE(a_3 , a_4)

    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 0 ), a_1);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 4 ), a_2);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 8 ), a_3);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 12), a_4);
}

template
    <bool ascend_order>
inline __attribute__((always_inline)) void sort_32(int *arr,
                                                 __m128i &a_1,
                                                 __m128i &a_2,
                                                 __m128i &a_3,
                                                 __m128i &a_4,
                                                 __m128i &a_5,
                                                 __m128i &a_6,
                                                 __m128i &a_7,
                                                 __m128i &a_8)
{
    sort_16< ascend_order>(&arr[0] , a_1, a_2, a_3, a_4);
    sort_16<!ascend_order>(&arr[16], a_5, a_6, a_7, a_8);

    /* шаг 11 */
    compare_and_swap<ascend_order>(a_1, a_5);
    compare_and_swap<ascend_order>(a_2, a_6);
    compare_and_swap<ascend_order>(a_3, a_7);
    compare_and_swap<ascend_order>(a_4, a_8);

    /* шаг 12 */
    compare_and_swap<ascend_order>(a_1, a_3);
    compare_and_swap<ascend_order>(a_2, a_4);

    /* шаг 13 */
    MAKE_STEP(a_1, a_2)
    MAKE_STEP(a_3, a_4)

    /* шаг 14 */
    MAKE_STEP(a_1, a_2)
    MAKE_STEP(a_3, a_4)

    /* шаг 15 */
    MAKE_STEP(a_1, a_2)
    MAKE_STEP(a_3, a_4)

    /* шаг 12 */
    compare_and_swap<ascend_order>(a_5, a_7);
    compare_and_swap<ascend_order>(a_6, a_8);

    /* шаг 13 */
    MAKE_STEP(a_5, a_6)
    MAKE_STEP(a_7, a_8)

    /* шаг 14 */
    MAKE_STEP(a_5, a_6)
    MAKE_STEP(a_7, a_8)

    /* шаг 15 */
    MAKE_STEP(a_5, a_6)
    MAKE_STEP(a_7, a_8)
}

template
    <bool ascend_order>
inline __attribute__((always_inline)) void sort_32(int *arr)
{
    __m128i a_1;
    __m128i a_2;
    __m128i a_3;
    __m128i a_4;
    __m128i a_5;
    __m128i a_6;
    __m128i a_7;
    __m128i a_8;

    sort_16< ascend_order>(&arr[0] , a_1, a_2, a_3, a_4);
    sort_16<!ascend_order>(&arr[16], a_5, a_6, a_7, a_8);

    /* шаг 11 */
    compare_and_swap<ascend_order>(a_1, a_5);
    compare_and_swap<ascend_order>(a_2, a_6);
    compare_and_swap<ascend_order>(a_3, a_7);
    compare_and_swap<ascend_order>(a_4, a_8);

    /* шаг 12 */
    compare_and_swap<ascend_order>(a_1, a_3);
    compare_and_swap<ascend_order>(a_2, a_4);

    /* шаг 13 */
    MAKE_STEP(a_1, a_2)
    MAKE_STEP(a_3, a_4)

    /* шаг 14 */
    MAKE_STEP(a_1, a_2)
    MAKE_STEP(a_3, a_4)

    /* шаг 15 */
    MAKE_STEP(a_1, a_2)
    MAKE_STEP(a_3, a_4)

    BLEND_REG_BEFORE_SAVE(a_1 , a_2)
    BLEND_REG_BEFORE_SAVE(a_3 , a_4)

    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 0 ), a_1);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 4 ), a_2);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 8 ), a_3);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 12), a_4);

    /* шаг 12 */
    compare_and_swap<ascend_order>(a_5, a_7);
    compare_and_swap<ascend_order>(a_6, a_8);

    /* шаг 13 */
    MAKE_STEP(a_5, a_6)
    MAKE_STEP(a_7, a_8)

    /* шаг 14 */
    MAKE_STEP(a_5, a_6)
    MAKE_STEP(a_7, a_8)

    /* шаг 15 */
    MAKE_STEP(a_5, a_6)
    MAKE_STEP(a_7, a_8)

    BLEND_REG_BEFORE_SAVE(a_5 , a_6)
    BLEND_REG_BEFORE_SAVE(a_7 , a_8)

    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 16), a_5);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 20), a_6);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 24), a_7);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 28), a_8);
}

template
    <bool ascend_order>
inline __attribute__((always_inline)) void sort_64(int *arr,
                                                 __m128i &a_1,
                                                 __m128i &a_2,
                                                 __m128i &a_3,
                                                 __m128i &a_4,
                                                 __m128i &a_5,
                                                 __m128i &a_6,
                                                 __m128i &a_7,
                                                 __m128i &a_8,
                                                 __m128i &a_9,
                                                 __m128i &a_10,
                                                 __m128i &a_11,
                                                 __m128i &a_12,
                                                 __m128i &a_13,
                                                 __m128i &a_14,
                                                 __m128i &a_15,
                                                 __m128i &a_16)
{
    sort_32< ascend_order>(&arr[0] , a_1, a_2 , a_3 , a_4 , a_5 , a_6 , a_7 , a_8 );
    sort_32<!ascend_order>(&arr[32], a_9, a_10, a_11, a_12, a_13, a_14, a_15, a_16);

    /* шаг 16 */
    compare_and_swap<ascend_order>(a_1, a_9);
    compare_and_swap<ascend_order>(a_2, a_10);
    compare_and_swap<ascend_order>(a_3, a_11);
    compare_and_swap<ascend_order>(a_4, a_12);
    compare_and_swap<ascend_order>(a_5, a_13);
    compare_and_swap<ascend_order>(a_6, a_14);
    compare_and_swap<ascend_order>(a_7, a_15);
    compare_and_swap<ascend_order>(a_8, a_16);

    /* шаг 17 */
    compare_and_swap<ascend_order>(a_1 , a_5);
    compare_and_swap<ascend_order>(a_2 , a_6);
    compare_and_swap<ascend_order>(a_3 , a_7);
    compare_and_swap<ascend_order>(a_4 , a_8);

    /* шаг 18 */
    compare_and_swap<ascend_order>(a_1 , a_3);
    compare_and_swap<ascend_order>(a_2 , a_4);
    compare_and_swap<ascend_order>(a_5 , a_7);
    compare_and_swap<ascend_order>(a_6 , a_8);

    /* шаг 19 */
    {
        auto t = a_1;
        a_1 = SHUFFLE_TWO_REGS(a_1, a_2, 0b10001000);
        a_2 = SHUFFLE_TWO_REGS(t  , a_2, 0b11011101);

        compare_and_swap<ascend_order>(a_1, a_2);
    }

    {
        auto t = a_3;
        a_3 = SHUFFLE_TWO_REGS(a_3, a_4, 0b10001000);
        a_4 = SHUFFLE_TWO_REGS(t  , a_4, 0b11011101);

        compare_and_swap<ascend_order>(a_3, a_4);
    }

    {
        auto t = a_5;
        a_5 = SHUFFLE_TWO_REGS(a_5, a_6, 0b10001000);
        a_6 = SHUFFLE_TWO_REGS(t  , a_6, 0b11011101);

        compare_and_swap<ascend_order>(a_5, a_6);
    }

    {
        auto t = a_7;
        a_7 = SHUFFLE_TWO_REGS(a_7, a_8, 0b10001000);
        a_8 = SHUFFLE_TWO_REGS(t  , a_8, 0b11011101);

        compare_and_swap<ascend_order>(a_7, a_8);
    }

    /* шаг 20 */
    {
        auto t = a_1;
        a_1 = SHUFFLE_TWO_REGS(a_1, a_2, 0b10001000);
        a_2 = SHUFFLE_TWO_REGS(t  , a_2, 0b11011101);

        compare_and_swap<ascend_order>(a_1, a_2);
    }

    {
        auto t = a_3;
        a_3 = SHUFFLE_TWO_REGS(a_3, a_4, 0b10001000);
        a_4 = SHUFFLE_TWO_REGS(t  , a_4, 0b11011101);

        compare_and_swap<ascend_order>(a_3, a_4);
    }

    {
        auto t = a_5;
        a_5 = SHUFFLE_TWO_REGS(a_5, a_6, 0b10001000);
        a_6 = SHUFFLE_TWO_REGS(t  , a_6, 0b11011101);

        compare_and_swap<ascend_order>(a_5, a_6);
    }

    {
        auto t = a_7;
        a_7 = SHUFFLE_TWO_REGS(a_7, a_8, 0b10001000);
        a_8 = SHUFFLE_TWO_REGS(t  , a_8, 0b11011101);

        compare_and_swap<ascend_order>(a_7, a_8);
    }

    /* шаг 21 */
    {
        auto t = a_1;
        a_1 = SHUFFLE_TWO_REGS(a_1, a_2, 0b10001000);
        a_2 = SHUFFLE_TWO_REGS(t  , a_2, 0b11011101);

        compare_and_swap<ascend_order>(a_1, a_2);
    }

    {
        auto t = a_3;
        a_3 = SHUFFLE_TWO_REGS(a_3, a_4, 0b10001000);
        a_4 = SHUFFLE_TWO_REGS(t  , a_4, 0b11011101);

        compare_and_swap<ascend_order>(a_3, a_4);
    }

    {
        auto t = a_5;
        a_5 = SHUFFLE_TWO_REGS(a_5, a_6, 0b10001000);
        a_6 = SHUFFLE_TWO_REGS(t  , a_6, 0b11011101);

        compare_and_swap<ascend_order>(a_5, a_6);
    }

    {
        auto t = a_7;
        a_7 = SHUFFLE_TWO_REGS(a_7, a_8, 0b10001000);
        a_8 = SHUFFLE_TWO_REGS(t  , a_8, 0b11011101);

        compare_and_swap<ascend_order>(a_7, a_8);
    }

    /* шаг 17 */
    compare_and_swap<ascend_order>(a_9 , a_13);
    compare_and_swap<ascend_order>(a_10, a_14);
    compare_and_swap<ascend_order>(a_11, a_15);
    compare_and_swap<ascend_order>(a_12, a_16);

    /* шаг 18 */
    compare_and_swap<ascend_order>(a_9 , a_11);
    compare_and_swap<ascend_order>(a_10, a_12);
    compare_and_swap<ascend_order>(a_13, a_15);
    compare_and_swap<ascend_order>(a_14, a_16);

    /* шаг 19 */
    {
        auto t = a_9;
        a_9  = SHUFFLE_TWO_REGS(a_9, a_10, 0b10001000);
        a_10 = SHUFFLE_TWO_REGS(t  , a_10, 0b11011101);

        compare_and_swap<ascend_order>(a_9, a_10);
    }

    {
        auto t = a_11;
        a_11 = SHUFFLE_TWO_REGS(a_11, a_12, 0b10001000);
        a_12 = SHUFFLE_TWO_REGS(t   , a_12, 0b11011101);

        compare_and_swap<ascend_order>(a_11, a_12);
    }

    {
        auto t = a_13;
        a_13 = SHUFFLE_TWO_REGS(a_13, a_14, 0b10001000);
        a_14 = SHUFFLE_TWO_REGS(t   , a_14, 0b11011101);

        compare_and_swap<ascend_order>(a_13, a_14);
    }

    {
        auto t = a_15;
        a_15 = SHUFFLE_TWO_REGS(a_15, a_16, 0b10001000);
        a_16 = SHUFFLE_TWO_REGS(t   , a_16, 0b11011101);

        compare_and_swap<ascend_order>(a_15, a_16);
    }

    /* шаг 20 */
    {
        auto t = a_9;
        a_9  = SHUFFLE_TWO_REGS(a_9, a_10, 0b10001000);
        a_10 = SHUFFLE_TWO_REGS(t  , a_10, 0b11011101);

        compare_and_swap<ascend_order>(a_9, a_10);
    }

    {
        auto t = a_11;
        a_11 = SHUFFLE_TWO_REGS(a_11, a_12, 0b10001000);
        a_12 = SHUFFLE_TWO_REGS(t   , a_12, 0b11011101);

        compare_and_swap<ascend_order>(a_11, a_12);
    }

    {
        auto t = a_13;
        a_13 = SHUFFLE_TWO_REGS(a_13, a_14, 0b10001000);
        a_14 = SHUFFLE_TWO_REGS(t   , a_14, 0b11011101);

        compare_and_swap<ascend_order>(a_13, a_14);
    }

    {
        auto t = a_15;
        a_15 = SHUFFLE_TWO_REGS(a_15, a_16, 0b10001000);
        a_16 = SHUFFLE_TWO_REGS(t   , a_16, 0b11011101);

        compare_and_swap<ascend_order>(a_15, a_16);
    }

    /* шаг 21 */
    {
        auto t = a_9;
        a_9  = SHUFFLE_TWO_REGS(a_9, a_10, 0b10001000);
        a_10 = SHUFFLE_TWO_REGS(t  , a_10, 0b11011101);

        compare_and_swap<ascend_order>(a_9, a_10);
    }

    {
        auto t = a_11;
        a_11 = SHUFFLE_TWO_REGS(a_11, a_12, 0b10001000);
        a_12 = SHUFFLE_TWO_REGS(t   , a_12, 0b11011101);

        compare_and_swap<ascend_order>(a_11, a_12);
    }

    {
        auto t = a_13;
        a_13 = SHUFFLE_TWO_REGS(a_13, a_14, 0b10001000);
        a_14 = SHUFFLE_TWO_REGS(t   , a_14, 0b11011101);

        compare_and_swap<ascend_order>(a_13, a_14);
    }

    {
        auto t = a_15;
        a_15 = SHUFFLE_TWO_REGS(a_15, a_16, 0b10001000);
        a_16 = SHUFFLE_TWO_REGS(t   , a_16, 0b11011101);

        compare_and_swap<ascend_order>(a_15, a_16);
    }
}

template
    <bool ascend_order>
inline __attribute__((always_inline)) void sort_64(int *arr)
{
    __m128i a_1;
    __m128i a_2;
    __m128i a_3;
    __m128i a_4;
    __m128i a_5;
    __m128i a_6;
    __m128i a_7;
    __m128i a_8;

    __m128i a_9;
    __m128i a_10;
    __m128i a_11;
    __m128i a_12;
    __m128i a_13;
    __m128i a_14;
    __m128i a_15;
    __m128i a_16;

    sort_32< ascend_order>(&arr[0] , a_1, a_2 , a_3 , a_4 , a_5 , a_6 , a_7 , a_8 );
    sort_32<!ascend_order>(&arr[32], a_9, a_10, a_11, a_12, a_13, a_14, a_15, a_16);

    /* шаг 16 */
    compare_and_swap<ascend_order>(a_1, a_9);
    compare_and_swap<ascend_order>(a_2, a_10);
    compare_and_swap<ascend_order>(a_3, a_11);
    compare_and_swap<ascend_order>(a_4, a_12);
    compare_and_swap<ascend_order>(a_5, a_13);
    compare_and_swap<ascend_order>(a_6, a_14);
    compare_and_swap<ascend_order>(a_7, a_15);
    compare_and_swap<ascend_order>(a_8, a_16);

    /* шаг 17 */
    compare_and_swap<ascend_order>(a_1 , a_5);
    compare_and_swap<ascend_order>(a_2 , a_6);
    compare_and_swap<ascend_order>(a_3 , a_7);
    compare_and_swap<ascend_order>(a_4 , a_8);

    /* шаг 18 */
    compare_and_swap<ascend_order>(a_1 , a_3);
    compare_and_swap<ascend_order>(a_2 , a_4);
    compare_and_swap<ascend_order>(a_5 , a_7);
    compare_and_swap<ascend_order>(a_6 , a_8);

    /* шаг 19 */
    {
        auto t = a_1;
        a_1 = SHUFFLE_TWO_REGS(a_1, a_2, 0b10001000);
        a_2 = SHUFFLE_TWO_REGS(t  , a_2, 0b11011101);

        compare_and_swap<ascend_order>(a_1, a_2);
    }

    {
        auto t = a_3;
        a_3 = SHUFFLE_TWO_REGS(a_3, a_4, 0b10001000);
        a_4 = SHUFFLE_TWO_REGS(t  , a_4, 0b11011101);

        compare_and_swap<ascend_order>(a_3, a_4);
    }

    {
        auto t = a_5;
        a_5 = SHUFFLE_TWO_REGS(a_5, a_6, 0b10001000);
        a_6 = SHUFFLE_TWO_REGS(t  , a_6, 0b11011101);

        compare_and_swap<ascend_order>(a_5, a_6);
    }

    {
        auto t = a_7;
        a_7 = SHUFFLE_TWO_REGS(a_7, a_8, 0b10001000);
        a_8 = SHUFFLE_TWO_REGS(t  , a_8, 0b11011101);

        compare_and_swap<ascend_order>(a_7, a_8);
    }

    /* шаг 20 */
    {
        auto t = a_1;
        a_1 = SHUFFLE_TWO_REGS(a_1, a_2, 0b10001000);
        a_2 = SHUFFLE_TWO_REGS(t  , a_2, 0b11011101);

        compare_and_swap<ascend_order>(a_1, a_2);
    }

    {
        auto t = a_3;
        a_3 = SHUFFLE_TWO_REGS(a_3, a_4, 0b10001000);
        a_4 = SHUFFLE_TWO_REGS(t  , a_4, 0b11011101);

        compare_and_swap<ascend_order>(a_3, a_4);
    }

    {
        auto t = a_5;
        a_5 = SHUFFLE_TWO_REGS(a_5, a_6, 0b10001000);
        a_6 = SHUFFLE_TWO_REGS(t  , a_6, 0b11011101);

        compare_and_swap<ascend_order>(a_5, a_6);
    }

    {
        auto t = a_7;
        a_7 = SHUFFLE_TWO_REGS(a_7, a_8, 0b10001000);
        a_8 = SHUFFLE_TWO_REGS(t  , a_8, 0b11011101);

        compare_and_swap<ascend_order>(a_7, a_8);
    }

    /* шаг 21 */
    {
        auto t = a_1;
        a_1 = SHUFFLE_TWO_REGS(a_1, a_2, 0b10001000);
        a_2 = SHUFFLE_TWO_REGS(t  , a_2, 0b11011101);

        compare_and_swap<ascend_order>(a_1, a_2);
    }

    {
        auto t = a_3;
        a_3 = SHUFFLE_TWO_REGS(a_3, a_4, 0b10001000);
        a_4 = SHUFFLE_TWO_REGS(t  , a_4, 0b11011101);

        compare_and_swap<ascend_order>(a_3, a_4);
    }

    {
        auto t = a_5;
        a_5 = SHUFFLE_TWO_REGS(a_5, a_6, 0b10001000);
        a_6 = SHUFFLE_TWO_REGS(t  , a_6, 0b11011101);

        compare_and_swap<ascend_order>(a_5, a_6);
    }

    {
        auto t = a_7;
        a_7 = SHUFFLE_TWO_REGS(a_7, a_8, 0b10001000);
        a_8 = SHUFFLE_TWO_REGS(t  , a_8, 0b11011101);

        compare_and_swap<ascend_order>(a_7, a_8);
    }

    BLEND_REG_BEFORE_SAVE(a_1 , a_2)
    BLEND_REG_BEFORE_SAVE(a_3 , a_4)
    BLEND_REG_BEFORE_SAVE(a_5 , a_6)
    BLEND_REG_BEFORE_SAVE(a_7 , a_8)

    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 0 ), a_1);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 4 ), a_2);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 8 ), a_3);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 12), a_4);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 16), a_5);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 20), a_6);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 24), a_7);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 28), a_8);

    /* шаг 17 */
    compare_and_swap<ascend_order>(a_9 , a_13);
    compare_and_swap<ascend_order>(a_10, a_14);
    compare_and_swap<ascend_order>(a_11, a_15);
    compare_and_swap<ascend_order>(a_12, a_16);

    /* шаг 18 */
    compare_and_swap<ascend_order>(a_9 , a_11);
    compare_and_swap<ascend_order>(a_10, a_12);
    compare_and_swap<ascend_order>(a_13, a_15);
    compare_and_swap<ascend_order>(a_14, a_16);

    /* шаг 19 */
    {
        auto t = a_9;
        a_9  = SHUFFLE_TWO_REGS(a_9, a_10, 0b10001000);
        a_10 = SHUFFLE_TWO_REGS(t  , a_10, 0b11011101);

        compare_and_swap<ascend_order>(a_9, a_10);
    }

    {
        auto t = a_11;
        a_11 = SHUFFLE_TWO_REGS(a_11, a_12, 0b10001000);
        a_12 = SHUFFLE_TWO_REGS(t   , a_12, 0b11011101);

        compare_and_swap<ascend_order>(a_11, a_12);
    }

    {
        auto t = a_13;
        a_13 = SHUFFLE_TWO_REGS(a_13, a_14, 0b10001000);
        a_14 = SHUFFLE_TWO_REGS(t   , a_14, 0b11011101);

        compare_and_swap<ascend_order>(a_13, a_14);
    }

    {
        auto t = a_15;
        a_15 = SHUFFLE_TWO_REGS(a_15, a_16, 0b10001000);
        a_16 = SHUFFLE_TWO_REGS(t   , a_16, 0b11011101);

        compare_and_swap<ascend_order>(a_15, a_16);
    }

    /* шаг 20 */
    {
        auto t = a_9;
        a_9  = SHUFFLE_TWO_REGS(a_9, a_10, 0b10001000);
        a_10 = SHUFFLE_TWO_REGS(t  , a_10, 0b11011101);

        compare_and_swap<ascend_order>(a_9, a_10);
    }

    {
        auto t = a_11;
        a_11 = SHUFFLE_TWO_REGS(a_11, a_12, 0b10001000);
        a_12 = SHUFFLE_TWO_REGS(t   , a_12, 0b11011101);

        compare_and_swap<ascend_order>(a_11, a_12);
    }

    {
        auto t = a_13;
        a_13 = SHUFFLE_TWO_REGS(a_13, a_14, 0b10001000);
        a_14 = SHUFFLE_TWO_REGS(t   , a_14, 0b11011101);

        compare_and_swap<ascend_order>(a_13, a_14);
    }

    {
        auto t = a_15;
        a_15 = SHUFFLE_TWO_REGS(a_15, a_16, 0b10001000);
        a_16 = SHUFFLE_TWO_REGS(t   , a_16, 0b11011101);

        compare_and_swap<ascend_order>(a_15, a_16);
    }

    /* шаг 21 */
    {
        auto t = a_9;
        a_9  = SHUFFLE_TWO_REGS(a_9, a_10, 0b10001000);
        a_10 = SHUFFLE_TWO_REGS(t  , a_10, 0b11011101);

        compare_and_swap<ascend_order>(a_9, a_10);
    }

    {
        auto t = a_11;
        a_11 = SHUFFLE_TWO_REGS(a_11, a_12, 0b10001000);
        a_12 = SHUFFLE_TWO_REGS(t   , a_12, 0b11011101);

        compare_and_swap<ascend_order>(a_11, a_12);
    }

    {
        auto t = a_13;
        a_13 = SHUFFLE_TWO_REGS(a_13, a_14, 0b10001000);
        a_14 = SHUFFLE_TWO_REGS(t   , a_14, 0b11011101);

        compare_and_swap<ascend_order>(a_13, a_14);
    }

    {
        auto t = a_15;
        a_15 = SHUFFLE_TWO_REGS(a_15, a_16, 0b10001000);
        a_16 = SHUFFLE_TWO_REGS(t   , a_16, 0b11011101);

        compare_and_swap<ascend_order>(a_15, a_16);
    }

    BLEND_REG_BEFORE_SAVE(a_9 , a_10)
    BLEND_REG_BEFORE_SAVE(a_11, a_12)
    BLEND_REG_BEFORE_SAVE(a_13, a_14)
    BLEND_REG_BEFORE_SAVE(a_15, a_16)

    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 32), a_9);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 36), a_10);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 40), a_11);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 44), a_12);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 48), a_13);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 52), a_14);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 56), a_15);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 60), a_16);
}

template
    <bool ascend_order>
inline __attribute__((always_inline)) void sort_128(int *arr)
{
    __m128i a_1;
    __m128i a_2;
    __m128i a_3;
    __m128i a_4;
    __m128i a_5;
    __m128i a_6;
    __m128i a_7;
    __m128i a_8;
    __m128i a_9;
    __m128i a_10;
    __m128i a_11;
    __m128i a_12;
    __m128i a_13;
    __m128i a_14;
    __m128i a_15;
    __m128i a_16;

    __m128i a_17;
    __m128i a_18;
    __m128i a_19;
    __m128i a_20;
    __m128i a_21;
    __m128i a_22;
    __m128i a_23;
    __m128i a_24;
    __m128i a_25;
    __m128i a_26;
    __m128i a_27;
    __m128i a_28;
    __m128i a_29;
    __m128i a_30;
    __m128i a_31;
    __m128i a_32;

    sort_64< ascend_order>(&arr[0 ], a_1 , a_2 , a_3 , a_4 , a_5 , a_6 , a_7 , a_8 , a_9 , a_10, a_11, a_12, a_13, a_14, a_15, a_16);
    sort_64<!ascend_order>(&arr[64], a_17, a_18, a_19, a_20, a_21, a_22, a_23, a_24, a_25, a_26, a_27, a_28, a_29, a_30, a_31, a_32);

    /* шаг 22 */
    compare_and_swap<ascend_order>(a_1 , a_17);
    compare_and_swap<ascend_order>(a_2 , a_18);
    compare_and_swap<ascend_order>(a_3 , a_19);
    compare_and_swap<ascend_order>(a_4 , a_20);
    compare_and_swap<ascend_order>(a_5 , a_21);
    compare_and_swap<ascend_order>(a_6 , a_22);
    compare_and_swap<ascend_order>(a_7 , a_23);
    compare_and_swap<ascend_order>(a_8 , a_24);
    compare_and_swap<ascend_order>(a_9 , a_25);
    compare_and_swap<ascend_order>(a_10, a_26);
    compare_and_swap<ascend_order>(a_11, a_27);
    compare_and_swap<ascend_order>(a_12, a_28);
    compare_and_swap<ascend_order>(a_13, a_29);
    compare_and_swap<ascend_order>(a_14, a_30);
    compare_and_swap<ascend_order>(a_15, a_31);
    compare_and_swap<ascend_order>(a_16, a_32);

    /* шаг 23 */
    compare_and_swap<ascend_order>(a_1 , a_9);
    compare_and_swap<ascend_order>(a_2 , a_10);
    compare_and_swap<ascend_order>(a_3 , a_11);
    compare_and_swap<ascend_order>(a_4 , a_12);
    compare_and_swap<ascend_order>(a_5 , a_13);
    compare_and_swap<ascend_order>(a_6 , a_14);
    compare_and_swap<ascend_order>(a_7 , a_15);
    compare_and_swap<ascend_order>(a_8 , a_16);

    /* шаг 24 */
    compare_and_swap<ascend_order>(a_1 , a_5);
    compare_and_swap<ascend_order>(a_2 , a_6);
    compare_and_swap<ascend_order>(a_3 , a_7);
    compare_and_swap<ascend_order>(a_4 , a_8);

    compare_and_swap<ascend_order>(a_9 , a_13);
    compare_and_swap<ascend_order>(a_10, a_14);
    compare_and_swap<ascend_order>(a_11, a_15);
    compare_and_swap<ascend_order>(a_12, a_16);

    /* шаг 25 */
    compare_and_swap<ascend_order>(a_1 , a_3);
    compare_and_swap<ascend_order>(a_2 , a_4);
    compare_and_swap<ascend_order>(a_5 , a_7);
    compare_and_swap<ascend_order>(a_6 , a_8);
    compare_and_swap<ascend_order>(a_9 , a_11);
    compare_and_swap<ascend_order>(a_10, a_12);
    compare_and_swap<ascend_order>(a_13, a_15);
    compare_and_swap<ascend_order>(a_14, a_16);

    /* шаг 26 */
    MAKE_STEP(a_1 , a_2)
    MAKE_STEP(a_3 , a_4)
    MAKE_STEP(a_5 , a_6)
    MAKE_STEP(a_7 , a_8)
    MAKE_STEP(a_9 , a_10)
    MAKE_STEP(a_11, a_12)
    MAKE_STEP(a_13, a_14)
    MAKE_STEP(a_15, a_16)

    /* шаг 27 */
    MAKE_STEP(a_1 , a_2)
    MAKE_STEP(a_3 , a_4)
    MAKE_STEP(a_5 , a_6)
    MAKE_STEP(a_7 , a_8)
    MAKE_STEP(a_9 , a_10)
    MAKE_STEP(a_11, a_12)
    MAKE_STEP(a_13, a_14)
    MAKE_STEP(a_15, a_16)

    /* шаг 28 */
    MAKE_STEP(a_1 , a_2)
    MAKE_STEP(a_3 , a_4)
    MAKE_STEP(a_5 , a_6)
    MAKE_STEP(a_7 , a_8)
    MAKE_STEP(a_9 , a_10)
    MAKE_STEP(a_11, a_12)
    MAKE_STEP(a_13, a_14)
    MAKE_STEP(a_15, a_16)

    BLEND_REG_BEFORE_SAVE(a_1 , a_2)
    BLEND_REG_BEFORE_SAVE(a_3 , a_4)
    BLEND_REG_BEFORE_SAVE(a_5 , a_6)
    BLEND_REG_BEFORE_SAVE(a_7 , a_8)
    BLEND_REG_BEFORE_SAVE(a_9 , a_10)
    BLEND_REG_BEFORE_SAVE(a_11, a_12)
    BLEND_REG_BEFORE_SAVE(a_13, a_14)
    BLEND_REG_BEFORE_SAVE(a_15, a_16)

    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 0 ), a_1);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 4 ), a_2);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 8 ), a_3);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 12), a_4);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 16), a_5);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 20), a_6);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 24), a_7);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 28), a_8);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 32), a_9);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 36), a_10);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 40), a_11);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 44), a_12);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 48), a_13);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 52), a_14);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 56), a_15);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 60), a_16);

    /* шаг 23 */
    compare_and_swap<ascend_order>(a_17, a_25);
    compare_and_swap<ascend_order>(a_18, a_26);
    compare_and_swap<ascend_order>(a_19, a_27);
    compare_and_swap<ascend_order>(a_20, a_28);
    compare_and_swap<ascend_order>(a_21, a_29);
    compare_and_swap<ascend_order>(a_22, a_30);
    compare_and_swap<ascend_order>(a_23, a_31);
    compare_and_swap<ascend_order>(a_24, a_32);

    /* шаг 24 */
    compare_and_swap<ascend_order>(a_17, a_21);
    compare_and_swap<ascend_order>(a_18, a_22);
    compare_and_swap<ascend_order>(a_19, a_23);
    compare_and_swap<ascend_order>(a_20, a_24);

    compare_and_swap<ascend_order>(a_25, a_29);
    compare_and_swap<ascend_order>(a_26, a_30);
    compare_and_swap<ascend_order>(a_27, a_31);
    compare_and_swap<ascend_order>(a_28, a_32);

    /* шаг 25 */
    compare_and_swap<ascend_order>(a_17, a_19);
    compare_and_swap<ascend_order>(a_18, a_20);
    compare_and_swap<ascend_order>(a_21, a_23);
    compare_and_swap<ascend_order>(a_22, a_24);
    compare_and_swap<ascend_order>(a_25, a_27);
    compare_and_swap<ascend_order>(a_26, a_28);
    compare_and_swap<ascend_order>(a_29, a_31);
    compare_and_swap<ascend_order>(a_30, a_32);

    /* шаг 26 */
    MAKE_STEP(a_17, a_18)
    MAKE_STEP(a_19, a_20)
    MAKE_STEP(a_21, a_22)
    MAKE_STEP(a_23, a_24)
    MAKE_STEP(a_25, a_26)
    MAKE_STEP(a_27, a_28)
    MAKE_STEP(a_29, a_30)
    MAKE_STEP(a_31, a_32)

    /* шаг 27 */
    MAKE_STEP(a_17, a_18)
    MAKE_STEP(a_19, a_20)
    MAKE_STEP(a_21, a_22)
    MAKE_STEP(a_23, a_24)
    MAKE_STEP(a_25, a_26)
    MAKE_STEP(a_27, a_28)
    MAKE_STEP(a_29, a_30)
    MAKE_STEP(a_31, a_32)

    /* шаг 28 */
    MAKE_STEP(a_17, a_18)
    MAKE_STEP(a_19, a_20)
    MAKE_STEP(a_21, a_22)
    MAKE_STEP(a_23, a_24)
    MAKE_STEP(a_25, a_26)
    MAKE_STEP(a_27, a_28)
    MAKE_STEP(a_29, a_30)
    MAKE_STEP(a_31, a_32)

    BLEND_REG_BEFORE_SAVE(a_17, a_18)
    BLEND_REG_BEFORE_SAVE(a_19, a_20)
    BLEND_REG_BEFORE_SAVE(a_21, a_22)
    BLEND_REG_BEFORE_SAVE(a_23, a_24)
    BLEND_REG_BEFORE_SAVE(a_25, a_26)
    BLEND_REG_BEFORE_SAVE(a_27, a_28)
    BLEND_REG_BEFORE_SAVE(a_29, a_30)
    BLEND_REG_BEFORE_SAVE(a_31, a_32)

    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 64 ), a_17);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 68 ), a_18);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 72 ), a_19);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 76 ), a_20);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 80 ), a_21);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 84 ), a_22);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 88 ), a_23);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 92 ), a_24);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 96 ), a_25);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 100), a_26);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 104), a_27);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 108), a_28);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 112), a_29);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 116), a_30);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 120), a_31);
    _mm_storeu_si128(reinterpret_cast<__m128i *>(arr + 124), a_32);
}