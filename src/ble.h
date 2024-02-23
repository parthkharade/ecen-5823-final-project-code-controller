/*
 * ble.h
 *
 *  Created on: Feb 21, 2024
 *      Author: Parth Kharade, Parth.Kharade@Colorado.edu
 *
 *  @institution University of Colorado Boulder (UCB)
 *  @course ECEN 5823-001: IoT Embedded Firmware (Spring 2024)
*   @instructor David Sluiter
*
*   @assignment ecen5823-assignment3-ParthKharade
*   @due Feb 8, 2024
*
*   @resources Utilized Silicon Labs' EMLIB peripheral libraries to
*   implement functionality.
* -  sl_bt_api.h used for bluetooth stack api
*
*   @copyright All rights reserved. Distribution allowed only for the
*   use of assignment grading. Use of code excerpts allowed at the
*   discretion of author. Contact for permission.
 */

#ifndef SRC_BLE_H_
#define SRC_BLE_H_

#define UINT8_TO_BITSTREAM(p, n) { *(p)++ = (uint8_t)(n); }
#define UINT32_TO_BITSTREAM(p, n) { *(p)++ = (uint8_t)(n); *(p)++ = (uint8_t)((n) >> 8); \
*(p)++ = (uint8_t)((n) >> 16); *(p)++ = (uint8_t)((n) >> 24); }
#define INT32_TO_FLOAT(m, e) ( (int32_t) (((uint32_t) m) & 0x00FFFFFFU) | (((uint32_t) e) << 24) )

#include "stdbool.h"
#include "stdint.h"
#include "sl_bgapi.h"
#include "sl_bt_api.h"

// BLE Data Structure, save all of our private BT data in here.
// Modern C (circa 2021 does it this way)
// typedef ble_data_struct_t is referred to as an anonymous struct definition
typedef struct {
// values that are common to servers and clients
bd_addr myAddress;
uint8_t myAddressType;
// values unique for server

uint8_t connectionHandle;

// The advertising set handle allocated from Bluetooth stack.
uint8_t advertisingSetHandle;
bool connection_open;
bool ok_to_send_htm_indications;
bool indication_in_flight;


// values unique for client

} ble_data_struct_t;
/*
 * Event responder for systems BLE events
 * */
void handle_ble_event(sl_bt_msg_t *evt);

/*
 * getter function for ble conncetion meta data
 * */
ble_data_struct_t *get_ble_data();
#endif /* SRC_BLE_H_ */
