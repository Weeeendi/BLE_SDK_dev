/**
 * @file vl_sdk_globals.h
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,this file  all the global variables.
 * @version 1.0.0
 * @date 2024-03-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __VL_SDK_GLOBALS_H__
#define __VL_SDK_GLOBALS_H__

#include "vl_type.h"

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



/* global variables */

extern const UINT8 TEST_DP_LIST[];
extern const UINT8 AES128KEY[16];

extern vl_ble_obj_t vl_ble_obj;
extern vl_iot_obj_t vl_iot_obj;


#endif