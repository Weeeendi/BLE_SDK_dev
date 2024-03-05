/**
 * @file vl_uart_port.h
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of YJ_BLE_SDK
 * @version 1.0.0
 * @date 2024-02-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef YJ_UART_PORT_H
#define YJ_UART_PORT_H

#include "vl_type.h"
/**
 * @brief Initialize the UART port.
 * 
 */
void vl_uart_init(void);

/**
 * @brief Send Data by UART port.
 * 
 * @param data 
 * @param len 
 */
void vl_uart_write_buf(UINT8* data,UINT16 len);

/**
 * @brief Read Data by UART port.
 * 
 * @param data 
 * @param len 
 */
void vl_uart_read_buf(UINT8* data,UINT16 len);


#endif