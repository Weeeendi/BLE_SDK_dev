/**
 * @file vl_ble_base_port.h
 * @author wendi (wendy@vehiclink.com)
 * @brief 
 * @version 0.1
 * @date 2024-02-23
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef VL_BLE_PORT_H
#define VL_BLE_PORT_H

#include "vl_type.h"

#define BLE_SERVICE_USED        1       //用到的蓝牙服务数量
#define BLE_CHARACTER_USED      2       //用到的特征字数量

/**
 * @brief BLE control parameter configuration 
 */
typedef struct bt_control_param_t
{
    uint32_t multi_mode:1;  //0:disable connect multiple,1:enable connect multiple
    uint32_t reserved:31;  //
}bt_control_param;

/**
 * @brief BLE Service parameter configuration
 */
typedef struct bt_svc_param_t
{
    uint8_t svc_uuid[16];  /// 128 bits UUID

    struct bt_character{
        uint16_t uuid[16];  //128bit UUID
        uint16_t permission;//reference to bt_character_perm
    }character[BLE_CHARACTER_USED];
}bt_svc_param;

typedef struct{
    bt_control_param ctrl_param;    //蓝牙控制参数
    uint8_t device_name[32];        //蓝牙设备名.4字符以上有效。如全为0x00或无效名称，则使用自带默认名称
    uint8_t adv_data[31];           //广播数据，实际有效长度30
    uint8_t adv_data_len;           //广播数据
    uint8_t scan_rsp_data[31];      //广播应答数据，实际有效长度30
    uint8_t scan_rsp_data_len;      //广播应答数据
    uint8_t device_addr[6];        //MAC address

    //adv data
    bt_svc_param service[BLE_SERVICE_USED];
}ble_attr_param_t;


/** @brief 
 * Initializes the Bluetooth stack.
 * @param advertisementData A pointer to an array containing the advertisement data. This typically includes the device name, service UUIDs, etc.
 * @param scanResponseData A pointer to an array with the scan response data. It is used to provide additional information when the device is being scanned.
 * @param adv_len The length of the advertisementData array in bytes.
 * @param scan_rsp_len The length of the scanResponseData array in bytes.
 */
UINT8 ble_initialize(ble_attr_param_t attr_param);


/**
 * @brief 
 * Starts Bluetooth advertising.
 *
 * This function initiates the Bluetooth LE advertising process for the device,
 * broadcasting its presence and optionally providing a scan response when scanned by other devices.
 *
 * @param VOID
 * @return No return value (VOID).
 */
VOID ble_startAdv(VOID);


/**
 * @brief Stop the advertising.
 * @return No return value (VOID).
 */
VOID ble_stopAdv(VOID);

/**
 * @brief Change the advertising data.
 * @param advertisementData The advertisementData array.
 * @param adv_len The length of the advertisementData array in bytes.
 * @return No return value (VOID).
 */
VOID ble_changeAdvertisingData(UINT8* advertisementData,UINT8 adv_len);

/**
 * @brief Change the scan response data.
 * @param adv_interval The interval of the advertise interval.
 * @param max_interval The interval of the advertise maximum  interval.
 * @param min_interval The interval of the advertise minimum interval.
 * @return No return value (VOID).
 */
VOID ble_setAdvParameter(UINT16 adv_interval,UINT16 max_interval,UINT16 min_interval);


/**
 * @brief Disconnect the connection.
 * @return No return value (VOID).
 */
VOID ble_disconnect();

/**
 * @brief Set the connection parameter.
 * @param conn_interval The connection interval in 1.25ms units.
 * @param conn_latency The connection latency in 1.25ms units.
 * @param conn_timeout The connection timeout in 10ms units.
 * @return No return value (VOID).
 */
VOID ble_setConnectionParameter(UINT16 interval,UINT16 latency,UINT16 timeout,UINT16 min_interval,UINT16 max_interval);

/**
 * @brief Set the MTU size.
 * @param mtu The MTU size(ATT-3).
 * @return No return value (VOID).
 */
VOID ble_MTUset(UINT8 mtu);

/**
 * @brief Send data to the remote device.
 * @param data The data to be sent.
 * @param len The length of the data to be sent.
 * @return No return value (VOID).
 */
VOID ble_send_data(UINT8* data,UINT16 len);

/**
 * @brief Get the MAC address of the device.
 * @param mac The MAC address of the device.
 * @return No return value (VOID).
 */
VOID ble_mac_get(UINT8 *mac);

/**
 * @brief Generate a random number.
 * @param (VOID).
 * @return The random number.
 */
UINT32 ble_generate_random_number(VOID);

#endif