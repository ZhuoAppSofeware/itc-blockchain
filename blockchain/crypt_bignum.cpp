#include "crypt_bignum.h"

void _len(uint8 *a,uint32 &length)
{
	while(length)
	{
		if (a[length-1])
			break;
		length--;
	}
	if (!length)
		length=1;
}

int8 _cmp(uint8 *a,uint8 *b,uint32 length_a,uint32 length_b)
{
	int32 i;

	_len(a,length_a);
	_len(b,length_b);
	if (length_a<length_b)
		return -1;
	if (length_a>length_b)
		return 1;
	for (i=length_a-1;i>=0;i--)
	{
		if (a[i]<b[i])
			return -1;
		if (a[i]>b[i])
			return 1;
	}
	
	return 0;
}

int8 _cmp(uint8 *a,uint8 b,uint32 length_a)
{
	_len(a,length_a);
	if (length_a>1)
		return 1;
	if (a[0]<b)
		return -1;
	if (a[0]>b)
		return 1;
	
	return 0;
}

//uint8 *result:和+进位,max(length_a,length_b)+1
//uint8 *a:加数,length_a
//uint8 *b:被加数,length_b
//result/a/b可以同一地址
uint32 _add(uint8 *result,uint8 *a,uint8 *b,uint32 length_a,uint32 length_b)
{
	uint32 i;
	uint8 j;
	uint16 temp;
	uint8 flag;//0:length_a<length_b,1:length_a>=length_b
	uint32 length;

	_len(a,length_a);
	_len(b,length_b);
	if (length_a<length_b)
	{
		flag=0;
		math_swap(length_a,length_b);
	}
	else
		flag=1;
	j=0;
	for (i=0;i<length_a;i++)
	{
		if (flag)
		{
			temp=(uint16)a[i];
			if (i<length_b)
				temp+=(uint16)b[i];
		}
		else
		{
			temp=(uint16)b[i];
			if (i<length_b)
				temp+=(uint16)a[i];
		}
		temp+=j;
		result[i]=(uint8)temp;
		j=(uint8)(temp>>8);
	}
	length=length_a;
	if (j)
	{
		result[length_a]=1;
		length++;
	}
	_len(result,length);

	return length;
}

//uint8 *result:和+进位,length_a+1
//uint8 *a:加数,length_a
//uint8 b:被加数,1
//result/a可以同一地址,进位预先清0
uint32 _add(uint8 *result,uint8 *a,uint8 b,uint32 length_a)
{
	uint32 i;
	uint8 j;
	uint16 temp;
	uint32 length;

	_len(a,length_a);
	j=0;
	for (i=0;i<length_a;i++)
	{
		temp=(uint16)a[i];
		if (i<1)
			temp+=(uint16)b;
		temp+=j;
		result[i]=(uint8)temp;
		j=(uint8)(temp>>8);
	}
	length=length_a;
	if (j)
	{
		result[length_a]=1;
		length++;
	}
	_len(result,length);

	return length;
}

//uint8 *result:和+进位,length_a+1
//uint8 *a:加数,length_a
//uint8 b:被加数,1
//result/a可以同一地址,进位累加
uint32 _adc(uint8 *result,uint8 *a,uint8 b,uint32 length_a)
{
	uint32 i;
	uint8 j;
	uint16 temp;
	uint32 length;

	_len(a,length_a);
	j=0;
	for (i=0;i<length_a;i++)
	{
		temp=(uint16)a[i];
		if (i<1)
			temp+=(uint16)b;
		temp+=j;
		result[i]=(uint8)temp;
		j=(uint8)(temp>>8);
	}
	length=length_a;
	if (j)
	{
		result[length_a]++;
		length++;
	}
	_len(result,length);

	return length;
}

