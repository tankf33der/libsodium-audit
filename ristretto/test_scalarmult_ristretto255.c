#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

#include "sodium.h"



#define B_HEX "e2f2ae0a6abc4e71a884a961c500515f58e30b6aa582dd8db6a65945e08d2d76"

int
main(void)
{
    unsigned char *b =
        (unsigned char *) malloc(crypto_scalarmult_ristretto255_BYTES);
    unsigned char *n =
        (unsigned char *) malloc(crypto_scalarmult_ristretto255_SCALARBYTES);
    unsigned char *p =
        (unsigned char *) malloc(crypto_scalarmult_ristretto255_BYTES);
    unsigned char *p2 =
        (unsigned char *) malloc(crypto_scalarmult_ristretto255_BYTES);
    char          *hex =
        (char *) malloc(2 * crypto_scalarmult_ristretto255_BYTES + 1);
    int            i;

    sodium_hex2bin(b, crypto_scalarmult_ristretto255_BYTES,
                   B_HEX, sizeof B_HEX - (size_t) 1U, NULL, NULL, NULL);
    memset(n, 0, crypto_scalarmult_ristretto255_SCALARBYTES);
    for (i = 0; i < 2; i++) {
        crypto_scalarmult_ristretto255_base(p, n);
        if (crypto_scalarmult_ristretto255(p2, n, b) != 0) {
            printf("crypto_scalarmult_ristretto255(%d) != 0\n", i);
        }
        sodium_bin2hex(hex, 2 * crypto_scalarmult_ristretto255_BYTES + 1,
                       p, crypto_scalarmult_ristretto255_BYTES);
        printf("%s\n", hex);
        assert(memcmp(p, p2, crypto_scalarmult_ristretto255_BYTES) == 0);
        sodium_increment(n, crypto_scalarmult_ristretto255_SCALARBYTES);
    }

    memset(p, 0xfe, crypto_scalarmult_ristretto255_BYTES);
    //assert(crypto_scalarmult_ristretto255(guard_page, n, p) == -1);

    free(hex);
    free(p2);
    free(p);
    free(n);
    free(b);

    printf("ristretto: ok\n");

    return 0;
}
