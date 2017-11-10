#pragma once //rsa·Ç¶Ô³ÆËã·¨

////include
#include "crypt_bignum.h"
////define
#define RSA_STD 0
#define RSA_CRT 1
////struct
typedef struct
{
	uint8 *e;	//rsa public key
	uint8 *n;	//modulus
	uint8 *p;	//prime p
	uint8 *q;	//prime q
	uint8 *d;	//rsa private key
	uint8 *dp;	//d mod (p-1)
	uint8 *dq;	//d mod (q-1)
	uint8 *qp;	//q^-1 mod p
	uint32 le;	//e's byte length
	uint32 len;	//n/d's byte length
	uint32 lr;	//random's byte length(0 not use rand)
}rsa_para;
////function
void rsa_genprime(uint8 *content,uint32 length);
uint8 rsa_verifyprime(uint8 *content,uint32 length);
uint8 rsa_genkey(rsa_para *para,uint32 config);
uint32 rsa_enc(uint8 *result,uint8 *content,uint32 length,rsa_para *para);
uint32 rsa_dec(uint8 *result,uint8 *content,uint32 length,rsa_para *para,uint32 config);