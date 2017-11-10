#pragma once

//include
#include "math.h"
#include "afxcmn.h" //HANDLE
#include "afxmt.h" //CCriticalSection
//define
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
//function
template <class classtype>
void math_swap(classtype &a,classtype &b) {classtype temp=a;a=b;b=temp;}//交换
template <class classtype>
classtype math_max(classtype a,classtype b) {return a>b?a:b;}//最大值
template <class classtype>
classtype math_min(classtype a,classtype b) {return a<b?a:b;}//最小值
void mem_swap8(uint8 *result,uint32 len);
void mem_word2byte(uint8 *a,uint32 b);
uint32 mem_byte2word(uint8 *a);
int32 math_hashcode(int8 *a,uint32 length);
double math_distance(uint32 x1,uint32 y1,uint32 x2,uint32 y2);