//uint8 *result1:差,max(length_a,length_b)
//uint8 *result2:借位,1
//uint8 *a:减数,length_a
//uint8 *b:被减数,length_b
//result1/a/b可以同一地址
uint32 _sub(uint8 *result1,uint8 *result2,uint8 *a,uint8 *b,uint32 length_a,uint32 length_b)
{
	uint32 i;
	uint16 temp;
	uint8 flag;//0:length_a<length_b,1:length_a>=length_b
	uint32 length;

	_len(a,length_a);
	_len(b,length_b);
	if (length_a<length_b)
	{
		flag=0;
		math_swap(length_a,length_b);
	}
	else
		flag=1;
	*result2=0;
	for (i=0;i<length_a;i++)
	{
		if (flag)
		{
			temp=(uint16)a[i];
			if (i<length_b)
				temp-=(uint16)b[i];
		}
		else
		{
			temp=-(uint16)b[i];
			if (i<length_b)
				temp+=(uint16)a[i];
		}
		temp-=*result2;
		result1[i]=(uint8)temp;
		if (!flag)
			result1[i]=~result1[i];
		*result2=(uint8)(temp>>8) & 0x1;
	}
	if (!flag)
		_add(result1,result1,1,length_a);
	length=length_a;
	_len(result1,length);

	return length;
}

//uint8 *result1:差,length_a
//uint8 *result2:借位,1
//uint8 *a:减数,length_a
//uint8 b:被减数,1
//result1/a可以同一地址
uint32 _sub(uint8 *result1,uint8 *result2,uint8 *a,uint8 b,uint32 length_a)
{
	uint32 i;
	uint16 temp;
	uint32 length;

	_len(a,length_a);
	*result2=0;
	for (i=0;i<length_a;i++)
	{
		temp=(uint16)a[i];
		if (i<1)
			temp-=(uint16)b;
		temp-=*result2;
		result1[i]=(uint8)temp;
		*result2=(uint8)(temp>>8) & 0x1;
	}
	length=length_a;
	_len(result1,length);

	return length;
}

//uint8 *result:异或,max(length_a,length_b)
//uint8 *a:数1,length_a
//uint8 *b:数2,length_b
//result/a/b可以同一地址
uint32 _xor(uint8 *result,uint8 *a,uint8 *b,uint32 length_a,uint32 length_b)
{
	uint32 i;
	uint8 flag;//0:length_a<length_b,1:length_a>=length_b
	uint32 length;

	_len(a,length_a);
	_len(b,length_b);
	if (length_a<length_b)
	{
		flag=0;
		math_swap(length_a,length_b);
	}
	else
		flag=1;
	for (i=0;i<length_a;i++)
	{
		if (i<length_b)
			result[i]=a[i]^b[i];
		else
		{
			if (flag)
				result[i]=a[i];
			else
				result[i]=b[i];
		}
	}
	length=length_a;
	_len(result,length);

	return length;
}

//        A  B  C
//*          D  E
//----------------
//=    S  F  G  H
//+ T  I  J  K
//----------------
//= U  V  L  M  N
//uint8 *result:相乘,length_a+length_b
//uint8 *a:乘数,length_a
//uint8 *b:被乘数,length_b
//result和a/b可以同一地址
uint32 _mul(uint8 *result,uint8 *a,uint8 *b,uint32 length_a,uint32 length_b)
{
	uint32 i,j;
	uint8 k;
	uint16 temp;
	uint8 *value;
	uint32 length;
	
	_len(a,length_a);
	_len(b,length_b);
	value=new uint8[length_a+length_b+1];//+1预留给进位
	memset(value,0,length_a+length_b+1);
	for (i=0;i<length_b;i++)
	{
		k=0;
		for (j=0;j<length_a;j++)
		{
			temp=b[i]*a[j]+k;
			_add(&value[i+j],&value[i+j],(uint8)temp,length_a+length_b-i-j);
			k=(uint8)(temp>>8);
		}
		_add(&value[i+length_a],&value[i+length_a],k,length_b-i);
	}
	memcpy(result,value,length_a+length_b);
	delete[] value;
	length=length_a+length_b;
	_len(result,length);

	return length;
}

