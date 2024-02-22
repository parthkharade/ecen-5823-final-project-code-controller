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
#include "i2c.h"
#include "sl_power_manager.h"
#include "timer.h"
#include "gatt_db.h"
uint32_t schedulerPedningEvents = 0;
extern uint8_t data[2];
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

void schedulerSetEventI2CTRXSuccess(){
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  sl_bt_external_signal(eventI2CTRXSuccessful);
  CORE_EXIT_CRITICAL();
}
//uint32_t schedulerGetNextEvent(){
//  uint32_t event = 0;
//  uint32_t currentPendingEvents = 0;
//
//  CORE_DECLARE_IRQ_STATE;
//
///**
// * @brief
// * Make a copy of the pending events variable so that it doesn't change when deciding the next peding event.
// *
// */
//  CORE_ENTER_CRITICAL();
//  currentPendingEvents = schedulerPedningEvents;
//  CORE_EXIT_CRITICAL();
//
//  if(currentPendingEvents&eventI2CTRXSuccessful){
//      event=eventI2CTRXSuccessful;
//  }
//  else if(currentPendingEvents&eventLETComp1){
//      event=eventLETComp1;
//  }
//  else if(currentPendingEvents&eventLETUnderFlow){
//      event=eventLETUnderFlow;
//  }
//  /**
//   * This approach works well if the number of events are small. I can conditionally check for the event mask in the order of their priority and return once a pendinnd event is found.
//   * For larger number of events a for loop to extract the bits of the pending event variable would be a better approach.
//   *
//   */
//  CORE_ENTER_CRITICAL();
//  schedulerPedningEvents &= ~event;
//  CORE_EXIT_CRITICAL();
//  return event;
//}

/**
 * @brief The state machine has 5 states and state transitions are triggered by events. If an event otehr than the one exepected is recieved then the Machine moves back to it's idle state.
 * 
 * IDLE ---> TURN ON SENSOR and WAIT  ---> SEND WRITE and wait for Succeess----> Wait for Conversion TIME DELAY ---> Initiate READ ---> Wait for success and print result.
 * 
 * @param 
 */
void temperature_state_machine(sl_bt_msg_t *evt){
  state_t current_state;
  static state_t next_state = stateIdle;
  current_state = next_state;
  ble_data_struct_t *ble_data=get_ble_data();
  if(SL_BT_MSG_ID(evt->header) != sl_bt_evt_system_external_signal_id){
      return;
  }
  event_t event = evt->data.evt_system_external_signal.extsignals;
  switch(current_state){
    case stateIdle: // IDLE state. Wait for Timer Underflow to initiate transactions
      next_state = stateIdle;
      if((event&eventLETUnderFlow) &&
          ble_data->connection_open &&
          ble_data->ok_to_send_htm_indications){
          si7021_power(true); 
          timerWaitUs_irq(SI7021_ON_DELAY_US);
          next_state = stateTSensorOn;
      }
      break;
    case stateTSensorOn: // Sensor is on. Send Write command after 80ms COMP1 interrupt
      next_state = stateIdle;
      if(event&eventLETComp1){
          sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
          si7021_sendTempCmd();
          next_state = stateSentWCmd;
      }
      break;
    case stateSentWCmd: // Write Command transfer has been initiated. If successful wait for conversion time.
      next_state = stateIdle;
      if(event&eventI2CTRXSuccessful){
          sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
          timerWaitUs_irq(SI7021_READ_DELAY_US);
          next_state = stateWaitReadDelay;
      }
      break;
    case stateWaitReadDelay: // Wait for the conversion delay to expire. Then begin I2C transaction for reading data.
      next_state = stateIdle;
      if(event&eventLETComp1){
          sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
          si7021_readTempData();
          next_state = stateWaitForRead;
      }
      break;
    case stateWaitForRead: //wait for read transaction to complete. After that report the temperature if successful.
      next_state = stateIdle;
      if(event&eventI2CTRXSuccessful){
          si7021_power(false);
          sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
          uint16_t rawTempVal = ((data[0]<<8)|(data[1]));
          int temp = (int)((175.72 * rawTempVal)/65536 - 46.85);
          uint8_t temp_buff[5];
          temp_buff[0] = 0;
          int32_t ieee_temp = INT32_TO_FLOAT(temp*1000,-3);
          uint8_t *p = (temp_buff+1);
          UINT32_TO_BITSTREAM(p,ieee_temp);
          sl_bt_gatt_server_write_attribute_value(gattdb_temperature_measurement, 0, 5, temp_buff);
          if(ble_data->connection_open && ble_data->ok_to_send_htm_indications &&
              !(ble_data->indication_in_flight)){
              sl_bt_gatt_server_send_indication(ble_data->connectionHandle, gattdb_temperature_measurement,5, temp_buff);
          }
          LOG_INFO("Temperature Read Successful. Temp in C : %d\r\n",temp);
      }
      break;
  }
}
