#ifndef CONFIG_H_
#define CONFIG_H_

#include "hdr/hdr_bitband.h"

#define CRYSTAL								8000000ul	///< quartz crystal resonator which is connected to the chip
#define FREQUENCY							72000000ul	///< desired target frequency of the core

#define LED_GPIO							GPIOB		///< GPIO port to which the LED is connected
#define LED_pin								1			///< pin number of the LED

#define LED									(1 << LED_pin)
#define LED_ODR								LED_GPIO->ODR	///< output register for the LED
#define LED_bb								bitband_t m_BITBAND_PERIPH(&LED_ODR, LED_pin)	///< bit-band "variable" to directly handle the pin

#endif /* CONFIG_H_ */
