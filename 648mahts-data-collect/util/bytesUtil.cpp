#include "bytesUtil.h"
#include <iomanip>
#include <string>
#include <cmath>
#include <sstream>

using namespace std;

int bytes2int(const char *src,bool isReserve,int size)
{
	int intValue = 0;
	if(isReserve)
	{
		for(int i = 0 ; i < size; i++)
			intValue += (src[size - 1 - i] & 0xFF) << (8*(3-i));		
	}
	else
	{
		for(int i = 0 ; i < size; i++)
			intValue += (src[i] & 0xFF) << (8*(3-i));
	}	
	return intValue;
}

short bytes2short(const char *src,bool isReserve,int size)
{
	short intValue = 0;

	if(isReserve)
	{
		for(int i = 0 ; i < size; i++)
			intValue += (src[size - 1 - i] & 0xFF) << (8*(1-i));		
	}
	else
	{
		for(int i = 0 ; i < size; i++)
			intValue += (src[i] & 0xFF) << (8*(1-i));
	}
	return intValue;
}

char* int2bytes(int src, char *dst)
{
	dst[0] = src >> 24;
	dst[1] = src >> 16;
	dst[2] = src >> 8;
	dst[3] = src;
	return dst;
}

char* short2bytes(short src, char *dst)
{
	dst[0] = src >> 8;
	dst[1] = src;
	return dst;
}

//将两个 uint16_t 类型的数据解析为一个 float 类型的数值
float uint16float(uint16_t *recv)
{
	char hex[4] = {0};
	hex[0] = recv[1] & 0xFF;
	hex[1] =(recv[1]>>8) & 0xFF;
	hex[2] = recv[0] & 0xFF;
	hex[3] = (recv[0]>>8) & 0xFF;
	return *(float*)hex;
}

float uint16float_r(uint16_t *recv)
{
	char hex[4] = {0};
	hex[0] = recv[0] & 0xFF;
	hex[1] =(recv[0]>>8) & 0xFF;
	hex[2] = recv[1] & 0xFF;
	hex[3] = (recv[1]>>8) & 0xFF;
	return *(float*)hex;
}

float uint8float(uint8_t *recv)
{
	char hex[4] = {0};
	hex[0] = recv[3] & 0xFF;
	hex[1] =recv[2] & 0xFF;
	hex[2] = recv[1] & 0xFF;
	hex[3] = recv[0] & 0xFF;
	return *(float*)hex;
}

float uint8float_r(uint8_t *recv)
{
	char hex[4] = {0};
	hex[0] = recv[0] & 0xFF;
	hex[1] =recv[1] & 0xFF;
	hex[2] = recv[2] & 0xFF;
	hex[3] = recv[3] & 0xFF;
	return *(float*)hex;
}





float formatAndRoundFloat(float number, int precision) {
    // 使用 std::setprecision 设置小数位数
    std::stringstream stream;
    stream << std::fixed << std::setprecision(precision) << number;
    
    // 使用 std::round 执行四舍五入
    float roundedValue = std::stof(stream.str());
    
    // 再次格式化四舍五入后的值以确保小数位数正确
    stream.str("");
    stream << std::fixed << std::setprecision(precision) << roundedValue;
    
    return stof(stream.str());
}


unsigned char genCheckCode(const unsigned char *src,int len)
{
	unsigned char sum = 0;
	for(int i = 0; i < len; i++)
		sum += src[i];
	return sum % 256;
}


