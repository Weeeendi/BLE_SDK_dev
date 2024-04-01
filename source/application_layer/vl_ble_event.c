/**
 * @file vl_ble_event.c
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,ble event process
 * @version 1.0.0
 * @date 2024-03-12
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "vl_ble_event.h"
#include "vl_ble_handle.h"
#include "vl_iot_handle.h"
#include "vl_sdk_globals.h"
#include "vl_config.h"
#include "vl_iot_ota.h"
#include "vl_queue.h"
#include "vl_ble_queue_send.h"


#include "aes.h"
#include "vl_flash_port.h"
#include "vl_ble_port.h"
#include "vl_iot_utils.h"
#include "vl_ble_stdlib.h"

#if VL_USE_RTC
#include "vl_time.h"
#include "vl_rtc_port.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

const char TAG[] = "BLE_EVENT";

/* queue handler */

vl_queue_t xQueue_BLERecv;
// vl_queue_t xQueue_BLESend;

/*variable*/

BLE_BUFF_T BLE_Send;
BLE_BUFF_T BLE_Rev;

UINT32 conn_overtime = 0;   //connect ble but not communication
UINT32 pair_overtime = 0;   //pair ble overtime
UINT8  open_pair_enable = true; //pair ble enable
UINT8  adv_Needchange = false;  //ble adv need changed

static UINT32 vl_ble_sn = 0;
static UINT32 vl_ble_ack_sn = 0;

static aes_context productKeyAesCtx;

typedef VOID (*Event_Handler_Function)(ble_event_t *evt);

typedef struct {
    UINT16 event_type;
    Event_Handler_Function event_handler;
} Event_Handler_Entry;

// Function prototypes
vl_status_t vl_ble_devinfo_query_handler(ble_event_t *evt);
vl_status_t vl_ble_pair_handler(ble_event_t *evt);
vl_status_t vl_ble_dp_data_rev_handler(ble_event_t *evt);
vl_status_t vl_ble_dp_query_handler(ble_event_t *evt);
vl_status_t vl_ble_device_unbind_handler(ble_event_t *evt);
vl_status_t vl_ble_factory_reset_handler(ble_event_t *evt);
vl_status_t vl_Ble_ota_handler(ble_event_t *evt);
vl_status_t vl_ble_production_test_handler(ble_event_t *evt);
vl_status_t vl_ble_anomaly_unbound_handler(ble_event_t *evt);
vl_status_t vl_ble_app_sync_time_handler(ble_event_t *evt);

Event_Handler_Entry event_handlers[] = {
    {VL_BLE_EVT_DEVINFO_QUERY, vl_ble_devinfo_query_handler},
    {VL_BLE_EVT_PAIR, vl_ble_pair_handler},
    {VL_BLE_EVT_DP_DATA_REV, vl_ble_dp_data_rev_handler},
    {VL_BLE_EVT_DP_QUERY, vl_ble_dp_query_handler},
    {VL_BLE_EVT_DEVICE_UNBIND, vl_ble_device_unbind_handler},
    {VL_BLE_EVT_FACTORY_RESET, vl_ble_factory_reset_handler},
    {VL_BLE_EVT_OTA_START, vl_ota_req_handler},
    {VL_BLE_EVT_OTA_FILEINFO, vl_ota_req_handler},
    {VL_BLE_EVT_OTA_OFFSETREQ, vl_ota_req_handler},
    {VL_BLE_EVT_OTA_DATA, vl_ota_req_handler},
    {VL_BLE_EVT_OTA_OVER, vl_ota_req_handler},
    {VL_BLE_EVT_PRODUCTION_TEST, vl_ble_production_test_handler},
    {VL_BLE_EVT_ANOMALY_UNBOUND, vl_ble_anomaly_unbound_handler},
    {VL_BLE_EVT_APP_SYNC_TIME, vl_ble_app_sync_time_handler}
};


/**
 * @brief Set the Dev Status object
 * 
 * @param set_state @ref ble_status_s
 * @return VOID
 */
