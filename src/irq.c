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
#include "irq.h"
#include "em_letimer.h"
#include "gpio.h"
#include "em_core.h"
#include "scheduler.h"
#include "em_i2c.h"
void LETIMER0_IRQHandler(){
  uint32_t irqStatusFlag = LETIMER_IntGet(LETIMER0);

  //Clear Pending Interrupts.
  LETIMER_IntClear(LETIMER0, irqStatusFlag);
  NVIC_ClearPendingIRQ(LETIMER0_IRQn);

  if(irqStatusFlag&LETIMER_IF_UF){
      schedulerSetEventLETUF();
  }
  if(irqStatusFlag&LETIMER_IF_COMP1){
      LETIMER_IntDisable(LETIMER0, LETIMER_IEN_COMP1);
      schedulerSetEventLETComp1();
  }

}
void I2C0_IRQHandler(){
  I2C_TransferReturn_TypeDef transferStatus;
  transferStatus = I2C_Transfer(I2C0);
  if(transferStatus == i2cTransferDone){
      schedulerSetEventI2CTRXSuccess();
  }
  else if(transferStatus != i2cTransferInProgress){
      schedulerSetEventI2CTRXError();
  }
}
