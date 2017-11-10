#include "crypt_rsa.h"

//uint8 *content:初始为随机数
//uint32 length:生成素数的字节长(公钥长/2)
void rsa_genprime(uint8 *content,uint32 length)
{
	//小素数试除法产生素数
	uint32 i,j,k,r;
	uint8 prime[2],*quotient,*remainder;
	const uint8 table_smallprime[96]=
	{
		0x03,0x05,0x07,0x0b,0x0d,0x11,0x13,
		0x17,0x1d,0x1f,0x25,0x29,0x2b,0x2f,
        0x35,0x3b,0x3d,0x43,0x47,0x49,0x4f,
        0x53,0x59,0x61,0x65,0x67,0x6b,0x6d,
        0x71,0x7f,0x83,0x89,0x8b,0x95,0x97,
        0x9d,0xa3,0xa7,0xad,0xb3,0xb5,0xbf,
        0xc1,0xc5,0xc7,0xd3,0xdf,0xe3,0xe5,
        0xe9,0xef,0xf1,0xfb,0x01,0x07,0x0D,
        0x0F,0x15,0x19,0x1B,0x25,0x33,0x37,
        0x39,0x3D,0x4B,0x51,0x5B,0x5D,0x61,
        0x67,0x6F,0x75,0x7B,0x7F,0x85,0x8D,
        0x91,0x99,0xA3,0xA5,0xAF,0xB1,0xB7,
        0xBB,0xC1,0xC9,0xCD,0xCF,0xD3,0xDF,
        0xE7,0xEB,0xF3,0xF7,0xFD
	};

	content[length-1] |= 0xc0;//最高两位置11
	content[0] |= 0x01;//最低位置1
	quotient=new uint8[length-1+1];
	remainder=new uint8[2];
    while(1)
    {
    	j=0;
        for (i=0;i<96;i++)
        {
            prime[0]=table_smallprime[i];
			prime[1]=i>52 ? 0x01 : 0x00;
			_div(quotient,remainder,content,prime,length,2,k,r);
			if (!_cmp(remainder,0,r))//余数为0,即能被整除
            {
                j=1;
                break;
            }
        }
        if (!j && !rsa_verifyprime(content,length))
            break;
        _add(content,content,2,length);//以2递增
    }
	delete[] quotient;
	delete[] remainder;
}

//uint8 *content:待检素数
//uint32 length:素数字节长
//uint8:0-通过,1-不通过
uint8 rsa_verifyprime(uint8 *content,uint32 length)
{
	//miller-rabin素数检测(只能保证部分素数可检测,若检测为合数不可信)
    uint8 i;
	uint32 j,k;
	uint8 *temp,*value;
 
	//按位右移content-1
	temp=new uint8[length];
	_sub(temp,&i,content,1,length);
	j=0;
    while(!(temp[0] & 0x01))//最低位为0则循环
    {
    	_sr(temp,temp,length,1);//m右移一位
    	j++;
    }
	if (!j)//content-1的最低位是1,说明content是合数
	{
		delete[] temp;
		return 1;
	}
	//value=2^temp mod content
	value=new uint8[length*2];
	_modexp(value,2,temp,content,length,length);
	if (!_cmp(value,1,length))
	{
		delete[] temp;
		delete[] value;
		return 0;
	}
	k=1;
	while(1)
	{
		_add(temp,value,1,length);
		if (!_cmp(temp,content,length,length))//若value+1=content则退出
			break;
		if (j==k)//检测到合数
		{
			delete[] temp;
			delete[] value;
			return 1;
		}
        //value=value*value mod content
		_modmul(value,value,value,content,length,length,length);
        k++;
	}
	delete[] temp;
	delete[] value;

    return 0;
}

