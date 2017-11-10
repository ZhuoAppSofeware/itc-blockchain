#include "layer_dag.h"

extern volatile uint8 g_task;//传递给线程的过程标记
extern volatile uint32 g_index;//临时用来统计交易号码的(以后会用hash_t代替,计数从1开始)
extern CRITICAL_SECTION g_cs;
extern device_t *g_device;//设备数组

void print_return(device_t *device,uint8 task,uint8 flag)
{
	CString string;

	switch(flag)
	{
	case 0:
		string="ok";
		break;
	case RET_QUEUE_EMPTY:
		string="queue empty";
		break;
	case RET_QUEUE_FULL:
		string="queue full";
		break;
	case RET_TRANSACTION_EMPTY:
		string="transaction empty";
		break;
	case RET_TRANSACTION_FULL:
		string="transaction full";
		break;
	case RET_TRANSACTION_NONE:
		string="transaction none";
		break;
	case RET_TANGLE_FULL:
		string="tangle full";
		break;
	case RET_TANGLE_NONE:
		string="tangle none";
		break;
	}
	printf("thread_device%ld task%d %s\r\n",device->device_index,task,string);
}

void key_generate(device_t *device)
{
	uint8 flag;
	rsa_para para;

	device->pair[0]=0x01;
	device->pair[1]=0x00;
	device->pair[2]=0x01;
	device->pair[3]=0x00;
	para.le=4;
	para.len=KEY_LEN;
	para.lr=MASK_LEN;
	para.e=device->pair;
	para.n=&device->pair[para.le];
	para.d=&device->pair[para.le+para.len];
	para.p=&device->pair[para.le+para.len*2];
	para.q=&device->pair[para.le+para.len*2+para.len/2];
	para.dp=&device->pair[para.le+para.len*2+para.len/2*2];
	para.dq=&device->pair[para.le+para.len*2+para.len/2*3];
	para.qp=&device->pair[para.le+para.len*2+para.len/2*4];
	while(1)
	{
		_rand(para.p,para.len/2);
		_rand(para.q,para.len/2);
		flag=rsa_genkey(&para,RSA_CRT);
		if (!flag)
			break;
	}
}

void route_fillkey(device_t *device)
{
	route_t *route;
	rsa_para para;
	key_t key;

	para.le=4;
	para.len=KEY_LEN;
	para.e=device->pair;
	para.n=&device->pair[para.le];
	//
	key.device_index=device->device_index;
	memcpy(key.buffer,para.e,para.le);
	memcpy(&key.buffer[para.le],para.n,para.len);
	memcpy(&device->key[device->key_index],&key,sizeof(key_t));
	device->key_index++;
	//
	route=device->route;
	while(route)
	{
		memcpy(&g_device[route->device_index].key[g_device[route->device_index].key_index],&key,sizeof(key_t));
		g_device[route->device_index].key_index++;
		route=route->next;
	}
}

uint8 transaction_search(uint32 &trunk,uint32 &branch,device_t *device)
{
	uint32 i;
	uint32 count;

	//search tip(random algorithm),原则上需要使用手动构造出较宽的tangle(判断tip),这里我使用自动构造tangle(判断solid)
	if (!device->tangle_index)//genesis
		return 1;
	if (device->tangle_index==1)//point to genesis
	{
		trunk=0;
		branch=0;
	}
	else
	{
		//find tip's index
		count=0;
		for (i=0;i<device->tangle_index;i++)
			if (device->tangle[i].flag!=TRANSACTION_SOLID)
				count++;
		while(1)
		{
			trunk=rand()%count;
			branch=rand()%count;
			if (trunk!=branch)
				break;
		}
		//find tip's hash
		count=0;
		for (i=0;i<device->tangle_index;i++)
			if (device->tangle[i].flag!=TRANSACTION_SOLID)
			{
				if (trunk==count)
					trunk=i;
				if (branch==count)
					branch=i;
				count++;
			}
	}

	return 0;
}

