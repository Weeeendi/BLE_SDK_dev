/**
 * @file vl_ble_base_port.c
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,all functions by port layer,you need redefine it depending on your hardware.
 * @version 1.0.0
 * @date 2024-02-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "vl_ble_port.h"

/**
 * @brief Initialize ble stack
 * 
 * @return UINT8 
 */
__VL_BLE_WEAK UINT8 ble_initialize(void){

}

/**
 * @brief Open ble advertisement
 * This is a weak implementation. You can define the function implementation elsewhere
 * @param advertisementData 
 * @param scanResponseData 
 * @param adv_len 
 * @param scan_rsp_len 
 * @return void 
 */
__VL_BLE_WEAK void ble_startAdv(UINT8* advertisementData,UINT8* scanResponseData,UINT8 adv_len,UINT8 scan_rsp_len){

}

/**
 * @brief Change ble advertisement data
 * This is a weak implementation. You can define the function implementation elsewhere
 * @param advertisementData 
 * @param adv_len 
 * @return void 
 */
__VL_BLE_WEAK void ble_changeAdvertisingData(UINT8* advertisementData,UINT8 adv_len){

}

/**
 * @brief Stop ble advertisement
 * This is a weak implementation. You can define the function implementation elsewhere
 * @return void 
 */
__VL_BLE_WEAK void ble_stopAdv(void){

}

/**
 * @brief disconnect ble connection
 * This is a weak implementation. You can define the function implementation elsewhere
 * @return void 
 */
__VL_BLE_WEAK void ble_disconnect(void){

}


/**
 * @brief set ble connection parameter
 * 
 * @param interval 
 * @param latency 
 * @param timeout 
 * @param min_interval 
 * @param max_interval 
 * @return void 
 */
__VL_BLE_WEAK void ble_setConnectionParameter(UINT16 interval,UINT16 latency,UINT16 timeout,UINT16 min_interval,UINT16 max_interval){

}


/**
 * @brief set ble MTU (ATT-3)
 * 
 * @param data 
 * @param len 
 * @return void 
 */
__VL_BLE_WEAK void ble_MTUset(UINT8 mtu){

}

/**
 * @brief get device mac address
 * 
 * @param mac 
 * @return void 
 */
__VL_BLE_WEAK void ble_mac_get(uint8_t *mac){

}

/**
 * @brief send data to ble device
 * 
 * @param data 
 * @param len 
 * @return void 
 */
__VL_BLE_WEAK void ble_send_data(UINT8* data,UINT16 len){

}


