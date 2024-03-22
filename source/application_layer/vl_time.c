/**
 * @file vl_time.c
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,if you need RTC function,please enable VL_USE_RTC
 * @version 1.0.0
 * @date 2024-03-20
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "vl_time.h"
#include "vl_type.h"

static UINT32 unix_timestamp = 0;