uint8 transaction_verify(device_t *device,transaction_t *transaction)
{
	//通过rsa公钥验签验证交易
	uint32 i;
	rsa_para para;
	uint8 result[KEY_LEN];

	para.le=4;
	para.len=KEY_LEN;
	for (i=0;i<device->key_index;i++)
		if (device->key[i].device_index==transaction->device_index)
			break;
	para.e=device->key[i].buffer;
	para.n=&device->key[i].buffer[para.le];
	i=rsa_enc(result,transaction->cipher,para.len,&para);
	memset(&result[i],0,para.len-i);
	if (memcmp(result,transaction->plain,para.len))
		return 1;

	return 0;
}

uint32 transaction_pow(device_t *device,transaction_t *transaction)
{
	//通过sha256计算hash pow
	uint64 i;
	uint32 length;
	crypt_sha256 *sha256;
	uint8 content[KEY_LEN+1],result[HASH_LEN];

	length=KEY_LEN;
	memcpy(content,transaction->plain,length);
	sha256=new crypt_sha256;
	for (i=1;i<0x100000000;i++)
	{
		length=_add(content,content,1,length);
		sha256->sha256_init();
		sha256->sha256_update(content,length);
		sha256->sha256_final(result);
		if (_bitlen(result,HASH_LEN)<=HASH_LEN*8-COMPARE_LEN)
			break;
	}
	if (i==0x100000000)
		i=0;
	delete sha256;

	return (uint32)i;
}

void transaction_signature(transaction_t *transaction,device_t *device)
{
	//通过rsa私钥签名加密交易
	uint32 i;
	rsa_para para;

	para.le=4;
	para.len=KEY_LEN;
	para.lr=MASK_LEN;
	para.d=&device->pair[para.le+para.len];
	para.p=&device->pair[para.le+para.len*2];
	para.q=&device->pair[para.le+para.len*2+para.len/2];
	para.dp=&device->pair[para.le+para.len*2+para.len/2*2];
	para.dq=&device->pair[para.le+para.len*2+para.len/2*3];
	para.qp=&device->pair[para.le+para.len*2+para.len/2*4];
	i=rsa_dec(transaction->cipher,transaction->plain,para.len,&para,RSA_CRT);
	memset(&transaction->cipher[i],0,para.len-i);
}

uint8 transaction_generate(device_t *device,uint32 *pow)
{
	uint32 i;
	route_t *route;
	transaction_t transaction;

	if (device->transaction_index==TRANSACTION_LENGTH)//transaction array full
		return RET_TRANSACTION_FULL;
	if (device->queue_index==QUEUE_LENGTH)//queue full
		return RET_QUEUE_FULL;
	//generate transaction
	transaction.device_index=device->device_index;
	g_index++;
	transaction.index.code=g_index;
	transaction.weight_self=0;
	transaction.weight_accu=0;
	transaction.height=0;
	transaction.depth=0;
	transaction.integral=0;
	transaction.type=TRANSACTION_TYPE_VALUE;//rand()%2;
	transaction.flag=TRANSACTION_NONE;
	memset(&transaction.trunk,0,sizeof(hash_t));
	memset(&transaction.branch,0,sizeof(hash_t));
	_rand(transaction.plain,KEY_LEN);
	i=_mod(transaction.plain,transaction.plain,&device->pair[4],KEY_LEN,KEY_LEN);
	memset(&transaction.plain[i],0,KEY_LEN-i);
	if (transaction.type==TRANSACTION_TYPE_VALUE)
		transaction_signature(&transaction,device);
	memcpy(transaction.pow,pow,2*sizeof(uint32));
	device->queue[device->queue_index].device_index=device->device_index;
	device->queue[device->queue_index].info=INFO_TRANSACTION;
	memcpy((void *)device->queue[device->queue_index].buffer,&transaction,sizeof(transaction_t));
	//send transaction
	route=device->route;
	while(route)
	{
		if (g_device[route->device_index].queue_index==QUEUE_LENGTH)
		{
			route=route->next;
			continue;
		}
		g_device[route->device_index].queue[g_device[route->device_index].queue_index].device_index=device->device_index;
		g_device[route->device_index].queue[g_device[route->device_index].queue_index].info=INFO_TRANSACTION;
		memcpy((void *)g_device[route->device_index].queue[g_device[route->device_index].queue_index].buffer,(void *)device->queue[device->queue_index].buffer,sizeof(transaction_t));
		g_device[route->device_index].queue_index++;
		route=route->next;
	}
	device->queue_index++;

	return 0;
}

