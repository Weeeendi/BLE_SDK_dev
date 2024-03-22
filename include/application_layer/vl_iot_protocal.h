/**
 * @file vl_iot_protocal.h
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,This file define about ble applicate layer protocal.
 * @version 1.0.0
 * @date 2024-02-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "vl_type.h"
/***************************************DP ABOUT DEFINE***********************************/

//=============================================================================
//dp data point type
//=============================================================================
typedef enum {
    DP_TYPE_RAW = 0x00,      // RAW
    DP_TYPE_BOOL = 0x01,     // Bool
    DP_TYPE_VALUE = 0x02,    // Value
    DP_TYPE_STRING = 0x03,   // String
    DP_TYPE_ENUM = 0x04,     // Enum
    DP_TYPE_BITMAP = 0x05    // Bitmap
} DP_Type;

//=============================================================================
//download dp cmd
//=============================================================================
typedef struct {
  unsigned char dp_id;                            //dp id
  unsigned char dp_type;                          //dp type
} DOWNLOAD_CMD_S;
