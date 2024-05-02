/*
 * oscillator.c
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
* -  em_cmu.h used for oscillator initialiszaion
*
*   @copyright All rights reserved. Distribution allowed only for the
*   use of assignment grading. Use of code excerpts allowed at the
*   discretion of author. Contact for permission.
 */
#include <src/oscillator.h>
#include "em_cmu.h"
#include "stdbool.h"
#include "app.h"
void oscInit(){

  // LFXO is not available in EM3.
#if LOWEST_ENERGY_MODE == 3
  CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_ULFRCO);
#else
  CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);
  CMU->ADCCTRL |= CMU_ADCCTRL_ADC0CLKSEL_AUXHFRCO;
//  CMU_ClockSelectSet(cmuClock_AUX, cmuSelect_AUXHFRCO);
  CMU_ClockDivSet(cmuClock_LETIMER0 , cmuClkDiv_4);
#endif
  CMU_ClockEnable(cmuClock_LETIMER0, true);
  CMU_ClockEnable(cmuClock_ADC0, true);
}
