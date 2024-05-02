/*
  gpio.c
 
   Created on: Dec 12, 2018
       Author: Dan Walkes
   Updated by Dave Sluiter Dec 31, 2020. Minor edits with #defines.

   March 17
   Dave Sluiter: Use this file to define functions that set up or control GPIOs.
   
   Jan 24, 2023
   Dave Sluiter: Cleaned up gpioInit() to make it less confusing for students regarding
                 drive strength setting. 

 *
 * Student edit: Add your name and email address here:
 * @student    Parth Kharade, Parth.Kharade@Colorado.edu
 *
 
 */


// *****************************************************************************
// Students:
// We will be creating additional functions that configure and manipulate GPIOs.
// For any new GPIO function you create, place that function in this file.
// *****************************************************************************

#include <stdbool.h>
#include "em_gpio.h"
#include <string.h>

#include "gpio.h"


// Student Edit: Define these, 0's are placeholder values.
//
// See the radio board user guide at https://www.silabs.com/documents/login/user-guides/ug279-brd4104a-user-guide.pdf
// and GPIO documentation at https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/group__GPIO.html
// to determine the correct values for these.
// If these links have gone bad, consult the reference manual and/or the datasheet for the MCU.
// Change to correct port and pins:
#define LED_port   (gpioPortF)
#define LED0_pin   4
#define LED1_pin   5




// Set GPIO drive strengths and modes of operation
void gpioInit()
{
    // Student Edit:
   // Set sensor enable pin as a push pull output and low at start.
    GPIO_PinModeSet(gpioPortD, 15, gpioModePushPull, 0);
    GPIO_PinModeSet(gpioPortD, 12, gpioModeDisabled, 0); //set PC6 to DISABLE with no pull resistors
    GPIO->P[gpioPortD].OVTDIS |= (0x01 << 12); //Disable over-voltage tolerance for PC6
    GPIO_PinModeSet(gpioPortD, 10, gpioModeDisabled, 0); //set PC6 to DISABLE with no pull resistors
    GPIO->P[gpioPortD].OVTDIS |= (0x01 << 10); //Disable over-voltage tolerance for PC6
} // gpioInit()

void si7021_power(bool on){
  if(on){
      GPIO_PinOutSet(gpioPortD, 15);
  }
  else{
      GPIO_PinOutClear(gpioPortD, 15); // Turn off power to SI7021
  }
}
/*
 * Function to toggle the common input pin on the LCD display.
 * */
void gpioSetDisplayExtcomin(bool extPinState){
    if(extPinState){
        GPIO_PinOutSet(gpioPortD, 13);
    }
    else{
        GPIO_PinOutSet(gpioPortD, 13);
    }
}
#ifdef UNIT_TEST_TIMER
void gpioLed0SetOn()
{
  GPIO_PinOutSet(LED_port, LED0_pin);
}


void gpioLed0SetOff()
{
  GPIO_PinOutClear(LED_port, LED0_pin);
}


void gpioLed1SetOn()
{
  GPIO_PinOutSet(LED_port, LED1_pin);
}


void gpioLed1SetOff()
{
  GPIO_PinOutClear(LED_port, LED1_pin);
}
#endif



