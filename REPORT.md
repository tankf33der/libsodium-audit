### Raw list of all issues without any sorting:
```
libhydrogen/impl/hydrogen_p.h:50:[kernel] warning: pointer arithmetic: assert \inside_object(b1+i);
                  stack: hydro_mem_ct_cmp_u32 :: libhydrogen/impl/kx.h:121 <-
                         hydro_kx_aead_decrypt :: libhydrogen/impl/kx.h:293 <-
                         hydro_kx_kk_2 :: libhydrogen/tests.c:339 <-
                         test_kx_kk :: libhydrogen/tests.c:492 <-
                         main
[value] Stopping at nth alarm
[value] user error: Degeneration occurred:
                    results are not correct for lines of code that can be reached from the degeneration point.
```
