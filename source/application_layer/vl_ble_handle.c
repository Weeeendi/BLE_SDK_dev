/**
 * @file vl_ble_handle.c
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,ble task handler.
 * @version 1.0.0
 * @date 2024-03-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "vl_ble_handle.h"
#include "vl_ble_port.h"
#include "vl_ble_config.h"
#include "vl_flash_port.h"
#include "vl_iot_handle.h"
#include "vl_ble_event.h"
#include "vl_timer_port.h"
#include "vl_sdk_globals.h"
#include "vl_iot_handle.h"
#include "vl_type.h"
#include "vl_log.h"
#include "vl_ble_queue_send.h"


#ifdef __cplusplus
extern "C" {
#endif

const char TAG[] = "BLE_HANDLE";

#define ATT_UUID_128_LEN 16

#define BLE_ADV_DATA_LEN  (13+PRODUCTID_LEN)
#define BLE_SCAN_RSP_DATA_LEN  18

typedef void (*DpDataCallback)(const char* data, size_t size);

static vl_ble_attr_param_t ble_param;

vl_ble_timer_t disconnect_timer;

const unsigned char service_uuid[ATT_UUID_128_LEN] = {0xFB, 0x34, 0x9B, 0x5F,0x80, 0x00, 0x00, 0x80, 0x00, 0x00, 0x10, 0x00, 0x80, 0xFD, 0x00, 0x00};
const unsigned char write_uuid[ATT_UUID_128_LEN] = {0xD0, 0x07, 0x9B, 0x5F, 0x80, 0x00, 0x01, 0x80, 0x01, 0x10, 0x00, 0x00,0x01, 0x00, 0x00, 0x00};
const unsigned char notify_uuid[ATT_UUID_128_LEN] = {0xD0, 0x07, 0x9B, 0x5F,0x80, 0x00, 0x01, 0x80, 0x01, 0x10, 0x00, 0x00,0x02, 0x00, 0x00, 0x00};

/** @brief  GAP - Advertisement data (max size = 31 bytes, best kept short to conserve power) */
static const uint8_t adv_data_const[BLE_ADV_DATA_LEN] =
{
    0x02,
    0x01,
    0x06,
    0x03,
    0x02,
    0x80, 0xFD,
    0x0F,
    0x16,
    0x80, 0xFD,
    0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,0x00, 0x00,
};

/** @brief  GAP - scan response data (max size = 31 bytes) */

static const uint8_t scan_rsp_data_const[BLE_SCAN_RSP_DATA_LEN] =
{
    0x11,
    0x09,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};


/**
 * Retrieves the current BLE connection status.
 * 
 * @return - connect_state: ble_status_s
 */
ble_status_s vl_ble_status_get(){
    return vl_ble_obj.connect_state;
}

/**
 * Sets the BLE connection status.
 * 
 * @param status: ble_status_s
 * @return - status: vl_status_t
 */
vl_status_t vl_ble_status_set(ble_status_s status){
    vl_ble_obj.connect_state = status;
    return VL_SUCCESS;
}


static void vl_ble_disconnect_and_reset_timer_cb(vl_ble_timer_t timer)
{
    vl_ble_disconnect();
}

void vl_ble_connect_monitor_timer_init(void){
    vl_timer_creat(&disconnect_timer,vl_ble_connect_monitor_timeout_ms,VL_TIMER_ONCE,vl_ble_disconnect_and_reset_timer_cb);
}

void vl_ble_connect_monitor_timer_start(void){
     vl_timer_start(&disconnect_timer);
}

void vl_ble_connect_monitor_timer_stop(void){
     vl_timer_stop(&disconnect_timer);
}

void fill_advContext(vl_ble_attr_param_t* param){
    UINT8 bound_flag = vl_iot_boundf_read();

    if(param == NULL) return;

    if(SHARE_ENABLE){
        param->adv_data[11] |=0x04 ;
    }else{
        param->adv_data[11] &=(~0x04);
    }
    
    if(bound_flag == 1){
        param->adv_data[11] |=0x08;
    }else{
        param->adv_data[11] &=(~0x08);
    }

    param->adv_data[11] &=(~(PROTOCAL_VER<<4));
    param->adv_data[11] |= (PROTOCAL_VER<<4);

    memcpy(&param->scan_rsp_data[2],vl_iot_obj.deviceID,DEVICEID_LEN);
    memcpy((uint8_t*)&param->adv_data[13],PRODUCT_KEY, PRODUCTID_LEN);
}

vl_status_t read_current_advData(uint8_t *advData){
    if(advData == NULL) return VL_BLE_ERR_INVALID_PARAM;
    memcpy(ble_param.adv_data, advData, ble_param.adv_data_len);
    return VL_SUCCESS;
}

void vl_change_advData(void){
    UINT8 adv_changeParams[BLE_ADV_DATA_LEN] = {0};
    INT8 bound_flag = vl_iot_boundf_read();
    vl_ble_stopAdv();
    read_current_advData(adv_changeParams);
    if(bound_flag == 1){
        adv_changeParams[11] |=0x08;
    }else{
        adv_changeParams[11] &=(~0x08);
    }
    vl_ble_changeAdvData(adv_changeParams,BLE_ADV_DATA_LEN);
    BLE_LOG_INFO(TAG,"adv data changed ,current bound flag = %d",bound_flag);
}

void ble_timer_cb(vl_ble_timer_t timer){
    vl_status_t ret = VL_SUCCESS;
    ret = ble_state_chk();
    if(ret != VL_SUCCESS){
        VL_LOG_DEBUG(TAG,"ble state check error %d",ret);
    }
    ret = ble_recv_handle();
    if(ret != VL_SUCCESS){
        VL_LOG_DEBUG(TAG,"ble recv handle error %d",ret);
    }
    ret = ble_send_queue_handle();
    if(ret != VL_SUCCESS){
        VL_LOG_DEBUG(TAG,"ble send queue handle error %d",ret);
    }
}

void ble_task(PVOID paramptr){
    /*initialize ble struct*/

    memcpy(ble_param.adv_data, adv_data_const, BLE_ADV_DATA_LEN);
    memcpy(ble_param.scan_rsp_data, scan_rsp_data_const, BLE_SCAN_RSP_DATA_LEN);

    ble_param.adv_data_len = BLE_ADV_DATA_LEN;
    ble_param.scan_rsp_data_len = BLE_SCAN_RSP_DATA_LEN;

    memcpy(ble_param.service->svc_uuid, service_uuid, ATT_UUID_128_LEN);
    memcpy(ble_param.service->character[0].uuid, write_uuid, ATT_UUID_128_LEN);
    memcpy(ble_param.service->character[1].uuid, notify_uuid, ATT_UUID_128_LEN);

    ble_param.service->character[0].permission = GATT_PROP_WRITE_CMD;
    ble_param.service->character[1].permission = GATT_PROP_NOTI | GATT_PROP_INDI;

    fill_advContext(&ble_param);

    vl_ble_initialize(&ble_param);

    if(vl_iot_boundf_read() == 1){
        vl_ble_startAdv();
    }

    /* register callback function */
    vl_timer_creat(HIL_TIMER_BLE,50,VL_TIMER_REPEAT,ble_timer_cb);
    vl_timer_start(HIL_TIMER_BLE);
}


#ifdef __cplusplus
extern }
#endif