VOID setDevStatus(ble_status_s set_state){
   switch(set_state){
        case BLE_STATUS_START_ADV:
          vl_ble_startAdv();
          break;
        case BLE_STATUS_STOP_ADV:
          vl_ble_stopAdv();
          break;
        case BLE_STATUS_DISCONNECT:
          vl_ble_disconnect();
          break;
        case BLE_STATUS_CHG_ADV:
          vl_change_advData();
          break;
        default:
          BLE_LOG_ERROR("error state = %d",set_state);
          return VL_BLE_ERR_INVALID_STATE; 
   }
}


iot_conn_status_s get_BleBoundStatus(VOID)
{
    UINT8 status;
    UINT8 bound_status = vl_iot_boundf_read();
    static UINT8 ret;
    status = vl_ble_connect_status_get();
    if((status ^ 0x01) && bound_status){
        ret = CONNECT_BOUND;
    }
    else if((status ^ 0x01) && (bound_status == FALSE)){
        ret = CONNECT_UNBOUND;
    }      
    else if((status ^ 0x00)&&(bound_status)){
        ret = DISCONNECT_BOUND;
    }
    else if((status ^ 0x00)&&(bound_status == FALSE)){
        ret = DISCONNECT_UNBOUND;
    }
    else ret = UNKNOW_STATE;
    return ret;
}


VOID ble_state_monitor(VOID){
    static UINT8 bt_statue = UNKNOW_STATE;
    bt_statue = ble_state_get();
    switch (bt_statue)
    {
    case CONNECT_BOUND:
      VL_LOG_INFO(TAG,"BLE_STATE_MONITOR: CONNECT_BOUND\r\n");
      break;
    case CONNECT_UNBOUND:
      VL_LOG_INFO(TAG,"BLE_STATE_MONITOR: CONNECT_UNBOUND\r\n");
      break;
    case DISCONNECT_BOUND:
      VL_LOG_INFO(TAG,"BLE_STATE_MONITOR: DISCONNECT_BOUND\r\n");
      break;
    case DISCONNECT_UNBOUND:
      VL_LOG_INFO(TAG,"BLE_STATE_MONITOR: DISCONNECT_UNBOUND\r\n");
    
    default:
      VL_LOG_INFO(TAG,"BLE_STATE_MONITOR: UNKNOWN\r\n");
      break;
    }
}

/*****************************************************************************
Function name: bt_write_frame
Function description: Send a frame of data to the bt port
Input parameter: fr_type: bt protocal cmd type
                 len: data length

Return parameter: vl_status_t
*****************************************************************************/
__VL_BLE_INLINE vl_status_t bt_write_frame(unsigned short fr_type, unsigned short len)
{
    uint16_t crc16;
    uint8_t *tx_buf_ptr = NULL;
    uint8_t ret = VL_SUCCESS;
    tx_buf_ptr = &BLE_Send.curbuf[0];
    int_to_byte(vl_ble_sn++, tx_buf_ptr);
    // if report by mcu ack sn equare 0
    int_to_byte(vl_ble_ack_sn, tx_buf_ptr + ACK_SN);
    vl_ble_ack_sn = 0; // clear ack sn

    tx_buf_ptr[FUNC_CODE] = (fr_type >> 8) & 0xFF;
    tx_buf_ptr[FUNC_CODE + 1] = fr_type & 0xFF;
    tx_buf_ptr[LENGTH_HIGH] = len << 8;
    tx_buf_ptr[LENGTH_LOW] = len;
    crc16 = CRC16(tx_buf_ptr, 12 + len); // 4+4+2+2
    tx_buf_ptr[DATA_START + len] = crc16 & 0xFF;
    tx_buf_ptr[DATA_START + len + 1] = (crc16 >> 8) & 0xFF;
    len += 2;
    if (vl_iot_boundf_read())
    {
#if VL_BLE_LOG
        VL_LOG_HEXDUMP_DEBUG(TAG, "ble send origino msg:", (uint8_t *)BLE_Send.curbuf, DATA_START + len);
#endif
        /*After the device is bound, each bit is XOR with a random number, which requires decryption*/
        tal_xor_str((uint8_t *)&BLE_Send.curbuf[0], vl_iot_obj.random_num[3], DATA_START + len);
    }
    BLE_Send.len = len + DATA_START;
    ret = ble_send_enqueue(&BLE_Send);
    BLE_Send.len = 0; // clear len
    return ret;
}

