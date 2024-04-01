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


#define PRODUCTID_LEN 10
#define DEVICEID_LEN 16
#define RANDOMNUM_LEN 4
#define AES_SESSION_KEY_LENGTH 16


/** dp type */
#define DT_RAW 0  
#define DT_BOOL 1 
#define DT_VALUE 2 
#define DT_STRING 3 
#define DT_ENUM 4  
#define DT_BITMAP 5 

typedef enum
{
    DISCONNECT_UNBOUND,
    CONNECT_UNBOUND,
    CONNECT_BOUND,
    DISCONNECT_BOUND,
    UNKNOW_STATE,
} iot_conn_status_s;

typedef enum
{
    BLE_STATUS_INIT,
    BLE_STATUS_START_ADV,
    BLE_STATUS_STOP_ADV,
    BLE_STATUS_CHG_ADV,
    BLE_STATUS_CONNECT,
    BLE_STATUS_DISCONNECT,
    BLE_STATUS_UNKNOW,
} ble_status_s;

typedef enum{
    NORMAL_MODE,
    FACTORY_MODE,
    UPDATE_MODE,
    LOWERPOWER_MODE,
    UNKNOW_MODE,
} iot_work_mode_s;

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

} vl_status_t;

typedef vl_status_t (*DataPrcessPointer)(UINT8* ,UINT16);  // Function pointer type
// Other types can be added as needed...

/******BLE PARAMETERS*********************************************/
#define BLE_ADV_DATA_LEN       30
#define BLE_ADV_RSCAN_LEN      30
#define BLE_ADV_NAME_LEN       BLE_ADV_RSCAN_LEN

#define BLE_SERVICE_USED        1       //Number of Services used
#define BLE_CHARACTER_USED      2       //Number of Characters used

/** @defgroup GATT_PROP_BITMAPS_DEFINES GATT Attribute Access Permissions Bit Fields
 * @{
 */
#define GATT_PROP_BROADCAST             (1<<0)  //!< Attribute is able to broadcast
#define GATT_PROP_READ                  (1<<1)  //!< Attribute is Readable
#define GATT_PROP_WRITE_CMD             (1<<2)  //!< Attribute supports write with no response
#define GATT_PROP_WRITE_REQ             (1<<3)  //!< Attribute supports write request
#define GATT_PROP_NOTI                  (1<<4)  //!< Attribute is able to send notification
#define GATT_PROP_INDI                  (1<<5)  //!< Attribute is able to send indication
#define GATT_PROP_AUTH_SIG_WRTIE        (1<<6)  //!< Attribute supports authenticated signed write
#define GATT_PROP_EXTEND_PROP           (1<<7)  //!< Attribute supports extended properities
#define GATT_PROP_WRITE                 (1<<8)  //!< Attribute is Writable (support both write_req and write_cmd)
#define GATT_PROP_AUTHEN                (1<<9)  //!< Attribute requires Authentication

/**
 * @brief BLE Service parameter configuration
 */
typedef struct bt_svc_param_t
{
    uint8_t svc_uuid[16];  /// 128 bits UUID

    struct bt_character{
        uint16_t uuid[16];  //128bit UUID
        uint16_t permission;//reference to @ref GATT_PROP_BITMAPS_DEFINES
    }character[BLE_CHARACTER_USED];
}__VL_BLE_PACKED bt_svc_param;

// Define a structure to store Bluetooth device attributes parameters
typedef struct{
    uint8_t device_name[BLE_ADV_NAME_LEN];        // Bluetooth device name, at least 4 characters are valid. If all 0x00 or an invalid name, the default device name will be used
    uint8_t adv_data[BLE_ADV_DATA_LEN];           // Advertising data, with an actual effective length of 30 bytes
    uint8_t adv_data_len;           // Length of advertising data
    uint8_t scan_rsp_data[BLE_ADV_RSCAN_LEN];      // Scan response data, with an actual effective length of 30 bytes
    uint8_t scan_rsp_data_len;      // Length of scan response data
    uint8_t device_addr[6];         // Device MAC address

    // Services included in the advertising data
    bt_svc_param service[BLE_SERVICE_USED];
}__VL_BLE_PACKED vl_ble_attr_param_t;

/******BLE PARAMETERS**************************************************/

typedef struct vl_ble_type
{
    volatile ble_status_s connect_state;    // @ref BLE_STATUS_S
    volatile uint8_t current_mtu;
    BOOL test_flag;        // 0: not test, 1: test mode
    UINT8 mac[6];
}__VL_BLE_PACKED vl_ble_obj_t;


typedef struct vl_iot_type
{
    UINT8 productID[PRODUCTID_LEN];
    UINT8 deviceID[DEVICEID_LEN];
    UINT8 random_num[RANDOMNUM_LEN];
    BOOL random_num_valid;
    UINT8 aes128Key[AES_SESSION_KEY_LENGTH];
    BOOL authVerifed_flag;
    volatile iot_conn_status_s conn_state;
    volatile iot_work_mode_s work_mode;
    volatile BOOL bound_flag;    // 0: not bound, 1: bounded
    DataPrcessPointer dp_query_cb;
    DataPrcessPointer dp_recv_cb;
}__VL_BLE_PACKED vl_iot_obj_t;

#endif
