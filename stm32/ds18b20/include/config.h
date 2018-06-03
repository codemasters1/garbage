#ifndef __CONFIG_H_
#define __CONFIG_H_

#include "include/stm32f4xx.h"
#include "hdr/hdr_gpio.h"


#define CRYSTAL		8000000UL
#define FREQUENCY	168000000UL

void flash_latency(uint32_t frequency);
void fpu_enable(void);
uint32_t pll_start(uint32_t crystal, uint32_t frequency);

#endif
