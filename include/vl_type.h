/**
 * @file vl_ble_type.h
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK
 * @version 1.0.0
 * @date 2024-02-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef VL_BLE_TYPE_H
#define VL_BLE_TYPE_H

#include "vl_ble_stdlib.h"
#include "vl_config.h"


#if defined(__CC_ARM)
#pragma anon_unions
#elif defined(__ICCARM__)
#pragma language = extended
#elif defined(__GNUC__)
/* anonymous unions are enabled by default */
#endif


#if defined ( __CC_ARM )

    #ifndef __VL_BLE_ASM
        #define __VL_BLE_ASM               __asm
    #endif

    #ifndef __VL_BLE_INLINE
        #define __VL_BLE_INLINE            __inline
    #endif

    #ifndef __VL_BLE_WEAK
        #define __VL_BLE_WEAK              __weak
    #endif

    #ifndef __VL_BLE_ALIGN
        #define __VL_BLE_ALIGN(n)          __align(n)
    #endif

    #ifndef __VL_BLE_PACKED
        #define __VL_BLE_PACKED            __packed
    #endif

    #define VL_BLE_GET_SP()                __current_sp()

#elif defined ( __ICCARM__ )

    #ifndef __VL_BLE_ASM
        #define __VL_BLE_ASM               __asm
    #endif

    #ifndef __VL_BLE_INLINE
        #define __VL_BLE_INLINE            inline
    #endif

    #ifndef __VL_BLE_WEAK
        #define __VL_BLE_WEAK              __weak
    #endif

    #ifndef __VL_BLE_ALIGN
        #define VL_BLE_STRING_PRAGMA(x) _Pragma(#x)
        #define __VL_BLE_ALIGN(n) STRING_PRAGMA(data_alignment = n)
    #endif

    #ifndef __VL_BLE_PACKED
        #define __VL_BLE_PACKED            __packed
    #endif
    
    #define VL_BLE_GET_SP()                __get_SP()

#elif defined   ( __GNUC__ )

    #ifndef __VL_BLE_ASM
        #define __VL_BLE_ASM               __asm
    #endif

    #ifndef __VL_BLE_INLINE
        #define __VL_BLE_INLINE            inline
    #endif

    #ifndef __VL_BLE_WEAK
        #define __VL_BLE_WEAK              __attribute__((weak))
    #endif

    #ifndef __VL_BLE_ALIGN
        #define __VL_BLE_ALIGN(n)          __attribute__((aligned(n)))
    #endif

    #ifndef __VL_BLE_PACKED
        #define __VL_BLE_PACKED           __attribute__((packed)) 
    #endif

#endif

// Integer types
typedef signed char         INT8;     // 8-bit signed integer
typedef unsigned char       UINT8;    // 8-bit unsigned integer
typedef signed short        INT16;    // 16-bit signed integer
typedef unsigned short      UINT16;   // 16-bit unsigned integer
typedef signed int          INT32;    // 32-bit signed integer
typedef unsigned int        UINT32;   // 32-bit unsigned integer
typedef signed long long    INT64;    // 64-bit signed integer
typedef unsigned long long  UINT64;   // 64-bit unsigned integer

// Floating point types
typedef float               FLOAT32;  // 32-bit floating point
typedef double              FLOAT64;  // 64-bit floating point

// Boolean type
typedef unsigned char       BOOL;     // Boolean type
#define TRUE                1         // Boolean true value
#define FALSE               0         // Boolean false value

// Character type
typedef char                CHAR;     // Character type

// Byte type
typedef unsigned char       BYTE;     // Byte type

// Void type
typedef void                VOID;     // Void type

// Pointer to void type
typedef void*               PVOID;    // Pointer to void type

// Function pointer type
typedef void (*FunctionPointer)(void);  // Function pointer type

// Other types can be added as needed...

#define PRODUCTID_LEN 10
#define DEVICEID_LEN 16
#define RANDOMNUM_LEN 4

typedef enum
{
    NO_CONNECT_NO_BOUND,
    CONNECT_NO_BOUND,
    CONNECT_BOUND,
    NO_CONNECT_BOUND,
    UNKNOW_STATE,
} iot_conn_status_s;

typedef enum
{
    BLE_STATUS_INIT,
    BLE_STATUS_START_ADV,
    BLE_STATUS_STOP_ADV,
    BLE_STATUS_CONNECT,
    BLE_STATUS_DISCONNECT,
    BLE_STATUS_UNKNOW,
} ble_status_s;


typedef enum {
    VL_SUCCESS  = 0x00,

    // BLE errcode
    VL_BLE_ERR_INTERNAL,
    VL_BLE_ERR_NOT_FOUND,
    VL_BLE_ERR_NO_EVENT,
    VL_BLE_ERR_NO_MEM,
    VL_BLE_ERR_INVALID_ADDR,     // Invalid pointer supplied
    VL_BLE_ERR_INVALID_PARAM,    // Invalid parameter(s) supplied.
    VL_BLE_ERR_INVALID_STATE,    // Invalid state to perform operation.
    VL_BLE_ERR_INVALID_LENGTH,
    VL_BLE_ERR_DATA_SIZE,
    VL_BLE_ERR_TIMEOUT,
    VL_BLE_ERR_BUSY,
    VL_BLE_ERR_COMMON,
    VL_BLE_ERR_RESOURCES,
    VL_BLE_ERR_UNKNOWN,          // other ble sdk errors

    // UART errcode
    VL_UART_ERR_BUSY,        
    VL_UART_ERR_TIMEOUT,     
    VL_UART_ERR_OVERFLOW,   

    // Flash errcode
    VL_FLASH_ERR_ERASE,   
    VL_FLASH_ERR_WRITE,   
    VL_FLASH_ERR_READ,    

} vl_ble_status_t;

typedef struct vl_ble_type
{
    FunctionPointer callback; 
    UINT8 adv_state;        // 0: not advertising, 1: advertising
    ble_status_s connect_state;    // @ref BLE_STATUS_S
    UINT8 bound_flag;       // 0: not bound, 1: bounded
    UINT8 MAC_Addr[6];      
    /* data */
}vl_ble_type_t;

typedef struct vl_iot_type
{
    UINT8 productID[PRODUCTID_LEN];
    UINT8 deviceID[DEVICEID_LEN];
    UINT8 randomNum[RANDOMNUM_LEN];
    UINT8 randomNumVerified_flag;
    UINT8 authVerifed_flag;
    iot_conn_status_s conn_state;
    /* data */
}vl_iot_type_t;

#ifndef VL_LOG_LEVEL
#define VL_LOG_LEVEL
typedef enum
{
    VL_LOG_LEVEL_DEBUG = 0,
    VL_LOG_LEVEL_INFO,
    VL_LOG_LEVEL_WARN,
    VL_LOG_LEVEL_ERROR,
    VL_LOG_LEVEL_FATAL,
    VL_LOG_LEVEL_MAX
}vl_log_level_t;
#endif

#endif
