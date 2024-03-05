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


UINT8 ble_initialize();
/**
 * @brief 
 * Starts Bluetooth advertising.
 *
 * This function initiates the Bluetooth LE advertising process for the device, broadcasting its presence and optionally providing a scan response when scanned by other devices.
 *
 * @param advertisementData A pointer to an array containing the advertisement data. This typically includes the device name, service UUIDs, etc.
 * @param scanResponseData A pointer to an array with the scan response data. It is used to provide additional information when the device is being scanned.
 * @param adv_len The length of the advertisementData array in bytes.
 * @param scan_rsp_len The length of the scanResponseData array in bytes.
 * @return No return value (VOID).
 */
VOID ble_startAdv(UINT8* advertisementData,UINT8* scanResponseData,UINT8 adv_len,UINT8 scan_rsp_len);

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
 * @brief Stop the advertising.
 * @return No return value (VOID).
 */
VOID ble_stopAdv();

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