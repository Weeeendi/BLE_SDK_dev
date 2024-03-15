/**
 * @file vl_ble_queue_send.c
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,ble send with queue module
 * @version 1.0.0
 * @date 2024-03-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "vl_ble_queue_send.h"
#include "vl_ble_port.h"
#include "vl_ble_config.h"
#include "vl_ble_event.h"
#include "vl_queue.h"


#ifdef __cplusplus
extern "C"
{ 
#endif

#define BLE_SEND_QUEUE_SIZE 10

vl_queue_t xQueue_ble_send;

vl_status_t ble_send_queue_init(VOID){
    xQueue_ble_send.unit_size = sizeof(BLE_BUFF_T);
    xQueue_ble_send.size = BLE_SEND_QUEUE_SIZE;
    return vl_queue_init(&xQueue_ble_send, xQueue_ble_send.buf, xQueue_ble_send.unit_size, xQueue_ble_send.size);
}

vl_status_t ble_send_enqueue(PVOID in){
    return vl_enqueue(&xQueue_ble_send, in);
}

static void ble_send_queue_free(void)
{
    BLE_BUFF_T send_p;
	while(vl_dequeue(&xQueue_ble_send,&send_p)== VL_SUCCESS);
}

vl_status_t ble_send_queue_handle(VOID){
    BLE_BUFF_T send_p;
    iot_conn_status_s conn_s;

    conn_s = vl_iot_connect_status_get();
    while(vl_get_queue_used(&xQueue_ble_send)!= 0){
        if (conn_s != BLE_STATUS_CONNECT)
        {
            ble_send_queue_free();
            return VL_BLE_ERR_INVALID_STATE;
        }
        if (vl_dequeue(&xQueue_ble_send, &send_p) == VL_SUCCESS)
        {
           ble_send_data(send_p.curbuf,send_p.len);
        }
    }
}


#ifdef __cplusplus
}
#endif