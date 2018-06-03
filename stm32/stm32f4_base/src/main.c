#include "stm32/rcc/rcc.h"
#include "stm32/gpio/gpio.h"
#include "stm32/usart/usart.h"

void led_config(void)
{
	RCC_AHB1ENR_GPIODEN_bb = 1;

	GPIO_setMode(GPIOD, 13, GPIO_MODE_OUTPUT);
	GPIO_setSpeed(GPIOD, 13, GPIO_SPEED_HIGH);
	GPIO_setPP(GPIOD, 13);
}

int main(void)
{
   	PLL_init();
   	USART_init();
   	I2CMaster_init();
   	MS5611_init();

   	SysTick_Config(168);

   	USART_sendBuffer("\r\nSystem is started!\r\n", 22);

   	led_config();

	while(1)
	{
		uint32_t pressure = MS5611_readPressure(false);

		uint8_t buff[10] = {0};
		itoa(pressure, buff, 10);
		USART_sendBuffer(buff, 10);
		USART_sendBuffer("\r\n", 2);

		delay(500);
		GPIOD->ODR ^= (1 << 13);
	}
}
