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
#include "vl_ble_event.h"
#include "vl_sdk_globals.h"

#include "crc32.h"

#include "vl_flash_port.h"
#include "vl_ex_flash_port.h"

#define BLE_MAX_PACKET_SIZE          200   /* 200 bytes ,you can configure it size,but it must be less than MTU*/
#define SECTOR_SIZE                  4000
#define PAGE_SIZE                    256

#define DFU_TEMP_BUFFER_SIZE         SECTOR_SIZE
#define	APP_NEW_FW_MAX_SIZE          (65635*8)    //can protcal max offerset page limit

#ifndef SUPPORT_BREAK_RESUME
#define SUPPORT_BREAK_RESUME         0   /*support break resume*/
#endif

#if USE_EX_STORAGE
#define WRITE_FLASH_PORT              vl_ex_flash_write
#define READ_FLASH_PORT               vl_ex_flash_read
#else
#define WRITE_FLASH_PORT              vl_nv_flash_write
#define READ_FLASH_PORT               vl_nv_flash_read
#endif



/*LOG TAG*/
char TAG[] = "OTA";

/*OTA SETTINGS*/
static dfu_settings_t dfu_settings;

static UINT8 TempBufferHead[DFU_TEMP_BUFFER_SIZE];
/*FUNTIONS DEFINE*/
#if SUPPORT_BREAK_RESUME
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
vl_status_t resume_Breakpoint_write(UINT8 ota_flag,UINT32 file_crc32,UINT32 crc32_last,UINT16 offset,UINT8 comm_type){
  ota_img_t ota_img;
  ota_img.ota_resume_transfer_flag = ota_flag;
  ota_img.ota_file_crc32 = file_crc32;
  ota_img.ota_crc32_last = crc32_last;
  ota_img.ota_offset_last = offset;
  ota_img.ota_comm_type = comm_type; //if has others comm_type,can add it
  vl_nv_flash_write(SEC_IMAGE_OAD_HEADER_RESUME_FADDR,(UINT8*)&ota_img,sizeof(ota_img_t));
  return VL_SUCCESS;
} 

/**
 * @brief read ota resume break-point info
 * 
 * @param ota_img @ref ota_img_t
 * @return vl_status_t
 */
vl_status_t resume_Breakpoint_read(ota_img_t* ota_img){
    UINT8 ret = VL_SUCCESS;
    ret = vl_nv_flash_read(SEC_IMAGE_OAD_HEADER_RESUME_FADDR,(UINT8*)ota_img,sizeof(ota_img_t));
    return ret;
}

#endif
/**
 * @brief  This function can obtain local version of requested ota device
 * @param  UINT8  type - which device need to ota  @ref vl_ota_type_t
 *
*/
UINT32 get_versionNumber(UINT8 type){
    UINT32 version;
    switch (type)
    {
    case IoT_BLE:
        version = IOT_MCU_SOFTVER;
        break;
    case IoT_MCU:
        break;
    case IoT_4G:
        break;
    case Ebike_Dashboard:
        break;
    case Ebike_Mcu:
        break;
    case Ebike_BMS:
        break;
    case Ebike_Ex1:
        break;
    case Ebike_Ex2:
        break;
    default:
        VL_LOG_ERROR(TAG,"OTA type error %d \r\n",type);
        break;
    }
    return version;
}


vl_status_t vl_ota_init(VOID)
{
    iot_work_status_set(UPDATE_MODE);
    memset(&dfu_settings,'\0',sizeof(dfu_settings_t));
    OTA_OVERTIME_PARAM = xTaskGetTickCount();
    return VL_SUCCESS;
}

UINT32 transfer_ver2int(char* ver,UINT8 len){
    UINT32 ver_int = 0;
    UINT8 offset = 16;
    if(len < 5 || ver == NULL){
        return 0;
    }
    for(UINT8 i=0;i<5;i+=2)
    {
        if(ver[i]<'0' || ver[i]>'9')
            return 0;
        ver_int |= ((ver[i] - 0x30) << offset);
        offset -= 8;
    }
    return ver_int;
}


