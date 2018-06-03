#include "delay.h"

void delay_us(__IO uint32_t us)
{
	delay_counter = us;

	while(delay_counter > 0);
}

void delay_ms(uint32_t ms)
{
	uint32_t i = 0;
	for (; i < ms; i++)
	{
		delay_us(1000);
	}
}

void Delay_Config(uint32_t f)
{
	SysTick_Config(f / 1000000);
}

void SysTick_Handler(void)
{
	if (delay_counter != 0)
	{
		delay_counter--;
	}
}
