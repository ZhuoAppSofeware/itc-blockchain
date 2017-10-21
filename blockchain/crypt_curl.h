#pragma once

//include
#include "crypt_hash.h"
//define
#define CURL_LENGTH (HASH_LENGTH*3) //243*3
#define CURL_ROUND 27 //±ä»»ÂÖÊý
//typedef
//struct
//function
class crypt_curl
{
public:
	crypt_curl(void);
	~crypt_curl(void);
public:
	int32 state[CURL_LENGTH];
public:
	void curl_init(void);
	void curl_absort(int32 *trits,uint32 offset,uint32 length);
	void curl_squeeze(int32 *trits,uint32 offset,uint32 length);
private:
	void curl_transform(void);
};