static UINT8 vl_ota_start_req(UINT8*recv_data,UINT32 recv_len)
{
    UINT8 p_buf[20];
    UINT8 payload_len = 0;
    UINT32 current_version = 0;
    UINT8 index = DATA_START;
    UINT8 ret = 0;
  
    dfu_settings.file_info.firmware_file_type = recv_data[0];

    current_version = get_versionNumber(dfu_settings.file_info.firmware_file_type);
    
    if(dfu_settings.ota_status!=VL_OTA_STATUS_NONE)
    {
        VL_LOG_ERROR(TAG,"current ota status is not TUYA_OTA_STATUS_NONE  and is : %d !",dfu_settings.ota_status);
        ret = 1;
    }
 
    dfu_settings.packet_size = BLE_MAX_PACKET_SIZE;

    p_buf[payload_len++] = ret;   
    p_buf[payload_len++] = OTA_VER;
    p_buf[payload_len++] = dfu_settings.file_info.firmware_file_type;
    p_buf[payload_len++] = current_version>>24;
    p_buf[payload_len++] = current_version>>16;
    p_buf[payload_len++] = current_version>>8;
    p_buf[payload_len++] = current_version;
    p_buf[payload_len++] = dfu_settings.packet_size>>8;
    p_buf[payload_len++] = (dfu_settings.packet_size & 0xFF);
    dfu_settings.ota_status = VL_OTA_STATUS_START;
    //payload_len = 9;

	memcpy((UINT8*)&BLE_Send.curbuf[index],&p_buf,payload_len);
	bt_write_frame(VL_BLE_EVT_OTA_START,payload_len);

    return ret;
}


UINT32 cal_local_storageCrc(VOID){
    UINT16 page_num,last_packageData = 0;
    static UINT32 crc32,length;
    UINT8 *fblock;
    UINT16 i = 0;
    crc32 = 0;
    fblock = (UINT8 *)malloc(EXFLASH_PAGE_SIZE*sizeof(UINT8)); 
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
        WRITE_FLASH_PORT(SEC_BACKUP_OAD_HEADER_FADDR+i*EXFLASH_PAGE_SIZE,fblock,EXFLASH_PAGE_SIZE);
        crc32 = CRC32_HaveInitVal(crc32,fblock,EXFLASH_PAGE_SIZE);
    }
    if(last_packageData > 0){
        memset(fblock,0x00,last_packageData);
        WRITE_FLASH_PORT(SEC_BACKUP_OAD_HEADER_FADDR+i*EXFLASH_PAGE_SIZE,fblock,last_packageData);
        crc32 = CRC32_HaveInitVal(crc32,fblock,last_packageData);
    }
    free(fblock);
    return crc32;
}

static UINT8 vl_ota_start_req(UINT8*recv_data,UINT32 recv_len)
{
    UINT8 p_buf[20];
    UINT8 payload_len = 0;
    UINT32 current_version = 0;
    UINT8 index = DATA_START;
    UINT8 ret = 0;
  
    dfu_settings.file_info.firmware_file_type = recv_data[0];

    current_version = get_versionNumber(dfu_settings.file_info.firmware_file_type);
    
    if(dfu_settings.ota_status!=VL_OTA_STATUS_NONE)
    {
        VL_LOG_ERROR(TAG,"current ota status is not TUYA_OTA_STATUS_NONE  and is : %d !",dfu_settings.ota_status);
        ret = 1;
    }

    dfu_settings.packet_size = BLE_MAX_PACKET_SIZE;

    p_buf[payload_len++] = ret;   
    p_buf[payload_len++] = OTA_VER;
    p_buf[payload_len++] = dfu_settings.file_info.firmware_file_type;
    p_buf[payload_len++] = current_version>>24;
    p_buf[payload_len++] = current_version>>16;
    p_buf[payload_len++] = current_version>>8;
    p_buf[payload_len++] = current_version;
    p_buf[payload_len++] = dfu_settings.packet_size>>8;
    p_buf[payload_len++] = (dfu_settings.packet_size & 0xFF);
    dfu_settings.ota_status = VL_OTA_STATUS_START;
    //payload_len = 9;

	memcpy((UINT8*)&BLE_Send.curbuf[index],&p_buf,payload_len);
	bt_write_frame(VL_BLE_EVT_OTA_START,payload_len);

    return ret;
}

