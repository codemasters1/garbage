#include "stm32/i2c.h"
#include "stm32/gpio/gpio.h"
#include "i2c_master.h"

void I2CMaster_init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	GPIO_setMode(GPIOB, 10, GPIO_MODE_AF); // scl
	GPIO_setAF(GPIOB, 10, GPIO_AF_I2C2);
	GPIO_setOD(GPIOB, 10);

	GPIO_setMode(GPIOB, 11, GPIO_MODE_AF); // sda
	GPIO_setAF(GPIOB, 11, GPIO_AF_I2C2);
	GPIO_setOD(GPIOB, 11);

	RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
	I2C2->CR1 |= I2C_CR1_SWRST;
	I2C2->CR1 = 0;
	I2C2->CR2 |= 42; // 42MHz
	I2C2->CCR = 210;
	I2C2->TRISE = 43;
	I2C2->CR1 |= I2C_CR1_ACK | I2C_CR1_PE;
}

void I2CMaster_writeBuffer(uint8_t address, uint8_t * buffer, uint8_t length)
{
	uint32_t dummy;

	I2C2->CR1 |= I2C_CR1_START;
	while (!(I2C2->SR1 & I2C_SR1_SB));

	I2C2->DR = (address << 1) | I2C_WRITE;
	while (!(I2C2->SR1 & I2C_SR1_ADDR));

	dummy = I2C2->SR2;

	while (length--)
	{
		while (!(I2C2->SR1 & I2C_SR1_TXE));
		I2C2->DR = *(buffer++);

		while (!(I2C2->SR1 & I2C_SR1_TXE));
		while   (I2C2->SR1 & I2C_SR1_BTF);
	}

	I2C2->CR1 |= I2C_CR1_STOP;
}

void I2CMaster_readBuffer(uint8_t address, uint8_t * buffer, uint8_t length)
{
	uint32_t dummy;

	I2C2->CR1 |= I2C_CR1_START;
	while (!(I2C2->SR1 & I2C_SR1_SB));

	I2C2->DR = (address << 1) | I2C_READ;
	while (!(I2C2->SR1 & I2C_SR1_ADDR));

	dummy = I2C2->SR2;

	while (length--)
	{
		if (length == 0)
			I2C2->CR1 &= ~I2C_CR1_ACK;

		while(!(I2C2->SR1 & I2C_SR1_RXNE));
		*(buffer++) = I2C2->DR;
	}

	I2C2->CR1 |= I2C_CR1_ACK | I2C_CR1_STOP;
}

void I2CMaster_write(uint8_t address, uint8_t byte)
{
	I2CMaster_writeBuffer(address, &byte, 1);
}

uint8_t I2CMaster_read(uint8_t address)
{
	uint8_t byte;
	I2CMaster_readBuffer(address, &byte, 1);
	return byte;
}
