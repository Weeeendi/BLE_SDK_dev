/**
 * @file vl_ble_port.c
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,all functions by port layer,you need redefine it depending on your hardware.
 * @version 1.0.0
 * @date 2024-02-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "vl_ble_port.h"

#ifdef __cplusplus
extern "C" {
#endif


__VL_BLE_WEAK vl_status_t vl_ble_initialize(vl_ble_attr_param_t* attr_param){
    //TODO:You need redefine it depending on your hardware.
}


__VL_BLE_WEAK vl_status_t vl_ble_startAdv(VOID){
    //TODO:You need redefine it depending on your hardware.
}


__VL_BLE_WEAK vl_status_t vl_ble_stopAdv(VOID){
    //TODO:You need redefine it depending on your hardware.
}


__VL_BLE_WEAK vl_status_t vl_ble_changeAdvData(UINT8* advertisementData,UINT8 adv_len){
    //TODO:You need redefine it depending on your hardware.
}

__VL_BLE_WEAK vl_status_t vl_ble_setAdvParam(UINT16 adv_interval,UINT16 max_interval,UINT16 min_interval){
    //TODO:You need redefine it depending on your hardware.
}

__VL_BLE_WEAK vl_status_t vl_ble_disconnect(VOID){
    //TODO:You need redefine it depending on your hardware.
}

__VL_BLE_WEAK vl_status_t vl_ble_setConnParam(UINT16 interval,UINT16 latency,UINT16 timeout,UINT16 min_interval,UINT16 max_interval){
    //TODO:You need redefine it depending on your hardware.
}

__VL_BLE_WEAK vl_status_t vl_ble_MTUset(UINT8 mtu){
    //TODO:You need redefine it depending on your hardware.
}

__VL_BLE_WEAK vl_status_t vl_ble_mac_get(uint8_t *mac){
    //TODO:You need redefine it depending on your hardware.
}

__VL_BLE_WEAK UINT8 vl_ble_connect_status_get(VOID){
    //TODO:You need redefine it depending on your hardware.
}

__VL_BLE_WEAK vl_status_t vl_ble_send_data(UINT8* data,UINT16 len){
    //TODO:You need redefine it depending on your hardware.
}

__VL_BLE_WEAK UINT32 vl_ble_generate_randNum(VOID){
    //TODO:You need redefine it depending on your hardware.
    return 0;
}

__VL_BLE_WEAK VOID vl_ble_device_reset(VOID){
    //TODO:You need redefine it depending on your hardware.
}

#ifdef __cplusplus
}
#endif