static UINT8  vl_ota_file_info_req(UINT8*recv_data,UINT32 recv_len){
    UINT8 ret = 0;
    UINT8 p_buf[20];
    UINT8 payload_len = 0;
    UINT8 rev_index = 0;
    UINT8 write_index = DATA_START;
  
    OTA_OVERTIME_PARAM = xTaskGetTickCount();
    
    VL_LOG_HEXDUMP_DEBUG(TAG,"OTA INFO:",recv_data,recv_len);

    dfu_settings.file_info.firmware_file_type = recv_data[rev_index++];
    memcpy(dfu_settings.pid,(PVOID)&recv_data[1],PRODUCT_KEYLEN);
    rev_index += 10;
    do{
        if(!memcmp(dfu_settings.pid,vl_iot_obj.productID,10)){
            VL_LOG_ERROR(TAG,"receive pid is: %s ,not match this product ! \r\n", dfu_settings.pid);
            ret = 1;
            break;  
        }
        dfu_settings.file_info.firmware_file_version = byte_to_int(&recv_data[rev_index]);
        rev_index += 4;
        if(dfu_settings.file_info.firmware_file_version <= get_versionNumber(dfu_settings.file_info.firmware_file_type)){
            VL_LOG_ERROR(TAG,"receive version error,not support: %08x",dfu_settings.file_info.firmware_file_version);
            ret = 2;
            break;  
        }

        dfu_settings.file_info.firmware_file_length = byte_to_int(&recv_data[rev_index]);
        if(dfu_settings.file_info.firmware_file_type == IoT_MCU && dfu_settings.file_info.firmware_file_length > MAX_MCU_FIRMWARE_SIZE){
            VL_LOG_ERROR(TAG,"receive firmware size is over MCU storage : %d",dfu_settings.file_info.firmware_file_version);
            ret = 3;
            break;  
        }
        if(dfu_settings.file_info.firmware_file_type > IoT_4G && dfu_settings.file_info.firmware_file_length > APP_NEW_FW_MAX_SIZE){
            VL_LOG_ERROR(TAG,"receive firmware size is over CAN transport top limit: %d",dfu_settings.file_info.firmware_file_version);
            ret = 3;
            break;
        }

        dfu_settings.total_package = dfu_settings.file_info.firmware_file_length / dfu_settings.packet_size;
        if(dfu_settings.file_info.firmware_file_length % dfu_settings.packet_size){
            dfu_settings.total_package++;
        }
        
        rev_index += 4;
        dfu_settings.file_info.firmware_file_crc = byte_to_int(&recv_data[rev_index]);
        
        VL_LOG_DEBUG(TAG,"File Info:\n File size: %d byte \t File crc16: %04x \t File version: %08x \n",dfu_settings.file_info.firmware_file_length,dfu_settings.file_info.firmware_file_crc,dfu_settings.file_info.firmware_file_version);
        VL_LOG_DEBUG(TAG,"The total number of upgrade packages is % d \r\n",dfu_settings.total_package);
#if SUPPORT_BREAK_RESUME
        resume_Breakpoint_read(&ota_img);
        if(ota_img.ota_resume_transfer_flag && 
        (dfu_settings.file_info.firmware_file_crc == ota_img.ota_file_crc32) && 
        (ota_img.ota_offset_last % BLE_MAX_PACKET_SIZE == 0) && 
        (ota_img.ota_comm_type == dfu_settings.comm_type))
        {
            dfu_settings.progress.firmware_image_offset_last = ota_img.ota_offset_last;
            dfu_settings.progress.firmware_image_crc_last = ota_img.ota_crc32_last;
            last_package = dfu_settings.progress.firmware_image_offset_last / dfu_settings.packet_size;
        }else{
        resume_Breakpoint_write(0,0,0,0,0);
        dfu_settings.progress.firmware_image_offset_last = 0;
        dfu_settings.progress.firmware_image_crc_last = 0;
        }
#endif
    }while(0);

    if(dfu_settings.ota_status!=VL_OTA_STATUS_START)
    {
        VL_LOG_ERROR(TAG,"current ota status is not VL_OTA_STATUS_START  and is : %d !",dfu_settings.ota_status);
       // return;
    }
      
    p_buf[0] = dfu_settings.file_info.firmware_file_type;   
    p_buf[1] = ret;

    p_buf[2] = dfu_settings.progress.firmware_image_offset_last>>24;
    p_buf[3] = dfu_settings.progress.firmware_image_offset_last>>16;
    p_buf[4] = dfu_settings.progress.firmware_image_offset_last>>8;
    p_buf[5] = dfu_settings.progress.firmware_image_offset_last & 0xFF;

    p_buf[6] = dfu_settings.progress.firmware_image_crc_last>>24;
    p_buf[7] = dfu_settings.progress.firmware_image_crc_last>>16;
    p_buf[8] = dfu_settings.progress.firmware_image_crc_last>>8;
    p_buf[9] = dfu_settings.progress.firmware_image_crc_last & 0xFF;
    dfu_settings.ota_status = VL_OTA_STATUS_FILE_INFO;
    payload_len = 10;
    
    //Send OTA Start
    memcpy((UINT8*)&BLE_Send.curbuf[write_index],&p_buf,payload_len);
    bt_write_frame(VL_BLE_EVT_OTA_FILEINFO,payload_len);

    return  ret;
}

