### Raw list of all issues without any sorting:
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