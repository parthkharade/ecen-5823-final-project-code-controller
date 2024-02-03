/*
 * oscillator.h
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

#ifndef SRC_OSCILLATOR_H_
#define SRC_OSCILLATOR_H_

/**
 * Initializes the correct oscillator according to lowest energy mode.
 */
void oscInit();

#endif /* SRC_OSCILLATOR_H_ */