/**
 * @description: Unbind device module
 * @param {*}
 * @return Result of Execution  0-success other-fail 
 */
UINT8 ble_device_unbind(VOID){
    UINT8 ret = 0;
    vl_iot_obj.bound_flag = 0;
    vl_iot_obj.random_num_valid = 0;
    memset((UINT8*)&vl_iot_obj.random_num,0,4);
    //Operation flash erases and writes new pair information
    save_state_aboutBLE();
    open_pair_enable = true;
    setDevStatus(BLE_STATUS_DISCONNECT);
    setDevStatus(BLE_STATUS_CHG_ADV);
    return ret;
}

/*******************************EVENT CALLBACK***************************************/

// Event handler function implementations
vl_status_t vl_ble_devinfo_query_handler(ble_event_t *evt) {
    VL_LOG_INFO(TAG,"receive 0x0000 cmd \r\n");
    UINT8 FrameControl = 0;
    UINT8 aes_len = 0;
    UINT8 bound_flag = vl_iot_boundf_read();
    if(evt->len!=2){
        VL_LOG_WARNING("0x0000 cmd data length error\r\n");
        return;
    }
    UINT16 index = DATA_START;
    vl_ble_obj.current_mtu = (evt->data[0]<<8 | evt->data[1]);
    setMTU(vl_ble_obj.current_mtu + 3);
    
    memcpy((UINT8*)&BLE_Send.curbuf[index],PRODUCT_KEY,PRODUCT_KEYLEN);
    index += PRODUCT_KEYLEN;
    memcpy((UINT8*)&BLE_Send.curbuf[index],vl_iot_obj.deviceID,DEVICEID_LEN);
    index += DEVICEID_LEN;

    FrameControl |= (SHARE_ENABLE<<2 & 0xFF);
    FrameControl |= (bound_flag <<3 & 0xFF);
    FrameControl |= (PROTOCAL_VER<<4 & 0xFF);
    BLE_Send.curbuf[index++] = FrameControl;
    BLE_Send.curbuf[index++] = 0x00;

    memcpy((UINT8*)&BLE_Send.curbuf[index],vl_ble_obj.mac,MAC_LEN);
    index += MAC_LEN;
    
    if(vl_iot_obj.random_num_valid == 0){
      tal_int2byte(vl_ble_generate_randNum(),vl_iot_obj.random_num);
      VL_LOG_HEXDUMP_DEBUG(TAG,"Generate Random data :\t",vl_iot_obj.random_num,4);
      vl_iot_obj.random_num_valid = 1;
    }
    
    aes_set_key(vl_iot_obj.aes128Key,16,&productKeyAesCtx);
    aes_len = compute_out_length(4);
    aes_ecb_Zero_encrypt(vl_iot_obj.random_num,(UINT8*)&BLE_Send.curbuf[index],4,&productKeyAesCtx);
    index += aes_len;
    bt_write_frame(VL_BLE_EVT_DEVINFO_QUERY,index - DATA_START);
    return VL_SUCCESS;
}

