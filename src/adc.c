/*
 * adc.c
 *
 *  Created on: Apr 19, 2024
 *      Author: Parth
 */

#include <em_adc.h>
#include "adc.h"
#include "lcd.h"
/**
 * PA0 is ADC0
 */
ADC_Init_TypeDef adc0_init = ADC_INIT_DEFAULT;
ADC_InitSingle_TypeDef adc0_init_single = ADC_INITSINGLE_DEFAULT;
void adcInit(){

  adc0_init_single.reference = adcRef5V;
  adc0_init_single.resolution = adcRes8Bit;
  ADC_Init(ADC0, &adc0_init);
  ADC0->CTRL |= ADC_CTRL_ADCCLKMODE_ASYNC;
  adc0_init_single.posSel = adcPosSelAPORT3XCH4;
  ADC_InitSingle(ADC0, &adc0_init_single);
  ADC_IntEnable(ADC0, ADC_IF_SINGLE);
}

void readJoystick(){
  uint8_t adc0_val;

  adc0_init_single.posSel = adcPosSelAPORT3XCH4;
  ADC_InitSingle(ADC0, &adc0_init_single);
  ADC_Start(ADC0, adcStartSingle);
  while(! (ADC0->STATUS & ADC_STATUS_SINGLEDV));
  adc0_val = ADC_DataSingleGet(ADC0);
  displayPrintf(DISPLAY_ROW_9, "HOR is : %u ",adc0_val);

  adc0_init_single.posSel = adcPosSelAPORT3XCH2;
  ADC_InitSingle(ADC0, &adc0_init_single);
  ADC_Start(ADC0, adcStartSingle);
  while(! (ADC0->STATUS & ADC_STATUS_SINGLEDV));
  adc0_val = ADC_DataSingleGet(ADC0);
  displayPrintf(DISPLAY_ROW_JOYSTICK, "VER is : %u ",adc0_val);

}
