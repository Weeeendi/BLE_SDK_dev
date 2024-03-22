/**
 * @file vl_ble_queue.c
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,fifo queue module
 * @version 1.0.0
 * @date 2024-03-08
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "vl_queue.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * Initializes a queue.
 *
 * This function initializes a queue with a specified size and unit size. The units in the queue are contiguous blocks of bytes,
 * where each unit's size is determined by the parameter `unit_size`. The total number of units that can be held in the queue
 * is specified by the `queue_size` parameter.
 *
 * @param q Pointer to the queue structure to be initialized.
 * @param elem_size Specifies the size (in bytes) of each unit in the queue.
 * @param queue_depth Specifies the number of units the queue can hold.
 * @return A status code indicating the result of the operation. 
 *         Returns VL_SUCCESS if initialization succeeds;
 *         returns VL_BLE_ERR_INVALID_PARAM if parameters are invalid;
 *         returns VL_BLE_ERR_NO_MEM if memory allocation fails.
 */
vl_status_t vl_queue_init(vl_queue_t *q, void *buf, UINT16 elem_size, UINT8 queue_depth)
{
    if (elem_size == 0 || queue_depth == 0)
        return VL_BLE_ERR_INVALID_PARAM;

    if (q->buf != NULL)
        return VL_BLE_ERR_INVALID_STATE;

    q->size = queue_depth;
    q->unit_size = elem_size;
    q->buf = (UINT8 *)malloc(queue_depth * elem_size);
    if (q->buf == NULL)
    {
        free(q->buf);
        return VL_BLE_ERR_NO_MEM;
    }
    memset(q->buf, 0, queue_depth * sizeof(UINT8));
    q->rd_ptr = 0;
    q->wr_ptr = 0;
    q->used = 0;

    return VL_SUCCESS;
}

/**
 * Enqueues an element into the queue.
 *
 * This function adds a new element pointed by `in` to the end of the queue. It ensures that the queue's capacity is not exceeded,
 * and handles memory allocation errors.
 *
 * @param q Pointer to the queue structure.
 * @param in Pointer to the data block to be enqueued, with a size equal to the queue's unit size.
 * @return A status code indicating the result of the operation. 
 *         Returns VL_SUCCESS if the element was successfully enqueued;
 *         returns VL_BLE_ERR_NO_MEM if the queue is full.
 */
vl_status_t vl_enqueue(vl_queue_t *q,PVOID in)
{
    vl_ble_device_enter_critical();
    if (q->used == q->size * q->unit_size && q->buf != NULL)
    {
        //        VL_LOG_WARNING("queue has no memory");
        //        VL_LOG_HEXDUMP_DEBUG("queue buf:",in,q->unit_size);
        vl_ble_device_exit_critical();
        return VL_BLE_ERR_NO_MEM;
    }

    memcpy((UINT8 *)q->buf + q->wr_ptr, in, q->unit_size);

    q->wr_ptr = (q->wr_ptr + q->unit_size) % (q->size * q->unit_size);
    q->used += q->unit_size;
    vl_ble_device_exit_critical();
    // VL_LOG_HEXDUMP_DEBUG("enqueue:",in,q->unit_size);

    return VL_SUCCESS;
}

/**
 * Retrieves and dequeues an element from the queue.
 *
 * This function retrieves and removes the first element in the queue, copying it to the memory location pointed by `out`.
 * If there are no elements in the queue or the queue buffer is NULL, it returns an error status.
 *
 * @param q Pointer to the queue structure.
 * @param out Pointer to the memory location where the dequeued data block will be stored (size equal to the queue's unit size).
 * @return A status code indicating the result of the operation. 
 *         Returns VL_SUCCESS if a valid element was successfully retrieved;
 *         returns VL_BLE_ERR_NOT_FOUND if the queue is empty.
 *
 */
vl_status_t vl_queue_get(vl_queue_t *q, PVOID out){
    if (q->used > 0 && q->buf != NULL)
    {
        memcpy(out, (UINT8 *)q->buf + q->rd_ptr, q->unit_size);
        // VL_LOG_HEXDUMP_DEBUG("Get queue:",out,q->unit_size);
        return VL_SUCCESS;
    }
    else
    {
        return VL_BLE_ERR_NOT_FOUND;
    }
}

/**
 * Dequeues and retrieves an element from the queue.
 *
 * This function removes the first element in the queue and copies it to the memory location pointed by `out`.
 * If there are no elements in the queue or if the queue buffer is NULL, it returns an error status.
 *
 * @param q Pointer to the queue structure.
 * @param out Pointer to the memory location where the dequeued data block will be stored .
 * @return A status code indicating the result of the operation. 
 *         returns VL_SUCCESS if a valid element was successfully dequeued;
 *         returns VL_BLE_ERR_NOT_FOUND if the queue is empty.
 */
vl_status_t vl_dequeue(vl_queue_t *q,PVOID  out)
{
    if (q->used > 0 && q->buf != NULL)
    {
        memcpy(out, (UINT8 *)q->buf + q->rd_ptr, q->unit_size);
        q->rd_ptr = (q->rd_ptr + q->unit_size) % (q->size * q->unit_size);
        q->used -= q->unit_size;
        // VL_LOG_HEXDUMP_DEBUG("out queue:",out,q->unit_size);
        return VL_SUCCESS;
    }
    else if (out == NULL)
    {
        return VL_BLE_ERR_INVALID_ADDR;
    }
    else
    {
        return VL_BLE_ERR_NOT_FOUND;
    }
}


/**
 * Flushes the queue.
 *
 * This function resets the state of the queue by setting the read pointer, write pointer, and used size to zero.
 * As a result, all elements in the queue are effectively discarded, and the queue is ready for new data.

 * @param q Pointer to the queue structure to be flushed.
 *
 * After executing this function, the queue will appear as if it has just been initialized.
 */
void vl_queue_flush(vl_queue_t *q)
{
    q->rd_ptr = 0;
    q->wr_ptr = 0;
    q->used = 0;
}

/**
 * Retrieves the number of used units in a queue.
 *
 * This function returns the current number of units that have been enqueued in the
 *  specified queue structure.
 * 
 * @param q Pointer to the yunji_queue_t structure for which to retrieve the used unit count.
 * @return An unsigned 8-bit integer representing the number of units currently used in the queue.
 */
UINT8 vl_get_queue_used(vl_queue_t *q)
{
    return q->used;
}

#ifdef __cplusplus
}
#endif