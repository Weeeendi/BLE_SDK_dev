/**
 * @file yj_config.h
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


#define PRODUCT_KEY "XXXXXXXXXX"    //10位字符,产品唯一标识

//版本号信息 小于等于 16字节
#define IOT_MCU_SN      DEVICEID
#define IOT_MCU_SOFTVER "1.0.0"
#define IOT_MCU_HARDVER "1.0.0"


#ifndef VL_LOG_ENABLE
#define VL_LOG_ENABLE 1
#endif


#ifdef VL_LOG_ENABLE

#ifndef VL_APP_LOG_ENABLE
#define VL_APP_LOG_ENABLE 1
#endif

#ifndef VL_BLE_LOG_ENABLE
#define VL_BLE_LOG_ENABLE 1
#endif

#endif

/******************************************************************************
 								System working mode
 ******************************************************************************/
#define         NORMAL_MODE             0x00                                    //Normal working state
#define         FACTORY_MODE            0x01                                    //Factory model
#define         UPDATE_MODE             0X02      
#define         LOWERPOWER_MODE         0x03


/* DP define*/
/******************************************************************************
						  dp data point serial number redefinition
******************************************************************************/
//启动/熄火开关(可下发可上报)
//备注:
#define DPID_START 1

//解放设防(可下发可上报)
//备注 bool
#define DPID_GUARD 2

//档位模式(可下发可上报)
//备注:
#define DPID_LEVEL_MODE 3
//挡位(可下发可上报)
//备注:
#define DPID_LEVEL 4
//寻车(可下发可上报)
//备注:
#define DPID_SEARCH 5
//定速巡航(可下发可上报)
//备注:
#define DPID_CRUISE 6
//轮径设置(可下发可上报)
//备注:
#define DPID_WHEEL_DIAMETER 8
//助力推行(可下发可上报)
//备注:
#define DPID_BOOST_PUSH 9
//单位设置(可下发可上报)
//备注:
#define DPID_UNIT_SET 11
//速度限制(可下发可上报)
//备注:
#define DPID_SPEED_LIMIT 13
//前大灯开关(可下发可上报)
//备注:
#define DPID_HEADLIGHT_SWITCH 14
//尾灯开关(可下发可上报)
//备注:
#define DPID_TAILLIGHT_SWITCH 15
//speed(只上报)
//备注:
#define DPID_SPEED 16
//单次骑行时间(只上报)
//备注:




#endif