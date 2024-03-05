/**
 * @file vl_ble_queue.h
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,a simple queue function lib
 * @version 1.0.0
 * @date 2024-02-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef VL_BLE_QUEUE_H
#define VL_BLE_QUEUE_H

#include "vl_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void * buf;
    uint8_t size;
    uint8_t offset;
    uint8_t rd_ptr;
    uint8_t wr_ptr;
    uint8_t used;
} vl_ble_queue_t;

vl_ble_status_t vl_ble_queue_init(vl_ble_queue_t *q, void *buf, uint8_t size, uint8_t elem_size);
vl_ble_status_t vl_ble_enqueue(vl_ble_queue_t *q, void *in);
vl_ble_status_t vl_ble_queue_get(vl_ble_queue_t *q, void *out);
vl_ble_status_t vl_ble_dequeue(vl_ble_queue_t *q, void *out);
void vl_ble_queue_decrease(vl_ble_queue_t *q);
void vl_ble_queue_flush(vl_ble_queue_t *q);
uint8_t vl_ble_get_queue_used(vl_ble_queue_t *q);

#ifdef __cplusplus
}
#endif

#endif