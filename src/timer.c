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
#define INCLUDE_LOG_DEBUG 1

#include "timer.h"
#include "stdbool.h"
#include "em_letimer.h"
#include "src/log.h"
#include "em_cmu.h"

#ifdef UNIT_TEST_TIMER
#include "src/gpio.h"
#endif
#define LETIMER_PERIOD    (3000U)
#define ONE_US (1000000LU)
#define MAX_DELAY_US ((LETIMER_PERIOD)*(1000U))

static uint16_t comp0val; // making this global so that it can be accessed by waitimeruS
static float tickValuS;
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
  comp0val = (uint32_t) ((LETIMER_PERIOD * leTimerFreq)/1000);

  tickValuS =  (ONE_US)/(float)leTimerFreq; // Calculate the exact tick time and round off later during the actual delay function.


  LETIMER_CompareSet(LETIMER0, 0, comp0val);

  LETIMER_IntClear(LETIMER0, 0xFFFFFFFF);

  LETIMER_IntEnable(LETIMER0, (LETIMER_IEN_UF));

  LETIMER_Enable(LETIMER0, true);

}

void timerWaitUs(uint32_t delay){
  if(delay > MAX_DELAY_US){
    LOG_ERROR("Delay of %lu uS more than maximum permissible delay of %lu uS\r\n",delay,MAX_DELAY_US);
  }
  else{

      //Gets the current Count Value.
      uint32_t currCntVal = LETIMER_CounterGet(LETIMER0);

      // Calculate the number of ticks to wait for.
      uint16_t delayTicks = (uint16_t)(delay/tickValuS);

      // Wait atleast 1-tick if the calculated ticks is 0. Can happen when delay is less than tickValuS.
      if(delayTicks == 0) delayTicks = 1;


      // Check for wrap around condition.
      // If the number of ticks to wait for is more than what the timer can provide. The cnt value will be currVal + comp0val - delayticks + 1 (accounting for the tick from 0->comp0val);
      uint32_t waitVal = (currCntVal>delayTicks)?((currCntVal-delayTicks)):((comp0val - (delayTicks-currCntVal) + 1));
      while(LETIMER_CounterGet(LETIMER0)!=waitVal);
  }
}

#ifdef UNIT_TEST_TIMER
void testTimerWaitUs(){
  while(1){

      //Delay test of 10ms
      gpioLed0SetOn();
      timerWaitUs(10000);
      gpioLed0SetOff();
      timerWaitUs(10000);

      //Delay test of 100ms
      gpioLed0SetOn();
      timerWaitUs(100000);
      gpioLed0SetOff();
      timerWaitUs(100000);

      //Delay test of 1000ms
      gpioLed0SetOn();
      timerWaitUs(1000000);
      gpioLed0SetOff();
      timerWaitUs(1000000);

      //Delay test of 3000ms
      gpioLed0SetOn();
      timerWaitUs(3000000);
      gpioLed0SetOff();
      timerWaitUs(3000000);

      //Delay test of 4000ms (Invalid Delay)
      gpioLed0SetOn();
      timerWaitUs(4000000);
      gpioLed0SetOff();
      timerWaitUs(4000000);


  }
}
#endif

