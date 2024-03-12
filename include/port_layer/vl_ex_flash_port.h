/**
 * @file vl_ex_flash_port.h
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,If you need to use off chip storage, 
 * please implement this function interface, otherwise it can be ignored
 * @version 1.0.0
 * @date 2024-03-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef VL_EX_FLASH_PORT_H
#define VL_EX_FLASH_PORT_H

#include "vl_type.h"

/**
 * @brief This function initializes the external flash memory subsystem, preparing it for reading and writing data.
 * 
 * @return No return value (void).
 */
void vl_ex_flash_init();

/**
 * @brief This function writes data to the external flash memory subsystem. 
 * 
 * @param addr The address in external flash memory where the data is to be written.
 * 
 * @param data A pointer to the data to be written.
 * @param len The length of the data to be written.
 * 
 * @return No return value (void).
 * 
 */
void vl_ex_flash_write(UINT32 addr, UINT8* data,UINT16 len);

/**
 * @brief This function reads data from the external flash memory subsystem. 
 * 
 * @param addr The address in external flash memory where the data is to be read.
 * 
 * @param data A pointer to the data to be read.
 * @param len The length of the data to be read.
 * 
 * @return No return value (void).
 * 
 */
void vl_ex_flash_read(UINT32 addr,UINT8* data,UINT16 len);

#endif
