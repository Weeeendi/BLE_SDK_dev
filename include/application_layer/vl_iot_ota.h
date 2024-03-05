/**
 * @file vl_iot_ota.h
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,control OTA process
 * @version 1.0.0
 * @date 2024-02-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef VL_IOT_OTA_H
#define VL_IOT_OTA_H

/**
 * @brief OTA_STATE_T
 * 
 */
typedef enum{
    VL_IOT_OTA_STATUS_INIT,
    VL_IOT_OTA_STATUS_DOWNLOAD,
    VL_IOT_OTA_STATUS_VERIFY,
    VL_IOT_OTA_STATUS_UPDATE,
    VL_IOT_OTA_STATUS_UNKNOW,
}OTA_STATE_T;




#endif