/**
 * @brief ota data transfer request
 * 
 * @param recv_data 
 * @param recv_len 
 * @return UINT8 
 */
static UINT8 vl_ota_data_req(UINT8*recv_data,UINT32 recv_len)
{
    static UINT16 last_percentage = 0;
    UINT16 percentage;
    UINT8 ret = 0;
    UINT8 p_buf[20];
    UINT8 payload_len = 0;
    UINT8 write_index = DATA_START;
    UINT16 current_pkgLen = 0;
    
    UINT16 last_buf_len;
    UINT16 crc_rev,crc_cal;
    UINT16 offset;
    UINT16 multiple = DFU_TEMP_BUFFER_SIZE / dfu_settings.packet_size;
    if(recv_data == NULL) return 1;
    
    if(dfu_settings.ota_status != VL_OTA_STATUS_FILE_OFFSET &&\
        dfu_settings.ota_status != VL_OTA_STATUS_FILE_DATA && \
        dfu_settings.ota_status != VL_OTA_STATUS_FILE_INFO)
    {
        VL_LOG_ERROR(TAG,"current ota status is not VL_OTA_STATUS_FILE_OFFSET  and is : %d !",dfu_settings.ota_status);
        return;
    }
    do{
        if(dfu_settings.file_info.firmware_file_type != recv_data[0]){
            ret = 0x04; //file type error
            break;
        }
        
        dfu_settings.current_package = recv_data[1]<<8 | recv_data[2];
        //VL_LOG_INFO("receive pkt : %d\r\n",current_package);
        if(dfu_settings.current_package != dfu_settings.last_package + 1){
            ret = 0x01; //package number error
            break;
        }
        dfu_settings.last_package = dfu_settings.current_package;
        
        percentage = (dfu_settings.current_package*100)/dfu_settings.total_package;
        if(last_percentage != percentage){
            VL_LOG_DEBUG(TAG,"ota progress %d %%",percentage);
            last_percentage = percentage;
        } 
        
        current_pkgLen = recv_data[3]<<8 | recv_data[4];

        if((current_pkgLen > dfu_settings.packet_size) || current_pkgLen <= 0){
            ret = 0x02; //package lenth error
            break;
        }

        crc_rev = recv_data[6]<<8 | recv_data[5];
        crc_cal = CRC16(recv_data + 7,current_pkgLen);
        if(crc_rev !=crc_cal){
            ret = 0x03;
            VL_LOG_DEBUG(TAG,"receive crc error : %04x\r\n",crc_rev);
            break;
        }
        
        /****************  Store data ****************************/
        offset = (dfu_settings.current_package - 1) % multiple;
        memcpy((UINT8*)&TempBufferHead[offset*dfu_settings.packet_size],(UINT8*)&recv_data[7],current_pkgLen);
        
        if(dfu_settings.total_package == dfu_settings.current_package)
        {
           last_buf_len = offset*dfu_settings.packet_size + current_pkgLen;
           WRITE_FLASH_PORT(SEC_BACKUP_OAD_HEADER_FADDR + dfu_settings.progress.firmware_image_offset_last ,TempBufferHead,last_buf_len);
          // W25QXX_Write(TempBufferHead,SEC_BACKUP_OAD_HEADER_FADDR + dfu_settings.progress.firmware_image_offset_last,last_buf_len);
           dfu_settings.progress.firmware_image_offset_last += last_buf_len;
           dfu_settings.progress.firmware_image_crc_last = CRC32_HaveInitVal(dfu_settings.progress.firmware_image_crc_last,TempBufferHead,last_buf_len);
           
           break;
        }
        
        if(dfu_settings.current_package % multiple == 0){
            WRITE_FLASH_PORT(SEC_BACKUP_OAD_HEADER_FADDR + dfu_settings.progress.firmware_image_offset_last ,TempBufferHead,DFU_TEMP_BUFFER_SIZE);
           // W25QXX_Write(TempBufferHead,SEC_BACKUP_OAD_HEADER_FADDR + dfu_settings.progress.firmware_image_offset_last,DFU_TEMP_BUFFER_SIZE);
            dfu_settings.progress.firmware_image_offset_last += DFU_TEMP_BUFFER_SIZE;
            dfu_settings.progress.firmware_image_crc_last = CRC32_HaveInitVal(dfu_settings.progress.firmware_image_crc_last,TempBufferHead,DFU_TEMP_BUFFER_SIZE);
            #if SUPPORT_BREAK_RESUME
            resume_Breakpoint_write(1,dfu_settings.file_info.firmware_file_crc,dfu_settings.progress.firmware_image_crc_last,crc16_ForCan,dfu_settings.progress.firmware_image_offset_last);
            #endif
        }    
    }while (0);
    
    dfu_settings.ota_status = VL_OTA_STATUS_FILE_DATA; 

    p_buf[0] = dfu_settings.file_info.firmware_file_type;
    p_buf[1] = ret;
    payload_len = 2;

    memcpy((UINT8*)&BLE_Send.curbuf[write_index],&p_buf,payload_len);
    bt_write_frame(VL_BLE_EVT_OTA_DATA,payload_len);

    return ret;
}


