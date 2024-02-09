/*
 * scheduler.c
 *
 *  Created on: Feb 5, 2024
 *      Author: Parth
 */

#ifndef SRC_SCHEDULER_C_
#define SRC_SCHEDULER_C_

#include "stdint.h"

typedef enum{
  noEventPending = 0,
  eventLETUnderFlow = 1
}event_t;

void schedulerSetEventLETUF();
uint32_t schedulerGetNextEvent();

#endif /* SRC_SCHEDULER_C_ */
