#include "crypt_sha256.h"

//public
crypt_sha256::crypt_sha256(void)
{
}

crypt_sha256::~crypt_sha256(void)
{
}

void crypt_sha256::sha256_init(void)
{
	ctx.count[0]=0;
	ctx.count[1]=0;
	ctx.state[0]=0x6a09e667;
	ctx.state[1]=0xbb67ae85;
	ctx.state[2]=0x3c6ef372;
	ctx.state[3]=0xa54ff53a;
	ctx.state[4]=0x510e527f;
	ctx.state[5]=0x9b05688c;
	ctx.state[6]=0x1f83d9ab;
	ctx.state[7]=0x5be0cd19;
	ctx.len=0;
}

void crypt_sha256::sha256_update(uint8 *data,uint32 length)
{
	uint32 i,index;
	uint8 *point;

	index=ctx.count[0]+(length<<3);
	if (index<ctx.count[0])
		ctx.count[1]++;
	ctx.count[1]+=length>>29;
	ctx.count[0]=index;
	if (ctx.len)
	{
		point=ctx.buffer;
		if (length>=SHA256_BLOCK || length+ctx.len>=SHA256_BLOCK)
		{
			memcpy(point+ctx.len,data,SHA256_BLOCK-ctx.len);
			sha256_transform(point);
			ctx.len=SHA256_BLOCK-ctx.len;
			data+=ctx.len;
			length-=ctx.len;
			ctx.len=0;
			memset(point,0,SHA256_BLOCK);
		}
		else
		{
			memcpy(point+ctx.len,data,length);
			ctx.len+=length;
			return;
		}
	}
	for (i=0;i<length/SHA256_BLOCK;i++)
	{
		sha256_transform(data);
		data+=SHA256_BLOCK;
	}
	length-=length/SHA256_BLOCK*SHA256_BLOCK;
	if (length)
	{
		ctx.len=length;
		memcpy(ctx.buffer,data,length);
	}
}

void crypt_sha256::sha256_final(uint8 *result)
{
	uint32 i,len;

	len=ctx.len;
	ctx.buffer[len]=0x80;
	len++;
	if (len>SHA256_BLOCK-8)
	{
		memset(ctx.buffer+len,0,SHA256_BLOCK-len);
		len=0;
		sha256_transform(ctx.buffer);
	}
	memset(ctx.buffer+len,0,SHA256_BLOCK-8-len);
	mem_word2byte(ctx.buffer+SHA256_BLOCK-8,ctx.count[1]);
	mem_swap8(ctx.buffer+SHA256_BLOCK-8,4);
	mem_word2byte(ctx.buffer+SHA256_BLOCK-4,ctx.count[0]);
	mem_swap8(ctx.buffer+SHA256_BLOCK-4,4);
	sha256_transform(ctx.buffer);
	ctx.len=0;
	memset(ctx.buffer,0,SHA256_BLOCK);
	for (i=0;i<32/4;i++)
	{
		mem_word2byte(&result[i<<2],ctx.state[i]);
		mem_swap8(&result[i<<2],4);
	}
}

//private
void crypt_sha256::sha256_transform(uint8 *block)//64byte
{
	uint32 i;
	static const uint32 table_k256[64]=
	{
		0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,
		0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
		0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
		0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
		0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,
		0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
		0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,
		0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
		0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
		0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
		0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,
		0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
		0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,
		0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
		0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
		0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
	};
	uint32 s0,s1,t;
	uint32 value[8],x[16];

	memcpy(value,ctx.state,sizeof(uint32)*8);
	for (i=0;i<16;i++)
	{
		x[i]=mem_byte2word(&block[i*4]);
		mem_swap8((uint8 *)&x[i],4);
	}
	SHA256_ROUND_00_15(0,value[0],value[1],value[2],value[3],value[4],value[5],value[6],value[7]);
	SHA256_ROUND_00_15(1,value[7],value[0],value[1],value[2],value[3],value[4],value[5],value[6]);
	SHA256_ROUND_00_15(2,value[6],value[7],value[0],value[1],value[2],value[3],value[4],value[5]);
	SHA256_ROUND_00_15(3,value[5],value[6],value[7],value[0],value[1],value[2],value[3],value[4]);
	SHA256_ROUND_00_15(4,value[4],value[5],value[6],value[7],value[0],value[1],value[2],value[3]);
	SHA256_ROUND_00_15(5,value[3],value[4],value[5],value[6],value[7],value[0],value[1],value[2]);
	SHA256_ROUND_00_15(6,value[2],value[3],value[4],value[5],value[6],value[7],value[0],value[1]);
	SHA256_ROUND_00_15(7,value[1],value[2],value[3],value[4],value[5],value[6],value[7],value[0]);
	SHA256_ROUND_00_15(8,value[0],value[1],value[2],value[3],value[4],value[5],value[6],value[7]);
	SHA256_ROUND_00_15(9,value[7],value[0],value[1],value[2],value[3],value[4],value[5],value[6]);
	SHA256_ROUND_00_15(10,value[6],value[7],value[0],value[1],value[2],value[3],value[4],value[5]);
	SHA256_ROUND_00_15(11,value[5],value[6],value[7],value[0],value[1],value[2],value[3],value[4]);
	SHA256_ROUND_00_15(12,value[4],value[5],value[6],value[7],value[0],value[1],value[2],value[3]);
	SHA256_ROUND_00_15(13,value[3],value[4],value[5],value[6],value[7],value[0],value[1],value[2]);
	SHA256_ROUND_00_15(14,value[2],value[3],value[4],value[5],value[6],value[7],value[0],value[1]);
	SHA256_ROUND_00_15(15,value[1],value[2],value[3],value[4],value[5],value[6],value[7],value[0]);
	for (i=16;i<64;i+=8)
	{
		SHA256_ROUND_16_63(i+0,value[0],value[1],value[2],value[3],value[4],value[5],value[6],value[7],x);
		SHA256_ROUND_16_63(i+1,value[7],value[0],value[1],value[2],value[3],value[4],value[5],value[6],x);
		SHA256_ROUND_16_63(i+2,value[6],value[7],value[0],value[1],value[2],value[3],value[4],value[5],x);
		SHA256_ROUND_16_63(i+3,value[5],value[6],value[7],value[0],value[1],value[2],value[3],value[4],x);
		SHA256_ROUND_16_63(i+4,value[4],value[5],value[6],value[7],value[0],value[1],value[2],value[3],x);
		SHA256_ROUND_16_63(i+5,value[3],value[4],value[5],value[6],value[7],value[0],value[1],value[2],x);
		SHA256_ROUND_16_63(i+6,value[2],value[3],value[4],value[5],value[6],value[7],value[0],value[1],x);
		SHA256_ROUND_16_63(i+7,value[1],value[2],value[3],value[4],value[5],value[6],value[7],value[0],x);
	}
	for (i=0;i<8;i++)
		ctx.state[i]+=value[i];
}