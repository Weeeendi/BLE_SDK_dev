/**
 * @file vl_ble_config.h
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK. abort ble configure
 * @version 1.0.0
 * @date 2024-03-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef VL_BLE_CONFIG_H
#define VL_BLE_CONFIG_H

#include "vl_type.h"

#define MAC_LEN 	  6
#define MAC_STR_LEN   12

/******************************************************************************
							1: modify product information
******************************************************************************/

#define PROTOCAL_VER         1						  //protocal version
#define SHARE_ENABLE         0                        //Whether support share function device

/*
 * if 1 ,after update mac will reset.
 */
#ifndef  BLE_DEVICE_MAC_UPDATE_RESET
#define  BLE_DEVICE_MAC_UPDATE_RESET   0 
#endif

/******************************************************************************
						 2: define the sending and receiving cache:
******************************************************************************/				
#ifndef BLE_REV_DATA_QUEUE_SIZE
#define BLE_REV_DATA_QUEUE_SIZE   10			//The size of the data receiving queue can be reduced if the RAM of MCU is not enough.
#endif

#ifndef BLE_SEND_DATA_QUEUE_SIZE
#define BLE_SEND_DATA_QUEUE_SIZE  20
#endif 

#define BLE_BUF_LMT          255              //Firmware upgrade buffer, large cache required, must be greater than 220


/******************************************************************************
						 3: define ble adv connect param:
******************************************************************************/	
// 0.625ms as unit ，20ms ~ 10.24s
#define MIN_ADV_INV 320   // 200ms
#define MAX_ADV_INV 380  // 1000ms


//-1.25ms as unit，6~3200【7.5ms~4s】
#define MIN_CONN_INV 200   // 250ms
#define MAX_CONN_INV 240  // 300ms

/*
 * gatt mtu max sizes 
 */
#ifndef BLE_DATA_MTU_MAX
	#define BLE_DATA_MTU_MAX  247
#endif

#ifndef vl_ble_connect_monitor_timeout_ms
	#define vl_ble_connect_monitor_timeout_ms 3000*10  //30s(3000*10ms)
#endif


#endif