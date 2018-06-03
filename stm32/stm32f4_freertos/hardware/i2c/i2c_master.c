#include "hardware/bb/i2c.h"
#include "hardware/gpio/gpio_f4.h"

void i2c_master_init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	gpio_mode_config(GPIOB, 10, GPIO_MODE_AF); // scl
	gpio_af_config(GPIOB, 10, GPIO_AF_I2C2);

	gpio_mode_config(GPIOB, 11, GPIO_MODE_AF); // sda
	gpio_af_config(GPIOB, 11, GPIO_AF_I2C2);

	RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
	I2C2->CR1 |= I2C_CR1_SWRST;
	I2C2->CR1 = 0;
	I2C2->CR2 |= 42;
	I2C2->CCR = 210;
	I2C2->TRISE = 43;
	I2C2->CR1 |= I2C_CR1_ACK | I2C_CR1_PE;
}

void i2c_master_write(uint8_t address, uint8_t * data, uint8_t length)
{
	uint32_t dummy;

	I2C2->CR1 |= I2C_CR1_START;
	while (!(I2C2->SR1 & I2C_SR1_SB));

	I2C2->DR = address << 1;
	while (!(I2C2->SR1 & I2C_SR1_ADDR));

	dummy = I2C2->SR2;

	while (length--)
	{
		while (!(I2C2->SR1 & I2C_SR1_TXE));
		I2C2->DR = *(data++);

		while (!(I2C2->SR1 & I2C_SR1_TXE));
		while   (I2C2->SR1 & I2C_SR1_BTF);
	}

	I2C2->CR1 |= I2C_CR1_STOP;
}

void i2c_master_read(uint8_t address, uint8_t * data, uint8_t length)
{
	uint32_t dummy;

	I2C2->CR1 |= I2C_CR1_START;
	while (!(I2C2->SR1 & I2C_SR1_SB));

	I2C2->DR = (address << 1) | 0x01;
	while (!(I2C2->SR1 & I2C_SR1_ADDR));

	dummy = I2C2->SR2;

	while (length--)
	{
		if (length == 0)
			I2C2->CR1 &= ~I2C_CR1_ACK;

		while(!(I2C2->SR1 & I2C_SR1_RXNE));
		*(data++) = I2C2->DR;
	}

	I2C2->CR1 |= I2C_CR1_ACK | I2C_CR1_STOP;
}
