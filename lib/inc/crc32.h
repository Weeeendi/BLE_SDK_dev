#ifndef _CRC_32_H
#define _CRC_32_H
#include "stdint.h"

extern uint32_t CRC32_HaveInitVal(uint32_t initVal,uint8_t *puchMsg, uint16_t usDataLen);
extern uint32_t crc32(uint8_t* buf, uint32_t len);
extern uint16_t CalcCRC16_HaveInit(uint16_t crcVal,uint8_t *pCh,  uint16_t len);
extern uint16_t CalcCRC16(uint8_t *puchMsg, uint16_t usDataLen);
extern uint8_t checkSUM(void *pStart, uint16_t uSize);
#endif
