#include "layer_mainchain.h"

extern uint32 g_devicenum;//设备个数
extern uint32 g_devicerange;//设备坐标范围
extern uint32 g_devicestep;//设备步进值
extern CRITICAL_SECTION g_cs;
extern device_t *g_device;//设备数组
extern volatile uint8 g_task;//传递给线程的过程标记
extern volatile uint8 *g_init;//每个线程的初始化任务.0-未初始化,1-已初始化

uint8 process_mainchain(void)
{
	uint32 i,j;
	route_t *route;
	uint32 device_index;

	switch(g_task)
	{
	case TASK_DEVICE_INITIAL:
		break;
	//update device's property:0-device visible(master/slave),1-device invisible(free)
	//1.when is free device:->master then turn to slave;->free' then free' turn to slave;->slave then (connect its master turn to slave,else turn to master)
	//2.when is master device:->free then free turn to slave;->slave not connect;->master connect
	//3.when is slave device:->free/master/slave connect
	case TASK_DEVICE_CONNECT:
		if (!g_init[device->device_index])
		{
			EnterCriticalSection(&g_cs);
			device_index=device_seek(device);
			LeaveCriticalSection(&g_cs);
			g_init[device->device_index]=1;
			if (device_index==device->device_index)//no device visible
				return 1;
		}
		switch(g_device[device->device_index].queue[0].info)
		{
		case INFO_TX:
			route=route_check(device);
			if (!route)
				return 0;
			route_mark(device,route->device_index);
			switch(device->status)
			{
			case DEVICE_STATUS_FREE:
				device->status=g_device[route->device_index].status==DEVICE_STATUS_MASTER ? DEVICE_STATUS_SLAVE : DEVICE_STATUS_MASTER;
				break;
			case DEVICE_STATUS_MASTER:
				break;
			case DEVICE_STATUS_SLAVE:
				break;
			}
			InterlockedExchange((LPLONG)&g_device[device->device_index].queue[0].device_index,device->device_index);
			InterlockedExchange((LPLONG)&g_device[device->device_index].queue[0].info,INFO_TX);
			InterlockedExchange((LPLONG)&g_device[route->device_index].queue[0].device_index,device->device_index);
			InterlockedExchange((LPLONG)&g_device[route->device_index].queue[0].info,INFO_RX);
			break;
		case INFO_RX:
			switch(device->status)
			{
			case DEVICE_STATUS_FREE:
				device->status=DEVICE_STATUS_SLAVE;
				break;
			case DEVICE_STATUS_MASTER:
				break;
			case DEVICE_STATUS_SLAVE:
				break;
			}
			device_index=g_device[device->device_index].queue[0].device_index;
			route_mark(device,device_index);
			InterlockedExchange((LPLONG)&g_device[device->device_index].queue[0].device_index,device->device_index);
			InterlockedExchange((LPLONG)&g_device[device->device_index].queue[0].info,INFO_TX);
			InterlockedExchange((LPLONG)&g_device[device_index].queue[0].device_index,device->device_index);
			InterlockedExchange((LPLONG)&g_device[device_index].queue[0].info,INFO_RX);
			if (!route_check(device))
				return 0;
			break;
		}
		break;
	//1.recursive seek reachable device
	//2.update path with shortest hops
	case TASK_DEVICE_MERGE:
		if (!g_init[device->device_index])
		{
			if (device->status==DEVICE_STATUS_FREE)
				return 1;
			EnterCriticalSection(&g_cs);
			device_recurse(device);
			LeaveCriticalSection(&g_cs);
			g_init[device->device_index]=1;
		}
		switch(g_device[device->device_index].queue[0].info)
		{
		case INFO_TX:
			route=route_check(device);
			if (!route)
				return 0;
			route_mark(device,route->device_index);
			InterlockedExchange((LPLONG)&g_device[device->device_index].queue[0].device_index,device->device_index);
			InterlockedExchange((LPLONG)&g_device[device->device_index].queue[0].info,INFO_TX);
			InterlockedExchange((LPLONG)&g_device[route->device_index].queue[0].device_index,device->device_index);
			InterlockedExchange((LPLONG)&g_device[route->device_index].queue[0].info,INFO_RX);
			break;
		case INFO_RX:
			device_index=g_device[device->device_index].queue[0].device_index;
			route_mark(device,device_index);
			InterlockedExchange((LPLONG)&g_device[device->device_index].queue[0].device_index,device->device_index);
			InterlockedExchange((LPLONG)&g_device[device->device_index].queue[0].info,INFO_TX);
			InterlockedExchange((LPLONG)&g_device[device_index].queue[0].device_index,device->device_index);
			InterlockedExchange((LPLONG)&g_device[device_index].queue[0].info,INFO_RX);
			if (!route_check(device))
				return 0;
			break;
		}
		break;
	//1.update path with shortest hops
	//2.reduce piconets
	//3.delete unnecessary bridges
	case TASK_DEVICE_OPTIMIZE:
		/*
		if (g_device[i].status==DEVICE_STATUS_SLAVE)
		{
			route=g_device[i].route;
			while(route)
			{
				if (g_device[route->device_index].status==DEVICE_STATUS_MASTER && math_distance(device->x,device->y,g_device[route->device_index].x,g_device[route->device_index].y)<=MAX_METRIC)
					return route->device_index;
				route=route->next;
			}
		}*/
		break;
	//1.index dag by same device set
	case TASK_DEVICE_INDEXDAG:
		if (device->status==DEVICE_STATUS_FREE)
			return 1;
		EnterCriticalSection(&g_cs);
		switch(g_device[device->device_index].queue[0].info)
		{
		case INFO_TX:
			device->dag_index=device->queue[0].buffer[0] ? device->queue[0].buffer[0] : 1;
			for (i=0;i<g_devicenum;i++)
			{
				if (i==device->device_index || g_device[g_device[i].device_index].queue[0].info==INFO_RX)
					continue;
				route=route_exist(device,g_device[i].device_index);
				j=device->dag_index;
				if (route)
					g_device[g_device[i].device_index].queue[0].info=INFO_RX;
				else
					j++;
				g_device[i].queue[0].buffer[0]=(uint8)j;
			}
			break;
		case INFO_RX:
			device->dag_index=device->queue[0].buffer[0];
			break;
		}
		LeaveCriticalSection(&g_cs);
		break;
	case TASK_DEVICE_WALK:
		if (!g_init[device->device_index])
		{
			EnterCriticalSection(&g_cs);
			device_location(device);
			device_release(device);
			LeaveCriticalSection(&g_cs);
			g_init[device->device_index]=1;
		}
		break;
	}

	return 0;
}