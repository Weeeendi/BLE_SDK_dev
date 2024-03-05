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


/**
 * @brief Initialize SDK timer module
 * 
 */
void vl_timer_init(void);

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
 * @return No return value (void).
 */
void vl_timer_creat(UINT8 timer_id, UINT16 timeout, BOOL repeat, FunctionPointer callback);

/**
 * Starts a timer.
 *
 * This function begins the countdown for an already created timer with the specified ID. 
 * The timer will start running and trigger its associated action (if any) upon reaching 
 * its predefined timeout duration.
 *
 * @param timer_id This ID must correspond to a previously created timer.
 * @return No return value (void).
 */
void vl_timer_start(UINT8 timer_id);

/**
 * Stop a timer.
 *
 * This function stops an already created timer with the specified ID. 
 * The timer will stop running.
 *
 * @param timer_id This ID must correspond to a previously created timer.
 * @return No return value (void).
 */
void vl_timer_stop(UINT8 timer_id);


/**
 * Clear a timer.
 *
 * This function clears an already created timer with the specified ID. 
 * The timer will Clear the timer ID will be available for use again.
 *
 * @param timer_id This ID must correspond to a previously created timer.
 * @return No return value (void).
 */
void vl_timer_clear(UINT8 timer_id);


#ifdef __cplusplus
}
#endif

#endif