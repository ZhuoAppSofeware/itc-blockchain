#pragma once

//include
#include "crypt_hash.h"
#include "layer_device.h"
//define
#define DAG_TIP 0 //孤立于dag外
#define DAG_TANGLE 1 //已链入dag中

#define TIP_NONE 0 //tip不可靠(未经历交易验证)
#define TIP_SOLID 1 //tip可靠(经历交易验证)

#define MILESTONE_NONE 0 //milestone不可靠(未经历账本验证)
#define MILESTONE_SOLID 1 //milestone可靠(经历账本验证)

#define CACHE_LENGTH 10 //cache节点个数
//typedef
//struct
//function
uint8 process_dag(device_t *device);