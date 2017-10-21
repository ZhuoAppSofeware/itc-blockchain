#pragma once //sha256Ëã·¨ÊµÏÖ

//include
#include "include.h"
//define
#define SHA256_BLOCK 64

#define SHA256_SIGMA0_00_15(x) (MATH_ROTATE((x),30) ^ MATH_ROTATE((x),19) ^ MATH_ROTATE((x),10))
#define SHA256_SIGMA1_00_15(x) (MATH_ROTATE((x),26) ^ MATH_ROTATE((x),21) ^ MATH_ROTATE((x),7))
#define SHA256_SIGMA0_16_63(x) (MATH_ROTATE((x),25) ^ MATH_ROTATE((x),14) ^ ((x)>>3))
#define SHA256_SIGMA1_16_63(x) (MATH_ROTATE((x),15) ^ MATH_ROTATE((x),13) ^ ((x)>>10))

#define SHA256_CH(x,y,z) (((x) & (y)) ^ ((~(x)) & (z)))
#define SHA256_MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

#define	SHA256_ROUND_00_15(i,a,b,c,d,e,f,g,h) {t = x[(i)&0x0f];t += h + SHA256_SIGMA1_00_15(e) + SHA256_CH(e,f,g) + table_k256[i];h = SHA256_SIGMA0_00_15(a) + SHA256_MAJ(a,b,c);d += t;h += t;}
#define	SHA256_ROUND_16_63(i,a,b,c,d,e,f,g,h,x) {s0 = x[(i+1)&0x0f];s0 = SHA256_SIGMA0_16_63(s0);s1 = x[(i+14)&0x0f];s1 = SHA256_SIGMA1_16_63(s1);x[(i)&0x0f] += s0 + s1 + x[(i+9)&0x0f];SHA256_ROUND_00_15(i,a,b,c,d,e,f,g,h);}
//struct
typedef struct
{
    uint32 count[2];
    uint32 state[8];
    uint8 buffer[64];
	uint32 len;//data byte length
}sha256_ctx;
//function
class crypt_sha256
{
public:
	crypt_sha256(void);
	~crypt_sha256(void);
public:
	sha256_ctx ctx;
public:
	void sha256_init(void);
	void sha256_update(uint8 *data,uint32 length);
	void sha256_final(uint8 *result);
private:
	void sha256_transform(uint8 *block);
};