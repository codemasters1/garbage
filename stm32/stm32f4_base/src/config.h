#ifndef PLATFORM_SPECIFIC_H_
#define PLATFORM_SPECIFIC_H_

#include <stdbool.h>
#include "stm32/cmsis/stm32f4xx.h"

/** @note Definicje dotyczace predkosci taktowania. @{ */
#define HSE_FREQ              8000000
#define MCU_CLOCK_FREQ        168000000UL
#define AHB_CLOCK_FREQ        MCU_CLOCK_FREQ
#define APB2_CLOCK_FREQ       MCU_CLOCK_FREQ / 2
#define APB1_CLOCK_FREQ       MCU_CLOCK_FREQ / 4
/** @} */

#endif /* PLATFORM_SPECIFIC_H_ */