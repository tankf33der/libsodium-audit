#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
//#include <errno.h>
#include <string.h>
#include <stdlib.h>


#define FOR_T(type, i, start, end) for (type i = (start); i < (end); i++)
#define FOR(i, start, end)         FOR_T(size_t, i, start, end)
typedef int8_t   i8;
typedef uint8_t  u8;
typedef int16_t  i16;
typedef uint32_t u32;
typedef int32_t  i32;
typedef int64_t  i64;
typedef uint64_t u64;


u64 random_state = 44338454;

// Pseudo-random 64 bit number, based on xorshift*
u64 rand64()
{
    random_state ^= random_state >> 12;
    random_state ^= random_state << 25;
    random_state ^= random_state >> 27;
    return random_state * 0x4445F4577F6CDD1D; // magic constant
}


void randombytes_buf(u8 *out, size_t size)
{
    FOR (i, 0, size) {
        out[i] = (u8)rand64();
    }
}

void randombytes_buf_deterministic(u8 *buf, size_t size)
{
    FOR (i, 0, size) {
        buf[i] = (u8)rand64();
    }
}

uint32_t
randombytes_uniform(const uint32_t upper_bound)
{
    uint32_t min;
    uint32_t r;

    if (upper_bound < 2U) {
        return 0;
    }
    min = (1U + ~upper_bound) % upper_bound; /* = 2**32 mod upper_bound */
    do {
        r = (uint32_t)rand64();
    } while (r < min);
    /* r is now clamped to a set whose size mod upper_bound == 0
     * the worst case (2**31+1) requires 2 attempts on average */

    return r % upper_bound;
}



/*

char *
sodium_bin2hex(char *const hex, const size_t hex_maxlen,
               const unsigned char *const bin, const size_t bin_len)
{
    size_t       i = (size_t) 0U;
    unsigned int x;
    int          b;
    int          c;

    if (bin_len >= SIZE_MAX / 2 || hex_maxlen <= bin_len * 2U) {
    }
    while (i < bin_len) {
        c = bin[i] & 0xf;
        b = bin[i] >> 4;
        x = (unsigned char) (87U + c + (((c - 10U) >> 8) & ~38U)) << 8 |
            (unsigned char) (87U + b + (((b - 10U) >> 8) & ~38U));
        hex[i * 2U] = (char) x;
        x >>= 8;
        hex[i * 2U + 1U] = (char) x;
        i++;
    }
    hex[i * 2U] = 0U;

    return hex;
}


int
sodium_hex2bin(unsigned char *const bin, const size_t bin_maxlen,
               const char *const hex, const size_t hex_len,
               const char *const ignore, size_t *const bin_len,
               const char **const hex_end)
{
    size_t        bin_pos = (size_t) 0U;
    size_t        hex_pos = (size_t) 0U;
    int           ret     = 0;
    unsigned char c;
    unsigned char c_acc = 0U;
    unsigned char c_alpha0, c_alpha;
    unsigned char c_num0, c_num;
    unsigned char c_val;
    unsigned char state = 0U;

    while (hex_pos < hex_len) {
        c        = (unsigned char) hex[hex_pos];
        c_num    = c ^ 48U;
        c_num0   = (c_num - 10U) >> 8;
        c_alpha  = (c & ~32U) - 55U;
        c_alpha0 = ((c_alpha - 10U) ^ (c_alpha - 16U)) >> 8;
        if ((c_num0 | c_alpha0) == 0U) {
            if (ignore != NULL && state == 0U && strchr(ignore, c) != NULL) {
                hex_pos++;
                continue;
            }
            break;
        }
        c_val = (c_num0 & c_num) | (c_alpha0 & c_alpha);
        if (bin_pos >= bin_maxlen) {
            ret   = -1;
            //errno = ERANGE;
            break;
        }
        if (state == 0U) {
            c_acc = c_val * 16U;
        } else {
            bin[bin_pos++] = c_acc | c_val;
        }
        state = ~state;
        hex_pos++;
    }
    if (state != 0U) {
        hex_pos--;
        //errno = EINVAL;
        ret = -1;
    }
    if (ret != 0) {
        bin_pos = (size_t) 0U;
    }
    if (hex_end != NULL) {
        *hex_end = &hex[hex_pos];
    } else if (hex_pos != hex_len) {
        //errno = EINVAL;
        ret = -1;
    }
    if (bin_len != NULL) {
        *bin_len = bin_pos;
    }
    return ret;
}


int
sodium_is_zero(const unsigned char *n, const size_t nlen)
{
    size_t                 i;
    //volatile unsigned char d = 0U;
	// XXX Fix for TIS
	uint32_t d = 0;

    for (i = 0U; i < nlen; i++) {
        d |= n[i];
    }
    return 1 & ((d - 1) >> 8);
}


*/

size_t strlen (const char *str)
{
    for (size_t len = 0;;++len) if (str[len]==0) return len;
}

int
memcmp (const void *str1, const void *str2, size_t count)
{
  register const unsigned char *s1 = (const unsigned char*)str1;
  register const unsigned char *s2 = (const unsigned char*)str2;

  while (count-- > 0)
    {
      if (*s1++ != *s2++)
	  return s1[-1] < s2[-1] ? -1 : 1;
    }
  return 0;
}


void *
memset(void *__s, int __c, size_t __n)
{

	if (__n != 0) {
		char *d = __s;

		do {
			*d++ = __c;
		} while (--__n != 0);
	}
	return (__s);
}


/*
void
sodium_memzero(void * const pnt, const size_t len)
{
	memset(pnt, 0, len);
}
*/

void *memcpy(void *__restrict __dest, const void *__restrict __src, size_t __n)
{
   char *csrc = (char *)__src;
   char *cdest = (char *)__dest;
   for (size_t i=0; i<__n; i++)
       cdest[i] = csrc[i];
}

void *__memalign(size_t align, size_t len)
{
	unsigned char *mem, *new, *end;
	size_t header, footer;

	if ((align & -align) != align) {
		//errno = EINVAL;
		return NULL;
	}

	if (len > SIZE_MAX - align) {
		//errno = ENOMEM;
		return NULL;
	}

	if (align <= 4*sizeof(size_t)) {
		if (!(mem = malloc(len)))
			return NULL;
		return mem;
	}

	if (!(mem = malloc(len + align-1)))
		return NULL;

	new = (void *)((uintptr_t)mem + align-1 & -align);
	if (new == mem) return mem;

	header = ((size_t *)mem)[-1];

	if (!(header & 7)) {
		((size_t *)new)[-2] = ((size_t *)mem)[-2] + (new-mem);
		((size_t *)new)[-1] = ((size_t *)mem)[-1] - (new-mem);
		return new;
	}

	end = mem + (header & -8);
	footer = ((size_t *)end)[-2];

	((size_t *)mem)[-1] = header&7 | new-mem;
	((size_t *)new)[-2] = footer&7 | new-mem;
	((size_t *)new)[-1] = header&7 | end-new;
	((size_t *)end)[-2] = footer&7 | end-new;

	free(mem);
	return new;
}

void *__memalign(size_t, size_t);

int posix_memalign(void **res, size_t align, size_t len)
{
	if (align < sizeof(void *)) return -1;
	void *mem = __memalign(align, len);
	if (!mem) return -1;
	*res = mem;
	return 0;
}
