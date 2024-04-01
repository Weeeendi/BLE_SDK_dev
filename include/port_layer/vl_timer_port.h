/**
 * @file vl_timer_port.h
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK
 * @version 1.0.0
 * @date 2024-02-26
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef VL_TIMER_PORT_H
#define VL_TIMER_PORT_H
#include "vl_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum tag_timer_id 
{
	HIL_TIMER_SYSTICK,
	HIL_TIMER_UART,
	HIL_TIMER_BLE,
	HIL_TIMER_CAN,
	HIL_TIMER_ID0,
	HIL_TIMER_ID1,
	HIL_TIMER_ID2,
	HIL_TIMER_ID3,
	HIL_TIMER_ID4,
	HIL_TIMER_ID5,
	HIL_TIMER_ID6,
	HIL_TIMER_ID7,
	HIL_TIMER_ID8,
	HIL_TIMER_ID9
}timer_id_e;

typedef enum{
    VL_TIMER_ONCE = 0,
    VL_TIMER_REPEAT = 1,
}vl_timer_mode;


typedef void *vl_ble_timer_t;

/**
 * @brief Initialize SDK timer module
 * 
 */
vl_status_t vl_timer_init(void);

/**
 * Creates a timer with a callback function.
 *
 * This function initializes a timer with the given ID, timeout duration, 
 * and an optional repeat behavior. Upon expiration, the specified callback function is executed.
 *
 * @param timer_id The unique identifier for the timer
 * @param timeout The time interval (in units as milliseconds, depending on implementation) before the timer triggers its callback function.
 * @param repeat  If set to TRUE, the timer will repeatedly execute; otherwise, it executes only once.
 * @param callback A pointer to the callback function that will be invoked when the timer expires.
 * @return vl_status_t.
 */
vl_status_t vl_timer_creat(UINT8 timer_id, UINT16 timeout, BOOL repeat, FunctionPointer callback);

/**
 * Starts a timer.
 *
 * This function begins the countdown for an already created timer with the specified ID. 
 * The timer will start running and trigger its associated action (if any) upon reaching 
 * its predefined timeout duration.
 *
 * @param timer_id This ID must correspond to a previously created timer.
 * @return vl_status_t.
 */
vl_status_t vl_timer_start(UINT8 timer_id);

/**
 * Stop a timer.
 *
 * This function stops an already created timer with the specified ID. 
 * The timer will stop running.
 *
 * @param timer_id This ID must correspond to a previously created timer.
 * @return vl_status_t.
 */
vl_status_t vl_timer_stop(UINT8 timer_id);


#ifdef __cplusplus
}
#endif

#endif