#pragma once

//include
#include "math.h"
#include "afxcmn.h" //HANDLE
#include "afxmt.h" //CCriticalSection
//define
#define QUEUE_LENGTH 100 //消息队列长度
#define QUEUE_BUFFER 20 //消息队列信息长度

#define TANGLE_LENGTH 1000 //tangle长度(交易数)
#define TIP_LENGTH 100 //tip长度(交易数)

#define TASK_NONE 0
#define TASK_DEVICE_INITIAL 1
#define TASK_DEVICE_CONNECT 2
#define TASK_DEVICE_MERGE 3
#define TASK_DEVICE_OPTIMIZE 4
#define TASK_DEVICE_INDEXDAG 5
#define TASK_DEVICE_WALK 6
#define TASK_DAG_TRANSACTION 7
#define TASK_DAG_VALIDATION 8

#define MATH_ROTATE(a,n) (((a)<<(n))|(((a)&0xffffffff)>>(32-(n))))
//typedef
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef unsigned __int64 uint64;
typedef char int8;
typedef short int16;
typedef long int32;
typedef __int64 int64;
//struct
struct route_t
{
	uint8 flag;//0-未连接,1-连接
	uint32 device_index;//终设备索引(类似于唯一物理地址)
	uint32 hops;//跳跃间隔
	uint32 *path;//路由路径
	route_t *next;
};
struct queue_t
{
	volatile uint32 device_index;//设备索引(类似于唯一物理地址)
	volatile uint32 info;//消息类型.0-INFO_TX,1-INFO_RX,2-INFO_DONE,3-INFO_TRANSACTION
	volatile uint8 buffer[QUEUE_BUFFER];//具体信息
};
struct transaction_t
{
	uint32 weight_self;//自身权重
	uint32 weight_accu;//累积权重
	uint32 height;//高度(至创世块)
	uint32 depth;//深度(至最远tip)
	uint32 integral;//交易积分
	//hash_t address;//地址
	//hash_t trunk;//主交易
	//hash_t branch;//从交易
	//hash_t bundle;//包
	//hash_t tag;//标签
	volatile uint8 dag;//交易状态.0-tip,1-tangle
	volatile uint8 tip;//交易是否验证.0-none,1-solid
	volatile uint8 flag;//交易类型.0-none,1-milestone
	transaction_t *trunk_point;//主交易节点指向
	transaction_t *branch_point;//从交易节点指向
};
struct device_t
{
	//device
	uint32 x;
	uint32 y;
	uint8 line;//0-在线,1-掉线
	uint32 device_index;//设备索引(类似于唯一物理地址)
	uint8 status;//0-作为free,1-作为master,2-作为slave
	route_t *route;//连接设备路由链表
	queue_t queue[QUEUE_LENGTH];//设备的消息队列(发送/接收)
	//dag
	volatile uint32 dag_index;//子链索引.0-孤立点,其他-索引号
	transaction_t tangle[TANGLE_LENGTH];//主tangle队列
	uint32 tangle_index;//主tangle索引
	transaction_t tip[TIP_LENGTH];//tip队列
	uint32 tip_index;//tip索引
	//application
	uint32 account_id;//账户id
	uint32 account_money;//账户现金
};
//function
template <class classtype>
classtype math_max(classtype a,classtype b) {return a>b?a:b;}//最大值
template <class classtype>
classtype math_min(classtype a,classtype b) {return a<b?a:b;}//最小值
void mem_swap8(uint8 *result,uint32 len);
void mem_word2byte(uint8 *a,uint32 b);
uint32 mem_byte2word(uint8 *a);
int32 math_hashcode(int8 *a,uint32 length);
double math_distance(uint32 x1,uint32 y1,uint32 x2,uint32 y2);
