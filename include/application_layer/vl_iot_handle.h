/**
 * @file vl_iot_handle.h
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK
 * @version 1.0.0
 * @date 2024-02-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef VL_IOT_HANDLE_H
#define VL_IOT_HANDLE_H

#include "vl_type.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  iot struct
 * 
 */
vl_iot_type_t vl_iot_obj;


/**
 * @brief Initialize the iot struct
 * 
 * @return UINT8 
 */
UINT8 vl_iot_handle_init(VOID);











#ifdef __cplusplus
}
#endif

#endif