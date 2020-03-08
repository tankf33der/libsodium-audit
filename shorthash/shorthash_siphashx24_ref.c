//#include "crypto_shorthash_siphash24.h"
//#include "sodium/private/common.h"
//#include "shorthash_siphash_ref.h"

#include <stdint.h>


#define LOAD64_LE(SRC) load64_le(SRC)
static inline uint64_t
load64_le(const uint8_t src[8])
{
#ifdef NATIVE_LITTLE_ENDIAN
    uint64_t w;
    memcpy(&w, src, sizeof w);
    return w;
#else
    uint64_t w = (uint64_t) src[0];
    w |= (uint64_t) src[1] <<  8;
    w |= (uint64_t) src[2] << 16;
    w |= (uint64_t) src[3] << 24;
    w |= (uint64_t) src[4] << 32;
    w |= (uint64_t) src[5] << 40;
    w |= (uint64_t) src[6] << 48;
    w |= (uint64_t) src[7] << 56;
    return w;
#endif
}

#define STORE64_LE(DST, W) store64_le((DST), (W))
static inline void
store64_le(uint8_t dst[8], uint64_t w)
{
#ifdef NATIVE_LITTLE_ENDIAN
    memcpy(dst, &w, sizeof w);
#else
    dst[0] = (uint8_t) w; w >>= 8;
    dst[1] = (uint8_t) w; w >>= 8;
    dst[2] = (uint8_t) w; w >>= 8;
    dst[3] = (uint8_t) w; w >>= 8;
    dst[4] = (uint8_t) w; w >>= 8;
    dst[5] = (uint8_t) w; w >>= 8;
    dst[6] = (uint8_t) w; w >>= 8;
    dst[7] = (uint8_t) w;
#endif
}

#define SIPROUND             \
    do {                     \
        v0 += v1;            \
        v1 = ROTL64(v1, 13); \
        v1 ^= v0;            \
        v0 = ROTL64(v0, 32); \
        v2 += v3;            \
        v3 = ROTL64(v3, 16); \
        v3 ^= v2;            \
        v0 += v3;            \
        v3 = ROTL64(v3, 21); \
        v3 ^= v0;            \
        v2 += v1;            \
        v1 = ROTL64(v1, 17); \
        v1 ^= v2;            \
        v2 = ROTL64(v2, 32); \
    } while (0)

#define ROTL64(X, B) rotl64((X), (B))
static inline uint64_t
rotl64(const uint64_t x, const int b)
{
    return (x << b) | (x >> (64 - b));
}


int
crypto_shorthash_siphashx24(unsigned char *out, const unsigned char *in,
                            unsigned long long inlen, const unsigned char *k)
{
    uint64_t       v0 = 0x736f6d6570736575ULL;
    uint64_t       v1 = 0x646f72616e646f83ULL;
    uint64_t       v2 = 0x6c7967656e657261ULL;
    uint64_t       v3 = 0x7465646279746573ULL;
    uint64_t       b;
    uint64_t       k0 = LOAD64_LE(k);
    uint64_t       k1 = LOAD64_LE(k + 8);
    uint64_t       m;
    const uint8_t *end  = in + inlen - (inlen % sizeof(uint64_t));
    const int      left = inlen & 7;

    b = ((uint64_t) inlen) << 56;
    v3 ^= k1;
    v2 ^= k0;
    v1 ^= k1;
    v0 ^= k0;
    for (; in != end; in += 8) {
        m = LOAD64_LE(in);
        v3 ^= m;
        SIPROUND;
        SIPROUND;
        v0 ^= m;
    }
    switch (left) {
    case 7:
        b |= ((uint64_t) in[6]) << 48;
        /* FALLTHRU */
    case 6:
        b |= ((uint64_t) in[5]) << 40;
        /* FALLTHRU */
    case 5:
        b |= ((uint64_t) in[4]) << 32;
        /* FALLTHRU */
    case 4:
        b |= ((uint64_t) in[3]) << 24;
        /* FALLTHRU */
    case 3:
        b |= ((uint64_t) in[2]) << 16;
        /* FALLTHRU */
    case 2:
        b |= ((uint64_t) in[1]) << 8;
        /* FALLTHRU */
    case 1:
        b |= ((uint64_t) in[0]);
        break;
    case 0:
        break;
    }
    v3 ^= b;
    SIPROUND;
    SIPROUND;
    v0 ^= b;
    v2 ^= 0xee;
    SIPROUND;
    SIPROUND;
    SIPROUND;
    SIPROUND;
    b = v0 ^ v1 ^ v2 ^ v3;
    STORE64_LE(out, b);
    v1 ^= 0xdd;
    SIPROUND;
    SIPROUND;
    SIPROUND;
    SIPROUND;
    b = v0 ^ v1 ^ v2 ^ v3;
    STORE64_LE(out + 8, b);

    return 0;
}
