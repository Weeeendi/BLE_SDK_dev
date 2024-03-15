/**
 * @file vl_iot_protocal.c
 * @author wendi (wendy@vehiclink.com)
 * @brief This file is a part of VL_BLE_SDK,This file implements about ble applicate layer protocal.
 * @version 1.0.0
 * @date 2024-03-12
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "vl_iot_protocal.h"
#include "vl_iot_handle.h"
#include "vl_iot_utils.h"
#include "vl_ble_queue_send.h"
#include "vl_ble_event.h"
#include "vl_log.h"
#include "vl_type.h"

static UINT32 vl_ble_ack_sn = 0;
static UINT32 vl_ble_sn = 1;

char TAG[] = "vl_ble_protocal";


/*****************************************************************************
Function name: bt_write_frame
Function description: Send a frame of data to the bt port
Input parameter: fr_type:frame type
           len:data length

Return parameter: none
*****************************************************************************/
void bt_write_frame(unsigned short fr_type, unsigned short len)
{
    uint16_t crc16;
    uint8_t *tx_buf_ptr = NULL;
    tx_buf_ptr = &BLE_Send.curbuf[0];
    int_to_byte(vl_ble_sn++, tx_buf_ptr);
    // if report by mcu ack sn equare 0
    int_to_byte(vl_ble_ack_sn, tx_buf_ptr + ACK_SN);
    vl_ble_ack_sn = 0; // clear ack sn

    tx_buf_ptr[FUNC_CODE] = (fr_type >> 8) & 0xFF;
    tx_buf_ptr[FUNC_CODE + 1] = fr_type & 0xFF;
    tx_buf_ptr[LENGTH_HIGH] = len << 8;
    tx_buf_ptr[LENGTH_LOW] = len;
    crc16 = CRC16(tx_buf_ptr, 12 + len); // 4+4+2+2
    tx_buf_ptr[DATA_START + len] = crc16 & 0xFF;
    tx_buf_ptr[DATA_START + len + 1] = (crc16 >> 8) & 0xFF;
    len += 2;
    if (yj_ble_boundf_read())
    {
#if VL_BLE_LOG
        VL_LOG_HEXDUMP_DEBUG(TAG, "ble send origino msg:", (uint8_t *)BLE_Send.curbuf, DATA_START + len);
#endif
        /*After the device is bound, each bit is XOR with a random number, which requires decryption*/
        tal_xor_str((uint8_t *)&BLE_Send.curbuf[0], vl_iot_obj.randomNum[3], DATA_START + len);
    }
    BLE_Send.len = len + DATA_START;
    ble_send_enqueue(&BLE_Send);
    BLE_Send.len = 0; // clear len
}

/*****************************************************************************
Function name: set_bt_uart_byte
Function description: Writes 1 byte to BT_UART
Input parameters: dest: the actual address of the buffer area;
           byte:Write byte value
Return parameter: the total length after writing
*****************************************************************************/
unsigned short set_bt_uart_byte(unsigned short dest, unsigned char byte)
{
  unsigned char *obj = (unsigned char *)BLE_Send.curbuf +DATA_START+ dest;

  *obj = byte;
  dest += 1;

  return dest;
}

/*****************************************************************************
Function name: set_bt_uart_buffer
Function description: Writes a buffer to BT_UART
Input parameter: dest: destination address
           src:source address
           len:Data length
Return parameter: none
*****************************************************************************/
unsigned short set_bt_uart_buffer(unsigned short dest, unsigned char *src,unsigned short len)
{
  unsigned char *obj = (unsigned char *)BLE_Send.curbuf +DATA_START+ dest;

  memcpy(obj, src, len);

  dest += len;
  return dest;
}

/**
 * @brief check use which commincation to send data
 * 
 * @return unsigned char @ref vl_status_t
 */
vl_status_t chk_usedCommAndSend(uint8_t length){

  if(0==yj_ble_boundf_read() && vl_ble_obj.test_flag == 0)
    return VL_BLE_ERR_INVALID_STATE;
      
  if(vl_ble_connect_status_get() == BLE_STATUS_CONNECT){
    YJ_LOG_HEXDUMP_DEBUG(TAG, "ble send dp msg:",(uint8_t*)&BLE_Send.curbuf[DATA_START],length);
    bt_write_frame(VL_BLE_EVT_DP_DATA_REPORTED, length);
    return VL_SUCCESS;
  }else{
    return VL_BLE_ERR_COMMON;
  }
}

