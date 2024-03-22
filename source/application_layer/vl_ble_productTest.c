/**
 * @file vl_ble_productTest.c
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,product test module
 * @version 1.0.0
 * @date 2024-03-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "vl_ble_productTest.h"
#include "vl_sdk_globals.h"
#include "vl_iot_handle.h"
#include "vl_flash_port.h"
#include "vl_iot_utils.h"
#include "cjson.h"

#define EVENT_BUF_SIZE 128
#define PROD_PROTOCAL_HEAD 6

static char TAG[] = "BLE_PRODECT_TEST";
//param
static UINT8 producion_test_flag = 0;
static UINT8 produce_err = FALSE;


BOOL get_test_flag(VOID){
  return producion_test_flag;
}


UINT8 Is_Token(VOID){
    token_info_t token_info;
    read_TokenInfo(&token_info);
    return (true == token_info.token_verfiy);
}

/**
 * @brief wirte token information
 * 
 * @return  ---vl_status_t
 */
UINT8 write_TokenInfo(const token_info_t* token_info){
    return vl_nv_flash_write(TOKEN_STORAGE_ADDR_START,(UINT8*)token_info,sizeof(token_info_t));
}

/**
 * @brief read token information
 * 
 * @return  ---vl_status_t
 */
UINT8 read_TokenInfo(token_info_t* token_info){
    return vl_nv_flash_read(TOKEN_STORAGE_ADDR_START,(UINT8*)token_info,sizeof(token_info_t));
}


/**
 * @brief This file is a part of VL_BLE_SDK,it is Uart Interface for product Test
 * 
 * @param type 
 * @param pdata 
 * @param len 
 * @return uint32_t 
 */
static uint32_t vl_uart_prod_send(uint16_t type,UINT8 *pdata,UINT8 len)
{
    UINT8 uart_send_len = 7+len;
    UINT8 *uart_send_buffer = NULL;
    
    uart_send_buffer=(UINT8 *)malloc(uart_send_len);
    if(uart_send_buffer!=NULL)
    {
        uart_send_buffer[0] = 0x66;
        uart_send_buffer[1] = 0xAA;
        uart_send_buffer[2] = type>>8 & 0xff;
        uart_send_buffer[3] = type & 0xff;
        uart_send_buffer[4] = 0;
        uart_send_buffer[5] = len;        
        memcpy(uart_send_buffer+6,pdata,len);
        uart_send_buffer[6+len] = yj_check_sum(uart_send_buffer,6+len);
        vl_uart_write_buf(uart_send_buffer,7+len);
        free(uart_send_buffer);
    }
    else
    {
        VL_LOG_ERROR(TAG,"uart prod send buffer malloc failed.");    
        return 1;        
    }
    return 0;
}

/**
 * @brief Enter Test cmd callback
 * 
 * @return VOID 
 */
VOID enter_test_cb(VOID){
    UINT8 ret = 0;
    producion_test_flag = 1;
    if(vl_iot_boundf_read()){
        VL_LOG_ERROR(TAG,"AUC ENTER, BUT DEV IS BINDING");
        producion_test_flag = 0;
        return;
    }
    iot_work_status_set(NORMAL_MODE);    
    vl_uart_prod_send(VL_T_ENTER_TEST,&ret,1);
}

/**
 * @brief Exit Test cmd callback
 * 
 * @return VOID 
 */
VOID exit_test_cb(VOID){
    UINT8 ret = 0;
    if(producion_test_flag != 1){
        return;
    }
    producion_test_flag = 0;
    produce_err = true;
    vl_uart_prod_send(VL_T_EXIT_TEST,&ret,1);
}

/*
 * @brief  analays creat local password
 * @param  char* data 
 * @retval datalen
 */
int Receive_Token(const char* data,token_info_t* token_info)
{
    UINT16 data_len;
    cJSON* A;    
    cJSON* temp;  
    //u8 m,i;
    data_len = strlen(data);
    A = cJSON_Parse(data);
    if(A == NULL )
    {
        VL_LOG_ERROR(TAG,"Error Parse before: [%s]\n",cJSON_GetErrorPtr());
        return 0;
    }
    else
    {
        temp= cJSON_GetObjectItem(A,"deviceIotId");
        memcpy(token_info->deviceIotId,temp->valuestring,strlen(temp->valuestring));

        temp= cJSON_GetObjectItem(A,"deviceSecret");
        memcpy(token_info->deviceSecret,temp->valuestring,strlen(temp->valuestring));
        //VL_LOG_DEBUG(TAG,"deviceIotId: %s\n", token_info->deviceIotId);
        //VL_LOG_DEBUG(TAG,"deviceSecret: %s\n", token_info->deviceSecret);
        cJSON_Delete(A);                     //delete struct

        return data_len;
    }
}

/**
 * @brief Burn Token in flash
 * 
 * @param data 
 * @param len 
 */
VOID burn_token_cb(UINT8 data[],UINT8 len){
    UINT8 ret = 0;
    token_info_t token_info;
    memset(&token_info,'\0',sizeof(token_info_t));
    len = Receive_Token((char*)data,&token_info);
    if(0==len) {
        ret = 1;
        produce_err = TRUE;
    }
    token_info.token_verfiy = 1;
    write_TokenInfo(&token_info);
    vl_uart_prod_send(VL_T_BURN_TOKEN_INFO,&ret,1);
}

/**
 * @brief Cheak Token in flash
 * 
 * @return VOID 
 */
VOID chk_token_cb(VOID){
    CHAR ret[200];
    
    UINT16 data_len;
    token_info_t token_info;
    memset(&token_info,'\0',sizeof(token_info_t));
    
    if(producion_test_flag != 1){
        return;
    }
    memset(ret,'\0',sizeof(ret));
    
    read_TokenInfo(&token_info);
    data_len = sprintf(ret,"{\"deviceIotId\":\"%s\",\"deviceSecret\":\"%s\"}",token_info.deviceIotId,token_info.deviceSecret);
    vl_uart_prod_send(VL_T_CHEAK_TOKEN_INFO,(UINT8*)ret,data_len);
}

VOID chk_ProductInfo_cb(VOID){
    char ret[100];
    char mac_str[13];
  
    UINT16 data_len;

    if(producion_test_flag != 1){
        return;
    }
    memset(mac_str,'\0',13);
    memset(ret,'\0',sizeof(ret));
    
    //判断是否读到mac

    tal_util_hex2hexstr(vl_ble_obj.mac,(uint8_t*)mac_str,6);
    data_len = sprintf(ret,"{\"productId\":\"%s\",\"mac\":\"%s\"}",PRODUCT_KEY,mac_str);

    vl_uart_prod_send(VL_T_CHEAK_PRODUCT_INFO,(uint8_t*)ret,data_len);
}