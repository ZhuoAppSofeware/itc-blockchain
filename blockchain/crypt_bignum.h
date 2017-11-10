#pragma once //´óÊıÔËËã

////include
#include "include.h"
////define
////struct
////function
void _len(uint8 *a,uint32 &length);
int8 _cmp(uint8 *a,uint8 *b,uint32 length_a,uint32 length_b);
int8 _cmp(uint8 *a,uint8 b,uint32 length_a);
uint32 _add(uint8 *result,uint8 *a,uint8 *b,uint32 length_a,uint32 length_b);
uint32 _add(uint8 *result,uint8 *a,uint8 b,uint32 length_a);
uint32 _sub(uint8 *result1,uint8 *result2,uint8 *a,uint8 *b,uint32 length_a,uint32 length_b);
uint32 _sub(uint8 *result1,uint8 *result2,uint8 *a,uint8 b,uint32 length_a);
uint32 _xor(uint8 *result,uint8 *a,uint8 *b,uint32 length_a,uint32 length_b);
uint32 _mul(uint8 *result,uint8 *a,uint8 *b,uint32 length_a,uint32 length_b);
uint32 _mul(uint8 *result,uint8 *a,uint8 b,uint32 length_a);
void _div(uint8 *result1,uint8 *result2,uint8 *a,uint8 *b,uint32 length_a,uint32 length_b,uint32 &length_1,uint32 &length_2);
uint32 _mod(uint8 *result,uint8 *a,uint8 *b,uint32 length_a,uint32 length_b);
uint32 _gcd(uint8 *result,uint8 *a,uint8 *b,uint32 length_a,uint32 length_b);
uint32 _modadd(uint8 *result,uint8 *a,uint8 *b,uint8 *p,uint32 length_a,uint32 length_b,uint32 length_p);
uint32 _modsub(uint8 *result,uint8 *a,uint8 *b,uint8 *p,uint32 length_a,uint32 length_b,uint32 length_p);
uint32 _modmul(uint8 *result,uint8 *a,uint8 *b,uint8 *p,uint32 length_a,uint32 length_b,uint32 length_p);
uint32 _modexp(uint8 *result,uint8 *a,uint8 *b,uint8 *p,uint32 length_a,uint32 length_b,uint32 length_p);
uint32 _modexp(uint8 *result,uint8 a,uint8 *b,uint8 *p,uint32 length_b,uint32 length_p);
uint8 _modinv(uint8 *result,uint8 *a,uint8 *p,uint32 length_a,uint32 length_p,uint32 &length);
void _sr(uint8 *result,uint8 *a,uint32 length_a,uint32 length);
uint32 _bitlen(uint8 *a,uint32 length_a);
void _rand(uint8 *a,uint32 length_a);
