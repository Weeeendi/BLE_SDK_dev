/**
 * @file vl_flash_port.c
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK
 * @version 1.0.0
 * @date 2024-03-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "vl_type.h"
#include "vl_flash_port.h"


__VL_BLE_WEAK vl_status_t vl_nv_flash_init(){
    //TODO:You need redefine it depending on your hardware.
}

__VL_BLE_WEAK vl_status_t vl_nv_flash_write(UINT32 addr, UINT8* data,UINT16 len){
    //TODO:You need redefine it depending on your hardware.
}

__VL_BLE_WEAK vl_status_t vl_nv_flash_read(UINT32 addr,UINT8* data,UINT16 len){
    //TODO:You need redefine it depending on your hardware.
}