/**
 * @file vl_flash_port.h
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK
 * @version 1.0.0
 * @date 2024-02-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "vl_type.h"

#ifndef VL_FLASH_PORT_H
#define VL_FLASH_PORT_H

/**
 * Initializes Non-Volatile (NV) flash memory.
 *
 * This function initializes the non-volatile flash memory subsystem, preparing it for reading and writing data.
 * The NV flash memory is typically used to store persistent information that survives power cycles or resets.
 *
 * @return No return value (void).
 */
void vl_nv_flash_init();


/**
 * Writes data to Non-Volatile (NV) flash memory.
 *
 * This function writes data to the non-volatile flash memory subsystem. 
 * 
 * @param addr The address in NV flash memory where the data is to be written.
 * @param data A pointer to the data to be written.
 * @param len The length of the data to be written.
 * @return No return value (void).
 */
void vl_nv_flash_write(UINT32 addr, UINT8* data,UINT16 len);

/**
 * @brief This function reads data from the non-volatile flash memory subsystem. 
 * 
 * @param addr The address in NV flash memory where the data is to be read.
 * @param data A pointer to the data to be read.
 * @param len The length of the data to be read.
 * @return No return value (void).
 */
void vl_nv_flash_read(UINT32 addr,UINT8* data,UINT16 len);


/**
 * @brief This function erases data from the non-volatile flash memory subsystem. 
 * 
 * @param addr The address in NV flash memory where the data is to be erased.
 * @param len The length of the data to be erased.
 * @return No return value (void).
 */
void vl_nv_flash_erase(UINT32 addr,UINT8 len);



#endif

