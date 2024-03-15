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

#ifndef USE_EX_STORAGE
#define USE_EX_STORAGE  1
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
#define SEC_EX_BACKUP_OAD_HEADER_FADDR		     (0x10000)  /* Backup image area address start,You can configure it by your own device platfrom*/
#define SEC_EX_BACKUP_ALLOC_END_FADDR		     (0x68000)  /* Backup image area address end,You can configure it by your own device platfrom*/

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
	uint8_t OTA_Plan;//0x00 没有升级计划 01有升级计划
	uint32_t Target_SoftSize;//下载固件的大小
	uint32_t Target_StartAddr;//当前下载固件的起始地址
    uint32_t crc32;     //当前下载固件的crc32
	uint32_t Flash_Addr;//当前下载固件在flash中的起始地址
} OTA_Info_t;
#pragma pack()


typedef struct
{
    uint32_t firmware_file_version;
    uint32_t firmware_file_length;
    uint32_t firmware_file_crc;
    uint8_t  firmware_file_type;
} firmware_file_info_t;

typedef struct
{
    uint32_t firmware_image_offset_last;
    uint32_t firmware_image_crc_last;
} firmware_progress_t;

typedef struct
{
    firmware_file_info_t file_info;
    firmware_progress_t progress;
    uint16_t packet_size;
    uint16_t comm_type;
    uint16_t current_package;
    uint16_t last_package;
    uint16_t total_package;
    uint8_t  ota_status;
    uint8_t pid[PRODUCTID_LEN];    
} dfu_settings_t;


#pragma pack(1)

typedef struct
{
  uint8_t type;
  uint8_t datalen;
  uint8_t* data;
}ota_data_t;

typedef struct {
    uint8_t  flag;
    uint8_t  ota_version;
    uint8_t  type;
    uint32_t version;
    uint16_t package_maxlen;
} vl_ble_ota_req_rsp_t;

typedef struct {
    uint8_t  type;
    uint8_t  pid[8];
    uint32_t version;
    uint32_t file_len;
    uint32_t crc32;
} vl_ble_ota_file_info_t;

typedef struct {
    uint8_t  type;
    uint8_t  state;
    uint32_t old_file_len;
    uint32_t old_crc32;
} vl_ble_ota_file_info_rsp_t;

typedef struct {
    uint8_t  type;
    uint32_t offset;
} vl_ble_ota_file_offset_t;

typedef struct {
    uint8_t  type;
    uint32_t offset;
} vl_ble_ota_file_offset_rsp_t;

typedef struct {
    uint8_t  type;
    uint16_t pkg_id;
    uint16_t len;
    uint16_t crc16;
    uint8_t  data[];
} vl_ble_app_ota_data_t;

typedef struct {
    uint8_t type;
    uint8_t state;
} vl_ble_ota_data_rsp_t;

typedef struct {
    uint8_t type;
    uint8_t state;
} vl_ble_ota_end_rsp_t;


typedef struct{
  uint8_t  ota_resume_transfer_flag; 
  uint8_t  ota_comm_type;
  uint32_t ota_file_size;
  uint16_t ota_offset_last;
  uint32_t ota_crc32_last;
  uint32_t ota_file_crc32;
  uint16_t ota_crc16;
}ota_img_t;
#pragma pack()


extern vl_status_t vl_handle_ota_req(uint16_t cmd,uint8_t*recv_data,uint32_t recv_len);

extern uint32_t OTA_OVERTIME_PARAM;
extern vl_queue_t xQueue_OTA;

#endif /* !VL_BLE_OTA_H_ */
