#include "crypt_crc32.h"

//CRC8 = X8+X5+X4+1
//CRC-CCITT = X16+X12+X5+1
//CRC16=X16+X15+X2+1
//CRC12=X12+X11+X3+X2+1
//CRC32=X32+X26+X23+X22+X16+X12+X11+X10+X8+X7+X5+X4+X2+X+1
crypt_crc32::crypt_crc32(void)
{
	uint32 i,j;

	for (i=0;i<256;i++)
	{
		table_crc32[i]=i;
		for (j=0;j<8;j++)
			table_crc32[i]=(table_crc32[i]>>1)^(table_crc32[i] & 0x1 ? CRC32_POLYNOMIAL : 0);
	}
}

crypt_crc32::~crypt_crc32(void)
{
}

uint32 crypt_crc32::crc32_compute(uint8 *buf,uint32 len)
{
	uint32 i;
	uint32 result;

	result=0xffffffff;
	for (i=0;i<len;i++)
		result=(result>>8)^table_crc32[(result^buf[i]) & 0xff];
	result=~result;

    return result;
}