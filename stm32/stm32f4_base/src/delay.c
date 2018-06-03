#include <stdint.h>
#include "delay.h"

static uint32_t systick_delay_counter;

void delayMicroseconds(uint32_t microseconds)
{
	systick_delay_counter = microseconds;

	while(systick_delay_counter > 0);
}

void delay(uint32_t miliseconds)
{
	uint32_t i = 0;
	for (; i < miliseconds; i++)
	{
		delayMicroseconds(1000);
	}
}

void SysTick_Handler(void)
{
	if (systick_delay_counter != 0)
	{
		systick_delay_counter--;
	}
}