//uint8 *result:相乘,length_a+1
//uint8 *a:乘数,length_a
//uint8 b:被乘数,1
//result和a可以同一地址
uint32 _mul(uint8 *result,uint8 *a,uint8 b,uint32 length_a)
{
	uint32 i;
	uint8 k;
	uint16 temp;
	uint8 *value;
	uint32 length;
	
	_len(a,length_a);
	value=new uint8[length_a+1+1];//+1预留给进位
	memset(value,0,length_a+1+1);
	k=0;
	for (i=0;i<length_a;i++)
	{
		temp=b*a[i]+k;
		_add(&value[i],&value[i],(uint8)temp,length_a+1-i);
		k=(uint8)(temp>>8);
	}
	_add(&value[length_a],&value[length_a],k,1);
	memcpy(result,value,length_a+1);
	delete[] value;
	length=length_a+1;
	_len(result,length);

	return length;
}

//uint8 *result1:商,length_1=length_a-length_b+1
//uint8 *result2:余数,length_2=length_b
//uint8 *a:除数,length_a
//uint8 *b:被除数,length_b
//result1/a/b可以同一地址,result2/b可以同一地址
void _div(uint8 *result1,uint8 *result2,uint8 *a,uint8 *b,uint32 length_a,uint32 length_b,uint32 &length_1,uint32 &length_2)
{
	//除法的关键在于“试商”,然后就变成了乘法和减法,这里将被除数与除数的试商转化成了被除数最高位与除数最高位的试商
	uint8 i;
	uint8 flag;//0:length_a<length_b,1:length_a>=length_b
	uint16 temp;
	uint8 *backup,*value;
	uint32 length_backup;

	_len(a,length_a);
	_len(b,length_b);
	if (!_cmp(b,0,length_b))
	{
		length_1=0;
		length_2=0;
		return;
	}
	flag=length_a>=length_b;
	if (flag)
	{
		length_backup=length_a;
		backup=new uint8[length_backup];
		memcpy(backup,a,length_backup);
		value=new uint8[length_backup+1];
		memset(result1,0,length_a-length_b+1);
		while(_cmp(backup,b,length_backup,length_b)>0)
		{
			if (backup[length_backup-1]>b[length_b-1])
			{
				temp=backup[length_backup-1]/(b[length_b-1]+1);
				flag=0;
			}
			else if (length_backup>length_b)
			{
				temp=(backup[length_backup-1]<<8)+backup[length_backup-2];
				temp=b[length_b-1]==0xff?temp>>8:temp/(b[length_b-1]+1);
				flag=1;
			}
			else
			{
				_add(result1,result1,1,length_a-length_b+1);
				break;
			}
			_adc(&result1[length_backup-length_b-flag],&result1[length_backup-length_b-flag],(uint8)temp,1);
			memset(value,0,length_backup+1);
			_mul(&value[length_backup-length_b-flag],b,(uint8)temp,length_b);
			length_backup=_sub(backup,&i,backup,value,length_backup,length_backup);
		}
		memset(result2,0,length_b);
		if (!_cmp(backup,b,length_backup,length_b))
		{
			length_1=_add(result1,result1,1,length_a-length_b+1);
			length_2=1;
		}
		else
		{
			memcpy(result2,backup,length_backup);
			length_1=length_a-length_b+1;
			_len(result1,length_1);
			length_2=length_backup;
		}
		delete[] value;
		delete[] backup;
	}
	else
	{
		memset(result1,0,1);
		memcpy(result2,a,length_a);
		length_1=1;
		length_2=length_a;
	}
}

