/**
 * @file vl_rtc_port.c
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,if you need to use rtc,please implement this file
 * @version 1.0.0
 * @date 2024-03-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "vl_rtc_port.h"

__VL_BLE_WEAK VOID rtc_init(UINT8 WakeUpClock,UINT16 WakeUpCounter){
    
}

__VL_BLE_WEAK VOID rtc_start(VOID){
    
}

__VL_BLE_WEAK VOID rtc_stop(VOID){

}

__VL_BLE_WEAK VOID rtc_set_time(rtc_time_t *rtcTime){

}

__VL_BLE_WEAK VOID rtc_get_time(rtc_time_t *rtcTime){

}
