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
#include "scheduler.h"
#include "em_core.h"
uint32_t schedulerPedningEvents = 0;
void schedulerSetEventLETUF(){

  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();
  schedulerPedningEvents|=eventLETUnderFlow;
  CORE_EXIT_CRITICAL();

}
uint32_t schedulerGetNextEvent(){
  uint32_t event = 0;
  uint32_t currentPendingEvents = 0;

  CORE_DECLARE_IRQ_STATE;

  CORE_ENTER_CRITICAL();
  currentPendingEvents = schedulerPedningEvents;
  CORE_EXIT_CRITICAL();

  if(currentPendingEvents&eventLETUnderFlow) event=eventLETUnderFlow;

  CORE_ENTER_CRITICAL();
  schedulerPedningEvents &= ~event;
  CORE_EXIT_CRITICAL();
  return event;
}

