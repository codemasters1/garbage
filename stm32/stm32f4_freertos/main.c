#include "hardware/core/core_init.h"
#include "hardware/usart/usart.h"
#include "hardware/bb/common.h"
#include "hardware/gpio/gpio_f4.h"
#include "hardware/bb/spi.h"

#define RST_bb bitband_t BITBAND_PERIPH(&GPIOE->ODR, 9)
#define CS_bb bitband_t BITBAND_PERIPH(&GPIOE->ODR, 8)
#define DC_bb bitband_t BITBAND_PERIPH(&GPIOE->ODR, 7)

uint8_t LCD_WriteByte(uint8_t byte)
{
	SPI1->DR = byte;
	while(SPI1_SR_TXE_bb == 0);
	return SPI1->DR;
}

void LCD_Write_COM(uint8_t com)
{
	CS_bb = 0;
	DC_bb = 0;
	LCD_WriteByte(com);
	CS_bb = 1;
}

uint8_t LCD_Write_DATA(uint8_t data)
{
	uint8_t temp;
	CS_bb = 0;
	DC_bb = 1;
	temp = LCD_WriteByte(data);
	CS_bb = 1;
	return temp;
}


void spi_init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOEEN;

	gpio_mode_config(GPIOA, 5, GPIO_MODE_AF); // sck
	gpio_af_config(GPIOA, 5, GPIO_AF_SPI1);
	gpio_speed_config(GPIOA, 5, GPIO_SPEED_HIGH);

	gpio_mode_config(GPIOA, 6, GPIO_MODE_AF); // miso
	gpio_af_config(GPIOA, 6, GPIO_AF_SPI1);
	gpio_speed_config(GPIOA, 6, GPIO_SPEED_HIGH);


	gpio_mode_config(GPIOA, 7, GPIO_MODE_AF); // mosi
	gpio_af_config(GPIOA, 7, GPIO_AF_SPI1);
	gpio_speed_config(GPIOA, 7, GPIO_SPEED_HIGH);


	gpio_mode_config(GPIOE, 7, GPIO_MODE_OUTPUT); // dc
	gpio_speed_config(GPIOE, 7, GPIO_SPEED_HIGH);

	gpio_mode_config(GPIOE, 8, GPIO_MODE_OUTPUT); // cs
	gpio_speed_config(GPIOE, 8, GPIO_SPEED_HIGH);

	gpio_mode_config(GPIOE, 9, GPIO_MODE_OUTPUT); // rst
	gpio_speed_config(GPIOE, 9, GPIO_SPEED_HIGH);

	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	SPI1->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR | SPI_CR1_SPE | SPI_CR1_BR_2;
}

void lcd_task(void * params)
{
	//portENTER_CRITICAL();

//	LCD_Write_COM(0x01);
//	LCD_Write_COM(0x28);
//
//	LCD_Write_COM(0xCF);
//	LCD_Write_DATA(0x00);
//	LCD_Write_DATA(0x83);
//	LCD_Write_DATA(0x30);

	LCD_Write_COM(0xEF);
	LCD_Write_DATA(0x03);
	LCD_Write_DATA(0x80);
	LCD_Write_DATA(0x02);

	LCD_Write_COM(0xCF);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0XC1);
	LCD_Write_DATA(0X30);

	LCD_Write_COM(0xED);
	LCD_Write_DATA(0x64);
	LCD_Write_DATA(0x03);
	LCD_Write_DATA(0X12);
	LCD_Write_DATA(0X81);

	LCD_Write_COM(0xE8);
	LCD_Write_DATA(0x85);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x78);

	LCD_Write_COM(0xCB);
	LCD_Write_DATA(0x39);
	LCD_Write_DATA(0x2C);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x34);
	LCD_Write_DATA(0x02);

	LCD_Write_COM(0xF7);
	LCD_Write_DATA(0x20);

	LCD_Write_COM(0xEA);
	LCD_Write_DATA(0x00);
	LCD_Write_DATA(0x00);

	//portEXIT_CRITICAL();

	while(1){}
}

int main(void)
{
   	core_init();
   	usart_init();
   	spi_init();

   	xTaskCreate(lcd_task, "lcd", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
   	vTaskStartScheduler();

	while(1);
}
