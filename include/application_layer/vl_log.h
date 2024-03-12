/**
 * @file vl_log.h
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK.It is using for SDK log module
 * @version 1.0.0
 * @date 2024-03-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#ifndef VL_LOG_H
#define VL_LOG_H

#include "vl_type.h"
#include "vl_config.h"

#if VL_LOG_ENABLE
    #define VL_PRINTF(...)		 vl_printf(__VA_ARGS__)
    #define VL_HEXDUMP(...)      vl_printf_hexdump(__VA_ARGS__)             
#else
    #define VL_PRINTF(...)
    #define VL_HEXDUMP(...)
#endif 


typedef enum
{
    VL_LOG_LEVEL_DEBUG = 0,
    VL_LOG_LEVEL_INFO,
    VL_LOG_LEVEL_WARN,
    VL_LOG_LEVEL_ERROR,
    VL_LOG_LEVEL_FATAL,
    VL_LOG_LEVEL_MAX
}vl_log_level_t;

#ifndef VL_LOG_DEFAULT_LEVEL
#define VL_LOG_DEFAULT_LEVEL              VL_LOG_LEVEL_DEBUG
#endif

#define VL_LOG_MODULE_NAME "[VL_LOG]"
#define VL_LOG_BREAK      ": "
#define VL_LOG_CRLF      "\r\n"

#define VL_ERROR_PREFIX    "[E] " VL_LOG_MODULE_NAME  VL_LOG_BREAK
#define VL_WARNING_PREFIX  "[W] " VL_LOG_MODULE_NAME  VL_LOG_BREAK
#define VL_INFO_PREFIX     "[I] " VL_LOG_MODULE_NAME  VL_LOG_BREAK
#define VL_DEBUG_PREFIX    "[D] " VL_LOG_MODULE_NAME  VL_LOG_BREAK


#define VL_LOG_INTERNAL_ERROR(TAG,_fmt_, ...)                                       \
do {                                                                            \
    if (vl_log_current_level >= VL_LOG_LEVEL_ERROR)                                     \
    {                                                                           \
        VL_PRINTF(VL_ERROR_PREFIX _fmt_ "[%s]"  VL_LOG_CRLF,TAG, ##__VA_ARGS__);   \
    }                                                                           \
} while(0)

#define VL_LOG_INTERNAL_WARNING(TAG,_fmt_, ...)                                     \
do {                                                                            \
    if (vl_log_current_level >= VL_LOG_LEVEL_WARNING)                                   \
    {                                                                           \
        VL_PRINTF(VL_WARNING_PREFIX  _fmt_ "[%s]" VL_LOG_CRLF,TAG, ##__VA_ARGS__); \
    }                                                                           \
} while(0)

#define VL_LOG_INTERNAL_INFO(TAG,_fmt_, ...)                                        \
do {                                                                            \
    if (vl_log_current_level >= VL_LOG_LEVEL_INFO)                                      \
    {                                                                           \
        VL_PRINTF(VL_INFO_PREFIX  _fmt_  "[%s]" VL_LOG_CRLF,TAG, ##__VA_ARGS__);    \
    }                                                                           \
} while(0)

#define VL_LOG_INTERNAL_DEBUG(TAG,_fmt_, ...)                                       \
do {                                                                            \
    if (vl_log_current_level >= VL_LOG_LEVEL_DEBUG)                                     \
    {                                                                           \
        VL_PRINTF(VL_DEBUG_PREFIX _fmt_ "[%s]" VL_LOG_CRLF,TAG, ##__VA_ARGS__);   \
    }                                                                           \
} while(0)

#define VL_LOG_INTERNAL_HEXDUMP_ERROR(TAG,_fmt_, p_data, len)                                    \
do {                                                                            \
    if (vl_log_current_level >= VL_LOG_LEVEL_ERROR)                                     \
    {                                                                           \
        VL_PRINTF(VL_ERROR_PREFIX _fmt_ "[%s]" " [len=%d] :" VL_LOG_CRLF,TAG,len ); \
        VL_HEXDUMP(p_data, len);                                                \
        VL_PRINTF(VL_LOG_CRLF);                                      \
    }                                                                           \
} while(0)

#define VL_LOG_INTERNAL_HEXDUMP_WARNING(TAG,_fmt_, p_data, len)                                    \
do {                                                                            \
    if (vl_log_current_level >= VL_LOG_LEVEL_WARNING)                                     \
    {                                                                           \
        VL_PRINTF(VL_WARNING_PREFIX _fmt_ "[%s]" " [len=%d] :" VL_LOG_CRLF,TAG,len ); \
        VL_HEXDUMP(p_data, len);                                                \
        VL_PRINTF(VL_LOG_CRLF);                                     \
    }                                                                           \
} while(0)

#define VL_LOG_INTERNAL_HEXDUMP_INFO(TAG,_fmt_, p_data, len)                                    \
do {                                                                            \
    if (vl_log_current_level >= VL_LOG_LEVEL_INFO)                                     \
    {                                                                           \
        VL_PRINTF(VL_INFO_PREFIX _fmt_ "[%s]" " [len=%d] :" VL_LOG_CRLF,TAG,len ); \
        VL_HEXDUMP(p_data, len);                                                \
        VL_PRINTF(VL_LOG_CRLF);                                      \
    }                                                                           \
} while(0)

#define VL_LOG_INTERNAL_HEXDUMP_DEBUG(TAG,_fmt_, p_data, len)                                    \
do {                                                                            \
    if (vl_log_current_level >= VL_LOG_LEVEL_DEBUG)                                     \
    {                                                                           \
        VL_PRINTF(VL_DEBUG_PREFIX _fmt_ "[%s]" " [len=%d] :" VL_LOG_CRLF,TAG,len ); \
        VL_HEXDUMP(p_data, len);                                                \
        VL_PRINTF(VL_LOG_CRLF);                                      \
    }                                                                           \
} while(0)


#if VL_LOG_ENABLE

#define VL_LOG_ERROR(TAG,...)                       VL_LOG_INTERNAL_ERROR(TAG,__VA_ARGS__)
#define VL_LOG_WARNING(TAG,...)                     VL_LOG_INTERNAL_WARNING(TAG, __VA_ARGS__)
#define VL_LOG_INFO(TAG,...)                        VL_LOG_INTERNAL_INFO(TAG, __VA_ARGS__)
#define VL_LOG_DEBUG(TAG,...)                       VL_LOG_INTERNAL_DEBUG(TAG, __VA_ARGS__)

#define VL_LOG_HEXDUMP_ERROR(TAG,...)               VL_LOG_INTERNAL_HEXDUMP_ERROR(TAG,__VA_ARGS__)
#define VL_LOG_HEXDUMP_WARNING(TAG,...)             VL_LOG_INTERNAL_HEXDUMP_WARNING(TAG, __VA_ARGS__)
#define VL_LOG_HEXDUMP_INFO(TAG,...)                VL_LOG_INTERNAL_HEXDUMP_INFO(TAG,__VA_ARGS__)
#define VL_LOG_HEXDUMP_DEBUG(TAG,...)               VL_LOG_INTERNAL_HEXDUMP_DEBUG(TAG,__VA_ARGS__)

#else 

#define VL_LOG_ERROR(...)
#define VL_LOG_WARNING(...)
#define VL_LOG_INFO(...)
#define VL_LOG_DEBUG(...)

#define VL_LOG_HEXDUMP_ERROR(...)        
#define VL_LOG_HEXDUMP_WARNING(...)     
#define VL_LOG_HEXDUMP_INFO(...)       
#define VL_LOG_HEXDUMP_DEBUG(...) 

#endif


/*****Functions*******/

/**
 * @brief  Change SDK log level
 * 
 * @param level Change log level @ref vl_log_level_t
 */
void vl_log_change_level(vl_log_level_t level);

/*******Variables*********/
vl_log_level_t vl_log_current_level;

#endif