//uint8 *result:模,length_b
//uint8 *a:模数,length_a
//uint8 *b:被模数,length_b
//result/a/b可以同一地址
uint32 _mod(uint8 *result,uint8 *a,uint8 *b,uint32 length_a,uint32 length_b)
{
	//求模与求商原理相同
	uint8 i;
	uint8 flag;//0:length_a<length_b,1:length_a>=length_b
	uint16 temp;
	uint8 *backup,*value;
	uint32 length,length_backup;

	_len(a,length_a);
	_len(b,length_b);
	if (!_cmp(b,0,length_b))
		return 0;
	flag=length_a>=length_b;
	if (flag)
	{
		length_backup=length_a;
		backup=new uint8[length_backup];
		memcpy(backup,a,length_backup);
		value=new uint8[length_backup+1];
		while(_cmp(backup,b,length_backup,length_b)>0)
		{
			if (backup[length_backup-1]>b[length_b-1])
			{
				temp=backup[length_backup-1]/(b[length_b-1]+1);
				flag=0;
			}
			else if (length_backup>length_b)
			{
				temp=(backup[length_backup-1]<<8)+backup[length_backup-2];
				temp=b[length_b-1]==0xff?temp>>8:temp/(b[length_b-1]+1);
				flag=1;
			}
			else
				break;
			memset(value,0,length_backup+1);
			_mul(&value[length_backup-length_b-flag],b,(uint8)temp,length_b);
			length_backup=_sub(backup,&i,backup,value,length_backup,length_backup);
		}
		delete[] value;
		switch(_cmp(backup,b,length_backup,length_b))
		{
		case 0:
			memset(result,0,1);
			length=1;
			break;
		case -1:
			memcpy(result,backup,length_backup);
			length=length_backup;
			break;
		case 1:
			length=_sub(result,&i,backup,b,length_backup,length_b);
			break;
		}
		delete[] backup;
	}
	else
	{
		memcpy(result,a,length_a);
		length=length_a;
	}

	return length;
}

//uint8 *result:最大公约数,min(length_a,length_b)
//uint8 *a:数1,length_a
//uint8 *b:数2,length_b
//result/b可以同一地址
uint32 _gcd(uint8 *result,uint8 *a,uint8 *b,uint32 length_a,uint32 length_b)
{
	//欧几里德算法
	uint8 *temp;
	uint8 *temp_a,*temp_b;
	uint32 length;

	_len(a,length_a);
	_len(b,length_b);
	temp_a=new uint8[length_a];
	temp_b=new uint8[length_b];
	memcpy(temp_a,a,length_a);
	memcpy(temp_b,b,length_b);
	if (_cmp(a,b,length_a,length_b)==-1)
	{
		math_swap(temp_a,temp_b);
		math_swap(length_a,length_b);
	}
	temp=new uint8[length_b];
	while(1)
	{
		if (!_cmp(temp_b,0,length_b))
		{
			memcpy(result,temp_a,length_a);
			break;
		}
		length=_mod(temp,temp_a,temp_b,length_a,length_b);
		_len(temp_a,length_a);
		_len(temp_b,length_b);
		memcpy(temp_a,temp_b,length_b);
		memcpy(temp_b,temp,length);
		length_a=length_b;
		length_b=length;
	}
	delete[] temp_a;
	delete[] temp_b;
	delete[] temp;
	length=length_a;
	_len(result,length);

	return length;
}

//uint8 *result:模加,max(length_a,length_b,length_p)
//uint8 *a:加数,length_a
//uint8 *b:被加数,length_b
//uint8 *p:模数,length_p
//result/a/b/p可以同一地址.P27
uint32 _modadd(uint8 *result,uint8 *a,uint8 *b,uint8 *p,uint32 length_a,uint32 length_b,uint32 length_p)
{
	uint32 length;

	_len(a,length_a);
	_len(b,length_b);
	_len(p,length_p);
	length=_add(result,a,b,length_a,length_b);
	length=_mod(result,result,p,length,length_p);

	return length;
}

//uint8 *result:模减,max(length_a,length_b,length_p)
//uint8 *a:减数,length_a
//uint8 *b:被减数,length_b
//uint8 *p:模数,length_p
//result/a/b/p可以同一地址.P27
uint32 _modsub(uint8 *result,uint8 *a,uint8 *b,uint8 *p,uint32 length_a,uint32 length_b,uint32 length_p)
{
	uint8 i;
	uint32 length;

	_len(a,length_a);
	_len(b,length_b);
	_len(p,length_p);
	if (_cmp(a,b,length_a,length_b)==-1)
	{
		length=_sub(result,&i,b,a,length_b,length_a);
		length=_mod(result,result,p,length,length_p);
		if (_cmp(result,0,length))
			length=_sub(result,&i,p,result,length_p,length);
	}
	else
	{
		length=_sub(result,&i,a,b,length_a,length_b);
		length=_mod(result,result,p,length,length_p);
	}

	return length;
}