vl_status_t vl_ble_pair_handler(ble_event_t *evt) {
    UINT8 aes_unpack_buf[16] = {0};
    UINT16 index = DATA_START;
    UINT8 ret = VL_SUCCESS;
    if(evt->len!=16){
        VL_LOG_WARNING("0x0001 cmd data length error\r\n");
        return;
    }
    VL_LOG_HEXDUMP_DEBUG("0x0001 cmd data :\t",evt->data,evt->len);
    do{
      if(vl_iot_boundf_read()){
        BLE_Send.curbuf[index++] = 0x02;
        break;
      }
      aes_set_key(vl_iot_obj.aes128Key,16,&productKeyAesCtx);
      aes_ecb_decrypt(evt->data,aes_unpack_buf,evt->len,&productKeyAesCtx);
      //bit 
      for(UINT8 i=0;i<4;i++){
          aes_unpack_buf[i] ^= 0xFF;
      }
      
      VL_LOG_HEXDUMP_DEBUG("Rev Random data :\t",aes_unpack_buf,4);
      VL_LOG_HEXDUMP_DEBUG("Local Random data :\t",vl_iot_obj.random_num,4);
      
      if(0 == memcmp((UINT8*)aes_unpack_buf,(UINT8*)&vl_iot_obj.random_num[0],4)){
          BLE_Send.curbuf[index++] = 0x00;
          VL_LOG_DEBUG("BLE is bounded!\r\n");
      }else{
          BLE_Send.curbuf[index++] = 0x01;
          ret = VL_BLE_ERR_INVALID_STATE;
          break;
      }
    }while(0);
    bt_write_frame(VL_BLE_EVT_PAIR,index - DATA_START);
    if(ret == VL_BLE_ERR_INVALID_STATE){
        setDevStatus(BLE_STATUS_DISCONNECT);//if pair fail disconnnect ble
        vl_iot_obj.random_num_valid = 0;
        VL_LOG_ERROR("Random number verification failed. Disconnected Bluetooth \r\n");
        return;
    }
    vl_iot_obj.bound_flag = 1;
    write_status_aboutBle();
    setDevStatus(BLE_STATUS_CHG_ADV);
    return VL_SUCCESS;
}

vl_status_t vl_ble_dp_data_rev_handler(ble_event_t *evt) {
    vl_status_t ret = VL_SUCCESS;
#if VL_TEST
    memcpy(BLE_Send.curbuf+DATA_START,evt->data,evt->len);
    bt_write_frame(VL_BLE_EVT_DP_DATA_REV,evt->len);
    return ret;
#endif

    if(vl_iot_obj.dp_recv_cb != NULL){
        vl_iot_obj.dp_recv_cb(evt->data,evt->len);
    }else{
        ret = VL_BLE_ERR_NOT_FOUND;
    }
    return ret; // Placeholder return value
}

vl_status_t vl_ble_dp_query_handler(ble_event_t *evt) {
    vl_status_t ret = VL_SUCCESS;
    UINT16 index = DATA_START;
    if (vl_iot_obj.dp_query_cb == NULL)
    {
        ret = vl_iot_obj.dp_query_cb(evt->data,evt->len);
    }else{
        ret = VL_BLE_ERR_NOT_FOUND;
    }       
    BLE_Send.curbuf[index] = TRUE;
    bt_write_frame(VL_BLE_EVT_DP_QUERY,sizeof(ret)); 
}

vl_status_t vl_ble_device_unbind_handler(ble_event_t *evt) {
    UINT8 ret = VL_SUCCESS;
    UINT16 index = DATA_START;
    ret = ble_device_unbind();
    BLE_Send.curbuf[index] = ret;
    bt_write_frame(VL_BLE_EVT_DEVICE_UNBIND,sizeof(ret));
    return ret;
}

vl_status_t vl_ble_factory_reset_handler(ble_event_t *evt) {
    // Implementation goes here
    ble_device_unbind();
    return 0; // Placeholder return value
}

vl_status_t vl_ota_req_handler(ble_event_t *evt) {
    // Implementation goes here
    return vl_handle_ota_req(evt->cmd,evt->data,evt->len);
}

#if SUPPORT_APP_TEST
/*********************App Product Test******************************/
VOID vl_AppTestEnter(UINT8 *data,UINT16 len){
    UINT16 max_datalen;
    max_datalen = data[0]<<8 | data[1];
    vl_ble_MTUset(max_datalen);
    vl_ble_obj.test_flag = 1;
    VL_LOG_DEBUG(TAG,"Enter App test");
}

