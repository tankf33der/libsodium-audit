#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

#include "sodium.h"



static unsigned char k[crypto_secretbox_KEYBYTES];
static unsigned char n[crypto_secretbox_NONCEBYTES];
static unsigned char m[2048];
static unsigned char c[2048];
static unsigned char m2[2048];

int
main(void)
{
    size_t mlen;
    size_t i;

    for (mlen = 0; mlen < 64 && mlen + crypto_secretbox_ZEROBYTES < sizeof m;
         ++mlen) {
        crypto_secretbox_keygen(k);
        randombytes_buf(n, crypto_secretbox_NONCEBYTES);
        randombytes_buf(m + crypto_secretbox_ZEROBYTES, mlen);
        crypto_secretbox(c, m, mlen + crypto_secretbox_ZEROBYTES, n, k);
        if (crypto_secretbox_open(m2, c, mlen + crypto_secretbox_ZEROBYTES, n,
                                  k) == 0) {
            for (i = 0; i < mlen + crypto_secretbox_ZEROBYTES; ++i) {
                if (m2[i] != m[i]) {
                    printf("bad decryption\n");
                    break;
                }
            }
        } else {
            printf("ciphertext fails verification\n");
        }
    }
    printf("secretbox7: ok\n");
    return 0;
}
