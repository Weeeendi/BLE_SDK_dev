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

/***************************************DP ABOUT DEFINE***********************************/

//=============================================================================
//Byte order of the frame
//=============================================================================
#define         SN                              0
#define         ACK_SN                          4      
#define         FUNC_CODE                       8
#define         LENGTH_HIGH                     10
#define         LENGTH_LOW                      11
#define         DATA_START                      12

//=============================================================================
//app data event type
//=============================================================================
#define VL_BLE_EVT_BASE                         0x0000
#define VL_BLE_EVT_DEVINFO_QUERY                0x0000
#define VL_BLE_EVT_PAIR                         0x0001
#define	VL_BLE_EVT_DP_DATA_REV                  0x0002
#define	VL_BLE_EVT_DP_QUERY                     0x0003
#define VL_BLE_EVT_DEVICE_UNBIND                0x0005
#define VL_BLE_EVT_FACTORY_RESET                0x0006
#define VL_BLE_EVT_OTA_START                    0x000C
#define VL_BLE_EVT_OTA_FILEINFO                 0x000D
#define VL_BLE_EVT_OTA_OFFSETREQ                0x000E
#define VL_BLE_EVT_OTA_DATA                     0x000F
#define VL_BLE_EVT_OTA_OVER                     0x0010
#define VL_BLE_EVT_PRODUCTION_TEST              0x0012
#define VL_BLE_EVT_ANOMALY_UNBOUND              0x0014
#define VL_BLE_EVT_APP_SYNC_TIME                0x0025    //When the device is online, the application sends this message to the device

#define	VL_BLE_EVT_DP_DATA_REPORTED             0x8001
#define VL_BLE_EVT_DP_DATA_WITH_TIME_REPORTED   0x8002
#define VL_BLE_EVT_TIME_REQ                     0x8003
//=============================================================================
//dp data point type
//=============================================================================
#define         DP_TYPE_RAW                     0x00				//RAW
#define         DP_TYPE_BOOL                    0x01	      //Bool
#define         DP_TYPE_VALUE                   0x02	      //Value
#define         DP_TYPE_STRING                  0x03				//String
#define         DP_TYPE_ENUM                    0x04				//Enum
#define         DP_TYPE_BITMAP                  0x05				//Bitmap

//=============================================================================
//BT work state
//=============================================================================
#define         BT_UN_BIND                      0x00        //Unbound state
#define         BT_NOT_CONNECTED                0x01        //Bound but not connected
#define         BT_CONNECTED                    0x02        //Bound and connected
#define         BT_CONN_NOBOUND                 0x03
#define         BT_SATE_UNKNOW                  0xff

//=============================================================================
//download dp cmd
//=============================================================================
typedef struct {
  unsigned char dp_id;                            //dp id
  unsigned char dp_type;                          //dp type
} DOWNLOAD_CMD_S;


//=============================================================================
//Ble data frame
//=============================================================================

void bt_write_frame(unsigned short fr_type, unsigned short len);