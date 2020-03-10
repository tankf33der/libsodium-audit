#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

#include "sodium.h"



static struct {
    const char *in_hex;
    const char *key_hex;
    const char *out_hex;
} tests[] = {
    {
        "",
        "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f",
        "10ebb67700b1868efb4417987acf4690ae9d972fb7a590c2f02871799aaa4786b5e996e8f0f4eb981fc214b005f42d2ff4233499391653df7aefcbc13fc51568"
    },
    {
        "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f404142434445464748494a4b4c4d4e4f505152535455565758595a5b5c5d5e5f606162636465666768696a6b6c6d6e6f707172737475767778797a7b7c7d7e7f808182838485868788898a8b8c8d8e8f909192939495969798999a9b9c9d9e9fa0a1a2a3a4a5a6a7a8a9aaabacadaeafb0b1b2b3b4b5b6b7b8b9babbbcbdbebfc0c1c2c3c4c5c6c7c8c9cacbcccdcecfd0d1d2d3d4d5d6d7d8d9dadbdcdddedfe0e1e2e3e4e5e6e7e8e9eaebecedeeeff0f1f2f3f4f5f6f7f8f9fafbfcfdfe",
        "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f303132333435363738393a3b3c3d3e3f",
        "142709d62e28fcccd0af97fad0f8465b971e82201dc51070faa0372aa43e92484be1c1e73ba10906d5d1853db6a4106e0a7bf9800d373d6dee2d46d62ef2a461"
    }
};

static int
tv(void)
{
    unsigned char *expected_out;
    unsigned char *in;
    unsigned char *key;
    unsigned char *out;
    size_t         i = 0U;
    size_t         in_len;

    key = (unsigned char *) malloc(crypto_generichash_KEYBYTES_MAX);
    out = (unsigned char *) malloc(crypto_generichash_BYTES_MAX);
    expected_out = (unsigned char *) malloc(crypto_generichash_BYTES_MAX);
    do {
        assert(strlen(tests[i].key_hex) == 2 * crypto_generichash_KEYBYTES_MAX);
        sodium_hex2bin(key, crypto_generichash_KEYBYTES_MAX,
                       tests[i].key_hex, strlen(tests[i].key_hex),
                       NULL, NULL, NULL);
        assert(strlen(tests[i].out_hex) == 2 * crypto_generichash_BYTES_MAX);
        sodium_hex2bin(expected_out, crypto_generichash_BYTES_MAX,
                       tests[i].out_hex, strlen(tests[i].out_hex),
                       NULL, NULL, NULL);
        in_len = strlen(tests[i].in_hex) / 2;
        in = (unsigned char *) malloc(in_len);
        sodium_hex2bin(in, in_len, tests[i].in_hex, strlen(tests[i].in_hex),
                       NULL, NULL, NULL);
        crypto_generichash(out, crypto_generichash_BYTES_MAX,
                           in, (unsigned long long) in_len,
                           key, crypto_generichash_KEYBYTES_MAX);
        if (memcmp(out, expected_out, crypto_generichash_BYTES_MAX) != 0) {
            printf("Test vector #%u failed\n", (unsigned int) i);
        }
        free(in);
    } while (++i < (sizeof tests) / (sizeof tests[0]));
    free(key);
    free(out);
    free(expected_out);

    return 0;
}

int
main(void)
{
    tv();
	printf("generichash: ok\n");
    return 0;
}
