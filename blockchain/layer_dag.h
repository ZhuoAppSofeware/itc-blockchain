#pragma once

//include
#include "layer.h"
#include "crypt_rsa.h"
#include "crypt_sha256.h"
//define
#define TRANSACTION_TYPE_NORMAL 0 //普通交易
#define TRANSACTION_TYPE_VALUE 1 //有价交易(需输入签名)

#define TRANSACTION_NONE 0 //未交易认证transaction
#define TRANSACTION_TIP 1 //已链入tangle成为tip
#define TRANSACTION_TANGLE 2 //已链入tangle并被其他交易指引
#define TRANSACTION_SOLID 3 //已交易认证transaction
#define TRANSACTION_MILESTONE 4 //已账本验证milestone

#define RET_QUEUE_EMPTY 1
#define RET_QUEUE_FULL 2
#define RET_TRANSACTION_EMPTY 3
#define RET_TRANSACTION_FULL 4
#define RET_TRANSACTION_NONE 5
#define RET_TANGLE_FULL 6
#define RET_TANGLE_NONE 7

#define MASK_LEN 0 //指数盲化字节数
#define HASH_LEN 32 //哈希字节数
#define COMPARE_LEN 4 //比较位数
#define CACHE_LEN 10 //cache节点个数
//typedef
//struct
//function
uint8 process_dag(device_t *device);