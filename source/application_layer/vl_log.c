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


/**
 * @brief Print log
 * 
 * @param fmt 
 * @param ... 
 * @return VOID 
 */
VOID vl_printf(CHAR *fmt, ...)
{
	static UINT8 Log_data[512];
	UINT16 Log_data_len = 0;
    if(get_test_flag()) return;

    va_list args;
    va_start(args, fmt);
    Log_data_len = vsprintf((CHAR *)Log_data, fmt, args);
    va_end(args);
	if(Log_data_len > 512){
        DBG_PRINT("DBG buffer is full!");
		return;
    }
    
    vl_log_write((CHAR *)Log_data,Log_data_len);
}

/**
 * @brief Print hex dump
 * 
 * @param p_data 
 * @param len 
 * @return VOID 
 */
VOID vl_printf_hexdump(UINT8 *p_data,UINT16 len)
{
    for(UINT16 i = 0; i < len; i++)
    {
      DBPrintf("%02x ",p_data[i]);
      if((i+1)%16==0)
      {
          DBPrintf("\r\n");
      }
    }
}

/**
 * @brief Change log level
 * 
 * @param level 
 * @return VOID 
 */
VOID vl_log_change_level(vl_log_level_t level){
    if(level > VL_LOG_LEVEL_MAX) return;
    vl_log_current_level = level;
}


#ifdef __cplusplus
}
#endif