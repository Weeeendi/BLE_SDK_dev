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
    uint16_t unit_size;
    uint8_t size;
    uint8_t offset;
    uint8_t rd_ptr;
    uint8_t wr_ptr;
    uint8_t used;
} vl_queue_t;

vl_status_t vl_queue_init(vl_queue_t *q, void *buf, uint16_t elem_size, uint8_t queue_depth);
vl_status_t vl_enqueue(vl_queue_t *q, void *in);
vl_status_t vl_queue_get(vl_queue_t *q, void *out);
vl_status_t vl_dequeue(vl_queue_t *q, void *out);
void vl_queue_decrease(vl_queue_t *q);
void vl_queue_flush(vl_queue_t *q);
uint8_t vl_get_queue_used(vl_queue_t *q);

#ifdef __cplusplus
}
#endif

#endif