//rsa_para *para:rsa参数结构体
//para->e:rsa公钥
//uint32 config:0-std,1-crt
//uint8:0-正确,1-错误
uint8 rsa_genkey(rsa_para *para,uint32 config)
{
	uint8 i;
	uint32 len,length;
	uint8 *m;

	len=para->len>>1;
    //调用素数生成流程产生素数p和q,选择小素数试除方法产生素数
    rsa_genprime(para->p,len);
    rsa_genprime(para->q,len);
	//测试1:两者不相等.如果p==q,返回错误
	if (!_cmp(para->p,para->q,len,len))
    	return 1;
    //测试2:e和m互质,得到d
	m=new uint8[para->len];
    _mul(para->n,para->p,para->q,len,len);//n=p*q
	_sub(para->p,&i,para->p,1,len);//p=p-1
	_sub(para->q,&i,para->q,1,len);//q=q-1
    _mul(m,para->p,para->q,len,len);//欧拉函数m=(p-1)*(q-1)
    i=_modinv(para->d,para->e,m,para->le,para->len,length);//私钥d=e^-1%m
	if (i)//若e和m非质
	{
		delete[] m;
		return 1;
	}
    if (config)
    {
    	//生成crt解密需要的dp=d%(p-1),dq=d%(q-1),qp=(q^-1)%p
	    _div(m,para->dp,para->d,para->p,para->len,len,length,length);
	    _div(m,para->dq,para->d,para->q,para->len,len,length,length);
    	//恢复p/q
		_add(para->p,para->p,1,len);
		_add(para->q,para->q,1,len);
		_modinv(para->qp,para->q,para->p,len,len,length);
    }
    else
    {
    	//恢复p/q
		_add(para->p,para->p,1,len);
		_add(para->q,para->q,1,len);
    }
	delete[] m;

    return 0;
}

//uint8 *result:输出密文
//uint8 *content:输入明文
//uint32 length:数据字节数
//rsa_para *para:rsa参数结构体
//para->e:rsa公钥
//para->n:欧拉数
uint32 rsa_enc(uint8 *result,uint8 *content,uint32 length,rsa_para *para)
{
	//C=M^e%n
	return _modexp(result,content,para->e,para->n,length,para->le,para->len);
}

//uint8 *result:输出明文
//uint8 *content:输入密文
//uint32 length:数据字节数
//rsa_para *para:rsa参数结构体
//para->p:
//para->q:
//para->d:(config=0输入)
//para->n:(config=0输入)
//para->dp:(config=1输入)
//para->dq:(config=1输入)
//para->qp:(config=1输入)
//uint32 config:0-std,1-crt
uint32 rsa_dec(uint8 *result,uint8 *content,uint32 length,rsa_para *para,uint32 config)
{
	uint8 i;
	uint8 *m1,*m2,*temp,*rand;
	uint32 len,length_1,length_2;
	uint32 len_d;
	
	if (para->lr)
	{
		rand=new uint8[para->lr];
		_rand(rand,para->lr);
	}
	len=para->len>>1;
	if (!config)
	{
		if (para->lr)//仅支持std的指数盲化,para->d需要预留随机数空间
		{
			temp=new uint8[len+len+para->lr];
			m1=new uint8[len];
			m2=new uint8[len];
			//m=(p-1)*(q-1)
			_sub(m1,&i,para->p,1,len);
			_sub(m2,&i,para->q,1,len);
			len_d=_mul(temp,m1,m2,len,len);
			//d'=d+rand*m
			len_d=_mul(temp,temp,rand,len_d,para->lr);
			len_d=_add(para->d,para->d,temp,para->len,len_d);
			//release
			delete[] m1;
			delete[] m2;
			delete[] temp;
		}
		else
			len_d=para->len;
		//m=c^d mod n
		length=_modexp(result,content,para->d,para->n,length,len_d,para->len);
	}
	else
	{
		temp=new uint8[len];
		m1=new uint8[length+len];
		m2=new uint8[length+len];
		//c1=c mod p,m1=(c1 ^ dp) mod p
		length_1=_mod(temp,content,para->p,length,len);
		length_1=_modexp(m1,temp,para->dp,para->p,length_1,len,len);
		//c2=c mod q,m2=(c2 ^ dq) mod q
		length_2=_mod(temp,content,para->q,length,len);
		length_2=_modexp(m2,temp,para->dq,para->q,length_2,len,len);
		//m=m2+[(m1-m2)*qp mod p]*q
		length=_modsub(result,m1,m2,para->p,length_1,length_2,len);
		length=_modmul(result,result,para->qp,para->p,length,len,len);
		length=_mul(result,result,para->q,length,len);
		length=_add(result,result,m2,length,length_2);
		//release
		delete[] m1;
		delete[] m2;
		delete[] temp;
	}
	if (para->lr)
		delete[] rand;

    return length;
}