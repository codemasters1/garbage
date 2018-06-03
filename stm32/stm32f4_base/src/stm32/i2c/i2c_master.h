#ifndef _I2C_MASTER_H
#define _I2C_MASTER_H

#include <stdint.h>

#define I2C_WRITE (0)
#define I2C_READ  (1)

void I2CMaster_init(void);
void I2CMaster_readBuffer(uint8_t address, uint8_t * data, uint8_t length);
void I2CMaster_writeBuffer(uint8_t address, uint8_t * data, uint8_t length);
uint8_t I2CMaster_read(uint8_t address);
void I2CMaster_write(uint8_t address, uint8_t byte);

#endif
