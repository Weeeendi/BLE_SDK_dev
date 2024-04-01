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
#include "vl_ble_productTest.h"
#include "vl_ble_port.h"
#include "vl_log.h"
#include "vl_config.h"

const char TAG[] = "VL_IOT";


VOID vl_test_process(VOID){

}


VOID vl_iot_process(VOID){

#if VL_TEST
    vl_test_process();
#endif

   //Do Loop by iot work mode
    switch(vl_iot_obj.work_mode){
        case NORMAL_MODE:
            break;
        case UPDATE_MODE:
            break;
        case FACTORY_MODE:
            break;
        case LOWERPOWER_MODE:
            break;
        default:
            break;
    }
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

VOID vl_test_init(VOID){

}

VOID vl_iot_init(VOID){
    token_info_t token_info;
    UINT8 mac_addr[6];
    UINT8 mac_str[12];
    read_TokenInfo(&token_info);

    vl_iot_obj.work_mode = NORMAL_MODE;

    vl_ble_mac_get(mac_addr);

#if VL_TEST
    vl_test_init();
#endif 

    if(Is_Token())
    {
        memcpy(vl_iot_obj.deviceID,token_info.deviceIotId,DEVICEID_LEN);
    }
    else
    {
        YJ_LOG_DEBUG("No Auth Device");
        strcpy((char*)vl_iot_obj.deviceID,(char*)"No Auth Device");
    }

    VL_LOG_INFO(TAG,"productID: %s",PRODUCT_KEY);
    VL_LOG_INFO(TAG,"deviceID: %s",vl_iot_obj.deviceID);
    VL_LOG_INFO(TAG,"MAC: %s",mac_addr);

    //start ble task
    ble_task();
}