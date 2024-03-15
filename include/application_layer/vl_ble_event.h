/**
 * @file yj_ble_event.h
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK
 * @version 1.0.0
 * @date 2024-02-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef VL_BLE_EVENT_H
#define VL_BLE_EVENT_H

#include "vl_log.h"
#include "vl_ble_config.h"
#include "vl_queue.h"

#pragma pack (1)
typedef struct {
    UINT8 len; //当前数组已使用数据的长度 
    UINT8 curbuf[BLE_BUF_LMT]; //当前数组
}BLE_BUFF_T;
#pragma pack ()

/* queue handler */

extern vl_queue_t xQueue_BLERecv;
extern vl_queue_t xQueue_BLESend;

/*variable*/

extern BLE_BUFF_T BLE_Send;
extern BLE_BUFF_T BLE_Rev;

extern ble_status_s vl_ble_connect_status_get(VOID);

#endif