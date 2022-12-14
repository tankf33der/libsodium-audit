#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

#include "sodium.h"


int
main(void)
{
    unsigned char *v16, *v16x;
    unsigned char *v32, *v32x;
    unsigned char *v64, *v64x;
    uint32_t       r;
    uint8_t        o;
    int            i;

    v16  = (unsigned char *) malloc(16);
    v16x = (unsigned char *) malloc(16);
    v32  = (unsigned char *) malloc(32);
    v32x = (unsigned char *) malloc(32);
    v64  = (unsigned char *) malloc(64);
    v64x = (unsigned char *) malloc(64);
    for (i = 0; i < 1000; i++) {
        randombytes_buf(v16, 16);
        randombytes_buf(v32, 32);
        randombytes_buf(v64, 64);

        memcpy(v16x, v16, 16);
        memcpy(v32x, v32, 32);
        memcpy(v64x, v64, 64);

        if (crypto_verify_16(v16, v16x) != 0 ||
            crypto_verify_32(v32, v32x) != 0 ||
            crypto_verify_64(v64, v64x) != 0 ||
            memcmp(v16, v16x, 16) != 0 ||
            memcmp(v32, v32x, 32) != 0 ||
            memcmp(v64, v64x, 64) != 0) {
            printf("Failed\n");
        }
    }

    for (i = 0; i < 1000; i++) {
        r = randombytes_random();
        o = (uint8_t) randombytes_random();
        if (o == 0) {
            continue;
        }
        v16x[r & 15U] ^= o;
        v32x[r & 31U] ^= o;
        v64x[r & 63U] ^= o;
        if (crypto_verify_16(v16, v16x) != -1 ||
            crypto_verify_32(v32, v32x) != -1 ||
            crypto_verify_64(v64, v64x) != -1 ||
            memcmp(v16, v16x, 16) == 0 ||
            memcmp(v32, v32x, 32) == 0 ||
            memcmp(v64, v64x, 64) == 0) {
            printf("Failed\n");
        }
        v16x[r & 15U] ^= o;
        v32x[r & 31U] ^= o;
        v64x[r & 63U] ^= o;
    }
    printf("OK\n");

    //assert(crypto_verify_16_bytes() == 16U);
    //assert(crypto_verify_32_bytes() == 32U);
    //assert(crypto_verify_64_bytes() == 64U);

    free(v16);
    free(v16x);
    free(v32);
    free(v32x);
    free(v64);
    free(v64x);

    return 0;
}
