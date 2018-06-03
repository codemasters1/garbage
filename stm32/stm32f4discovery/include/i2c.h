#ifndef INCLUDE_I2C_H_
#define INCLUDE_I2C_H_

#include <stm32f4xx.h>
#include "include/gpio.h"

void I2C_Init(void);
void I2C_Write(uint8_t, uint8_t *, uint8_t);
void I2C_Read(uint8_t, uint8_t *, uint8_t);

#endif /* INCLUDE_I2C_H_ */
