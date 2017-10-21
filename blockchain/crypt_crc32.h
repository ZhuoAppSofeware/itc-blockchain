#pragma once //crc32À„∑® µœ÷

//include
#include "include.h"
//define
#define CRC32_POLYNOMIAL 0xedb88320
//struct
//function
class crypt_crc32
{
public:
	crypt_crc32(void);
	~crypt_crc32(void);
public:
	uint32 crc32_compute(uint8 *buf,uint32 len);
private:
	uint32 table_crc32[256];
};