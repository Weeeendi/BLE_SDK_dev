/**
 * @file vl_log.c
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK.Log module
 * @version 1.0.0
 * @date 2024-03-08
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "vl_log.h"
#include "vl_type.h"
#include "vl_log_port.h"

#ifdef __cplusplus
extern "C" {
#endif

vl_log_level_t vl_log_current_level = VL_LOG_DEFAULT_LEVEL;

void vl_printf(char *fmt, ...)
{
	static uint8_t Log_data[512];
	uint16_t Log_data_len = 0;
    if(get_test_flag()) return;

    va_list args;
    va_start(args, fmt);
    Log_data_len = vsprintf((char *)Log_data, fmt, args);
    va_end(args);
	if(Log_data_len > 512){
        DBG_PRINT("DBG buffer is full!");
		return;
    }
    
    vl_log_write((char *)Log_data,Log_data_len);
}

void vl_printf_hexdump(uint8_t *p_data,uint16_t len)
{
    for(uint16_t i = 0; i < len; i++)
    {
      DBPrintf("%02x ",p_data[i]);
      if((i+1)%16==0)
      {
          DBPrintf("\r\n");
      }
    }
}

void vl_log_change_level(vl_log_level_t level){
    if(level > VL_LOG_LEVEL_MAX) return;
    vl_log_current_level = level;
}


#ifdef __cplusplus
}
#endif