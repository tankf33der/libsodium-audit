#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

#include "sodium.h"
#define MAXLEN 64

static struct {
	const char *tt;
} tests[] = {
	{"a3817f04ba25a8e66df67214c7550293"},
	{"da87c1d86b99af44347659119b22fc45"},
	{"8177228da4a45dc7fca38bdef60affe4"},
	{"9c70b60c5267a94e5f33b6b02985ed51"},
	{"f88164c12d9c8faf7d0f6e7c7bcd5579"},
	{"1368875980776f8854527a07690e9627"},
	{"14eeca338b208613485ea0308fd7a15e"},
	{"a1f1ebbed8dbc153c0b84aa61ff08239"},
	{"3b62a9ba6258f5610f83e264f31497b4"},
	{"264499060ad9baabc47f8b02bb6d71ed"},
	{"00110dc378146956c95447d3f3d0fbba"},
	{"0151c568386b6677a2b4dc6f81e5dc18"},
	{"d626b266905ef35882634df68532c125"},
	{"9869e247e9c08b10d029934fc4b952f7"},
	{"31fcefac66d7de9c7ec7485fe4494902"},
	{"5493e99933b0a8117e08ec0f97cfc3d9"},
	{"6ee2a4ca67b054bbfd3315bf85230577"},
	{"473d06e8738db89854c066c47ae47740"},
	{"a426e5e423bf4885294da481feaef723"},
	{"78017731cf65fab074d5208952512eb1"},
	{"9e25fc833f2290733e9344a5e83839eb"},
	{"568e495abe525a218a2214cd3e071d12"},
	{"4a29b54552d16b9a469c10528eff0aae"},
	{"c9d184ddd5a9f5e0cf8ce29a9abf691c"},
	{"2db479ae78bd50d8882a8a178a6132ad"},
	{"8ece5f042d5e447b5051b9eacb8d8f6f"},
	{"9c0b53b4b3c307e87eaee08678141f66"},
	{"abf248af69a6eae4bfd3eb2f129eeb94"},
	{"0664da1668574b88b935f3027358aef4"},
	{"aa4b9dc4bf337de90cd4fd3c467c6ab7"},
	{"ea5c7f471faf6bde2b1ad7d4686d2287"},
	{"2939b0183223fafc1723de4f52c43d35"},
	{"7c3956ca5eeafc3e363e9d556546eb68"},
	{"77c6077146f01c32b6b69d5f4ea9ffcf"},
	{"37a6986cb8847edf0925f0f1309b54de"},
	{"a705f0e69da9a8f907241a2e923c8cc8"},
	{"3dc47d1f29c448461e9e76ed904f6711"},
	{"0d62bf01e6fc0e1a0d3c4751c5d3692b"},
	{"8c03468bca7c669ee4fd5e084bbee7b5"},
	{"528a5bb93baf2c9c4473cce5d0d22bd9"},
	{"df6a301e95c95dad97ae0cc8c6913bd8"},
	{"801189902c857f39e73591285e70b6db"},
	{"e617346ac9c231bb3650ae34ccca0c5b"},
	{"27d93437efb721aa401821dcec5adf89"},
	{"89237d9ded9c5e78d8b1c9b166cc7342"},
	{"4a6d8091bf5e7d651189fa94a250b14c"},
	{"0e33f96055e7ae893ffc0e3dcf492902"},
	{"e61c432b720b19d18ec8d84bdc63151b"},
	{"f7e5aef549f782cf379055a608269b16"},
	{"438d030fd0b7a54fa837f2ad201a6403"},
	{"a590d3ee4fbf04e3247e0d27f286423f"},
	{"5fe2c1a172fe93c4b15cd37caef9f538"},
	{"2c97325cbd06b36eb2133dd08b3a017c"},
	{"92c814227a6bca949ff0659f002ad39e"},
	{"dce850110bd8328cfbd50841d6911d87"},
	{"67f14984c7da791248e32bb5922583da"},
	{"1938f2cf72d54ee97e94166fa91d2a36"},
	{"74481e9646ed49fe0f6224301604698e"},
	{"57fca5de98a9d6d8006438d0583d8a1d"},
	{"9fecde1cefdc1cbed4763674d9575359"},
	{"e3040c00eb28f15366ca73cbd872e740"},
	{"7697009a6a831dfecca91c5993670f7a"},
	{"5853542321f567a005d547a4f04759bd"},
	{"5150d1772f50834a503e069a973fbd7c"},
};


int
main(void)
{
    unsigned char in[MAXLEN];
    unsigned char out[crypto_shorthash_siphashx24_BYTES];
    unsigned char k[crypto_shorthash_siphashx24_KEYBYTES];
	unsigned char *r;
    size_t        i;
	r = (unsigned char *) malloc(crypto_shorthash_siphashx24_BYTES);

    for (i = 0; i < crypto_shorthash_siphashx24_KEYBYTES; ++i) {
        k[i] = (unsigned char) i;
    }
    for (i = 0; i < MAXLEN; ++i) {
		sodium_hex2bin(r, crypto_shorthash_siphashx24_BYTES, tests[i].tt,
			strlen(tests[i].tt), NULL, NULL, NULL);
        in[i] = (unsigned char) i;
        crypto_shorthash_siphashx24(out, in, (unsigned long long) i, k);
		/*
        for (j = 0; j < crypto_shorthash_siphashx24_BYTES; ++j) {
            printf("%02x", (unsigned int) out[j]);
        }*/
        if (memcmp(out, r, 16) != 0) {
        	printf("fail\n");
        }
        //printf("\n");
    }
    /*
    assert(crypto_shorthash_siphashx24_KEYBYTES >= crypto_shorthash_siphash24_KEYBYTES);
    assert(crypto_shorthash_siphashx24_BYTES > crypto_shorthash_siphash24_BYTES);
    assert(crypto_shorthash_siphashx24_bytes() == crypto_shorthash_siphashx24_BYTES);
    assert(crypto_shorthash_siphashx24_keybytes() == crypto_shorthash_siphashx24_KEYBYTES);
	*/
	printf("OK\n");
    return 0;
}
