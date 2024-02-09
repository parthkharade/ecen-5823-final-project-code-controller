/*
 * scheduler.c
 *
 *  Created on: Feb 5, 2024
 *      Author: Parth
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

