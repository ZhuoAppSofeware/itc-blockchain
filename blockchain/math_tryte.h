#pragma once

//include
#include "crypt_hash.h"
//define
//typedef
//struct
//function
class math_tryte
{
public:
	math_tryte(void);
	~math_tryte(void);
public:
	int8 map_byte2trits[BYTE_LENGTH][NUM_BYTE2TRITS];//value in [-1,0,1]
	int8 map_tryte2trits[TRYTE_LENGTH][NUM_TRYTE2TRITS];//value in [-1,0,1]
public:
	int64 tryte_int64(int8 *trits,uint32 offset,uint8 length);
	void tryte_trits2byte(int8 *byte,int8 *trits,uint32 offset,uint32 len);
	void tryte_byte2trits(int8 *trits,int8 *byte,uint32 len);
	void tryte_tryte2trits(int8 *trits,int8 *tryte,uint32 len);
	void tryte_trits2tryte(int8 *tryte,int8 *trits,uint32 offset,uint32 len);
};