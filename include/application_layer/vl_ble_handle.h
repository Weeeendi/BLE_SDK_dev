/**
 * @file vl_ble_handle.h
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,ble task handler.
 * @version 1.0.0
 * @date 2024-03-13
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __VL_BLE_HANDLE_H__
#define __VL_BLE_HANDLE_H__

#include "vl_type.h"



/* global variables */
extern vl_ble_obj_t vl_ble_obj;

/* functions */
extern ble_status_s vl_ble_status_get(void);
extern vl_status_t vl_ble_status_set(ble_status_s status);
extern vl_status_t read_current_advData(uint8_t *advData);
extern void vl_change_advData(void);
extern vl_status_t save_state_aboutBLE(void);
extern vl_status_t load_state_aboutBLE(void);

extern void vl_ble_connect_monitor_timer_init(void);
extern void vl_ble_connect_monitor_timer_start(void);
extern void vl_ble_connect_monitor_timer_stop(void);
#endif