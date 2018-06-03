#ifndef _I2C_MASTER_H
#define _I2C_MASTER_H

#include <stdint.h>

#define I2C_WRITE (0)
#define I2C_READ  (1)

void i2c_master_init(void);
void i2c_master_read(uint8_t address, uint8_t * data, uint8_t length);
void i2c_master_write(uint8_t address, uint8_t * data, uint8_t length);

#endif
