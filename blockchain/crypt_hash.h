#pragma once

//include
#include "include.h"
//define
#define NUM_BYTE2TRITS 5 //243 represent in 5 bits(0-242)
#define NUM_TRYTE2TRITS 3 //27 represent in 3 bits(0-26)

#define BYTE_LENGTH 243 //pow(3,NUM_BYTE2TRITS)=243
#define TRYTE_LENGTH 27 //pow(3,NUM_TRYTE2TRITS)=27=1+26
#define HASH_LENGTH BYTE_LENGTH //243

#define HASH_TRITSLENGTH HASH_LENGTH //243
#define HASH_BYTELENGTH (HASH_LENGTH/NUM_BYTE2TRITS+1) //243/5+1=49
#define HASH_TRYTELENGTH (HASH_LENGTH/NUM_TRYTE2TRITS) //243/3=81
//typedef
//struct
struct hash_t
{
	int8 trits[HASH_TRITSLENGTH];//(-1,0,1)
	int8 byte[HASH_BYTELENGTH];//
	int8 tryte[HASH_TRYTELENGTH];//string
	int32 code;//hash code
};
//function