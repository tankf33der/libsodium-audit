### Raw list of uniq issues without any sorting:
```
verify/verify.c:75:[kernel] warning: pointer arithmetic: assert \inside_object(x+i);
                  stack: crypto_verify_n :: verify/verify.c:85 <-
                         crypto_verify_16 :: verify/test_verify1.c:35 <-
                         main
```
```
libhydrogen/impl/hydrogen_p.h:50:[kernel] warning: pointer arithmetic: assert \inside_object(b1+i);
                  stack: hydro_mem_ct_cmp_u32 :: libhydrogen/impl/kx.h:121 <-
                         hydro_kx_aead_decrypt :: libhydrogen/impl/kx.h:293 <-
                         hydro_kx_kk_2 :: libhydrogen/tests.c:339 <-
                         test_kx_kk :: libhydrogen/tests.c:492 <-
                         main
```

```
libhydrogen/impl/core.h:26:[kernel] warning: pointer arithmetic: assert \inside_object(pnt_+tmp);
                                      (tmp from i++)
                  stack: hydro_memzero :: libhydrogen/tests.c:141 <-
                         test_core :: libhydrogen/tests.c:486 <-
                         main
```


```
int
sodium_is_zero(const unsigned char *n, const size_t nlen)
{
    size_t                 i;
    volatile unsigned char d = 0U;

    for (i = 0U; i < nlen; i++) {
        d |= n[i];
    }
    return 1 & ((d - 1) >> 8);
}

xchacha20/my_utils.c:167:[value] warning: The following sub-expression cannot be evaluated:
                 (int)d | (int)*(n + i)

                 All sub-expressions with their values:
                 int  (int)*(n + i) ∈ {0}
                 int  (int)d ∈ [0..255]
                 unsigned char const *  n + i ∈ {{ &__malloc_tv_stream_xchacha20_l132[0] }}
                 unsigned char  *(n + i) ∈ {0}
                 unsigned char const *  n ∈ {{ &__malloc_tv_stream_xchacha20_l132[0] }}
                 size_t  i ∈ {0}
                 unsigned char  d ∈ [0..255]

                 Stopping
                 stack: sodium_is_zero :: xchacha20/test_xchacha20.c:136 <-
                        tv_stream_xchacha20 :: xchacha20/test_xchacha20.c:440 <-
                        main

```

```
code:
https://github.com/jedisct1/libsodium/blob/master/src/libsodium/crypto_secretbox/xchacha20poly1305/secretbox_xchacha20poly1305.c#L34

TIS:
xchacha20/secretbox_xchacha20poly1305.c:34:[kernel] warning: pointer comparison: assert \pointer_comparable((void *)c, (void *)m);
                  stack: crypto_secretbox_xchacha20poly1305_detached :: xchacha20/secretbox_xchacha20poly1305.c:85 <-
                         crypto_secretbox_xchacha20poly1305_easy :: xchacha20/test_xchacha20.c:256 <-
                         tv_secretbox_xchacha20poly1305 :: xchacha20/test_xchacha20.c:441 <-
                         main
[value] Stopping at nth alarm
[value] user error: Degeneration occurred:
                    results are not correct for lines of code that can be reached from the degeneration point.


compcert-interp:
Stuck state: in function crypto_secretbox_xchacha20poly1305_detached, expression
  <ptr> > <ptr> && (unsigned long long) c - (unsigned long long) m < mlen
    || (unsigned long long) m > (unsigned long long) c
         && (unsigned long long) m - (unsigned long long) c < mlen
Stuck subexpression:
  <ptr> > <ptr> && (unsigned long long) c - (unsigned long long) m < mlen
    || (unsigned long long) m > (unsigned long long) c
         && (unsigned long long) m - (unsigned long long) c < mlen
ERROR: Undefined behavior
```

