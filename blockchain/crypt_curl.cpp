#include "crypt_curl.h"

//public
crypt_curl::crypt_curl(void)
{
}

crypt_curl::~crypt_curl(void)
{
}

void crypt_curl::curl_init(void)
{
	memset(state,0,CURL_LENGTH*sizeof(int32));
}

void crypt_curl::curl_absort(int32 *trits,uint32 offset,uint32 length)
{
	uint32 len;

	while(1)
	{
		len=length<HASH_LENGTH ? length : HASH_LENGTH;
		memcpy(state,&trits[offset],len*sizeof(int32));
		curl_transform();
		offset+=HASH_LENGTH;
		length-=len;
		if (!length)
			break;
	}
}

void crypt_curl::curl_squeeze(int32 *trits,uint32 offset,uint32 length)
{
	uint32 len;

	while(1)
	{
		len=length<HASH_LENGTH ? length : HASH_LENGTH;
		memcpy(&trits[offset],state,len*sizeof(int32));
		curl_transform();
		offset+=HASH_LENGTH;
		length-=len;
		if (!length)
			break;
	}
}

//private
void crypt_curl::curl_transform(void)
{
	uint8 i,j;
	uint32 index[2];
	int32 temp[CURL_LENGTH];
	int8 table_truth[11]={1,0,-1,2,1,-1,0,2,-1,1,0};//according to index=a+a'*4+5 (while a in [-1,0,1]), so index in [0,10]

	index[0]=0;
	for (i=0;i<CURL_ROUND;i++)
	{
		memcpy(temp,state,CURL_LENGTH*sizeof(int32));
		for (j=0;j<CURL_LENGTH;j++)
		{
			index[1]=index[0];
			if (index[0]<=CURL_LENGTH/2)
				index[0]+=CURL_LENGTH/2;
			else
				index[0]-=CURL_LENGTH-CURL_LENGTH/2;
			state[j]=table_truth[temp[index[1]]+temp[index[0]]*4+5];
		}
	}
}