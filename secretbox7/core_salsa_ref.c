
#include <stdint.h>
#include <stdlib.h>

#include "sodium/crypto_core_salsa20.h"
#include "sodium/crypto_core_salsa2012.h"
#include "sodium/crypto_core_salsa208.h"
#include "sodium/private/common.h"


#define LOAD32_LE22(SRC) load32_le22(SRC)
static inline uint32_t
load32_le22(const uint8_t src[4])
{
#ifdef NATIVE_LITTLE_ENDIAN
    uint32_t w;
    memcpy(&w, src, sizeof w);
    return w;
#else
    uint32_t w = (uint32_t) src[0];
    w |= (uint32_t) src[1] <<  8;
    w |= (uint32_t) src[2] << 16;
    w |= (uint32_t) src[3] << 24;
    return w;
#endif
}


#define ROTL3222(X, B) rotl3222((X), (B))
static inline uint32_t
rotl3222(const uint32_t x, const int b)
{
    return (x << b) | (x >> (32 - b));
}


#define STORE32_LE22(DST, W) store32_le22((DST), (W))
static inline void
store32_le22(uint8_t dst[4], uint32_t w)
{
#ifdef NATIVE_LITTLE_ENDIAN
    memcpy(dst, &w, sizeof w);
#else
    dst[0] = (uint8_t) w; w >>= 8;
    dst[1] = (uint8_t) w; w >>= 8;
    dst[2] = (uint8_t) w; w >>= 8;
    dst[3] = (uint8_t) w;
#endif
}



static void
crypto_core_salsa(unsigned char *out, const unsigned char *in,
                  const unsigned char *k, const unsigned char *c,
                  const int rounds)
{
    uint32_t x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14,
        x15;
    uint32_t j0, j1, j2, j3, j4, j5, j6, j7, j8, j9, j10, j11, j12, j13, j14,
        j15;
    int i;

    j0  = x0  = 0x61707865;
    j5  = x5  = 0x3320646e;
    j10 = x10 = 0x79622d32;
    j15 = x15 = 0x6b206574;
    if (c != NULL) {
        j0  = x0  = LOAD32_LE22(c + 0);
        j5  = x5  = LOAD32_LE22(c + 4);
        j10 = x10 = LOAD32_LE22(c + 8);
        j15 = x15 = LOAD32_LE22(c + 12);
    }
    j1  = x1  = LOAD32_LE22(k + 0);
    j2  = x2  = LOAD32_LE22(k + 4);
    j3  = x3  = LOAD32_LE22(k + 8);
    j4  = x4  = LOAD32_LE22(k + 12);
    j11 = x11 = LOAD32_LE22(k + 16);
    j12 = x12 = LOAD32_LE22(k + 20);
    j13 = x13 = LOAD32_LE22(k + 24);
    j14 = x14 = LOAD32_LE22(k + 28);

    j6  = x6  = LOAD32_LE22(in + 0);
    j7  = x7  = LOAD32_LE22(in + 4);
    j8  = x8  = LOAD32_LE22(in + 8);
    j9  = x9  = LOAD32_LE22(in + 12);

    for (i = 0; i < rounds; i += 2) {
        x4  ^= ROTL3222(x0  + x12, 7);
        x8  ^= ROTL3222(x4  + x0, 9);
        x12 ^= ROTL3222(x8  + x4, 13);
        x0  ^= ROTL3222(x12 + x8, 18);
        x9  ^= ROTL3222(x5  + x1, 7);
        x13 ^= ROTL3222(x9  + x5, 9);
        x1  ^= ROTL3222(x13 + x9, 13);
        x5  ^= ROTL3222(x1  + x13, 18);
        x14 ^= ROTL3222(x10 + x6, 7);
        x2  ^= ROTL3222(x14 + x10, 9);
        x6  ^= ROTL3222(x2  + x14, 13);
        x10 ^= ROTL3222(x6  + x2, 18);
        x3  ^= ROTL3222(x15 + x11, 7);
        x7  ^= ROTL3222(x3  + x15, 9);
        x11 ^= ROTL3222(x7  + x3, 13);
        x15 ^= ROTL3222(x11 + x7, 18);
        x1  ^= ROTL3222(x0  + x3, 7);
        x2  ^= ROTL3222(x1  + x0, 9);
        x3  ^= ROTL3222(x2  + x1, 13);
        x0  ^= ROTL3222(x3  + x2, 18);
        x6  ^= ROTL3222(x5  + x4, 7);
        x7  ^= ROTL3222(x6  + x5, 9);
        x4  ^= ROTL3222(x7  + x6, 13);
        x5  ^= ROTL3222(x4  + x7, 18);
        x11 ^= ROTL3222(x10 + x9, 7);
        x8  ^= ROTL3222(x11 + x10, 9);
        x9  ^= ROTL3222(x8  + x11, 13);
        x10 ^= ROTL3222(x9  + x8, 18);
        x12 ^= ROTL3222(x15 + x14, 7);
        x13 ^= ROTL3222(x12 + x15, 9);
        x14 ^= ROTL3222(x13 + x12, 13);
        x15 ^= ROTL3222(x14 + x13, 18);
    }
    STORE32_LE22(out + 0,  x0  + j0);
    STORE32_LE22(out + 4,  x1  + j1);
    STORE32_LE22(out + 8,  x2  + j2);
    STORE32_LE22(out + 12, x3  + j3);
    STORE32_LE22(out + 16, x4  + j4);
    STORE32_LE22(out + 20, x5  + j5);
    STORE32_LE22(out + 24, x6  + j6);
    STORE32_LE22(out + 28, x7  + j7);
    STORE32_LE22(out + 32, x8  + j8);
    STORE32_LE22(out + 36, x9  + j9);
    STORE32_LE22(out + 40, x10 + j10);
    STORE32_LE22(out + 44, x11 + j11);
    STORE32_LE22(out + 48, x12 + j12);
    STORE32_LE22(out + 52, x13 + j13);
    STORE32_LE22(out + 56, x14 + j14);
    STORE32_LE22(out + 60, x15 + j15);
}

