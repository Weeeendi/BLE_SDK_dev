/**
 * @file vl_config.h
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK
 * @version 1.0.0
 * @date 2024-02-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#ifndef VL_BLE_BASE_PORT_H
#define VL_BLE_BASE_PORT_H

#define PRODUCT_KEY "XXXXXXXXXX" // 10位字符,产品唯一标识
#define PRODUCT_KEYLEN 10      //产品id 10位
#define DEVICEID_LEN   16

// 版本号信息 小于等于 16字节
#define IOT_MCU_SN DEVICEID
#define IOT_MCU_SOFTVER "1.0.0"
#define IOT_MCU_HARDVER "1.0.0"

#ifndef VL_LOG_ENABLE
#define VL_LOG_ENABLE 1						//wheater enable LOG
#endif

#ifdef VL_LOG_ENABLE
#ifndef VL_APP_LOG_ENABLE
#define VL_APP_LOG_ENABLE 1					//wheater enable App LOG
#endif
#ifndef VL_BLE_LOG_ENABLE
#define VL_BLE_LOG_ENABLE 1					//wheater enable BLE LOG
#endif
#endif

#ifndef SUPPORT_APP_TEST					//wheater support App test
#define SUPPORT_APP_TEST 0
#endif

#ifndef VL_USE_RTC
#define VL_USE_RTC 0						//wheater use rtc
#endif

#ifndef VL_USE_OTA
#define VL_USE_OTA 0						//wheater use ota
#endif


/******************************************************************************
								Flash Address define
 ******************************************************************************/
#ifndef VL_FLASH_ADDR_START
#define VL_FLASH_ADDR_START 0x8000000			//redefine it by your own device platform
#endif

#define TOKEN_STORAGE_ADDR_START 	(0xA000+VL_FLASH_ADDR_START)
#define TOKEN_STORAGE_ADDR_END   	(0xAFFF+VL_FLASH_ADDR_START)
#define BLE_INFO_STORAGE_ADDR_START (0xB000+VL_FLASH_ADDR_START)
#define BLE_INFO_STORAGE_ADDR_END   (0xBFFF+VL_FLASH_ADDR_START)
#define USER_APP_START_ADDR         (0xE000+VL_FLASH_ADDR_START)

#define FLASH_PAGE_SIZE 0x1000

/* DP define*/
/******************************************************************************
		dp data point number redefinition,just for example
******************************************************************************/
// 启动/熄火开关(可下发可上报)
// 备注:
#define DPID_START 1

// 解防设防(可下发可上报)
// 备注 bool
#define DPID_GUARD 2

// 档位模式(可下发可上报)
// 备注:
#define DPID_LEVEL_MODE 3

// 挡位(可下发可上报)
// 备注:
#define DPID_LEVEL 4

// 寻车(可下发可上报)
// 备注:
#define DPID_SEARCH 5

// 助力推行(可下发可上报)
// 备注:
#define DPID_BOOST_PUSH 9

// 单位设置(可下发可上报)
// 备注:
#define DPID_UNIT_SET 11

// 速度限制(可下发可上报)
// 备注:
#define DPID_SPEED_LIMIT 13

// 前大灯开关(可下发可上报)
// 备注:
#define DPID_HEADLIGHT_SWITCH 14

// 尾灯开关(可下发可上报)
// 备注:
#define DPID_TAILLIGHT_SWITCH 15

// speed(只上报)
// 备注:
#define DPID_SPEED 16


#endif