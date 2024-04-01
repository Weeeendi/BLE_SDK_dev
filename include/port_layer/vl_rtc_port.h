/**
 * @file vl_rtc_port.h
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,if your need to use rtc,please include this file
 * @version 1.0.0
 * @date 2024-03-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef VL_RTC_PORT_H
#define VL_RTC_PORT_H

#include "vl_type.h"
#include "vl_config.h"

#ifdef __cplusplus
extern "C" {
#endif


//time struct
typedef struct rtc_time_struct
{
    uint16_t ui16Year;       // 1970~2038
    uint8_t ui8Month;       // 1~12
    uint8_t ui8DayOfMonth;  // 1~31
    uint8_t ui8Week;        // 1-7
    uint8_t ui8Hour;        // 0~23
    uint8_t ui8Minute;      // 0~59
    uint8_t ui8Second;      // 0~59
    int16_t time_zone;      //-12~12
}rtc_time_t;

/**
 * @brief RTC init
 * 
 * @param WakeUpClock    clock source 
 * @param WakeUpCounter  0~0xFFFF
 */
VOID rtc_init(UINT8 WakeUpClock,UINT16 WakeUpCounter);

/**
 * @brief RTC start
 * 
 */
VOID rtc_start(VOID);

/**
 * @brief RTC stop
 * 
 * @return VOID 
 */
VOID rtc_stop(VOID);

/**
 * @brief set rtc time
 * 
 * @param rtcTime ---@ref rtc_time_t
 * @return VOID 
 */
VOID rtc_set_time(rtc_time_t *rtcTime);

/**
 * @brief get rtc time
 * 
 * @param rtcTime ---@ref rtc_time_t
 * @return VOID 
 */
VOID rtc_get_time(rtc_time_t *rtcTime);

#ifdef __cplusplus
}
#endif

#endif
