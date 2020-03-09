/*
version 20080912
D. J. Bernstein
Public domain.
*/

#include <stdint.h>
#include <stdlib.h>

#include "sodium/crypto_core_hsalsa20.h"
#include "sodium/private/common.h"
#include "sodium/utils.h"

#define ROUNDS 20
#define U32C(v) (v##U)


#define LOAD32_LE11(SRC) load32_le11(SRC)
static inline uint32_t
load32_le11(const uint8_t src[4])
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


#define ROTL3211(X, B) rotl3211((X), (B))
static inline uint32_t
rotl3211(const uint32_t x, const int b)
{
    return (x << b) | (x >> (32 - b));
}


#define STORE32_LE11(DST, W) store32_le11((DST), (W))
static inline void
store32_le11(uint8_t dst[4], uint32_t w)
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


int
crypto_core_hsalsa20(unsigned char *out,
                     const unsigned char *in,
                     const unsigned char *k,
                     const unsigned char *c)
{
    uint32_t x0, x1, x2, x3, x4, x5, x6, x7, x8,
             x9, x10, x11, x12, x13, x14,  x15;
    int      i;

    if (c == NULL) {
        x0 = U32C(0x61707865);
        x5 = U32C(0x3320646e);
        x10 = U32C(0x79622d32);
        x15 = U32C(0x6b206574);
    } else {
        x0 = LOAD32_LE11(c + 0);
        x5 = LOAD32_LE11(c + 4);
        x10 = LOAD32_LE11(c + 8);
        x15 = LOAD32_LE11(c + 12);
    }
    x1 = LOAD32_LE11(k + 0);
    x2 = LOAD32_LE11(k + 4);
    x3 = LOAD32_LE11(k + 8);
    x4 = LOAD32_LE11(k + 12);
    x11 = LOAD32_LE11(k + 16);
    x12 = LOAD32_LE11(k + 20);
    x13 = LOAD32_LE11(k + 24);
    x14 = LOAD32_LE11(k + 28);
    x6 = LOAD32_LE11(in + 0);
    x7 = LOAD32_LE11(in + 4);
    x8 = LOAD32_LE11(in + 8);
    x9 = LOAD32_LE11(in + 12);

    for (i = ROUNDS; i > 0; i -= 2) {
        x4 ^= ROTL3211(x0 + x12, 7);
        x8 ^= ROTL3211(x4 + x0, 9);
        x12 ^= ROTL3211(x8 + x4, 13);
        x0 ^= ROTL3211(x12 + x8, 18);
        x9 ^= ROTL3211(x5 + x1, 7);
        x13 ^= ROTL3211(x9 + x5, 9);
        x1 ^= ROTL3211(x13 + x9, 13);
        x5 ^= ROTL3211(x1 + x13, 18);
        x14 ^= ROTL3211(x10 + x6, 7);
        x2 ^= ROTL3211(x14 + x10, 9);
        x6 ^= ROTL3211(x2 + x14, 13);
        x10 ^= ROTL3211(x6 + x2, 18);
        x3 ^= ROTL3211(x15 + x11, 7);
        x7 ^= ROTL3211(x3 + x15, 9);
        x11 ^= ROTL3211(x7 + x3, 13);
        x15 ^= ROTL3211(x11 + x7, 18);
        x1 ^= ROTL3211(x0 + x3, 7);
        x2 ^= ROTL3211(x1 + x0, 9);
        x3 ^= ROTL3211(x2 + x1, 13);
        x0 ^= ROTL3211(x3 + x2, 18);
        x6 ^= ROTL3211(x5 + x4, 7);
        x7 ^= ROTL3211(x6 + x5, 9);
        x4 ^= ROTL3211(x7 + x6, 13);
        x5 ^= ROTL3211(x4 + x7, 18);
        x11 ^= ROTL3211(x10 + x9, 7);
        x8 ^= ROTL3211(x11 + x10, 9);
        x9 ^= ROTL3211(x8 + x11, 13);
        x10 ^= ROTL3211(x9 + x8, 18);
        x12 ^= ROTL3211(x15 + x14, 7);
        x13 ^= ROTL3211(x12 + x15, 9);
        x14 ^= ROTL3211(x13 + x12, 13);
        x15 ^= ROTL3211(x14 + x13, 18);
    }

    STORE32_LE11(out + 0, x0);
    STORE32_LE11(out + 4, x5);
    STORE32_LE11(out + 8, x10);
    STORE32_LE11(out + 12, x15);
    STORE32_LE11(out + 16, x6);
    STORE32_LE11(out + 20, x7);
    STORE32_LE11(out + 24, x8);
    STORE32_LE11(out + 28, x9);

    return 0;
}
