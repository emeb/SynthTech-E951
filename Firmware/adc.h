/*
 * adc.h - f030 breakout ADC setup
 */

#ifndef __adc__
#define __adc__

#include "stm32f0xx.h"

void adc_init(void);
uint16_t adc_get_data(uint8_t index);

#endif
