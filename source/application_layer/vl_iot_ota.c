/**
 * @file vl_iot_ota.c
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,IoT OTA function related implementation
 * @version 1.0.0
 * @date 2024-03-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include "vl_iot_ota.h"
#include "vl_log.h"

#include "crc32.h"

#include "vl_flash_port.h"
#include "vl_ex_flash_port.h"

#define BLE_MAX_PACKET_SIZE          200   /* 200 bytes ,you can configure it size,but it must be less than MTU*/
#define SECTOR_SIZE                  4000
#define PAGE_SIZE                    256

#define DFU_TEMP_BUFFER_SIZE         SECTOR_SIZE
#define SUPPORT_BREAK_RESUME         0   /*support break resume*/

/*QUEUE*/
vl_queue_t xQueue_OTA;

/*LOG TAG*/
char TAG[] = "OTA";

/*OTA SETTINGS*/
static dfu_settings_t dfu_settings;

/*FUNTIONS DEFINE*/

/**
 * @brief wirte ota resume break-point info
 * 
 * @param ota_flag 0 or 1 ota state
 * @param file_crc32 firmware total crc32
 * @param crc32_last crc32 last
 * @param comm_type communication type
 * @param offset 
 * @return vl_status_t 
 */
vl_status_t resume_Breakpoint_write(uint8_t ota_flag,uint32_t file_crc32,uint32_t crc32_last,uint16_t offset,uint8_t comm_type){
  ota_img_t ota_img;
  ota_img.ota_resume_transfer_flag = ota_flag;
  ota_img.ota_file_crc32 = file_crc32;
  ota_img.ota_crc32_last = crc32_last;
  ota_img.ota_offset_last = offset;
  ota_img.ota_comm_type = comm_type; //if has others comm_type,can add it
  vl_nv_flash_write(SEC_IMAGE_OAD_HEADER_RESUME_FADDR,(uint8_t*)&ota_img,sizeof(ota_img_t));
  return VL_SUCCESS;
} 

/**
 * @brief read ota resume break-point info
 * 
 * @param ota_img @ref ota_img_t
 * @return vl_status_t
 */
vl_status_t resume_Breakpoint_read(ota_img_t* ota_img){
    uint8_t ret = VL_SUCCESS;
    ret = vl_nv_flash_read(SEC_IMAGE_OAD_HEADER_RESUME_FADDR,(uint8_t*)ota_img,sizeof(ota_img_t));
    return ret;
}

vl_status_t vl_handle_ota_req(uint16_t cmd,uint8_t*recv_data,uint32_t recv_len){

}

uint32_t yj_ota_init(void)
{
    iot_work_status_set(UPDATE_MODE);
    memset(&dfu_settings,'\0',sizeof(dfu_settings_t));
    dfu_settings.ota_status = VL_OTA_STATUS_NONE;
    
    OTA_OVERTIME_PARAM = xTaskGetTickCount();
    return 0;
}

uint32_t transfer_ver2int(char* ver,uint8_t len){
    uint32_t ver_int = 0;
    uint8_t offset = 16;
    if(len < 5 || ver == NULL){
        return 0;
    }
    for(uint8_t i=0;i<5;i+=2)
    {
        if(ver[i]<'0' || ver[i]>'9')
            return 0;
        ver_int |= ((ver[i] - 0x30) << offset);
        offset -= 8;
    }
    return ver_int;
}


uint32_t cal_local_storageCrc(void){
    uint16_t page_num,last_packageData = 0;
    static uint32_t crc32,length;
    uint8_t *fblock;
    uint16_t i = 0;
    crc32 = 0;
    fblock = (uint8_t *)malloc(EXFLASH_PAGE_SIZE*sizeof(uint8_t)); 
    if(fblock == NULL)
    {
        VL_LOG_ERROR(TAG,"fblock malloc fail");
        return 0;
    }
    length = dfu_settings.file_info.firmware_file_length;
    page_num = length / EXFLASH_PAGE_SIZE;
    last_packageData = length % EXFLASH_PAGE_SIZE;
    for(;i<page_num;i++){
        memset(fblock,0x00,EXFLASH_PAGE_SIZE);

#if USE_EX_STORAGE
        vl_ex_flash_write(SEC_EX_BACKUP_OAD_HEADER_FADDR+i*EXFLASH_PAGE_SIZE,fblock,EXFLASH_PAGE_SIZE);
#else
        vl_nv_flash_write(SEC_IMAGE_OAD_HEADER_APP_FADDR+i*EXFLASH_PAGE_SIZE,EXFLASH_PAGE_SIZE,fblock);
#endif
        crc32 = CRC32_HaveInitVal(crc32,fblock,EXFLASH_PAGE_SIZE);
        //crc16_ForCan = CalcCRC16_HaveInit(crc16_ForCan,fblock,EXFLASH_PAGE_SIZE);
    }
    if(last_packageData > 0){
        memset(fblock,0x00,last_packageData);
#if USE_EX_STORAGE
        vl_ex_flash_write(SEC_EX_BACKUP_OAD_HEADER_FADDR+i*EXFLASH_PAGE_SIZE,fblock,last_packageData);
#else
        W25QXX_Read(SEC_BACKUP_OAD_HEADER_FADDR+i*EXFLASH_PAGE_SIZE,last_packageData,fblock);
#endif
        crc32 = CRC32_HaveInitVal(crc32,fblock,last_packageData);
        //crc16_ForCan = CalcCRC16_HaveInit(crc16_ForCan,fblock,last_packageData);
    }
    free(fblock);
    return crc32;
}