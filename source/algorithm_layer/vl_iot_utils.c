/**
 * @file vl_iot_utils.c
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,function of data format conversion
 * @version 1.0.0
 * @date 2024-03-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "vl_iot_utils.h"

VOID tal_inverted_array(UINT8 *array,UINT16 length)
{
    UINT8 temp;
    uint16_t i,j;
    for (i = 0,j=length-1; i<j; i++,j--)
    {
        temp=array[i];
        array[i]=array[j];
        array[j]=temp;
    }
}


VOID tal_util_hextoasciiUpper(UINT8 *hexbuf,UINT8 len,UINT8 *ascbuf)
{
    UINT8 i =0,j =0,temp = 0;

    for(i = 0; i<len; i++) {
        temp = (hexbuf[i]>>4)&0xf;
        if(temp <=9) {
            ascbuf[j] = temp + 0x30;
        }
        else {
            ascbuf[j] = temp + 87;
        }
        j++;
        temp = (hexbuf[i])&0xf;
        if(temp <=9) {
            ascbuf[j] = temp + 0x30;
        }
        else {
            ascbuf[j] = temp + 87;
        }
        j++;
    }
    ascbuf[j] = 0x0;
}

VOID tal_util_hextoasciiLower(UINT8 *hexbuf,UINT8 len,UINT8 *ascbuf){
    UINT8 i =0,j =0,temp = 0;

    for(i = 0; i<len; i++) {
        temp = (hexbuf[i]>>4)&0xf;
        if(temp <=9) {
            ascbuf[j] = temp + 0x30;
        }
        else {
            ascbuf[j] = temp + 87 - 0x20;
        }
        j++;
        temp = (hexbuf[i])&0xf;
        if(temp <=9) {
            ascbuf[j] = temp + 0x30;
        }
        else {
            ascbuf[j] = temp + 87 - 0x20;
        }
        j++;
    }
    ascbuf[j] = 0x0;
}

uint8_t tal_util_str_hexchar2int(uint8_t hexChar)
{
    switch (hexChar) {
        case '0':return 0;
        case '1':return 1;
        case '2':return 2;
        case '3':return 3;
        case '4':return 4;
        case '5':return 5;
        case '6':return 6;
        case '7':return 7;
        case '8':return 8;
        case '9':return 9;
        case 'a':
        case 'A':return 10;
        case 'b':
        case 'B':return 11;
        case 'c':
        case 'C':return 12;
        case 'd':
        case 'D':return 13;
        case 'e':
        case 'E':return 14;
        case 'f':
        case 'F':return 15;
        default: return (uint8_t)-1;
    }
}

/**
 * @brief Converts a hexadecimal string to an integer.
 * 
 * @param hexStr Pointer to the hexadecimal string.
 * @param size Size of the hexadecimal string.
 * @return Converted integer value, or -1 if an error occurs.
 */
INT32 tal_util_str_hexstr2int(UINT8* hexStr, UINT8 size) {
    if (hexStr == NULL || size == 0) {
        // Error: Invalid input parameters
        return -1;
    }

    // Ensure that the size is even (each hexadecimal character corresponds to 4 bits)
    if (size % 2 != 0) {
        // Error: Invalid size
        return -1;
    }

    // Ensure that the size does not exceed the maximum value representable by an integer
    if (size > (sizeof(int32_t) * 2)) {
        // Error: Input size too large
        return -1;
    }

    // Initialize the result
    INT32 result = 0;

    // Convert hexadecimal string to integer
    for (size_t i = 0; i < size; i ++)
    {
        UINT8 digit = tal_util_str_hexchar2int(hexStr[i]);
        if(digit<0) return -1;
        result = result<<8 | digit;
    }

    return result;
}

uint8_t tal_util_str_int2hexchar(int isHEX, uint8_t intNum)
{
    switch (intNum) {
        case 0:return '0';
        case 1:return '1';
        case 2:return '2';
        case 3:return '3';
        case 4:return '4';
        case 5:return '5';
        case 6:return '6';
        case 7:return '7';
        case 8:return '8';
        case 9:return '9';
        case 10:return (isHEX ? 'A' : 'a');
        case 11:return (isHEX ? 'B' : 'b');
        case 12:return (isHEX ? 'C' : 'c');
        case 13:return (isHEX ? 'D' : 'd');
        case 14:return (isHEX ? 'E' : 'e');
        case 15:return (isHEX ? 'F' : 'f');
        default:return (uint8_t)-1;
    }
}

UINT32 tal_util_byte2int(const UINT8 value[4]){
    return (value[0] << 24) | (value[1] << 16) | (value[2] << 8) | value[3];
}

VOID tal_int2byte(UINT32 number,UINT8 value[4]){
    value[0] = number >> 24;
    value[1] = number >> 16;
    value[2] = number >> 8;
    value[3] = number & 0xff;
}

VOID tal_xor_str(UINT8 *str,const UINT8 xor_c,UINT8 strLen){
    while(strLen>0){
     *str ^= xor_c;
     str++;
     strLen--;
   }
}


UINT32 tal_util_str_int2intstr(UINT8 num, UINT8* intStr, UINT32 size){
    UINT32 idx = 0;
    UINT32 tmp = 0;

    tmp = num;
    do {
        tmp /= 10;
        idx++;
    } while (tmp != 0);

    if (size < idx) {
        return 0;
    }

    tmp = num;
    for (idx=0; tmp!=0; idx++) {
        intStr[idx] = tal_util_str_int2hexchar(true,tmp % 10);
        tmp /= 10;
    }

    tal_inverted_array(intStr, idx);

    return idx;
}


UINT32 tal_util_str_intstr2int(UINT8* intStr, UINT32 size, UINT32* num);
INT8  tal_datacmp(const UINT8* data1,const UINT8* data2,UINT16 cmpLen){
    while ((*data1 == *data2) && cmpLen>0)
    {
        data1++;
        data2++;
        cmpLen--;
    }
    if(cmpLen ==0) return 1;
    return -1;
}


