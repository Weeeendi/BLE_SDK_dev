/**
 * @file vl_timer_port.c
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,implement SDK timer module
 * @version 1.0.0
 * @date 2024-03-18
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "vl_timer_port.h"


#ifdef __cplusplus
extern "C" {
#endif

__VL_BLE_WEAK vl_status_t vl_timer_init(void)
{
    //TODO:You need redefine it depending on your hardware.
    return VL_SUCCESS;
}

__VL_BLE_WEAK vl_status_t vl_timer_creat(UINT8 timer_id, UINT16 timeout, BOOL repeat, FunctionPointer callback){
    //TODO:You need redefine it depending on your hardware.
    return VL_SUCCESS;
}

__VL_BLE_WEAK vl_status_t vl_timer_start(UINT8 timer_id)
{
    //TODO:You need redefine it depending on your hardware.
    return VL_SUCCESS;
}

__VL_BLE_WEAK vl_status_t vl_timer_stop(UINT8 timer_id)
{
    //TODO:You need redefine it depending on your hardware.
    return VL_SUCCESS;
}

#ifdef __cplusplus
}
#endif