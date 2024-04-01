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

VOID vl_iot_handle_init(VOID);
BOOL vl_iot_boundf_read(VOID);

#ifdef __cplusplus
}
#endif

#endif