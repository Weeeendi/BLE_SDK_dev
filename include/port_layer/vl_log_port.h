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
 * @brief  Change SDK log level
 * 
 * @param level Change log level @ref vl_log_level_t
 */
void vl_log_change_level(vl_log_level_t level);

/**
 * @brief Send SDK LOG with String
 * 
 * @param tag 
 * @param format 
 * @param ... 
 */
void vl_log_write(const CHAR* tag, const CHAR* format, ...);


/**
 * @brief Send SDK LOG with hex data
 * 
 * @param tag 
 * @param format 
 * @param ... 
 */
void vl_log_write_hex( const CHAR* tag, const CHAR* format, ...);

#ifdef __cplusplus
}
#endif

#endif