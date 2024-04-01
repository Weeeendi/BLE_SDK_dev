/**
 * @file vl_time.h
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,if you need RTC function,please enable VL_USE_RTC
 * @version 1.0.0
 * @date 2024-03-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef VL_TIME_H
#define VL_TIME_H

#include "vl_rtc_port.h"

#if VL_USE_RTC

#include "vl_type.h"
#include "vl_rtc_port.h"

#ifdef __cplusplus
extern "C" {
#endif

#define YEAR_BASE 2000  //RTC record  year only have information of year 2000~2038

/**
 * @brief rtc to unix time to Beijing time
 * @param {uint32_t} unixTime ---unix timestamp
 * @param {rtc_time_t} *rtcTime ---rtc timestamp
 * @param {bool} daylightSaving ---summer time or not
 * @return {*}
 */
void covUnixTimeStp2Rtc(uint32_t unixTime, rtc_time_t *rtcTime,bool daylightSaving);

/**
 * @brief rtc time convert to unix time
 * @param {rtc_time_t} *rtcTime need to convert time
 * @param {bool} daylightSaving 
 * @return {*}Unix timestamp 
 */
uint32_t covRtc2UnixTimeStp(rtc_time_t *rtcTime,bool daylightSaving );

/**
 * @brief Set the rtc time object
 * 
 * @param LocalTime 
 */
VOID set_rtc_time(rtc_time_t LocalTime);

/**
 * @brief  get the current unix TimeStamp 
 */
uint32_t get_UnixTimeStamp(bool daylightSaving);

#ifdef __cplusplus
}
#endif

#endif

#endif