/*
 * scheduler.h
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

#ifndef SRC_SCHEDULER_C_
#define SRC_SCHEDULER_C_

#include "stdint.h"

typedef enum{
  noEventPending = 0,
  eventLETUnderFlow = 1
}event_t;


/**
 * Set the LET Timer underflow event as a pending task.
 * */
void schedulerSetEventLETUF();

/**
 * Get the next highest pending priority event.
 * */
uint32_t schedulerGetNextEvent();

#endif /* SRC_SCHEDULER_C_ */