/*****************************************************************************
Function name: mcu_dp_raw_update
Function description: raw dp data upload
Return parameter: dpid:id
           value:dp date
           len:length
Return parameter: none
*****************************************************************************/
unsigned char mcu_dp_raw_update(unsigned char dpid, const unsigned char value[], unsigned short len)
{
  unsigned short length = 0;
  unsigned char ret = 0;
  //
  length = set_bt_uart_byte(length, dpid);
  length = set_bt_uart_byte(length, DP_TYPE_RAW);
  //
  length = set_bt_uart_byte(length, len / 0x100);
  length = set_bt_uart_byte(length, len % 0x100);
  //
  length = set_bt_uart_buffer(length, (unsigned char *)value, len);\
  
  ret = chk_usedCommAndSend(length);

  return ret;
}
/*****************************************************************************
Function name: mcu_dp_bool_update
Function description: Bool dp data upload
Input parameter: dpid:id number
           value:
Return parameter: none
*****************************************************************************/
unsigned char mcu_dp_bool_update(unsigned char dpid, unsigned char value)
{
  unsigned short length = 0;
  unsigned char ret = 0;

  length = set_bt_uart_byte(length, dpid);
  length = set_bt_uart_byte(length, DP_TYPE_BOOL);
  //
  length = set_bt_uart_byte(length, 0);
  length = set_bt_uart_byte(length, 1);
  //
  if (value == 0)
  {
    length = set_bt_uart_byte(length, 0);
  }
  else
  {
    length = set_bt_uart_byte(length, 1);
  }

  ret = chk_usedCommAndSend(length);
  return ret;
}
/*****************************************************************************
Function name: mcu_dp_value_update
Function description: value dp data upload
Input parameter: dpid:id number
           value:
Return parameter: none
*****************************************************************************/
unsigned char mcu_dp_value_update(unsigned char dpid, unsigned long value)
{
  unsigned short length = 0;
  unsigned char ret = 0;

  length = set_bt_uart_byte(length, dpid);
  length = set_bt_uart_byte(length, DP_TYPE_VALUE);
  //
  length = set_bt_uart_byte(length, 0);
  length = set_bt_uart_byte(length, 4);
  //
  length = set_bt_uart_byte(length, value >> 24);
  length = set_bt_uart_byte(length, value >> 16);
  length = set_bt_uart_byte(length, value >> 8);
  length = set_bt_uart_byte(length, value & 0xff);

  ret = chk_usedCommAndSend(length);
  return ret;
}
/*****************************************************************************
Function name: mcu_dp_string_update
Function description: string dp data upload
Input parameter: dpid:id number
           value:current DP value pointer
           len:data length
Return parameter: none
*****************************************************************************/
unsigned char mcu_dp_string_update(unsigned char dpid, const unsigned char value[], unsigned short len)
{
  unsigned short length = 0;
  unsigned char ret = 0;
  //
  length = set_bt_uart_byte(length, dpid);
  length = set_bt_uart_byte(length, DP_TYPE_STRING);
  //
  length = set_bt_uart_byte(length, len / 0x100);
  length = set_bt_uart_byte(length, len % 0x100);
  //
  length = set_bt_uart_buffer(length, (unsigned char *)value, len);

  ret = chk_usedCommAndSend(length);
  return ret;
}
/*****************************************************************************
Function name: mcu_dp_enum_update
Function description: enum dp data upload
Input parameter: dpid:id number
           value:
Return parameter: none
*****************************************************************************/
unsigned char mcu_dp_enum_update(unsigned char dpid, unsigned char value)
{
  unsigned short length = 0;
  unsigned char ret = 0;

  length = set_bt_uart_byte(length, dpid);
  length = set_bt_uart_byte(length, DP_TYPE_ENUM);
  //
  length = set_bt_uart_byte(length, 0);
  length = set_bt_uart_byte(length, 1);
  //
  length = set_bt_uart_byte(length,value);
  
  ret = chk_usedCommAndSend(length);
  return ret;
}
/*****************************************************************************
Function name: mcu_dp_fault_update
Function description: bitmap dp data upload
Input parameter: dpid:id number
           value:current dp value
Return parameter: none
*****************************************************************************/
unsigned char mcu_dp_fault_update(unsigned char dpid, unsigned long value)
{
  unsigned short length = 0;
  unsigned char ret = 0;

  length = set_bt_uart_byte(length, dpid);
  length = set_bt_uart_byte(length, DP_TYPE_BITMAP);
  //
  length = set_bt_uart_byte(length, 0);

  if ((value | 0xff) == 0xff)
  {
    length = set_bt_uart_byte(length, 1);
    length = set_bt_uart_byte(length, value);
  }
  else if ((value | 0xffff) == 0xffff)
  {
    length = set_bt_uart_byte(length, 2);
    length = set_bt_uart_byte(length, value >> 8);
    length = set_bt_uart_byte(length, value & 0xff);
  }
  else
  {
    length = set_bt_uart_byte(length, 4);
    length = set_bt_uart_byte(length, value >> 24);
    length = set_bt_uart_byte(length, value >> 16);
    length = set_bt_uart_byte(length, value >> 8);
    length = set_bt_uart_byte(length, value & 0xff);
  }

  ret = chk_usedCommAndSend(length);
  return ret;
}