/**
 * @brief ota over request
 * 
 * @param recv_data 
 * @param recv_len 
 * @return vl_status_t 
 */
vl_status_t vl_ota_over_req(UINT8* recv_data,UINT32 recv_len)
{
    UINT8 ret = VL_SUCCESS;
    uint8_t p_buf[20];
    uint8_t write_index = DATA_START;
    uint8_t payload_len = 0;
    static uint32_t crc32_local;

    if(recv_data == NULL){
        return 1;
    }

    OTA_OVERTIME_PARAM = xTaskGetTickCount();

    do{ 
        if(dfu_settings.ota_status != VL_OTA_STATUS_FILE_DATA)
        {
            VL_LOG_ERROR(TAG,"current ota status is not VL_OTA_STATUS_FILE_DATA  and is : %d !",dfu_settings.ota_status);
            break;
        }
        
        
        if(dfu_settings.file_info.firmware_file_type != recv_data[0]){
            VL_LOG_ERROR(TAG,"ota type check error \r\n");
            ret = 0x03; //file type error
            break;
        }

        if(dfu_settings.file_info.firmware_file_length != dfu_settings.progress.firmware_image_offset_last){
            VL_LOG_ERROR(TAG,"ota total length check error \r\n");
            ret = 0x01;
            break;
        }
        
        if(dfu_settings.file_info.firmware_file_crc != dfu_settings.progress.firmware_image_crc_last)   
        {
            VL_LOG_ERROR(TAG,"ota total crc check error \r\n");  
            ret = 0x02;
            break;
        }else{
            VL_LOG_INFO(TAG,"waiting checkout local flash storage file ...\r\n");
            crc32_local = cal_local_storageCrc(); 
            if(dfu_settings.file_info.firmware_file_crc != crc32_local){
                VL_LOG_ERROR(TAG,"checkout local flash storage error \r\n");  
                ret = 0x02;
                break;
            }

            // iot_work_status_set(NORMAL_MODE);
            if(dfu_settings.file_info.firmware_file_type > IoT_4G){
                //start ota transfer
#if SUPPORT_PARTTHIRD_OTA
                #error "support part third ota,and use vl_ota_updata_otherDev to updata process"
                //Do part third device ota
#endif
            }
            else{
                VL_LOG_DEBUG(TAG,"ota success \r\n");
            }
        }
    }while (0);

    dfu_settings.ota_status = VL_OTA_STATUS_FILE_END;
    
    iot_work_status_set(NORMAL_MODE);
    
    p_buf[0] = dfu_settings.file_info.firmware_file_type;
    p_buf[1] = ret;
    payload_len = 2;

    memcpy((uint8_t*)&BLE_Send.curbuf[write_index],&p_buf,payload_len);
    bt_write_frame(VL_BLE_EVT_OTA_OVER,payload_len);

    //ota_storage_info_write(0,0);
#if SUPPORT_BREAK_RESUME
    resume_Breakpoint_write(0,0,0,0,0);
#endif

    if(ret == 0 && dfu_settings.file_info.firmware_file_type == IoT_MCU)
    {
        OTA_Info_t OTA_Info;
        OTA_Info.OTA_Plan = TRUE;
        OTA_Info.Target_StartAddr = SEC_IMAGE_OAD_HEADER_APP_FADDR;
        OTA_Info.Target_SoftSize = dfu_settings.file_info.firmware_file_length;
        OTA_Info.crc32 = dfu_settings.file_info.firmware_file_crc;
        OTA_Info.Flash_Addr = SEC_BACKUP_OAD_HEADER_FADDR;
        WRITE_FLASH_PORT(SEC_IMAGE_OAD_HEADER_APP_FADDR,(uint8_t*)&OTA_Info,sizeof(OTA_Info_t));
        ble_device_reset();
    }
    else if(ret == 0 && dfu_settings.file_info.firmware_file_type > IoT_4G){
        ble_device_reset();  
    }
    
    return ret;
}


