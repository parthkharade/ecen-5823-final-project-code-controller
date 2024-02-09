/*
 * timer.h
 *
 *  Created on: Jan 31, 2024
 *      Author: Parth Kharade, Parth.Kharade@Colorado.edu
 *
 *  @institution University of Colorado Boulder (UCB)
 *  @course ECEN 5823-001: IoT Embedded Firmware (Spring 2024)
*   @instructor David Sluiter
*
*   @assignment ecen5823-assignment2-ParthKharade
*   @due Feb 1, 2024
*
*   @resources Utilized Silicon Labs' EMLIB peripheral libraries to
*   implement functionality.
* -  em_letimer.h used for timer initialization
* -  em_cmu.h used to get LETimer Frequency
*
*   @copyright All rights reserved. Distribution allowed only for the
*   use of assignment grading. Use of code excerpts allowed at the
*   discretion of author. Contact for permission.
 */

#ifndef SRC_TIMER_H_
#define SRC_TIMER_H_



#include "stdint.h"
/*
 * Function to intialize LETimer0 with a period of 2.25ms.
 * Enables COMP1 and UF interrupt.
 * */

// Uncomment this line to enable the unit-test function for waitTimerUs
//#define UNIT_TEST_TIMER
void lettimer0Init();
void timerWaitUs(uint32_t delay);

#ifdef UNIT_TEST_TIMER
void testTimerWaitUs();
#endif
#endif /* SRC_TIMER_H_ */
