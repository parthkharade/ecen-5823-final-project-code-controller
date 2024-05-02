/*
 * scheduler.c
 *
 *  Created on: Feb 5, 2024
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
* -  em_core.h used for cricitcal section execution
*
*   @copyright All rights reserved. Distribution allowed only for the
*   use of assignment grading. Use of code excerpts allowed at the
*   discretion of author. Contact for permission.
 */
#define INCLUDE_LOG_DEBUG 1

#include "log.h"
#include "scheduler.h"
#include "gpio.h"
#include "em_core.h"
#include "sl_power_manager.h"
#include "timer.h"
#include "gatt_db.h"
#include "em_adc.h"
#include "adc.h"
uint32_t schedulerPedningEvents = 0;
void schedulerSetEventLETUF(){
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  sl_bt_external_signal(eventLETUnderFlow);
  CORE_EXIT_CRITICAL();
}

void schedulerSetEventLETComp1(){
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  sl_bt_external_signal(eventLETComp1);
  CORE_EXIT_CRITICAL();
}
void schedulerSetEventADCComplete(){
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  sl_bt_external_signal(eventADCComplete);
  CORE_EXIT_CRITICAL();
}
void joystickReadStateMachine(sl_bt_msg_t *evt){
  ble_data_struct_t *ble_data = get_ble_data();
  uint8_t joystick_state = 0;
  if (SL_BT_MSG_ID(evt->header) != sl_bt_evt_system_external_signal_id)
    {
      return;
    }
  event_t event = evt->data.evt_system_external_signal.extsignals;
  uint8_t sc = 0;
      if(event&eventADCComplete){
          uint8_t adc0_val = ADC_DataSingleGet(ADC0);
          uint8_t joystick_state = 0;
          if(adc0_val > 42 && adc0_val < 127){
              joystick_state = 0;
          }
          else if(adc0_val>=127){
              joystick_state = 1;
          }
          else if(adc0_val<=42){
              joystick_state = 2;
          }
          if (ble_data->connection_open == true
                    && ble_data->ok_to_send_htm_indications)
                  { //send indication to client only if
                    //ble connection is open and indications are
                    //enabled for this characteristic
                    sc = sl_bt_gatt_server_send_notification (ble_data->connectionHandle, //handle
                        gattdb_joystick_state,                            //characteristic
                        sizeof(joystick_state),                            //size of value
                        &joystick_state);                              //value to indicate
                    displayPrintf (DISPLAY_ROW_JOYSTICK, "Dir=%d", joystick_state); //show Joystick Direction on display
                    if (sc != SL_STATUS_OK)
                      {
                        LOG_ERROR("sl_bt_gatt_server_send_indication() returned != 0 status=0x%04x\r", (unsigned int) sc);
                      }
                  }
      }
}
