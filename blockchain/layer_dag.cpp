#include "layer_dag.h"

extern volatile uint8 g_task;//传递给线程的过程标记

uint8 process_dag(device_t *device)
{
//	route_t *route;
//	uint32 device_index;

	switch(g_task)
	{
	//1.generate transaction by random
	//2.search tip and broadcast
	case TASK_DAG_TRANSACTION:
		//generate transaction
		if (rand()%2)
		{
			//device->

		}
		/*
		switch(g_infotype[device->device_index])
		{
		case INFO_TX:
			route=route_check(device);
			if (!route)
				return 0;
			route_mark(device,route->device_index);
			InterlockedExchange((LPLONG)&g_infodata[device->device_index],device->device_index);
			InterlockedExchange((LPLONG)&g_infotype[device->device_index],INFO_TX);
			InterlockedExchange((LPLONG)&g_infodata[route->device_index],device->device_index);
			InterlockedExchange((LPLONG)&g_infotype[route->device_index],INFO_RX);
			break;
		case INFO_RX:
			device_index=g_infodata[device->device_index];
			route_mark(device,device_index);
			InterlockedExchange((LPLONG)&g_infodata[device->device_index],device->device_index);
			InterlockedExchange((LPLONG)&g_infotype[device->device_index],INFO_TX);
			InterlockedExchange((LPLONG)&g_infodata[device_index],device->device_index);
			InterlockedExchange((LPLONG)&g_infotype[device_index],INFO_RX);
			if (!route_check(device))
				return 0;
			break;
		}*/
		break;
	//1.transaction validation:validation->broadcast->waiting for device's response,synch device's queue
	case TASK_DAG_VALIDATION:
		/*
		if (!g_init[device->device_index])
		{
			EnterCriticalSection(&g_cs);
			device_index=device_seek(device);
			LeaveCriticalSection(&g_cs);
			g_init[device->device_index]=1;
			if (device_index==device->device_index)//no device visible
				return 1;
		}*/
		break;
	}

	return 0;
}