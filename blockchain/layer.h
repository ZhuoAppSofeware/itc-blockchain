#pragma once

//include
#include "include.h"
#include "crypt_hash.h"
//define
#define DEVICE_LENGTH 100 //预估设备最大数
#define QUEUE_LENGTH 100 //消息队列长度
#define TRANSACTION_LENGTH 100 //transaction长度(交易数)
#define TANGLE_LENGTH 1000 //tangle长度(交易数)

#define QUEUE_LEN 80 //消息队列信息长度
#define KEY_LEN 4 //密钥对字节数

#define INFO_TX 0
#define INFO_RX 1
#define INFO_KEY 2
#define INFO_TRANSACTION 3
#define INFO_TANGLE 4
#define INFO_VALID_TRANSACTION 5
#define INFO_VALID_LEDGER 6

#define TASK_NONE 0
#define TASK_DEVICE_INITIAL 1
#define TASK_DEVICE_CONNECT 2
#define TASK_DEVICE_MERGE 3
#define TASK_DEVICE_OPTIMIZE 4
#define TASK_DEVICE_INDEXDAG 5
#define TASK_DEVICE_WALK 6
#define TASK_DAG_INITIAL 7
#define TASK_DAG_TANGLE 8
//typedef
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
	volatile uint32 info;//消息类型.0-INFO_TX,1-INFO_RX,2-INFO_TRANSACTION,3-INFO_TANGLE,4-INFO_VALID_TRANSACTION,5-INFO_VALID_LEDGER
	volatile uint8 buffer[QUEUE_LEN];//具体信息
};
struct key_t
{
	uint32 device_index;//设备索引(类似于唯一物理地址)
	uint8 buffer[65*4];//公钥rsa=1(e)+64(n)=65 word
};
struct transaction_t
{
	volatile uint32 device_index;//设备索引(类似于唯一物理地址)
	hash_t index;//交易索引
	uint32 weight_self;//自身权重
	uint32 weight_accu;//累积权重
	uint32 height;//高度(至创世块)
	uint32 depth;//深度(至最远tip)
	uint32 integral;//交易积分
	uint32 nonce;//临时随机数(证明是从当前device发出,防止女巫攻击和批量交易攻击)
	uint8 type;//交易类型.0-普通信息,1-有价信息
	uint8 plain[KEY_LEN];//明文验证
	uint8 cipher[KEY_LEN];//密文验证
	uint32 pow[2];//按计算规则得到的前序trunk/branch的pow值
	//hash_t address;//地址
	//hash_t trunk;//主交易
	//hash_t branch;//从交易
	//hash_t bundle;//包
	//hash_t tag;//标签
	volatile uint8 flag;//交易状态.0-none,1-solid,2-tangle,3-milestone
	hash_t trunk;//主交易节点->交易索引
	hash_t branch;//从交易节点->交易索引
};
struct device_t
{
	//device
	uint32 x;
	uint32 y;
	uint8 node;//0-重节点(包含全账本),1-轻节点(无账本)
	uint8 line;//0-在线,1-掉线
	uint32 device_index;//设备索引(类似于唯一物理地址)
	uint8 status;//0-作为free,1-作为master,2-作为slave
	route_t *route;//连接设备路由链表
	queue_t queue[QUEUE_LENGTH];//设备的消息队列(发送/接收)
	uint32 queue_index;//queue索引长度
	//dag
	volatile uint32 dag_index;//子链索引.0-孤立点,其他-索引号
	transaction_t tangle[TANGLE_LENGTH];//主tangle队列
	volatile uint32 tangle_index;//主tangle索引长度
	transaction_t transaction[TRANSACTION_LENGTH];//transaction队列
	volatile uint32 transaction_index;//transaction索引长度
	key_t key[DEVICE_LENGTH];//key公钥队列
	volatile uint32 key_index;//key索引长度
	uint8 pair[300*4];//密钥对rsa=1(e)+64(n)+64(d)+32(p)+32(q)+32(dp)+32(dq)+32(qp)=289 word
	//application
	uint32 account_id;//账户id
	uint32 account_money;//账户现金
};
//function
