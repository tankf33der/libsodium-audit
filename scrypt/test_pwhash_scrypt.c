#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

#include "sodium.h"

#define OUT_LEN 128
#define OPSLIMIT 1000000
#define MEMLIMIT 10000000

static void
tv(void)
{
    static struct {
        const char        *passwd_hex;
        size_t             passwdlen;
        const char        *salt_hex;
        size_t             outlen;
        unsigned long long opslimit;
        size_t             memlimit;
    } tests[] = {
        { "a347ae92bce9f80f6f595a4480fc9c2fe7e7d7148d371e9487d75f5c23008ffae0"
          "65577a928febd9b1973a5a95073acdbeb6a030cfc0d79caa2dc5cd011cef02c08d"
          "a232d76d52dfbca38ca8dcbd665b17d1665f7cf5fe59772ec909733b24de97d6f5"
          "8d220b20c60d7c07ec1fd93c52c31020300c6c1facd77937a597c7a6",
          127,
          "5541fbc995d5c197ba290346d2c559dedf405cf97e5f95482143202f9e74f5c2",
          155, 481326, 7256678 },
    };
    char          passwd[256];
    unsigned char salt[crypto_pwhash_scryptsalsa208sha256_SALTBYTES];
    unsigned char out[256];
    char          out_hex[256 * 2 + 1];
    size_t        i = 0U;

    do {
        sodium_hex2bin((unsigned char *) passwd, sizeof passwd,
                       tests[i].passwd_hex, strlen(tests[i].passwd_hex), NULL,
                       NULL, NULL);
        sodium_hex2bin(salt, sizeof salt, tests[i].salt_hex,
                       strlen(tests[i].salt_hex), NULL, NULL, NULL);
        if (crypto_pwhash_scryptsalsa208sha256(
                out, (unsigned long long) tests[i].outlen, passwd,
                tests[i].passwdlen, (const unsigned char *) salt,
                tests[i].opslimit, tests[i].memlimit) != 0) {
            printf("pwhash failure\n");
        }
        sodium_bin2hex(out_hex, sizeof out_hex, out, tests[i].outlen);
        //printf("%s\n", out_hex);
    } while (++i < (sizeof tests) / (sizeof tests[0]));
}

static void
str_tests(void)
{
    char       *str_out;
    char       *str_out2;
    char       *salt;
    const char *passwd = "Correct Horse Battery Staple";

    salt = (char *) malloc(crypto_pwhash_scryptsalsa208sha256_SALTBYTES);
    str_out =
        (char *) malloc(crypto_pwhash_scryptsalsa208sha256_STRBYTES);
    str_out2 =
        (char *) malloc(crypto_pwhash_scryptsalsa208sha256_STRBYTES);
    memcpy(salt, "[<~A 32-bytes salt for scrypt~>]",
           crypto_pwhash_scryptsalsa208sha256_SALTBYTES);
    if (crypto_pwhash_scryptsalsa208sha256_str(str_out, passwd, strlen(passwd),
                                               OPSLIMIT, MEMLIMIT) != 0) {
        printf("pwhash_str failure\n");
    }
    if (crypto_pwhash_scryptsalsa208sha256_str(str_out2, passwd, strlen(passwd),
                                               OPSLIMIT, MEMLIMIT) != 0) {
        printf("pwhash_str(2) failure\n");
    }
    if (strcmp(str_out, str_out2) == 0) {
        printf("pwhash_str doesn't generate different salts\n");
    }
    if (crypto_pwhash_scryptsalsa208sha256_str_needs_rehash
        (str_out, OPSLIMIT, MEMLIMIT) != 0) {
        printf("needs_rehash() false positive\n");
    }
    if (crypto_pwhash_scryptsalsa208sha256_str_needs_rehash
        (str_out, OPSLIMIT, MEMLIMIT / 2) != 1 ||
        crypto_pwhash_scryptsalsa208sha256_str_needs_rehash
        (str_out, OPSLIMIT / 2, MEMLIMIT) != 1 ||
        crypto_pwhash_scryptsalsa208sha256_str_needs_rehash
        (str_out, OPSLIMIT, MEMLIMIT * 2) != 1 ||
        crypto_pwhash_scryptsalsa208sha256_str_needs_rehash
        (str_out, OPSLIMIT * 2, MEMLIMIT) != 1) {
        printf("needs_rehash() false negative\n");
    }
    if (crypto_pwhash_scryptsalsa208sha256_str_needs_rehash
        (str_out + 1, OPSLIMIT, MEMLIMIT) != -1) {
        printf("needs_rehash() didn't fail with an invalid hash string\n");
    }
    if (crypto_pwhash_scryptsalsa208sha256_str_verify(str_out, passwd,
                                                      strlen(passwd)) != 0) {
        printf("pwhash_str_verify failure\n");
    }
    if (crypto_pwhash_scryptsalsa208sha256_str_verify(str_out, passwd,
                                                      strlen(passwd)) != 0) {
        printf("pwhash_str_verify failure\n");
    }
    str_out[14]++;
    if (crypto_pwhash_scryptsalsa208sha256_str_verify(str_out, passwd,
                                                      strlen(passwd)) == 0) {
        printf("pwhash_str_verify(2) failure\n");
    }
    str_out[14]--;

    assert(str_out[crypto_pwhash_scryptsalsa208sha256_STRBYTES - 1U] == 0);

    assert(crypto_pwhash_scryptsalsa208sha256_str_needs_rehash
           (str_out, 0, 0) == 1);
    assert(crypto_pwhash_str_needs_rehash(str_out, 0, 0) == -1);
    assert(crypto_pwhash_str_needs_rehash(str_out, OPSLIMIT, MEMLIMIT) == -1);
    assert(crypto_pwhash_scryptsalsa208sha256_str_needs_rehash
           ("", OPSLIMIT, MEMLIMIT) == -1);

    free(salt);
    free(str_out);
    free(str_out2);
}


int
main(void)
{
    tv();
    str_tests();

/*
    assert(crypto_pwhash_scryptsalsa208sha256_bytes_min() > 0U);
    assert(crypto_pwhash_scryptsalsa208sha256_bytes_max() >
           crypto_pwhash_scryptsalsa208sha256_bytes_min());
    assert(crypto_pwhash_scryptsalsa208sha256_passwd_max() >
           crypto_pwhash_scryptsalsa208sha256_passwd_min());
    assert(crypto_pwhash_scryptsalsa208sha256_saltbytes() > 0U);
    assert(crypto_pwhash_scryptsalsa208sha256_strbytes() > 1U);
    assert(crypto_pwhash_scryptsalsa208sha256_strbytes() >
           strlen(crypto_pwhash_scryptsalsa208sha256_strprefix()));

    assert(crypto_pwhash_scryptsalsa208sha256_opslimit_min() > 0U);
    assert(crypto_pwhash_scryptsalsa208sha256_opslimit_max() > 0U);
    assert(crypto_pwhash_scryptsalsa208sha256_memlimit_min() > 0U);
    assert(crypto_pwhash_scryptsalsa208sha256_memlimit_max() > 0U);
    assert(crypto_pwhash_scryptsalsa208sha256_opslimit_interactive() > 0U);
    assert(crypto_pwhash_scryptsalsa208sha256_memlimit_interactive() > 0U);
    assert(crypto_pwhash_scryptsalsa208sha256_opslimit_sensitive() > 0U);
    assert(crypto_pwhash_scryptsalsa208sha256_memlimit_sensitive() > 0U);
*/
    printf("scrypt: ok\n");

    return 0;
}
