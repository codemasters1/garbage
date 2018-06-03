#include <stdint.h>
#include "inc/stm32f10x.h"
#include "config.h"
#include "hdr/hdr_rcc.h"
#include "hdr/hdr_gpio.h"
#include "gpio.h"

static void flash_latency(uint32_t frequency);
static uint32_t pll_start(uint32_t crystal, uint32_t frequency);
static void system_init(void);


int main(void)
{
	volatile uint32_t count, count_max = 1000000;

	system_init();
	pll_start(CRYSTAL, FREQUENCY);

	gpio_pin_cfg(LED_GPIO, LED_pin, GPIO_CRx_MODE_CNF_OUT_PP_10M_value);

	while (1)
	{
		for (count = 0; count < count_max; count++);	// delay
		LED_bb = 1;
		for (count = 0; count < count_max; count++);	// delay
		LED_bb = 0;
	}
}

static void flash_latency(uint32_t frequency)
{
	uint32_t wait_states;

	if (frequency < 24000000UL)				// 0 wait states for core speed below 24MHz
		wait_states = 0;
	else if (frequency < 48000000UL)		// 1 wait state for core speed between 24MHz and 48MHz
		wait_states = 1;
	else									// 2 wait states for core speed over 48MHz
		wait_states = 2;

	FLASH->ACR |= wait_states;				// set the latency
}

static uint32_t pll_start(uint32_t crystal, uint32_t frequency)
{
	uint32_t mul;

	RCC_CR_HSEON_bb = 1;					// enable HSE clock
	flash_latency(frequency);				// configure Flash latency for desired frequency

	mul = frequency / crystal;				// PLL multiplier calculation

	if (mul > 16)							// max PLL multiplier is 16
		mul = 16;

	frequency = crystal * mul;

	RCC->CFGR |= ((mul - 2) << RCC_CFGR_PLLMUL_bit) | RCC_CFGR_PLLSRC | RCC_CFGR_PPRE1_DIV2;	// configuration of PLL: HSE x (mul), APB1 clk = /2

	while (!RCC_CR_HSERDY_bb);				// wait for stable clock

	RCC_CR_PLLON_bb = 1;					// enable PLL
	while (!RCC_CR_PLLRDY_bb);				// wait for PLL lock

	RCC->CFGR |= RCC_CFGR_SW_PLL;			// change SYSCLK to PLL
	while (((RCC->CFGR) & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);	// wait for switch

	return frequency;
}

static void system_init(void)
{
	gpio_init();
}
