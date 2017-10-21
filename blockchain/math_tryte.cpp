#include "math_tryte.h"

static const int8 table_tryte[TRYTE_LENGTH]={'9','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

//public
math_tryte::math_tryte(void)
{
	uint32 i,j;

	for (i=0;i<BYTE_LENGTH;i++)
		for (j=0;j<NUM_BYTE2TRITS;j++)
		{
			map_byte2trits[i][j]++;
			if (map_byte2trits[i][j]<=1)
				break;
			map_byte2trits[i][j]=-1;
		}
	for (i=0;i<TRYTE_LENGTH;i++)
		for (j=0;j<NUM_TRYTE2TRITS;j++)
		{
			map_tryte2trits[i][j]++;
			if (map_tryte2trits[i][j]<=1)
				break;
			map_tryte2trits[i][j]=-1;
		}
}

math_tryte::~math_tryte(void)
{
}

//int8 *trits:三进制位,取值[-1,0,1]
//uint8 len:三进制位个数
int64 math_tryte::tryte_int64(int8 *trits,uint32 offset,uint8 len)
{
	uint8 i;
	int64 result;

	result=0;
	for (i=0;i<len;i++)
	{
		result*=3;
		result+=trits[offset+i];
	}

	return result;
}

//int8 *byte:输出byte数组
//int8 *trits:输入三进制位,取值[-1,0,1]
//uint32 len:三进制位个数
void math_tryte::tryte_trits2byte(int8 *byte,int8 *trits,uint32 offset,uint32 len)
{
	//trits数组转化为byte数组,每5位代表1个byte
	uint32 i,j;
	int8 value;

	for (i=0;i<(len+NUM_BYTE2TRITS-1)/NUM_BYTE2TRITS;i++)
	{
		value=0;
		for (j=len-i*NUM_BYTE2TRITS<NUM_BYTE2TRITS ? len-i*NUM_BYTE2TRITS : NUM_BYTE2TRITS;j--;)
		{
			value*=3;
			value+=trits[offset+i*NUM_BYTE2TRITS+j];
		}
		byte[i]=value;
	}
}

//int8 *trits:输出三进制位,取值[-1,0,1]
//int8 *byte:输入byte数组
//uint32 len:byte数组长度
void math_tryte::tryte_byte2trits(int8 *trits,int8 *byte,uint32 len)
{
	uint32 i;

	for (i=0;i<len;i++)
		memcpy(&trits[i*NUM_BYTE2TRITS],map_byte2trits[byte[i]<0 ? byte[i]+BYTE_LENGTH : byte[i]],NUM_BYTE2TRITS*sizeof(int8));
}

//int8 *trits:输出三进制位,取值[-1,0,1]
//int8 *tryte:输入三进制字母表,取值[9ABCDEFGHIJKLMNOPQRSTUVWXYZ]
//uint32 len:三进制字母表长度
void math_tryte::tryte_tryte2trits(int8 *trits,int8 *tryte,uint32 len)
{
	uint32 i,j;

	for (i=0;i<len;i++)
	{
		for (j=0;j<TRYTE_LENGTH;j++)
			if (table_tryte[j]==tryte[i])
				break;
		memcpy(&trits[i*NUM_TRYTE2TRITS],map_tryte2trits[j],NUM_TRYTE2TRITS);
	}
}

//int8 *tryte:输出三进制字母表,取值[9ABCDEFGHIJKLMNOPQRSTUVWXYZ]
//int8 *trits:输入三进制位,取值[-1,0,1]
//uint32 len:三进制位长度
void math_tryte::tryte_trits2tryte(int8 *tryte,int8 *trits,uint32 offset,uint32 len)
{
	uint32 i,j;
	int8 value;

	for (i=0;i<(len+NUM_TRYTE2TRITS-1)/NUM_TRYTE2TRITS;i++)
	{
		value=0;
		for (j=NUM_TRYTE2TRITS;j--;)
		{
			value*=3;
			value+=trits[offset+i*NUM_TRYTE2TRITS+j];
		}
		if (value<0)
			value+=TRYTE_LENGTH;
		tryte[i]=table_tryte[value];
	}
}