# BLE SDK for Vehiclink

This is the BLE SDK for Vehiclink.Using this SDK,you can easily connect to Vehiclink App and send data to Vehiclink App.

## 1.Overview
```
─config
├─driver
│  └─N32W452CC
├─include
│  ├─algorithm_layer
│  ├─application_layer
│  └─port_layer
├─lib
└─source
    ├─algorithm_layer
    ├─application_layer
    └─port_layer
```
user need implement all functions in the file "source/port_layer/**".


## 2.flash address
<table>
 <tr>
    <th>Function</th>
    <th>Description</th>
    <th>Address offset</th>
 </tr>
 <tr>
    <td >BOOT</td>
    <td>-</td>
    <td>0x0000~0xA000</td>
 </tr>
 <tr>
    <td rowspan="3">IOT INFO</td>
    <td>iot token</td>
    <td>0xA000~0xB000</td>
 </tr>
 <tr>
    <td>ble status</td>
    <td>0xB000~0xB000</td>
 </tr>
  <tr>
    <td>reserved</td>
    <td>0xB000~0xE000</td>
 </tr>
 <tr>
    <td>APP</td>
    <td>user application</td>
    <td>0xE000~0xXXXXX</td>
 </tr>
</table>

You can see or edit the flash address in the file "config/vl_config.h".

The OTA (Over-The-Air) related configurations need to be set according to whether the hardware has external storage. 

The configuration details are as follows: "include/Application_layer/vl_iot_ota.h".


## 3.Protocol

DP Structure:

<table>
  <tr>
    <th colspan="5">dp group 1</th>
    <th>~</th>
    <th colspan="5">dp group n</th>
  </tr>
  <tr>
    <td>1</td><td>2</td><td>3</td><td>4</td><td>5</td>
    <td>~</td>
    <td>1</td><td>2</td><td>3</td><td>4</td><td>5</td>
  </tr>
  <tr>
    <td>dp_id</td><td>dp_type</td>
    <td colspan="2">dp_len</td><td>dp_value</td>
    </td><td>~</td>
    <td>dp_id</td><td>dp_type</td>
    <td colspan="2">dp_len</td><td>dp_value</td>
  </tr>
</table>


dp_id： 1 byte, You can refer to the contents in the function list.
dp_type：1 byte  
```c
#define DT_RAW 0  
#define DT_BOOL 1 
#define DT_VALUE 2 
#define DT_STRING 3 
#define DT_ENUM 4  
#define DT_BITMAP 5 
```
dp_data_len：2 bytes  
dp_data_value：n bytes, up to dp_data_len bytes


<p><mark>Notice: Support totle dp len is less than MTU,when the device send dp to Vehiclink App</mark></p>

## 4.Application

SDK provides examples: vl_demo_app.c


Enjoy it!
