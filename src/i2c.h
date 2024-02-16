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



#ifndef SRC_I2C_H_
#define SRC_I2C_H_

#define SI7021_ON_DELAY_US    ((80)*(1000))

#define SI7021_READ_DELAY_US  ((10800))
#include "stdbool.h"
void I2C0init();

/**
 * Initiate transfer to read data from SI7021
 * */
void si7021_readTempData();

/**
 * Send command to initiate temperature measurement.
 * */
void si7021_sendTempCmd();

/**
 * Turn the power to SI7021 on or off
 * */
void si7021_power(bool on);

/**
 * Call cmd and read data functions to obtain data from the SI7021 temperature sensor.
 */
void si7021_getData();

#endif /* SRC_I2C_H_ */
