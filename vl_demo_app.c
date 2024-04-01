/**
 * @file vl_demo_app.c
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK
 * @version 1.0.0
 * @date 2024-02-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "vl_type.h"
#include "vl_ble_event.h"
#include "vl_ble_handle.h"
#include "vl_ble_productTest.h"
#include "vl_sdk_globals.h"


#ifdef __cplusplus
extern "C" {
#endif

void sdk_param_init(void){
    vl_iot_obj.dp_query_cb = NULL;
    vl_iot_obj.dp_recv_cb = NULL;
}

// 注册回调函数
void RegisterCallback(DataPrcessPointer TagetFunc,DataPrcessPointer callback) {
    TagetFunc = callback;
}


/**
 * @brief dp query callback function,need implement by user
 * 
 * @param data if data equare NULL, means query all dp,else query by data
 *             exemple: "0x01,0x02,0x03" means query dp 1,2,3
 * @param data_len if data_len equare 0, means query all dp,else query by data
 */
vl_status_t user_dp_query_cb( uint8_t *data, uint16_t data_len){
    if(vl_iot_obj.dp_query_cb != NULL){
        //VL_LOG_DEBUG(TAG,"dp query callback\n");
        //report all dp data to app
        //for example
        if(data_len != 0){
            for(UINT8 i = 0;i<data_len;i++){
                switch (data[i])
                {
                case DPID_START:
                    /* code */
                    vl_ble_dp_report(DPID_START,DT_BOOL,0x01,0x0001);
                    break;

                case DPID_LEVEL:
                    /* code */
                    break;
                
                default:
                    break;
                }
            }
           
        }     
    }
}

UINT8 data_point_handle(const UINT8 *value){
    UINT8 dp_id, index;
    UINT8 dp_type;
    uint16_t dp_len;

    dp_id = value[0];
    dp_type = value[1];
    dp_len = value[2] * 0x100;
    dp_len += value[3];

    //You need to process the data here

    //recv dp need respond
    vl_ble_dp_report(dp_id,dp_type,dp_len,value);
    return dp_len + 4;
}


/**
 * @brief dp query callback function,need implement by user
 * 
 * @param data if data equare NULL, means query all dp,else query by data
 *             exemple: "0x01,0x02,0x03" means query dp 1,2,3
 * @param data_len if data_len equare 0, means query all dp,else query by data
 */
vl_status_t recv_dp_byApp( uint8_t *data, uint16_t data_len){
    uint8_t offset = 0;
    if(vl_iot_obj.dp_recv_cb != NULL){
        while(offset<data_len){
            //If the data contains multiple dps, process them one by one
            offset += data_point_handle(data+offset);
        }
    }
}


void main(void){
    sdk_param_init();
    RegisterCallback(vl_iot_obj.dp_query_cb,user_dp_query_cb);
    RegisterCallback(vl_iot_obj.dp_recv_cb,recv_dp_byApp);

    iot_process_handle();

    while(1){
        
    }
}

#ifdef __cplusplus
}
#endif