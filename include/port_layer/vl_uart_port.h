/**
 * @file vl_uart_port.h
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,It is Uart Interface
 * @version 1.0.0
 * @date 2024-02-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef VL_UART_PORT_H
#define VL_UART_PORT_H

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