/**
 * @file vl_ble_queue_send.h
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,ble send with queue module
 * @version 1.0.0
 * @date 2024-03-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef VL_BLE_QUEUE_SEND_H
#define VL_BLE_QUEUE_SEND_H

#include "vl_type.h"


vl_status_t ble_send_queue_init(VOID);
vl_status_t ble_send_enqueue(PVOID in);
vl_status_t ble_send_queue_handle(VOID);

#endif

