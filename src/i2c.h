/*
 * i2c.h
 *
 *  Created on: Feb 5, 2024
 *      Author: Parth
 */

#ifndef SRC_I2C_H_
#define SRC_I2C_H_

void I2C0init();

void si7021_readTempData();
void si7021_sendTempCmd();
void si7021_turnON();
void si7021_turnOFF();
void si7021_getData();

#endif /* SRC_I2C_H_ */
