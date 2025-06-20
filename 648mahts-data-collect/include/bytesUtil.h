#ifndef BYTESUTIL_H
#define BYTESUTIL_H

#include <string>

char* int2bytes(int src, char *dst);
char* short2bytes(short src, char *dst);
int bytes2int(const char *src,bool isReserve = false,int size = 4);
short bytes2short(const char *src,bool isReserve = false,int size = 2);


float uint8float(uint8_t *recv);
float uint8float_r(uint8_t *recv);

float uint16float(uint16_t *recv);
float uint16float_r(uint16_t *recv);
float formatAndRoundFloat(float number, int precision); 


unsigned char genCheckCode(const unsigned char *src,int len);



#endif