uint8 transaction_recv(device_t *device)
{
	uint32 i;

	if (!device->queue_index)//queue empty
		return RET_QUEUE_EMPTY;
	for (i=0;i<device->queue_index;i++)
		if (device->queue[i].info==INFO_TRANSACTION)
			break;
	if (i==device->queue_index)//no transaction in queue
		return RET_TRANSACTION_NONE;
	if (device->transaction_index==TRANSACTION_LENGTH)//transaction array full
		return RET_TRANSACTION_FULL;
	//update transaction
	memcpy(&device->transaction[device->transaction_index],(void *)device->queue[i].buffer,sizeof(transaction_t));
	device->transaction_index++;
	//reset queue
	device->queue_index--;
	for (;i<device->queue_index;i++)
		memcpy(&device->queue[i],&device->queue[i+1],sizeof(queue_t));
	memset(&device->queue[device->queue_index],0,sizeof(queue_t));

	return 0;
}

uint8 tangle_join(device_t *device,uint32 trunk,uint32 branch)
{
	//将transaction队列的一项加入tangle队列
	uint32 i;
	route_t *route;
	
	if (!device->transaction_index)//transaction array empty
		return RET_TRANSACTION_EMPTY;
	if (device->tangle_index==TANGLE_LENGTH)//tangle array full
		return RET_TANGLE_FULL;
	//join tangle
	memcpy(&device->tangle[device->tangle_index],&device->transaction[0],sizeof(transaction_t));
	device->tangle[device->tangle_index].flag=TRANSACTION_TIP;
	if (device->tangle_index)
	{
		memcpy(&device->tangle[device->tangle_index].trunk,&device->tangle[trunk].index,sizeof(hash_t));
		memcpy(&device->tangle[device->tangle_index].branch,&device->tangle[branch].index,sizeof(hash_t));
		device->tangle[trunk].flag=TRANSACTION_TANGLE;
		device->tangle[branch].flag=TRANSACTION_TANGLE;
	}
	//reset transaction
	device->transaction_index--;
	for (i=0;i<device->transaction_index;i++)
		memcpy(&device->transaction[i],&device->transaction[i+1],sizeof(transaction_t));
	memset(&device->transaction[device->transaction_index],0,sizeof(transaction_t));
	//send tangle
	route=device->route;
	while(route)
	{
		if (g_device[route->device_index].queue_index==QUEUE_LENGTH)
		{
			route=route->next;
			continue;
		}
		g_device[route->device_index].queue[g_device[route->device_index].queue_index].device_index=device->device_index;
		g_device[route->device_index].queue[g_device[route->device_index].queue_index].info=INFO_TANGLE;
		memcpy((void *)g_device[route->device_index].queue[g_device[route->device_index].queue_index].buffer,&device->tangle[device->tangle_index],sizeof(transaction_t));
		g_device[route->device_index].queue_index++;
		route=route->next;
	}
	device->tangle_index++;

	return 0;
}

uint8 tangle_check(device_t *device,transaction_t *transaction)
{
	//检查tangle中是否有存在参照的transaction
	uint32 i;

	if (!device->tangle_index)//tangle empty
		return 0;
	for (i=0;i<device->tangle_index;i++)
		if (!memcmp(&device->tangle[i].index,&transaction->index,sizeof(hash_t)))
			break;
	if (i==device->tangle_index)//no same transaction in tangle
		return 1;

	return 0;
}