VOID vl_APPTestData(UINT8 *data,UINT16 len){
    UINT8 dp_cnt = sizeof(TEST_DP_LIST) / sizeof(TEST_DP_LIST[0]);
    if(len == 0){
        for(UINT8 i = 0;i<dp_cnt;i++){
            #error "report all test list dp."
        }
    }

    while(len--){
         #error "report app queue dp."
        dp_report_byID(data[len]);
    }
    VL_LOG_DEBUG(TAG,"App Dp test");
}

VOID vl_AppTestQuit(VOID){
    if(vl_ble_obj.test_flag){
        VL_LOG_DEBUG(TAG,"Quit App test");
        vl_ble_obj.test_flag = 0;
        setDevStatus(BLE_STATUS_DISCONNECT); 
    }
}

#endif

vl_status_t vl_ble_production_test_handler(ble_event_t *evt) {

#if SUPPORT_APP_TEST

    UINT8 p_buf[8] = {0};
    UINT8 p_bufLen = 0;
    UINT8 index = DATA_START;
    UINT16 subcmd =  evt->data[2]<<8 | evt->data[3];
    UINT16 subcmdLen = evt->data[4]<<8 | evt->data[5];
    UINT8 *subdata;
    subdata = evt->data + 6; 

    memcpy(p_buf,evt->data,4);
    p_bufLen+=4;

    p_buf[p_bufLen++] = 0x00;
    p_buf[p_bufLen++] = 0x01;
    
    do{
        if( evt->data[evt->len-1] != tal_check_sum(evt->data,evt->len-1)){
            //VL_LOG_DEBUG("checksum error");
            p_buf[p_bufLen++] = 0x01;
            break;
        }

        switch (subcmd)
        {
            case 0xFF00:
                vl_AppTestEnter(subdata,subcmdLen);
                break;
            
            case 0xFF01:
                vl_AppTestQuit();
                break;

            case 0x0000:
                vl_APPTestData(subdata,subcmdLen);
                break;
            
            default:
                break;
        }

        p_buf[p_bufLen++] = 0x00;
    }while(0);

    UINT8 chk_num = tal_check_sum(p_buf,p_bufLen);
    p_buf[p_bufLen++] = chk_num;
    memcpy((UINT8*)&BLE_Send.curbuf[index],&p_buf,p_bufLen);
    bt_write_frame(VL_BLE_EVT_PRODUCTION_TEST,p_bufLen);

    return VL_SUCCESS;
#endif
}

vl_status_t vl_ble_anomaly_unbound_handler(ble_event_t *evt) {
    UINT8 ret = VL_SUCCESS;
    UINT16 index = DATA_START;
    ret = ble_device_unbind();
    BLE_Send.curbuf[index] = ret;
    bt_write_frame(VL_BLE_EVT_ANOMALY_UNBOUND,sizeof(ret));
    return ret;
}

vl_status_t vl_ble_app_sync_time_handler(ble_event_t *evt) {
    uint8_t ret = 0;
    uint16_t index = DATA_START;
#if VL_USE_RTC
    rtc_time_t revTime;
    if(evt->data != NULL && evt->len == 9){
        revTime.ui16Year = YEAR_BASE + evt->data[0];
        revTime.ui8Month = evt->data[1];
        revTime.ui8DayOfMonth = evt->data[2];
        revTime.ui8Hour = evt->data[3];
        revTime.ui8Minute = evt->data[4];
        revTime.ui8Second = evt->data[5];
        revTime.ui8Week = evt->data[6];
        if(revTime.ui8Week == 0){ 
          revTime.ui8Week = 7;
        }
        revTime.time_zone = (int16_t)((evt->data[7] & 0xFF)<<8 | evt->data[8]);
        revTime.time_zone /= 100;
        set_rtc_time(revTime);
    }else{    
        ret = 1;
    }
#endif
    BLE_Send.curbuf[index] = ret;
    bt_write_frame(VL_BLE_EVT_APP_SYNC_TIME,sizeof(ret));
}