int
crypto_core_salsa20(unsigned char *out, const unsigned char *in,
                    const unsigned char *k, const unsigned char *c)
{
    crypto_core_salsa(out, in, k, c, 20);
    return 0;
}

size_t
crypto_core_salsa20_outputbytes(void)
{
    return crypto_core_salsa20_OUTPUTBYTES;
}

size_t
crypto_core_salsa20_inputbytes(void)
{
    return crypto_core_salsa20_INPUTBYTES;
}

size_t
crypto_core_salsa20_keybytes(void)
{
    return crypto_core_salsa20_KEYBYTES;
}

size_t
crypto_core_salsa20_constbytes(void)
{
    return crypto_core_salsa20_CONSTBYTES;
}

#ifndef MINIMAL
/* LCOV_EXCL_START */
int
crypto_core_salsa2012(unsigned char *out, const unsigned char *in,
                      const unsigned char *k, const unsigned char *c)
{
    crypto_core_salsa(out, in, k, c, 12);
    return 0;
}

size_t
crypto_core_salsa2012_outputbytes(void)
{
    return crypto_core_salsa2012_OUTPUTBYTES;
}

size_t
crypto_core_salsa2012_inputbytes(void)
{
    return crypto_core_salsa2012_INPUTBYTES;
}

size_t
crypto_core_salsa2012_keybytes(void)
{
    return crypto_core_salsa2012_KEYBYTES;
}

size_t
crypto_core_salsa2012_constbytes(void)
{
    return crypto_core_salsa2012_CONSTBYTES;
}

int
crypto_core_salsa208(unsigned char *out, const unsigned char *in,
                     const unsigned char *k, const unsigned char *c)
{
    crypto_core_salsa(out, in, k, c, 8);
    return 0;
}

size_t
crypto_core_salsa208_outputbytes(void)
{
    return crypto_core_salsa208_OUTPUTBYTES;
}

size_t
crypto_core_salsa208_inputbytes(void)
{
    return crypto_core_salsa208_INPUTBYTES;
}

size_t
crypto_core_salsa208_keybytes(void)
{
    return crypto_core_salsa208_KEYBYTES;
}

size_t
crypto_core_salsa208_constbytes(void)
{
    return crypto_core_salsa208_CONSTBYTES;
}
/* LCOV_EXCL_END */
#endif