/**
 * @brief Updata the PartThird device OTA progress,when you need OTA the PartThird device
 * 
 * @param percent --OTA progress percent :0~100
 * @return vl_status_t 
 */
vl_status_t vl_ota_updata_otherDev(uint8_t percent){
    UINT8 write_index = DATA_START;
    UINT8 payload_len = 2;
    UINT8 ret = VL_SUCCESS;
    BLE_Send.curbuf[write_index] = dfu_settings.file_info.firmware_file_type;
    BLE_Send.curbuf[write_index+1] = percent;
    bt_write_frame(VL_BLE_EVT_OTA_DATA,payload_len);

    return ret;
}


/**
 * @brief Handle the OTA request
 * 
 * @param cmd --OTA event cmd
 * @param recv_data --OTA event data
 * @param recv_len --OTA event data length
 * @return vl_status_t 
 */
vl_status_t vl_handle_ota_req(UINT16 cmd,UINT8*recv_data,UINT32 recv_len){
    if(recv_data == NULL || recv_len == 0){
        return VL_BLE_ERR_INVALID_PARAM;
    }

    switch (cmd)
    {
    case VL_EVT_OTA_START:
        vl_ota_start_req(recv_data,recv_len);
        break;
    case VL_EVT_OTA_FILEINFO:
        vl_ota_file_info_req(recv_data,recv_len);
        break;
    case VL_EVT_OTA_OFFSETREQ:
        vl_ota_offset_req(recv_data,recv_len);
        break;
    case VL_EVT_OTA_DATA:
        vl_ota_data_req(recv_data,recv_len);
        break;
    case VL_EVT_OTA_OVER:
        vl_ota_over_req(recv_data,recv_len);
        break;
    default:
        VL_LOG_ERROR(TAG,"vl_handle_ota_req: unknown event %d", cmd);
        break;
    }
}