VOID vl_ble_event_handler(ble_event_t* event)
{
    if(event == NULL) return;

    for (size_t i = 0; i < sizeof(event_handlers) / sizeof(event_handlers[0]); i++) {
        if (event->cmd == event_handlers[i].event_type) {
            event_handlers[i].event_handler(event);           
            return;
        }
    }
    VL_LOG_ERROR(TAG,"unknown event type:%d", event->cmd);
}

/**
 * @brief Report data point to the App
 * 
 * @param dp_id 
 * @param dp_type 
 * @param dp_data 
 * @param dp_len 
 * @return vl_status_t 
 */
vl_status_t vl_ble_dp_report(UINT8 dp_id,UINT8 dp_type,UINT8 *dp_data,UINT16 dp_len){ 
    UINT8 ret = VL_SUCCESS;
    UINT8 index = DATA_START;
    UINT8 *buf_p;
    if(get_BleBoundStatus() != CONNECT_BOUND){
        ret = VL_BLE_ERR_INVALID_STATE;
    }

    if(dp_len > MAX_DP_DATA_LEN - 12 || dp_len == 0){
        ret = VL_BLE_ERR_INVALID_PARAM;
    }
    buf_p = BLE_Send.curbuf + index;
    buf_p[0] = dp_id;
    buf_p[1] = dp_type;
    buf_p[2] = dp_len>>8;
    buf_p[3] = dp_len && 0xff;
    memcpy(buf_p+4,dp_data,dp_len);

    bt_write_frame(VL_BLE_EVT_DP_DATA_REPORTED,dp_len + 4);
    return ret;
}

VOID vl_ble_receive_handler(VOID)
{
    BLE_BUFF_T rev = {0};
    ble_event_t *evt_param;
    while(VL_SUCCESS == vl_queue_get(&xQueue_BLERecv,&rev)){
        if(rev.len < DATA_START + 2) return;

        evt_param = (ble_event_t*)malloc(sizeof(rev.len));
        if(NULL == evt_param){
            VL_LOG_ERROR(TAG,"vl_ble_event_handler: malloc failed\r\n");
            return;
        }

        if(vl_iot_boundf_read()){
          /*After the device is bound, each bit is XOR with a random number, which requires decryption*/
          tal_xor_str(rev.curbuf,vl_iot_obj.random_num[3],rev.len);
          
          #if VL_BLE_LOG
            VL_LOG_HEXDUMP_DEBUG(TAG,"rand num:",vl_iot_obj.random_num,4); 
            VL_LOG_HEXDUMP_DEBUG(TAG,"Receive App data:",rev.curbuf,rev.len);
          #endif
        }
        
        evt_param->sn = tal_util_byte2int(rev.curbuf[SN]);
        evt_param->ack_sn = tal_util_byte2int(rev.curbuf[ACK_SN]);
        evt_param->cmd = (rev.curbuf[FUNC_CODE]<<8) | (rev.curbuf[FUNC_CODE+1] & 0xFF);
        evt_param->len = (rev.curbuf[LENGTH_HIGH]<<8) | (rev.curbuf[LENGTH_LOW] & 0xFF);

        if(evt_param->len > 255) return; 
        memcpy(evt_param->data,rev.curbuf+DATA_START,evt_param->len);
        vl_ble_event_handler(evt_param);

        vl_dequeue(&xQueue_BLERecv,&rev); //destroy the data of queue top 
        free(evt_param);
    }
}

vl_status_t vl_eventTask(VOID){
    //Init queue
    xQueue_BLERecv.unit_size = sizeof(BLE_BUFF_T);
    xQueue_BLERecv.size = BLE_RECV_QUEUE_SIZE;
    vl_queue_init(&xQueue_BLERecv, xQueue_BLERecv.buf, xQueue_BLERecv.unit_size, xQueue_BLERecv.size);

    ble_send_queue_init();

    //event process loop
    for(;;){    
        vl_ble_receive_handler();
        ble_send_queue_handle();
    }
}

#ifdef __cplusplus
}
#endif