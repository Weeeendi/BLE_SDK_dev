/**
 * @file vl_ble_productTest.h
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK
 * @version 1.0.0
 * @date 2024-02-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef VL_BLE_PRODUCT_TEST_H
#define VL_BLE_PRODUCT_TEST_H

#include "vl_uart_port.h"
#include "vl_ble_port.h"

#define VL_T_ENTER_TEST          0xFF00
#define VL_T_EXIT_TEST           0xFF01
#define VL_T_BURN_TOKEN_INFO     0xAA00
#define VL_T_CHEAK_TOKEN_INFO    0xAA01
#define VL_T_CHEAK_PRODUCT_INFO  0xAA02
#define VL_T_BLE_FUNC_TEST       0x0000

#pragma pack (1)
typedef struct
{
    UINT8 token_verfiy;
    UINT8 deviceIotId[17];
    UINT8 deviceSecret[33];
}token_info_t;
#pragma pack()


//function

BOOL get_test_flag(VOID);
UINT8 read_TokenInfo(token_info_t* token_info);
BOOL Is_Token(VOID);
VOID vl_ProdTestTask(PVOID pvParameters);

#endif