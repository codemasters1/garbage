#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f4xx.h"

static __IO uint32_t delay_counter;

void delay_us(uint32_t);
void delay_ms(uint32_t);
void delay_config(uint32_t);

#endif
