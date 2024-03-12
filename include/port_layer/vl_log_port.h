/**
 * @file vl_log_port.h
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK
 * @version 1.0.0
 * @date 2024-02-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef VL_LOG_PORT_H
#define VL_LOG_PORT_H

#include "vl_type.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief Initialize SDK log module
 * 
 */
void vl_log_init(void);


/**
 * @brief Send SDK LOG with String
 * 
 * @param tag 
 * @param format 
 * @param ... 
 */
void vl_log_write(UINT8* log_buf,UINT16 bufsize);



#ifdef __cplusplus
}
#endif

#endif