```
code:
https://git.envs.net/mpech/libsodium-audit/src/branch/master/xchacha20/verify.c#L75


xchacha20/verify.c:75:[kernel] warning: pointer arithmetic: assert \inside_object(x+i);
                  stack: crypto_verify_n :: xchacha20/verify.c:85 <-
                         crypto_verify_16 :: xchacha20/poly1305_donna.c:112 <-
                         crypto_onetimeauth_poly1305_donna_verify :: xchacha20/onetimeauth_poly1305.c:63 <-
                         crypto_onetimeauth_poly1305_verify :: xchacha20/secretbox_xchacha20poly1305.c:108 <-
                         crypto_secretbox_xchacha20poly1305_open_detached :: xchacha20/secretbox_xchacha20poly1305.c:154 <-
                         crypto_secretbox_xchacha20poly1305_open_easy :: xchacha20/test_xchacha20.c:268 <-
                         tv_secretbox_xchacha20poly1305 :: xchacha20/test_xchacha20.c:441 <-
                         main
```

```
xchacha20/scalarmult_curve25519.c:25:[value] warning: The following sub-expression cannot be evaluated:
                 (int)d_0 | (int)*(q + i)

                 All sub-expressions with their values:
                 int  (int)*(q + i) ∈ {213}
                 int  (int)d_0 ∈ [0..255]
                 unsigned char *  q + i ∈ {{ &s[0] }}
                 unsigned char  *(q + i) ∈ {213}
                 unsigned char *  q ∈ {{ &s[0] }}
                 size_t  i ∈ {0}
                 unsigned char  d_0 ∈ [0..255]

                 Stopping
                 stack: crypto_scalarmult_curve25519 :: xchacha20/box_curve25519xchacha20poly1305.c:48 <-
                        crypto_box_curve25519xchacha20poly1305_beforenm :: xchacha20/box_curve25519xchacha20poly1305.c:73 <-
                        crypto_box_curve25519xchacha20poly1305_detached :: xchacha20/box_curve25519xchacha20poly1305.c:107 <-
                        crypto_box_curve25519xchacha20poly1305_easy :: xchacha20/test_xchacha20.c:343 <-
                        tv_box_xchacha20poly1305 :: xchacha20/test_xchacha20.c:439 <-
                        main
```

```
xchacha20/verify.c:75:[kernel] warning: pointer arithmetic: assert \inside_object(x+i);
                  stack: crypto_verify_n :: xchacha20/verify.c:85 <-
                         crypto_verify_16 :: xchacha20/poly1305_donna.c:112 <-
                         crypto_onetimeauth_poly1305_donna_verify :: xchacha20/onetimeauth_poly1305.c:63 <-
                         crypto_onetimeauth_poly1305_verify :: xchacha20/secretbox_xchacha20poly1305.c:108 <-
                         crypto_secretbox_xchacha20poly1305_open_detached :: xchacha20/box_curve25519xchacha20poly1305.c:117 <-
                         crypto_box_curve25519xchacha20poly1305_open_detached_afternm :: xchacha20/box_curve25519xchacha20poly1305.c:133 <-
                         crypto_box_curve25519xchacha20poly1305_open_detached :: xchacha20/box_curve25519xchacha20poly1305.c:161 <-
                         crypto_box_curve25519xchacha20poly1305_open_easy :: xchacha20/test_xchacha20.c:355 <-
                         tv_box_xchacha20poly1305 :: xchacha20/test_xchacha20.c:439 <-
                         main
```

```
secretbox8/verify.c:75:[kernel] warning: pointer arithmetic: assert \inside_object(x+i);
                  stack: crypto_verify_n :: secretbox8/verify.c:85 <-
                         crypto_verify_16 :: secretbox8/poly1305_donna.c:112 <-
                         crypto_onetimeauth_poly1305_donna_verify :: secretbox8/onetimeauth_poly1305.c:63 <-
                         crypto_onetimeauth_poly1305_verify :: secretbox8/secretbox_xsalsa20poly1305.c:38 <-
                         crypto_secretbox_xsalsa20poly1305_open :: secretbox8/crypto_secretbox.c:60 <-
                         crypto_secretbox_open :: secretbox8/test_secretbox8.c:32 <-
                         main
```