//uint8 *result:模乘,max(length_p,length_a+length_b)
//uint8 *a:乘数,length_a
//uint8 *b:被乘数,length_b
//uint8 *p:模数,length_p
//result和a/b/p可以同一地址
uint32 _modmul(uint8 *result,uint8 *a,uint8 *b,uint8 *p,uint32 length_a,uint32 length_b,uint32 length_p)
{
	uint32 length;

	_len(a,length_a);
	_len(b,length_b);
	_len(p,length_p);
	length=_mul(result,a,b,length_a,length_b);
	length=_mod(result,result,p,length,length_p);

	return length;
}

//uint8 *result:模幂,max(length_p,length_a+length_b)
//uint8 *a:底数,length_a
//uint8 *b:幂数,length_b
//uint8 *p:模数,length_p
//result和a/b不能同一地址,和p可以同一地址
uint32 _modexp(uint8 *result,uint8 *a,uint8 *b,uint8 *p,uint32 length_a,uint32 length_b,uint32 length_p)
{
	//二元法:b二进制化,模乘运算
	int32 i;
	uint8 *value,*temp;
	uint32 len,length;

	_len(a,length_a);
	_len(b,length_b);
	_len(p,length_p);
	value=new uint8[length_p];
	length=_mod(value,a,p,length_a,length_p);
	temp=new uint8[math_max(length_p*length_p,length_a+length_b)];
	memset(result,0,length_p);
	len=length;
	length=length_p;
	for (i=_bitlen(b,length_b)-2;i>=0;i--)
	{
		length=_modmul(temp,value,value,p,len,len,length_p);
		memcpy(value,temp,length);
		len=length;
		if ((b[i>>3]>>(i%8)) & 1)
		{
			length=_modmul(temp,a,value,p,length_a,len,length_p);
			memcpy(value,temp,length); 
			len=length;
		}
	}
	memcpy(result,value,len);
	length=len;
	delete[] temp;
	delete[] value;

	return length;
}

//uint8 *result:模幂,max(length_p,1+length_b)
//uint8 a:底数,1
//uint8 *b:幂数,length_b
//uint8 *p:模数,length_p
//result和a/b不能同一地址,和p可以同一地址
uint32 _modexp(uint8 *result,uint8 a,uint8 *b,uint8 *p,uint32 length_b,uint32 length_p)
{
	//二元法:b二进制化,模乘运算
	int32 i;
	uint8 *value,*temp;
	uint32 length,len;

	_len(b,length_b);
	_len(p,length_p);
	value=new uint8[length_p];
	length=_mod(value,&a,p,1,length_p);
	temp=new uint8[math_max(length_p*length_p,1+length_b)];
	memset(result,0,length_p);
	len=length;
	length=length_p;
	for (i=_bitlen(b,length_b)-2;i>=0;i--)
	{
		length=_modmul(temp,value,value,p,len,len,length_p);
		memcpy(value,temp,length);
		len=length;
		if ((b[i>>3]>>(i%8)) & 1)
		{
			length=_modmul(temp,&a,value,p,1,len,length_p);
			memcpy(value,temp,length); 
			len=length;
		}
	}
	memcpy(result,value,len);
	length=len;
	delete[] temp;
	delete[] value;

	return length;
}

