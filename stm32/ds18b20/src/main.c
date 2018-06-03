#include <stdint.h>
#include <newlib.h>
#include "include/config.h"
#include "include/stm32f4xx.h"
#include "include/gpio.h"
#include "include/delay.h"
#include "include/serial.h"
#include "include/onewire.h"
#include "include/ds18b20.h"

static OWBus owbus = {GPIOE, 2};

//int strlen(char*);

//char line_buffer[512];
//static char erase_seq[] = "\b \b";
//static char tab_seq[] = "        ";

char itoa( int value, char* result, int base )
{
	// check that the base if valid
	if (base < 2 || base > 36)
	{
		*result = '\0';
		return 0;
	}

	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do
	{
		tmp_value = value;
		value /= base;
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
	} while ( value );

	// Apply negative sign
	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1 < ptr)
	{
		tmp_char = *ptr;
		*ptr--= *ptr1;
		*ptr1++ = tmp_char;
	}
	return 1;
}


void putint(int x, int base) {
	char buff[5];

	itoa(x, buff, base);

	serial_puts(buff);
}
/*
int readline(const char *prompt) {
	char *p = line_buffer;
	int plen = 0;
	int col = 0;

	if (prompt)
	{
		plen = strlen((char*) prompt);
		serial_puts((char*) prompt);
	}

	col = plen;

	for (;;)
	{
		char c = serial_getc();

		//char buff[3];
		//itoa(c, buff, 16);
		//serial_puts(buff);

		switch(c)
		{
		case '\r':
		case '\n':
			*p = '\0';
			serial_puts("\r\n");

			return (line_buffer - p);
		case 0x03:
			line_buffer[0] = 0;
			return -1;

		case 0x08:
		case 0x7F:
			if (col > plen)
			{
				serial_puts(erase_seq);
				col--;
				p--;
			}

			continue;
		default:
			*p++ = c;
			col++;
			serial_putc(c);
		}
	}

	return 0;
}

int strcmp(const char* s1, const char* s2)
{
    while(*s1 && (*s1 == *s2))
        s1++,s2++;
    return *(const unsigned char*) s1 - *(const unsigned char*) s2;
}

int strlen(char* s)
{
	int i = 0;
	for( ; *s; s++, i++);
	return i;
}

void memset(char *s, char c, int len)
{
	for (; len; len--, s++)
		*s = c;
}*/

void HardFault_Handler(void)
{

}



void PWM_init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // w³¹czenie zegara GPIOA
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN; // w³¹czenie zegara TIM1

	GPIOA->MODER |= GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1; // funkcje alternatywne dla PA8, PA9
	GPIOA->AFR[1] = (1 << 0) | (1 << 4); // AF1 dla PA8, PA9

	//gpio_pin_cfg(GPIOA, 1, GPIO_AFRx_AF1_value);

	TIM1->PSC = 4200; // preskaler 1
	TIM1->ARR = 100; // wartosc dla ktorej nastompi zmiana
	TIM1->CCMR1 = TIM_CCMR1_OC1M_1 | // PWM mode 1 (110), kana³ 1
				  TIM_CCMR1_OC1M_2 | //
				  TIM_CCMR1_OC1PE  |

				  TIM_CCMR1_OC2M_1 | // mode 1 dla kana³u 2
				  TIM_CCMR1_OC2M_2 |
				  TIM_CCMR1_OC2PE;

	TIM1->CCER  |= TIM_CCER_CC1E | TIM_CCER_CC2E; // 1 i 2 kana³
	TIM1->EGR 	|= TIM_EGR_UG; // update generation
	TIM1->BDTR  |= TIM_BDTR_MOE; // time-break, dead-time
	TIM1->CR1   |= TIM_CR1_CEN; // w³¹czenie timera

	TIM1->CCR1 = 80;
	TIM2->CCR2 = 80;
}


int main(void)
{
	fpu_enable();
	uint32_t pll_speed = pll_start(CRYSTAL, FREQUENCY);
	delay_config(pll_speed);
	flash_latency(pll_speed);

	gpio_init();
	gpio_pin_cfg(GPIOD, 12, GPIO_OUT_PP_2MHz);
	gpio_pin_cfg(GPIOD, 13, GPIO_OUT_PP_2MHz);
	gpio_pin_cfg(GPIOD, 14, GPIO_OUT_PP_2MHz);
	gpio_pin_cfg(GPIOD, 15, GPIO_OUT_PP_2MHz);

	serial_init();

	serial_puts("System started with PLL_SPEED = ");
	putint(pll_speed, 10);
	serial_puts("\r\n");

	uint8_t data[9];
	uint8_t i;



	while(1)
 	{

		char c;
		if ((c = serial_getc()))
		{
			serial_putc(c);
			serial_putc('=');
			putint(c, 16);
			serial_puts("\r\n");
		}

		if (OW_Init(&owbus))
		{
			OW_WriteByte(&owbus, DS18B20_SKIP_ROM);
			OW_WriteByte(&owbus, DS18B20_CONVERT_T);

			OW_Set(&owbus);
			OW_Delay(&owbus, 750000);


			if (! OW_Init(&owbus))
			{
				serial_puts("Error: could not read from sensor\r\n");
			}

			OW_WriteByte(&owbus, DS18B20_SKIP_ROM);
			OW_WriteByte(&owbus, DS18B20_READ_SCRATCHPAD);

			serial_puts("0x");

			for(i = 0; i < 9; i++)
			{
				data[i] = OW_ReadByte(&owbus);
				putint(data[i], 16);
			}

			serial_puts("\r\n");


			float temp = (float)((data[1] << 8) | data[0]) / 16;

			char buf[3] = {0};
			itoa(temp, buf, 10);

			serial_puts("Current temperature: ");
			serial_puts(buf);
			serial_putc('.');
			serial_putc('0' + ((int)(temp * 10) % 10));
			serial_puts("*C\r\n");
		}
		else
		{
			serial_puts("Error: No sensor connected\r\n");
			delay_ms(1000);
		}


		/*int len = readline("~> ");

		if (len <= 0)
		{
			continue;
		}

		if (strcmp("led", line_buffer) == 0)
		{
			serial_puts("led toggled\r\n");
			GPIOD->ODR ^= (1 << 13);
		}
		else if(strcmp("reset", line_buffer) == 0)
		{
			serial_puts("Device will be restarted NOW!\r\n");
			delay_ms(1000);
			NVIC_SystemReset();
		}
		else
		{
			serial_puts("Unrecognized command\r\n");
		}
		*/
 	}

	return 0;
}
