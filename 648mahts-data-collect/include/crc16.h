#ifndef CRC16_H
#define CRC16_H

#include <stdint.h>

short modbusCRC(const char* pData, int len);
short modbusCRC(const char* pData, int index, int len);

#endif