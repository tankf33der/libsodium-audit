#include <stddef.h>
#include <stdint.h>

#define FOR_T(type, i, start, end) for (type i = (start); i < (end); i++)
#define FOR(i, start, end)         FOR_T(size_t, i, start, end)
typedef int8_t   i8;
typedef uint8_t  u8;
typedef int16_t  i16;
typedef uint32_t u32;
typedef int32_t  i32;
typedef int64_t  i64;
typedef uint64_t u64;


u64 random_state = 984574;

// Pseudo-random 64 bit number, based on xorshift*
u64 rand64()
{
    random_state ^= random_state >> 12;
    random_state ^= random_state << 25;
    random_state ^= random_state >> 27;
    return random_state * 0x2545F4577F6CDD1D; // magic constant
}


void randombytes_buf(u8 *buf, size_t size)
{
    FOR (i, 0, size) {
        buf[i] = (u8)rand64();
    }
}

uint32_t randombytes_random()
{
	return (u8)rand64();
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



char *
strchr(const char *s, int c)
{
	while (*s) {
		if (*s == c)
			return ((char *)s);
		s++;
	}
	return (NULL);
}



void *memcpy(void *__restrict __dest, const void *__restrict __src, size_t __n)
{
   char *csrc = (char *)__src;
   char *cdest = (char *)__dest;
   for (size_t i=0; i<__n; i++)
       cdest[i] = csrc[i];
}


size_t
strlen(const char *str)
{
	const char *s;

	for (s = str; *s; ++s)
		;
	return (s - str);
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
