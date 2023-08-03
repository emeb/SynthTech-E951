/*
 * systick.h - systick routines
 */

#ifndef __systick__
#define __systick__

#include "stm32f0xx.h"

void systick_init(void);
uint32_t systick_goal_ms(uint32_t ms);
uint32_t systick_check(uint32_t goal);
void systick_delayms(uint32_t ms);
uint8_t systick_get_button(uint8_t num);
uint8_t systick_get_trigger(uint8_t num);
uint8_t systick_get_switch(void);

#endif
