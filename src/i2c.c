/*
 * i2c.h
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
* -  si_i2cspm.h used for oscillator initialiszaion
*
*   @copyright All rights reserved. Distribution allowed only for the
*   use of assignment grading. Use of code excerpts allowed at the
*   discretion of author. Contact for permission.
 */
#define INCLUDE_LOG_DEBUG 1
#include "i2c.h"
#include "sl_i2cspm.h"
#include "src/log.h"
#include "src/timer.h"
I2C_TransferReturn_TypeDef i2cStatus;
I2C_TransferSeq_TypeDef i2cSeq;
uint8_t cmd;
uint8_t data[2];
#define SI7021_DEVICE_ADDR  0x40
#define SI7021_TEMP_CMD     0xF3
void I2C0init(){

  I2CSPM_Init_TypeDef i2cInit;

  i2cInit.port = I2C0; // Points to the I2C0 base address


  i2cInit.i2cClhr = i2cClockHLRStandard; // Equal duty cycle for clock high and low phases
  i2cInit.i2cMaxFreq = I2C_FREQ_STANDARD_MAX; // Standard frequency defined at 92KHz.
  i2cInit.i2cRefFreq = 0; // this causes the emlib function to fetch the i2c clock frequency.

  i2cInit.sclPort = gpioPortC;
  i2cInit.sclPin =  10;

  i2cInit.sclPort = gpioPortC;
  i2cInit.sclPin =  11;

  i2cInit.portLocationScl = 14;
  i2cInit.portLocationSda = 16;

  I2CSPM_Init(&i2cInit); // Initialise the i2c module using the parameters specified above

}

void si7021_readTempData(){
  i2cSeq.addr = (SI7021_DEVICE_ADDR<<1); // Pass the slave address shifted by one bit. Last bit is R/W set by SPM_Transfer.
  i2cSeq.flags = I2C_FLAG_READ; // Perform a read operations
  i2cSeq.buf[0].data = data; // Store result into data.
  i2cSeq.buf[0].len = sizeof(data); // Data is an array of size 2.
  i2cStatus = I2CSPM_Transfer(I2C0, &i2cSeq); // Begin the transefer

  if(i2cStatus != i2cTransferDone){
      LOG_ERROR("Temp read from sensor failed with i2c error code : %d\r\n",i2cStatus); // Print error if transfer not successful.
      return;
  }
  uint16_t rawTempVal = ((data[0]<<8)|(data[1]));
  int temp = (int)((175.72 * rawTempVal)/65536 - 46.85);

  LOG_INFO("Temperature Read Successful. Temp in C : %d\r\n",temp);
}
void si7021_sendTempCmd(){
  cmd = SI7021_TEMP_CMD;
  i2cSeq.addr = SI7021_DEVICE_ADDR<<1;
  i2cSeq.flags = I2C_FLAG_WRITE;
  i2cSeq.buf[0].data = &cmd;
  i2cSeq.buf[0].len = sizeof(cmd);
  i2cStatus = I2CSPM_Transfer(I2C0, &i2cSeq);

  if(i2cStatus != i2cTransferDone){
      LOG_ERROR("Temp Command to sensor failed with i2c error code : %d\r\n",i2cStatus);
      return;
  }
}

void si7021_power(bool on){
  if(on){
      GPIO_PinOutSet(gpioPortD, 15);
      timerWaitUs((80)*(1000U)); // Wait for the SI7021 to power on.
  }
  else{
      GPIO_PinOutClear(gpioPortD, 15); // Turn off power to SI7021
  }
}
void si7021_getData(){
  si7021_power(true);
  si7021_sendTempCmd();
  timerWaitUs((11)*(1000U));
  si7021_readTempData();
  si7021_power(false);
}
