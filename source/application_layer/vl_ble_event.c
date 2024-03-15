/**
 * @file vl_ble_event.c
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,ble event process
 * @version 1.0.0
 * @date 2024-03-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "vl_ble_event.h"
#include "vl_ble_handle.h"
#include "vl_iot_handle.h"
#include "vl_queue.h"

#include "aes.h"
#include "vl_flash_port.h"
#include "vl_ble_port.h"

#ifdef __cplusplus
extern "C" {
#endif
/* queue handler */

vl_queue_t xQueue_BLERecv;
vl_queue_t xQueue_BLESend;

/*variable*/

BLE_BUFF_T BLE_Send;
BLE_BUFF_T BLE_Rev;

UINT32 conn_overtime = 0;   //connect ble but not communication
UINT32 pair_overtime = 0;   //pair ble overtime
UINT8  open_pair_enable = true; //pair ble enable
UINT8  adv_Needchange = false;  //ble adv need changed

static aes_context productKeyAesCtx;


void vl_change_advData(void){
    int32_t ret;
    
    vl_ble_stopAdv();



    memcpy(&bt_init.scan_rsp_data[2],vl_iot_obj.deviceID,DEVICE_ID_LEN);
    memcpy((uint8_t*)&bt_init.adv_data[13],PRODUCT_KEY, 10);
       
    BLE_LOG_INFO("adv data changed ,current bound flag = %d",vl_ble_obj.bound_flag);
    

    ret = bt_ware_init(&bt_init, (bt_event_callback_handler_t)bt_event_callback_func);
    
    if (ret != VL_SUCCESS)
    {
        YJ_LOG_DEBUG("bt init failed.");
    }
    else
    {
        YJ_LOG_DEBUG("bt init ok.");
    }
    
    yj_ble_adv_open();
}

/**
 * @brief Set the Dev Status object
 * 
 * @param set_state @refitem devStatus_t
 */
VOID setDevStatus(ble_status_s set_state){
   switch(set_state){
        case BLE_STATUS_START_ADV:
          vl_ble_startAdv();
          break;
        case BLE_STATUS_STOP_ADV:
          vl_ble_stopAdv();
          break;
        case BLE_STATUS_DISCONNECT:
          vl_ble_disconnect();
          break;
        case BLE_STATUS_CHG_ADV:
          vl_ble_changeAdvData();
          break; 
   }

}


#ifdef __cplusplus
}
#endif