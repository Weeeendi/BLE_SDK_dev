/**
 * @file vl_iot_ota.h
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,control OTA process
 * @version 1.0.0
 * @date 2024-02-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef VL_BLE_OTA_H_
#define VL_BLE_OTA_H_

#include "vl_iot_protocal.h"
#include "vl_queue.h"
#include "vl_type.h"

              
/* OTA Configure */

#ifndef USE_EX_STORAGE                  //If using external flash
#define USE_EX_STORAGE  1
#endif

#ifndef SUPPORT_BREAK_RESUME             //support break resume
#define SUPPORT_BREAK_RESUME 0
#endif

#ifndef SUPPORT_PARTTHIRD_OTA         //support third party device OTA
#define SUPPORT_PARTTHIRD_OTA 0
#endif

#define OTA_VER 0x01
#define EXFLASH_PAGE_SIZE 0x1000
#define MAX_MCU_FIRMWARE_SIZE 0x32000     
#define OTA_OVERTIME_LEN                 (10*1000)    //over 10s, then quit OTA state


#if USE_EX_STORAGE //If using flash on chip,need redefine the flash address

//SECURE OTA FLAG ADDRDESS
#define SEC_IMAGE_OAD_HEADER_APP_FADDR       (0x01000)  /* Boot Reading area address,You can configure it by your own device platfrom*/
#define SEC_IMAGE_OAD_HEADER_RESUME_FADDR	 (0x02000)  /* Break resume area address,You can configure it by your own device platfrom*/
//SECURE OTA IMG ADDRDESS EX
#define SEC_BACKUP_OAD_HEADER_FADDR		     (0x10000)  /* Backup image area address start,You can configure it by your own device platfrom*/
#define SEC_BACKUP_ALLOC_END_FADDR		     (0x68000)  /* Backup image area address end,You can configure it by your own device platfrom*/

#else

//SECURE OTA FLAG ADDRDESS
#define SEC_IMAGE_OAD_HEADER_APP_FADDR       (0xFFFFF)  /* Boot Reading area address,You can configure it by your own device platfrom*/
#define SEC_IMAGE_OAD_HEADER_RESUME_FADDR	 (0xFFFFF)  /* Break resume area address,You can configure it by your own device platfrom*/
//SECURE OTA IMG ADDRDESS,if use this Need to synchronize adjust SEC_IMAGE_OAD_HEADER_APP_RUNNING
#define SEC_BACKUP_OAD_HEADER_FADDR		     (0xE000)  /* Backup image area address start,You can configure it by your own device platfrom*/
#define SEC_BACKUP_ALLOC_END_FADDR		     (0x38000)  /* Backup image area address end,You can configure it by your own device platfrom*/
#endif

#define VL_EVT_OTA_START                    0x000C
#define VL_EVT_OTA_FILEINFO                 0x000D
#define VL_EVT_OTA_OFFSETREQ                0x000E
#define VL_EVT_OTA_DATA                     0x000F
#define VL_EVT_OTA_OVER                     0x0010

typedef enum 
{
    VL_OTA_STATUS_NONE,
    VL_OTA_STATUS_START,
    VL_OTA_STATUS_FILE_INFO,
    VL_OTA_STATUS_FILE_OFFSET,
    VL_OTA_STATUS_FILE_DATA,
    VL_OTA_STATUS_FILE_END,
    VL_OTA_STATUS_MAX,
}vl_ota_status_t;

typedef enum{
    IoT_BLE, 
    IoT_MCU,
    IoT_4G,   
    Ebike_Dashboard,
    Ebike_Mcu,
    Ebike_BMS,
    Ebike_Ex1,  //reserve
    Ebike_Ex2,  //reserve
}vl_ota_type_t;

/***********************************************************************
 ********************* struct ******************************************
 **********************************************************************/
#pragma pack (4)
typedef struct{
	UINT8 OTA_Plan;//0x00 no need updata 01 need updata 
	UINT32 Target_SoftSize;//Download firmware size
	UINT32 Target_StartAddr;//Download firmware addr
    UINT32 crc32;     //Download firmware crc32
	UINT32 Flash_Addr;//Download firmware start on flash
} OTA_Info_t;
#pragma pack()


typedef struct
{
    UINT32 firmware_file_version;
    UINT32 firmware_file_length;
    UINT32 firmware_file_crc;
    UINT8  firmware_file_type;
} firmware_file_info_t;

typedef struct
{
    UINT32 firmware_image_offset_last;
    UINT32 firmware_image_crc_last;
} firmware_progress_t;

typedef struct
{
    firmware_file_info_t file_info;
    firmware_progress_t progress;
    UINT16 packet_size;
    UINT16 comm_type;
    UINT16 current_package;
    UINT16 last_package;
    UINT16 total_package;
    UINT8  ota_status;
    UINT8 pid[PRODUCTID_LEN];    
} dfu_settings_t;


#pragma pack(1)

typedef struct
{
  UINT8 type;
  UINT8 datalen;
  UINT8* data;
}ota_data_t;

typedef struct {
    UINT8  flag;
    UINT8  ota_version;
    UINT8  type;
    UINT32 version;
    UINT16 package_maxlen;
} vl_ble_ota_req_rsp_t;

typedef struct {
    UINT8  type;
    UINT8  pid[8];
    UINT32 version;
    UINT32 file_len;
    UINT32 crc32;
} vl_ble_ota_file_info_t;

typedef struct {
    UINT8  type;
    UINT8  state;
    UINT32 old_file_len;
    UINT32 old_crc32;
} vl_ble_ota_file_info_rsp_t;

typedef struct {
    UINT8  type;
    UINT32 offset;
} vl_ble_ota_file_offset_t;

typedef struct {
    UINT8  type;
    UINT32 offset;
} vl_ble_ota_file_offset_rsp_t;

typedef struct {
    UINT8  type;
    UINT16 pkg_id;
    UINT16 len;
    UINT16 crc16;
    UINT8  data[];
} vl_ble_app_ota_data_t;

typedef struct {
    UINT8 type;
    UINT8 state;
} vl_ble_ota_data_rsp_t;

typedef struct {
    UINT8 type;
    UINT8 state;
} vl_ble_ota_end_rsp_t;


typedef struct{
  UINT8  ota_resume_transfer_flag; 
  UINT8  ota_comm_type;
  UINT32 ota_file_size;
  UINT16 ota_offset_last;
  UINT32 ota_crc32_last;
  UINT32 ota_file_crc32;
  UINT16 ota_crc16;
}ota_img_t;
#pragma pack()


extern vl_status_t vl_handle_ota_req(UINT16 cmd,UINT8*recv_data,UINT32 recv_len);
extern vl_status_t vl_ota_updata_otherDev(UINT8 percent);
extern UINT32 OTA_OVERTIME_PARAM;

#endif /* !VL_BLE_OTA_H_ */