//扩展欧几里德算法:辗转相除法.0:a和p互质,1:a和p非质
//uint8 *result:模幂,max(length_p,length_a+length_b)
//uint8 *a:底数,length_a
//uint8 *p:模数,length_p
//result和p可以同一地址
uint8 _modinv(uint8 *result,uint8 *a,uint8 *p,uint32 length_a,uint32 length_p,uint32 &length)
{
	//result:模逆,length_p
	uint32 i;
	uint8 j;
	uint8 *g[3],*v[3],*temp[2],*backup;
	uint32 gl[3],vl[3],templ[2];

	_len(a,length_a);
	_len(p,length_p);
	if (!_cmp(p,1,length_p))
	{
		length=1;
		memset(result,0,length);
		return 0;
	}
	length=_gcd(result,a,p,length_a,length_p);
	if (_cmp(result,1,length))
	{
		length=0;
		return 1;
	}
	backup=new uint8[length_a];
	memcpy(backup,a,length_a);
	if (_cmp(backup,p,length_a,length_p)==1)
	{
		length=_mod(result,backup,p,length_a,length_p);
		length_a=length;
		memcpy(backup,result,length);
	}
	if (!_cmp(backup,1,length_a))
	{
		length=1;
		memset(result,0,length);
		result[0]=1;
		delete[] backup;
		return 0;
	}
	//辗转相除法
	for (i=0;i<3;i++)
	{
		g[i]=new uint8[length_p];
		v[i]=new uint8[length_p];
	}
	temp[0]=new uint8[length_p];
	temp[1]=new uint8[length_p+length_p];
	memcpy(g[0],p,length_p);
	memcpy(g[1],backup,length_a);
	memset(v[0],0,length_p);
	memset(v[1],0,length_p);
	v[1][0]=1;
	gl[0]=length_p;
	gl[1]=length_a;
	vl[0]=length_p;
	vl[1]=length_p;
	i=0;
	while(1)
	{
		gl[2]=_mod(g[2],g[0],g[1],gl[0],gl[1]);
		_div(temp[0],temp[1],g[0],g[1],gl[0],gl[1],templ[0],templ[1]);
		templ[1]=_mul(temp[1],temp[0],v[1],templ[0],vl[1]);
		vl[2]=_add(v[2],temp[1],v[0],templ[1],vl[0]);
		if (!_cmp(g[2],1,gl[2]))
		{
			if (i%2)
			{
				length=vl[2];
				memcpy(result,v[2],length);
				break;
			}
			else
			{
				length=_sub(result,&j,p,v[2],length_p,vl[2]);
				break;
			}
		}
		else
		{
			memcpy(g[0],g[1],gl[1]);
			gl[0]=gl[1];
			memcpy(g[1],g[2],gl[2]);
			gl[1]=gl[2];
			memcpy(v[0],v[1],vl[1]);
			vl[0]=vl[1];
			memcpy(v[1],v[2],vl[2]);
			vl[1]=vl[2];
		}
		i++;
	}
	for (i=0;i<3;i++)
	{
		delete[] g[i];
		delete[] v[i];
	}
	for (i=0;i<2;i++)
		delete[] temp[i];
	delete[] backup;

	return 0;
}

//uint8 *result:右移结果
//uint8 *a:操作数,length_a
//uint32 length:右移位数
void _sr(uint8 *result,uint8 *a,uint32 length_a,uint32 length)
{
	int32 i,j;
	uint32 len_div,len_mod;

	len_div=length>>3;
	if (length_a<=len_div)
		len_div=length_a;
	else
	{
		len_mod=length & 0x07;
		for (i=(int32)len_div,j=0;i<(int32)length_a-1;i++,j++)
			result[j]=(a[i]>>len_mod) | (a[i+1]<<(8-len_mod));
		result[length_a-len_div-1]=a[length_a-1]>>len_mod;
	}
	for (j=(int32)(length_a-len_div);j<(int32)length_a;j++)
		result[j]=0;
}

//uint8 *a:操作数,length_a
//uint32:获取从高到低第一个非0位与最低位之间的位数
uint32 _bitlen(uint8 *a,uint32 length_a)
{
	//从高到底遍历每个字节每一位，至非0位
	int32 i,j;
	uint32 result;

	result=length_a*8;
	for (i=length_a-1;i>=0;i--)
		for (j=7;j>=0;j--)
		{
			if (a[i] & (1<<j))
				return result;//非0则直接返回结果
			result--;
		}
	if (!result)
		result=1;

	return result;
}

//uint8 *a:操作数,length_a
void _rand(uint8 *a,uint32 length_a)
{
	uint32 i;

	for (i=0;i<length_a;i++)
		a[i]=rand() & 0xff;
}