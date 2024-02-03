/*
 * timer.c
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


#include "timer.h"
#include "stdbool.h"
#include "em_letimer.h"
#include "em_cmu.h"

#define LETIMER_PERIOD    (2250U)
#define LED_ON_TIME  (175U)
void lettimer0Init(){
  const LETIMER_Init_TypeDef LETimer0Init = {
  .enable = false,
  .debugRun = true,
  .bufTop = false,
  .comp0Top = true,
  .out0Pol = 0,
  .out1Pol = 0,
  .ufoa0 = letimerUFOANone,
  .ufoa1 = letimerUFOANone,
  .repMode = letimerRepeatFree};

  LETIMER_Init(LETIMER0, &LETimer0Init);

  uint32_t leTimerFreq = CMU_ClockFreqGet(cmuClock_LETIMER0);

  //Calculate comp0val which will be reloaded in CNT and corresponds to the total period.
  uint32_t comp0val = (uint32_t) ((LETIMER_PERIOD * leTimerFreq)/1000);

  //Calculate comp1val as a fraction of the total period.
  uint32_t comp1val = (uint32_t) ((LED_ON_TIME*comp0val)/LETIMER_PERIOD);


  LETIMER_CompareSet(LETIMER0, 0, comp0val);
  LETIMER_CompareSet(LETIMER0, 1, comp1val);

  LETIMER_IntClear(LETIMER0, 0xFFFFFFFF);

  LETIMER_IntEnable(LETIMER0, (LETIMER_IEN_UF | LETIMER_IEN_COMP1));

  LETIMER_Enable(LETIMER0, true);

}
