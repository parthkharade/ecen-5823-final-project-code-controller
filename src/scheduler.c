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
#include "em_core.h"
#include "i2c.h"
#include "sl_power_manager.h"
#include "timer.h"
uint32_t schedulerPedningEvents = 0;
extern uint8_t data[2];
void schedulerSetEventLETUF(){

  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();
  schedulerPedningEvents|=eventLETUnderFlow;
  CORE_EXIT_CRITICAL();

}

void schedulerSetEventLETComp1(){
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  schedulerPedningEvents|=eventLETComp1;
  CORE_EXIT_CRITICAL();
}

void schedulerSetEventI2CTRXSuccess(){
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  schedulerPedningEvents|=eventI2CTRXSuccessful;
  CORE_EXIT_CRITICAL();
}
uint32_t schedulerGetNextEvent(){
  uint32_t event = 0;
  uint32_t currentPendingEvents = 0;

  CORE_DECLARE_IRQ_STATE;

/**
 * @brief 
 * Make a copy of the pending events variable so that it doesn't change when deciding the next peding event.
 * 
 */
  CORE_ENTER_CRITICAL();
  currentPendingEvents = schedulerPedningEvents;
  CORE_EXIT_CRITICAL();

  if(currentPendingEvents&eventI2CTRXSuccessful){
      event=eventI2CTRXSuccessful;
  }
  else if(currentPendingEvents&eventLETComp1){
      event=eventLETComp1;
  }
  else if(currentPendingEvents&eventLETUnderFlow){
      event=eventLETUnderFlow;
  }
  /**
   * This approach works well if the number of events are small. I can conditionally check for the event mask in the order of their priority and return once a pendinnd event is found.
   * For larger number of events a for loop to extract the bits of the pending event variable would be a better approach.
   * 
   */
  CORE_ENTER_CRITICAL();
  schedulerPedningEvents &= ~event;
  CORE_EXIT_CRITICAL();
  return event;
}

/**
 * @brief The state machine has 5 states and state transitions are triggered by events. If an event otehr than the one exepected is recieved then the Machine moves back to it's idle state.
 * 
 * IDLE ---> TURN ON SENSOR and WAIT  ---> SEND WRITE and wait for Succeess----> Wait for Conversion TIME DELAY ---> Initiate READ ---> Wait for success and print result.
 * 
 * @param 
 */
void temperature_state_machine(event_t event){
  state_t current_state;
  static state_t next_state = stateIdle;

  current_state = next_state;
  switch(current_state){
    case stateIdle:
      next_state = stateIdle;
      if(event == eventLETUnderFlow){
          si7021_power(true); 
          timerWaitUs_irq(SI7021_ON_DELAY_US);
          next_state = stateTSensorOn;
      }
      break;
    case stateTSensorOn:
      next_state = stateIdle;
      if(event == eventLETComp1){
          sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
          si7021_sendTempCmd();
          next_state = stateSentWCmd;
      }
      break;
    case stateSentWCmd:
      next_state = stateIdle;
      if(event == eventI2CTRXSuccessful){
          sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
          timerWaitUs_irq(SI7021_READ_DELAY_US);
          next_state = stateWaitReadDelay;
      }
      break;
    case stateWaitReadDelay:
      next_state = stateIdle;
      if(event == eventLETComp1){
          sl_power_manager_add_em_requirement(SL_POWER_MANAGER_EM1);
          si7021_readTempData();
          next_state = stateWaitForRead;
      }
      break;
    case stateWaitForRead:
      next_state = stateIdle;
      if(event == eventI2CTRXSuccessful){
          si7021_power(false);
          sl_power_manager_remove_em_requirement(SL_POWER_MANAGER_EM1);
          uint16_t rawTempVal = ((data[0]<<8)|(data[1]));
          int temp = (int)((175.72 * rawTempVal)/65536 - 46.85);
          LOG_INFO("Temperature Read Successful. Temp in C : %d\r\n",temp);
      }
      break;
  }
}
