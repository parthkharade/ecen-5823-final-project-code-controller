/*
 * irq.h
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
*   @copyright All rights reserved. Distribution allowed only for the
*   use of assignment grading. Use of code excerpts allowed at the
*   discretion of author. Contact for permission.
 */
#define INCLUDE_LOG_DEBUG 1
#include "log.h"
#include "irq.h"
#include "em_letimer.h"
#include "gpio.h"
#include "em_core.h"
#include "scheduler.h"
#include "em_i2c.h"
#include "em_adc.h"
uint16_t LETimer0_underflowCount = 0;
void LETIMER0_IRQHandler(){
  uint32_t irqStatusFlag = LETIMER_IntGet(LETIMER0);

  //Clear Pending Interrupts.
  LETIMER_IntClear(LETIMER0, irqStatusFlag);
  NVIC_ClearPendingIRQ(LETIMER0_IRQn);

  if(irqStatusFlag&LETIMER_IF_UF){
      schedulerSetEventLETUF();
      ADC_Start(ADC0, adcStartSingle);
      LETimer0_underflowCount++;
  }
  if(irqStatusFlag&LETIMER_IF_COMP1){
      LETIMER_IntDisable(LETIMER0, LETIMER_IEN_COMP1);
      schedulerSetEventLETComp1();
  }

}
void ADC0_IRQHandler(void){
  uint32_t irqStatusFlag = ADC_IntGet(ADC0);
  //Clear Pending Interrupts.
  ADC_IntClear(ADC0, irqStatusFlag);
  uint8_t adc0_val = ADC_DataSingleGet(ADC0);
  NVIC_ClearPendingIRQ(ADC0_IRQn);
  schedulerSetEventADCComplete();
}

uint32_t letimerMilliseconds(){
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  uint32_t underflow_count = LETimer0_underflowCount;
  CORE_EXIT_CRITICAL();
  return underflow_count*LETIMER_PERIOD_MS + ((LETIMER_CompareGet(LETIMER0, 0) - LETIMER_CounterGet(LETIMER0))*getTickValueus()/1000);
}
