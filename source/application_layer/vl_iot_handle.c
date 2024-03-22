/**
 * @file vl_iot_handle.c
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK, iot task handler.
 * @version 1.0.0
 * @date 2024-03-15
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "vl_iot_handle.h"
#include "vl_sdk_globals.h"


VOID vl_iot_process(VOID){
    
}

/**
 * @brief This function is used to set the iot work status
 * 
 * @param status 
 * @return VOID 
 */
VOID iot_work_status_set(iot_work_mode_s status){
    vl_iot_obj.work_mode = status;
}

/**
 * @brief This function is used to get the iot work status
 * 
 * @return iot_work_mode_s 
 */
iot_work_mode_s iot_work_status_get(VOID){
    return vl_iot_obj.work_mode;
}

/**
 * @brief Retrieves the current App bound status.
 * 
 * @return BOOL 0: not bound, 1: bound 
 */
BOOL vl_iot_boundf_read(VOID){
    return vl_iot_obj.bound_flag;
}