uint8 tangle_recv(device_t *device)
{
	uint32 i;
	uint8 flag;

	if (!device->queue_index)//queue empty
		return RET_QUEUE_EMPTY;
	if (device->tangle_index==TANGLE_LENGTH)//tangle array full
		return RET_TANGLE_FULL;
	for (i=0;i<device->queue_index;i++)
		if (device->queue[i].info==INFO_TANGLE)
			break;
	if (i==device->queue_index)//no tangle in queue
		return RET_TANGLE_NONE;
	//check tangle's existance
	flag=tangle_check(device,(transaction_t *)device->queue[i].buffer);
	if (flag)//no transaction in tangle
	{
		//update tangle
		memcpy(&device->tangle[device->tangle_index],(void *)device->queue[i].buffer,sizeof(transaction_t));
		device->tangle_index++;
	}
	//reset queue
	device->queue_index--;
	for (;i<device->queue_index;i++)
		memcpy(&device->queue[i],&device->queue[i+1],sizeof(queue_t));
	memset(&device->queue[device->queue_index],0,sizeof(queue_t));

	return 0;
}

uint8 process_dag(device_t *device)
{
	static __declspec(thread) uint8 task;
	static __declspec(thread) uint32 trunk,branch;
	uint32 pow[2];
	uint8 flag;

	switch(g_task)
	{
	//1.if new device,build up current dag
	case TASK_DAG_INITIAL:
		task=0;
		EnterCriticalSection(&g_cs);
		key_generate(device);
		route_fillkey(device);
		LeaveCriticalSection(&g_cs);
		break;
	//1.generate transaction by random(一种是输入签名,一种是传输信息). broadcast
	//2.tangle join(search tip). broadcast
	//3.transaction validation:if most of tip reference to old transactions, then solid.validation->broadcast->waiting for device's response,synch device's queue*
	//4.ledger validation:check each solid transactions, then milestone. broadcast
	case TASK_DAG_TANGLE:
		while(1)
		{
			switch(task)
			{
			case 0://search tip/verify/generate transaction and broadcast
				EnterCriticalSection(&g_cs);
				flag=transaction_search(trunk,branch,device);
				if (!flag)
				{
					flag=transaction_verify(device,&device->tangle[trunk]);
					if (flag)
					{
						LeaveCriticalSection(&g_cs);
						break;
					}
					flag=transaction_verify(device,&device->tangle[branch]);
					if (flag)
					{
						LeaveCriticalSection(&g_cs);
						break;
					}
					pow[0]=transaction_pow(device,&device->tangle[trunk]);
					pow[1]=transaction_pow(device,&device->tangle[branch]);
				}
				else
					pow[0]=pow[1]=0;
				flag=transaction_generate(device,pow);
				//if (flag)
				//	print_return(device,task,flag);
				LeaveCriticalSection(&g_cs);
				break;
			case 1://recv transaction
				EnterCriticalSection(&g_cs);
				flag=transaction_recv(device);
				//if (flag)
				//	print_return(device,task,flag);
				LeaveCriticalSection(&g_cs);
				break;
			case 2://join tangle and broadcast
				EnterCriticalSection(&g_cs);
				flag=tangle_join(device,trunk,branch);
				//if (flag)
				//	print_return(device,task,flag);
				LeaveCriticalSection(&g_cs);
				break;
			case 3://recv tangle
				EnterCriticalSection(&g_cs);
				flag=tangle_recv(device);
				//if (flag)
				//	print_return(device,task,flag);
				LeaveCriticalSection(&g_cs);
				break;
			}
			//EnterCriticalSection(&g_cs);
			//printf("thread_device%ld task=%d\r\n",device->device_index,task);
			//LeaveCriticalSection(&g_cs);
			task++;
			if (task==4)
				task=0;
			//Sleep(3000);
			//while(1);
		}
		break;
	}

	return 0;
}