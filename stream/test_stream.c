#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

/*
int crypto_stream(unsigned char *c, unsigned long long clen,
                  const unsigned char *n, const unsigned char *k)
            __attribute__ ((nonnull));
*/

//#include "sodium.h"

int crypto_stream_salsa20(unsigned char *c, unsigned long long clen,
                          const unsigned char *n, const unsigned char *k)
            __attribute__ ((nonnull));
int crypto_hash_sha256(unsigned char *out, const unsigned char *in,
                       unsigned long long inlen) __attribute__ ((nonnull(1)));
int crypto_stream_salsa20_xor_ic(unsigned char *c, const unsigned char *m,
                                 unsigned long long mlen,
                                 const unsigned char *n, uint64_t ic,
                                 const unsigned char *k);
size_t crypto_stream_salsa20_keybytes(void);
size_t crypto_stream_salsa20_noncebytes(void);
size_t crypto_stream_salsa20_messagebytes_max(void);
char *sodium_bin2hex(char * const hex, const size_t hex_maxlen,
                     const unsigned char * const bin, const size_t bin_len);
int crypto_stream(unsigned char *c, unsigned long long clen,
                  const unsigned char *n, const unsigned char *k)
            __attribute__ ((nonnull));
int crypto_stream_xsalsa20_xor_ic(unsigned char *c, const unsigned char *m,
                                  unsigned long long mlen,
                                  const unsigned char *n, uint64_t ic,
                                  const unsigned char *k)
            __attribute__ ((nonnull));


#include "sodium/randombytes.h"

static const unsigned char firstkey[32] = {
    0x1b, 0x27, 0x55, 0x64, 0x73, 0xe9, 0x85,
    0xd4, 0x62, 0xcd, 0x51, 0x19, 0x7a, 0x9a,
    0x46, 0xc7, 0x60, 0x09, 0x54, 0x9e, 0xac,
    0x64, 0x74, 0xf2, 0x06, 0xc4, 0xee, 0x08,
    0x44, 0xf6, 0x83, 0x89
};

static const unsigned char nonce[24] = {
    0x69, 0x69, 0x6e, 0xe9, 0x55, 0xb6,
    0x2b, 0x73, 0xcd, 0x62, 0xbd, 0xa8,
    0x75, 0xfc, 0x73, 0xd6, 0x82, 0x19,
    0xe0, 0x03, 0x6b, 0x7a, 0x0b, 0x37
};

int
main(void)
{
    unsigned char  h[32];
    char          *hex;
    unsigned char *output;
    size_t         sizeof_hex = 17 * 64 * 2 + 1;
    size_t         sizeof_output = 8192;
    int            i;

    output = (unsigned char *) malloc(sizeof_output);
    hex = (char *) malloc(sizeof_hex);

    randombytes_buf(output, sizeof_output);
    crypto_stream(output, sizeof_output, nonce, firstkey);
    crypto_hash_sha256(h, output, sizeof_output);
    sodium_bin2hex(hex, sizeof_hex, h, sizeof h);
    //printf("%s\n", hex);

    assert(sizeof_output > 4000);

    crypto_stream_xsalsa20_xor_ic(output, output, 4000, nonce, 0U, firstkey);
    for (i = 0; i < 4000; i++) {
        assert(output[i] == 0);
    }
    crypto_stream_xsalsa20_xor_ic(output, output, 4000, nonce, 1U, firstkey);
    crypto_hash_sha256(h, output, sizeof_output);
    sodium_bin2hex(hex, sizeof_hex, h, sizeof h);
    //printf("%s\n", hex);

    for (i = 0; i < 64; i++) {
        memset(output, i, 64);
        crypto_stream(output, (int) (i & 0xff), nonce, firstkey);
        sodium_bin2hex(hex, sizeof_hex, output, 64);
        //printf("%s\n", hex);
    }

    memset(output, 0, 192);
    crypto_stream_xsalsa20_xor_ic(output, output, 192, nonce,
                                  (1ULL << 32) - 1ULL, firstkey);
    sodium_bin2hex(hex, 192 * 2 + 1, output, 192);
    //printf("%s\n", hex);

    for (i = 16; i > 0; i--) {
        memset(output, 0, 17 * 64);
        crypto_stream_xsalsa20_xor_ic(output, output, 17 * 64, nonce,
                                      (1ULL << 32) - (unsigned long long) i,
                                      firstkey);
        sodium_bin2hex(hex, 2 * 17 * 64 + 1, output, 17 * 64);
        //printf("%s\n", hex);
    }

    free(hex);
    free(output);

/*
    assert(crypto_stream_keybytes() > 0U);
    assert(crypto_stream_noncebytes() > 0U);
    assert(crypto_stream_messagebytes_max() > 0U);
    assert(strcmp(crypto_stream_primitive(), "xsalsa20") == 0);
    assert(crypto_stream_keybytes() == crypto_stream_xsalsa20_keybytes());
    assert(crypto_stream_noncebytes() == crypto_stream_xsalsa20_noncebytes());
    assert(crypto_stream_messagebytes_max() == crypto_stream_xsalsa20_messagebytes_max());
*/
	printf("stream: ok\n");
    return 0;
}
