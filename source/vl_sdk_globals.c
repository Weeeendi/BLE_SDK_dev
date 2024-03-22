/**
 * @file vl_sdk_globals.c
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,this file define all the global variables.
 * @version 1.0.0
 * @date 2024-03-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "vl_sdk_globals.h"
#include "vl_ble_stdlib.h"

const uint8_t TEST_DP_LIST[] = {DPID_START,DPID_HEADLIGHT_SWITCH,DPID_SPEED};
const UINT8 AES128KEY[16] = {0x6a, 0x36, 0x2f, 0x30, 0x8c, 0xc9, 0xf0, 0x3d, 0x16, 0xc5, 0xee, 0x84, 0xf9, 0xfc, 0x7c, 0xa1};

vl_ble_obj_t vl_ble_obj;
vl_iot_obj_t vl_iot_obj;

VOID set_iot_workMode(bool mode) {
    vl_iot_obj.work_mode = mode;
}

VOID get_iot_workMode(bool *mode) {
    *mode = vl_iot_obj.work_mode;
}


vl_status_t save_state_aboutBLE(VOID){
    return vl_nv_flash_write(BLE_INFO_STORAGE_ADDR_START,&vl_ble_obj, sizeof(vl_ble_obj));
}

vl_status_t load_state_aboutBLE(VOID){
    return vl_nv_flash_read(BLE_INFO_STORAGE_ADDR_START,&vl_ble_obj, sizeof(vl_ble_obj));
}

VOID sdk_globals_init() {
    // init global variables
    memset(&vl_ble_obj,0x00,sizeof(vl_ble_obj_t));
    memset(&vl_iot_obj,0x00,sizeof(vl_iot_obj_t));
}
