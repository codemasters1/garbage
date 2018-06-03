#ifndef __CONFIG_H_
#define __CONFIG_H_

#include <stm32f4xx.h>

#define CRYSTAL		8000000UL
#define FREQUENCY	168000000UL

void Flash_SetLatency(uint32_t frequency);
void FPU_Enable(void);
uint32_t PLL_Start(uint32_t crystal, uint32_t frequency);

#endif
