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
#include "vl_type.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ATT_UUID_128_LEN 16

#define BLE_ADV_DATA_LEN  (13+PRODUCTID_LEN)
#define BLE_SCAN_RSP_DATA_LEN  18

typedef void (*DpDataCallback)(const char* data, size_t size);


vl_ble_obj_t vl_ble_obj;


const unsigned char service_uuid[ATT_UUID_128_LEN] = {0xFB, 0x34, 0x9B, 0x5F,
                                                        0x80, 0x00, 0x00, 0x80, 
                                                        0x00, 0x00, 0x10, 0x00, 
                                                        0x80, 0xFD, 0x00, 0x00};

const unsigned char write_uuid[ATT_UUID_128_LEN] = {0xD0, 0x07, 0x9B, 0x5F,
                                                    0x80, 0x00, 0x01, 0x80, 
                                                    0x01, 0x10, 0x00, 0x00,
                                                    0x01, 0x00, 0x00, 0x00};

const unsigned char notify_uuid[ATT_UUID_128_LEN] = {0xD0, 0x07, 0x9B, 0x5F,
                                                     0x80, 0x00, 0x01, 0x80, 
                                                     0x01, 0x10, 0x00, 0x00,
                                                     0x02, 0x00, 0x00, 0x00};


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

vl_status_t ble_status_set(ble_status_s status){
    vl_ble_obj.connect_state = status;
    return VL_SUCCESS;
}


vl_status_t save_state_aboutBLE(void){
    return vl_nv_flash_write(BLE_INFO_STORAGE_ADDR_START,&vl_ble_obj, sizeof(vl_ble_obj));
}
vl_status_t load_state_aboutBLE(void){
    return vl_nv_flash_read(BLE_INFO_STORAGE_ADDR_START,&vl_ble_obj, sizeof(vl_ble_obj));
}

void vl_dp_event_callback(const char* data, size_t size){

}

void vl_ble_register_callback(DpDataCallback vl_dp_event_callback);

void fill_advContext(vl_ble_attr_param_t* param){
    if(param == NULL) return;

    if(SHARE_ENABLE){
        param->adv_data[11] |=0x04 ;
    }else{
        param->adv_data[11] &=(~0x04);
    }
    
    if(vl_ble_obj.bound_flag == 1){
        param->adv_data[11] |=0x08;
    }else{
        param->adv_data[11] &=(~0x08);
    }

    param->adv_data[11] &=(~(PROTOCAL_VER<<4));
    param->adv_data[11] |= (PROTOCAL_VER<<4);

    memcpy(&param->scan_rsp_data[2],vl_iot_obj.deviceID,DEVICEID_LEN);
    memcpy((uint8_t*)&param->adv_data[13],PRODUCT_KEY, PRODUCTID_LEN);
}

void ble_task(PVOID paramptr){
    /*initialize ble struct*/
    static vl_ble_attr_param_t ble_param;

    memcpy(ble_param.adv_data, adv_data_const, BLE_ADV_DATA_LEN);
    memcpy(ble_param.scan_rsp_data, scan_rsp_data_const, BLE_SCAN_RSP_DATA_LEN);

    ble_param.adv_data_len = BLE_ADV_DATA_LEN;
    ble_param.scan_rsp_data_len = BLE_SCAN_RSP_DATA_LEN;

    memcpy(ble_param.service->svc_uuid, service_uuid, ATT_UUID_128_LEN);
    memcpy(ble_param.service->character[0].uuid, write_uuid, ATT_UUID_128_LEN);
    memcpy(ble_param.service->character[1].uuid, notify_uuid, ATT_UUID_128_LEN);

    fill_advContext(&ble_param);

    vl_ble_initialize(&ble_param);

    /* register callback function */
    
}


#ifdef __cplusplus
extern }
#endif