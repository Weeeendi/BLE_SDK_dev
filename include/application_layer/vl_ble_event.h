/**
 * @file vl_ble_event.h
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

#include "vl_ble_config.h"
#include "vl_queue.h"

//=============================================================================
//Byte order of the frame
//=============================================================================
#define         SN                              0
#define         ACK_SN                          4      
#define         FUNC_CODE                       8
#define         LENGTH_HIGH                     10
#define         LENGTH_LOW                      11
#define         DATA_START                      12

//=============================================================================
//app data event type
//=============================================================================
#define VL_BLE_EVT_BASE                         0x0000

typedef enum{
    VL_BLE_EVT_DEVINFO_QUERY = VL_BLE_EVT_BASE,
    VL_BLE_EVT_PAIR          = 0x0001,
    VL_BLE_EVT_DP_DATA_REV   = 0x0002,
    VL_BLE_EVT_DP_QUERY = 0x0003,
    VL_BLE_EVT_DEVICE_UNBIND = 0x0005,
    VL_BLE_EVT_FACTORY_RESET = 0x0006,
    VL_BLE_EVT_OTA_START = 0x000C,
    VL_BLE_EVT_OTA_FILEINFO = 0x000D,
    VL_BLE_EVT_OTA_OFFSETREQ = 0x000E,
    VL_BLE_EVT_OTA_DATA = 0x000F,
    VL_BLE_EVT_OTA_OVER = 0x0010,
    VL_BLE_EVT_OTA_OTHER = 0x0011,
    VL_BLE_EVT_PRODUCTION_TEST = 0x0012,
    VL_BLE_EVT_ANOMALY_UNBOUND = 0x0014,
    VL_BLE_EVT_APP_SYNC_TIME = 0x0025,
    VL_BLE_EVT_DP_DATA_REPORTED = 0x8001,
    VL_BLE_EVT_DP_DATA_WITH_TIME_REPORTED = 0x8002,
    VL_BLE_EVT_TIME_REQ = 0x8003
}vl_ble_evt_t;

#pragma pack (1)
typedef struct {
    UINT8 len; //当前数组已使用数据的长度 
    UINT8 curbuf[BLE_BUF_LMT]; //当前数组
}BLE_BUFF_T;
#pragma pack ()

typedef struct 
{
    uint32_t sn;
    uint32_t ack_sn;
    vl_ble_evt_t cmd;
    uint16_t len;
    uint8_t data[0];
}ble_event_t;

/* queue handler */
extern vl_queue_t xQueue_BLERecv;
extern vl_queue_t xQueue_BLESend;

/*variable*/
extern BLE_BUFF_T BLE_Send;
extern BLE_BUFF_T BLE_Rev;

#define BLE_RECV_QUEUE_SIZE 20
#define MAX_DP_DATA_LEN 255

iot_conn_status_s get_BleBoundStatus(void);
//=============================================================================
//Ble data frame
//=============================================================================
vl_status_t bt_write_frame(unsigned short fr_type, unsigned short len);
#endif