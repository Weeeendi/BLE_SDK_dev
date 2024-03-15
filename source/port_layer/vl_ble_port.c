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

/**
 * @brief disconnect ble connection
 * This is a weak implementation. You can define the function implementation elsewhere
 * @return VOID 
 */
__VL_BLE_WEAK vl_status_t vl_ble_disconnect(VOID){
    //TODO:You need redefine it depending on your hardware.
}


/**
 * @brief set ble connection parameter
 * 
 * @param interval 
 * @param latency 
 * @param timeout 
 * @param min_interval 
 * @param max_interval 
 * @return VOID 
 */
__VL_BLE_WEAK vl_status_t vl_ble_setConnParam(UINT16 interval,UINT16 latency,UINT16 timeout,UINT16 min_interval,UINT16 max_interval){
    //TODO:You need redefine it depending on your hardware.
}


/**
 * @brief set ble MTU (ATT-3)
 * 
 * @param data 
 * @param len 
 * @return VOID 
 */
__VL_BLE_WEAK vl_status_t vl_ble_MTUset(UINT8 mtu){
    //TODO:You need redefine it depending on your hardware.
}

/**
 * @brief get device mac address
 * 
 * @param mac 
 * @return VOID 
 */
__VL_BLE_WEAK vl_status_t vl_ble_mac_get(uint8_t *mac){
    //TODO:You need redefine it depending on your hardware.
}

/**
 * @brief send data to ble device
 * 
 * @param data 
 * @param len 
 * @return VOID 
 */
__VL_BLE_WEAK vl_status_t vl_ble_send_data(UINT8* data,UINT16 len){
    //TODO:You need redefine it depending on your hardware.
}

__VL_BLE_WEAK UINT32 vl_ble_generate_randNum(VOID){
    //TODO:You need redefine it depending on your hardware.
    return 0;
}
