/**
 * @file vl_iot_utils.h
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,include function of data format conversion
 * @version 1.0.0
 * @date 2024-03-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef VL_IOT_UTILS_H
#define VL_IOT_UTILS_H
#include "vl_type.h"

VOID tal_inverted_array(UINT8 *array,UINT16 length);
VOID tal_util_hextoasciiUpper(UINT8 *hexbuf,UINT8 len,UINT8 *ascbuf);
VOID tal_util_hextoasciiLower(UINT8 *hexbuf,UINT8 len,UINT8 *ascbuf);
UINT32 tal_util_byte2int(const UINT8 value[4]);
VOID tal_int2byte(UINT32 number,UINT8 value[4]);
VOID tal_xor_str(UINT8 *str,const UINT8 xor_c,UINT8 strLen);
UINT32 tal_util_str_int2intstr(UINT8 num, UINT8* intStr, UINT32 size);
UINT32 tal_util_str_intstr2int(UINT8* intStr, UINT32 size, UINT32* num);
INT32 tal_util_str_hexstr2int(UINT8* hexStr, UINT8 size);
INT8  tal_datacmp(const UINT8* data1,const UINT8* data2,UINT16 cmpLen);

#endif