```
secretbox7/verify.c:75:[kernel] warning: pointer arithmetic: assert \inside_object(x+i);
                  stack: crypto_verify_n :: secretbox7/verify.c:85 <-
                         crypto_verify_16 :: secretbox7/poly1305_donna.c:112 <-
                         crypto_onetimeauth_poly1305_donna_verify :: secretbox7/onetimeauth_poly1305.c:63 <-
                         crypto_onetimeauth_poly1305_verify :: secretbox7/secretbox_xsalsa20poly1305.c:38 <-
                         crypto_secretbox_xsalsa20poly1305_open :: secretbox7/crypto_secretbox.c:60 <-
                         crypto_secretbox_open :: secretbox7/test_secretbox7.c:29 <-
                         main
```

```
$ cd scrypt
$ make undefined
<SKIP>
pwhash_scryptsalsa208sha256_nosse.c:122:22: runtime error: index 8 out of bounds for type 'uint64_t const[8]'
pwhash_scryptsalsa208sha256_nosse.c:122:9: runtime error: index 8 out of bounds for type 'uint64_t [8]'
pwhash_scryptsalsa208sha256_nosse.c:140:23: runtime error: index 8 out of bounds for type 'uint64_t const[8]'
pwhash_scryptsalsa208sha256_nosse.c:140:9: runtime error: index 8 out of bounds for type 'uint64_t [8]'
</SKIP>
```

```
$ cd scrypt
$ make interp
rm -rf *.o *.out mike.c
cat *.c >> mike.c
ccomp -interp -quiet mike.c
mike.c:1187: warning: control reaches end of non-void function [-Wreturn-type]
Stuck state: in function escrypt_alloc_region, expression
  <loc aligned> -= <ptr> & 63
Stuck subexpression: <ptr> & 63
ERROR: Undefined behavior
make: *** [Makefile:13: interp] Error 126
```

```
scrypt/scrypt_platform.c:66:[value] warning: The following sub-expression cannot be evaluated:
                 (unsigned long)aligned & (unsigned long)63

                 All sub-expressions with their values:
                 unsigned long  (unsigned long)aligned ∈ {{ (unsigned long)&__malloc_escrypt_alloc_region_l64[63] }}
                 unsigned long  (unsigned long)63 ∈ {63}
                 uint8_t *  aligned ∈ {{ &__malloc_escrypt_alloc_region_l64[63] }}
                 int  63 ∈ {63}

                 Stopping
                 stack: escrypt_alloc_region :: scrypt/pwhash_scryptsalsa208sha256_nosse.c:393 <-
                        escrypt_kdf_nosse :: scrypt/crypto_scrypt-common.c:257 <-
                        crypto_pwhash_scryptsalsa208sha256_ll :: scrypt/pwhash_scryptsalsa208sha256.c:179 <-
                        crypto_pwhash_scryptsalsa208sha256 :: scrypt/test_pwhash_scrypt.c:44 <-
                        tv :: scrypt/test_pwhash_scrypt.c:134 <-
                        main
```
```
generichash/utils.c:147:[kernel] warning: pointer arithmetic: assert \inside_object(pnt_+tmp);
                                      (tmp from i++)
                  stack: sodium_memzero :: generichash/blake2b-ref.c:227 <-
                         blake2b_init_key :: generichash/blake2b-ref.c:374 <-
                         _sodium_blake2b :: generichash/generichash_blake2b.c:23 <-
                         crypto_generichash_blake2b :: generichash/crypto_generichash.c:58 <-
                         crypto_generichash :: generichash/test_generichash.c:54 <-
                         tv :: generichash/test_generichash.c:72 <-
                         main
```
