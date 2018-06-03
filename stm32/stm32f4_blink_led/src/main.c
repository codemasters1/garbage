#include <stdint.h>
#include <stm32f4xx.h>
#include "include/config.h"
#include "include/gpio.h"
#include "include/serial.h"
#include "include/delay.h"

#define CS_1 GPIOE->ODR |= (1 << 3)
#define CS_0 GPIOE->ODR &= ~(1 << 3)

#define LCD_CS_0 GPIOC->ODR |= (1 << 5)
#define LCD_CS_1 GPIOC->ODR &= ~(1 << 5)

#define LCD_DC_0 GPIOC->ODR |= (1 << 4)
#define LCD_DC_1 GPIOC->ODR &= ~(1 << 4)

typedef struct {
	uint8_t x;
	uint8_t y;
	uint8_t z;
} acc_t;

void spi_init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	GPIO_ConfigPin(GPIOA, 5, GPIO_AF5_PP_50MHz); // sck
	GPIO_ConfigPin(GPIOA, 6, GPIO_AF5_PP_50MHz); // miso
	GPIO_ConfigPin(GPIOA, 7, GPIO_AF5_PP_50MHz); // mosi
	GPIO_ConfigPin(GPIOE, 3, GPIO_OUT_PP_50MHz); // cs
	CS_1;

	SPI1->CR1 |= SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_BR_1 | SPI_CR1_BR_0 | SPI_CR1_CPHA | SPI_CR1_CPOL;
	SPI1->CR1 |= SPI_CR1_SPE;
}

uint8_t spi_transfer(uint8_t dane)
{
	while(!(SPI1->SR & SPI_SR_TXE));
	SPI1->DR = dane;

	while(!(SPI1->SR & SPI_SR_RXNE));
	return SPI1->DR;
}




void mems_write(uint8_t rejestr, uint8_t wartosc)
{
	CS_0;
	spi_transfer(rejestr);
	spi_transfer(wartosc);
	CS_1;
}

uint8_t mems_read(uint8_t rejestr)
{
	uint8_t temp;
	CS_0;
	spi_transfer(rejestr | 0x80);
	temp = spi_transfer(0xFF);
	CS_1;
	return temp;
}

void mems_init(void)
{
    spi_init();
    mems_write(0x20, 0x47);
}

void mems_read_axis(acc_t *acc)
{
	acc->x = mems_read(0x29);
	acc->y = mems_read(0x2B);
    acc->z = mems_read(0x2D);
}

int itoa(int value, char *sp, int radix)
{
    char tmp[16];// be careful with the length of the buffer
    char *tp = tmp;
    int i;
    unsigned v;

    int sign = (radix == 10 && value < 0);
    if (sign)
        v = -value;
    else
        v = (unsigned)value;

    while (v || tp == tmp)
    {
        i = v % radix;
        v /= radix; // v/=radix uses less CPU clocks than v=v/radix does
        if (i < 10)
          *tp++ = i+'0';
        else
          *tp++ = i + 'a' - 10;
    }

    int len = tp - tmp;

    if (sign)
    {
        *sp++ = '-';
        len++;
    }

    while (tp > tmp)
        *sp++ = *--tp;

    return len;
}
/*
void LCD_SPI_Config(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	__DSB();
	SPI1->CR1 |= SPI_CR1_MSTR | SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE;

	GPIO_ConfigPin(GPIOA, 5, GPIO_AF5_PP_50MHz); // sck
	GPIO_ConfigPin(GPIOA, 6, GPIO_AF5_PP_50MHz); // miso
	GPIO_ConfigPin(GPIOA, 7, GPIO_AF5_PP_50MHz); // mosi

	GPIO_ConfigPin(GPIOC, 4, GPIO_OUT_PP_50MHz); // dc
	GPIO_ConfigPin(GPIOC, 5, GPIO_OUT_PP_50MHz); // cs


}

uint8_t LCD_WriteByte(uint8_t byte)
{
	SPI1->DR = byte;
	while(!(SPI1->SR & SPI_SR_TXE));
	return SPI1->DR;
}

void LCD_Write_COM(uint8_t com)
{
	LCD_CS_0;
	LCD_DC_0;
	spi_transfer(com);
}

uint8_t LCD_Write_DATA(uint8_t data)
{
	uint8_t temp;
	LCD_CS_0;
	LCD_DC_1;
	temp=LCD_WriteByte(data);
	return temp;
}

void LCD_WriteCommand(uint8_t command)
{
	LCD_CS_0;
	LCD_DC_0;
	spi_transfer(command);
	LCD_CS_1;
}

uint8_t LCD_WriteData(uint8_t data)
{
	uint8_t tmp;
	LCD_CS_0;
	LCD_DC_1;
	tmp = spi_transfer(data);
	LCD_CS_1;
	return tmp;
}

uint16_t LCD_WriteData16(uint16_t data)
{
	uint8_t tmp;
	LCD_CS_0;
	LCD_DC_1;
	tmp = spi_transfer(data >> 8) << 8;
	tmp |= spi_transfer(data & 0xFF);
	return tmp;
}

void LCD_SetCursor( uint16_t Xpos, uint16_t Ypos )
{
	LCD_WriteCommand(0x2A);
	LCD_WriteData16(Xpos);
	LCD_WriteData16(Xpos);
	LCD_WriteCommand(0x2B);
	LCD_WriteData16(Ypos);
	LCD_WriteData16(Ypos);
}

void LCD_SetPixel(uint16_t Xpos, uint16_t Ypos, uint16_t colour)
{
	LCD_WriteCommand(0x2A);
	LCD_WriteData16(Xpos);
	LCD_WriteData16(Xpos);
	LCD_WriteCommand(0x2B);
	LCD_WriteData16(Ypos);
	LCD_WriteData16(Ypos);
	LCD_WriteCommand(0x2c);
	LCD_WriteData16(colour);
}

void LCD_Init(void)
{
	LCD_WriteCommand(0xCB);
	LCD_WriteData(0x39);
	LCD_WriteData(0x2C);
	LCD_WriteData(0x00);
	LCD_WriteData(0x34);
	LCD_WriteData(0x02);

	LCD_WriteCommand(0xCF);
	LCD_WriteData(0x00);
	LCD_WriteData(0XC1);
	LCD_WriteData(0X30);

	LCD_WriteCommand(0xE8);
	LCD_WriteData(0x85);
	LCD_WriteData(0x00);
	LCD_WriteData(0x78);

	LCD_WriteCommand(0xEA);
	LCD_WriteData(0x00);
	LCD_WriteData(0x00);

	LCD_WriteCommand(0xED);
	LCD_WriteData(0x64);
	LCD_WriteData(0x03);
	LCD_WriteData(0X12);
	LCD_WriteData(0X81);

	LCD_WriteCommand(0xF7);
	LCD_WriteData(0x20);

	LCD_WriteCommand(0xC0);    	//Power control
	LCD_WriteData(0x23);   	//VRH[5:0]

	LCD_WriteCommand(0xC1);    	//Power control
	LCD_WriteData(0x10);   	//SAP[2:0];BT[3:0]

	LCD_WriteCommand(0xC5);    	//VCM control
	LCD_WriteData(0x3e);   	//Contrast
	LCD_WriteData(0x28);

	LCD_WriteCommand(0xC7);    	//VCM control2
	LCD_WriteData(0x86);  	 //--

	LCD_WriteCommand(0x36);    	// Memory Access Control
	LCD_WriteData(0x48);  	//C8	   //48

	LCD_WriteCommand(0x3A);
	LCD_WriteData(0x55);

	LCD_WriteCommand(0xB1);
	LCD_WriteData(0x00);
	LCD_WriteData(0x18);

	LCD_WriteCommand(0xB6);    	// Display Function Control
	LCD_WriteData(0x08);
	LCD_WriteData(0x82);
	LCD_WriteData(0x27);

	LCD_WriteCommand(0xF2);    	// 3Gamma Function Disable
	LCD_WriteData(0x00);

	LCD_WriteCommand(0x26);    	//Gamma curve selected
	LCD_WriteData(0x01);

	LCD_WriteCommand(0xE0);    	//Set Gamma
	LCD_WriteData(0x0F);
	LCD_WriteData(0x31);
	LCD_WriteData(0x2B);
	LCD_WriteData(0x0C);
	LCD_WriteData(0x0E);
	LCD_WriteData(0x08);
	LCD_WriteData(0x4E);
	LCD_WriteData(0xF1);
	LCD_WriteData(0x37);
	LCD_WriteData(0x07);
	LCD_WriteData(0x10);
	LCD_WriteData(0x03);
	LCD_WriteData(0x0E);
	LCD_WriteData(0x09);
	LCD_WriteData(0x00);

	LCD_WriteCommand(0XE1);    	//Set Gamma
	LCD_WriteData(0x00);
	LCD_WriteData(0x0E);
	LCD_WriteData(0x14);
	LCD_WriteData(0x03);
	LCD_WriteData(0x11);
	LCD_WriteData(0x07);
	LCD_WriteData(0x31);
	LCD_WriteData(0xC1);
	LCD_WriteData(0x48);
	LCD_WriteData(0x08);
	LCD_WriteData(0x0F);
	LCD_WriteData(0x0C);
	LCD_WriteData(0x31);
	LCD_WriteData(0x36);
	LCD_WriteData(0x0F);

	LCD_WriteCommand(0x11);    	//Exit Sleep
	delay_ms(120);

	LCD_WriteCommand(0x29);    //Display on
	LCD_WriteCommand(0x2c);
}

*/

void i2c_init(void)
{
	GPIO_ConfigPin(GPIOB, 8, GPIO_AF4_OD_25MHz_PULL_UP);	//sda
	GPIO_ConfigPin(GPIOB, 9, GPIO_AF4_OD_25MHz_PULL_UP);	//scl

	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

	I2C1->CR1 |= I2C_CR1_SWRST;            //reset I2C
	I2C1->CR1 &= ~I2C_CR1_SWRST;

	I2C1->CR2 = 18;                  // 18Mhz Periph clock
	I2C1->TRISE = 19;                // limit slope 1000ns
	I2C1->CCR = 90;                  // setup speed (100kHz)
	I2C1->CR1 |= I2C_CR1_PE;         // enable peripheral


}

void I2C_WRITE_REG(uint8_t address, uint8_t reg)
{
	uint32_t dummy;

	I2C1->CR1 |= I2C_CR1_START;				// request a start
	while(!(I2C1->SR1 & I2C_SR1_SB));		// wait for start to finish
	dummy = I2C1->SR1;						// read of SR1 clears the flag
	I2C1->DR = address;						// transfer address
	while (!(I2C1->SR1 & I2C_SR1_ADDR));	// wait for address transfer
	dummy = I2C1->SR1;						// clear the flag
	dummy = I2C1->SR2;

	while (!(I2C1->SR1 & I2C_SR1_TXE));	// wait for DR empty
	I2C1->DR = reg;					// transfer one byte, increment pointer

	while ((I2C1->SR1 & I2C_SR1_TXE) == RESET || (I2C1->SR1 & I2C_SR1_BTF) == SET);
										// wait for bus not-busy
	I2C1->CR1 |= I2C_CR1_STOP;
}

void I2C_READ_REG( uint8_t adres, uint8_t reg_adres, uint8_t * dane, uint8_t len )
{
	uint32_t dummy;

	I2C1->CR1 |= I2C_CR1_START;
	while( !( I2C1->SR1 & I2C_SR1_SB ));
	I2C1->DR = adres;
	while( !( I2C1->SR1 & I2C_SR1_ADDR ));
	dummy = I2C1->SR2;
	while( !( I2C1->SR1 & I2C_SR1_TXE ));
	I2C1->DR = reg_adres;
	while( !( I2C1->SR1 & I2C_SR1_BTF ));
	I2C1->CR1 |= I2C_CR1_START;
	while( !( I2C1->SR1 & I2C_SR1_SB ));
	I2C1->DR = adres | 0x01;
	while( !( I2C1->SR1 & I2C_SR1_ADDR ));
	dummy = I2C1->SR2;

	while( len )
	{
	   if( len == 1 )
	       I2C1->CR1 &= ~I2C_CR1_ACK;

	   while( !( I2C1->SR1 & I2C_SR1_RXNE ));
	   *( dane++ ) = I2C1->DR;

	   len--;
	}

	I2C1->CR1 |= I2C_CR1_STOP;
}

uint32_t readtemp(void)
{
	I2C_WRITE_REG(0x77, 0x50 + 1);
	delay_ms(10);

	uint32_t b = 0;
	I2C_READ_REG(0x77, 0, (uint8_t*) b, 3);
	return b;
}

int main(void)
{
	FPU_Enable();
	PLL_Start(CRYSTAL, FREQUENCY);
	GPIO_InitAll();
	Delay_Config(168000000UL);
	Serial_Init();
	Serial_Puts("test usart3...\r\n");

	i2c_init();



	//mems_init();
	//LCD_SPI_Config();
	//LCD_Init();
	//LCD_SetPixel(1, 1, 0xDEAD);

	//GPIO_ConfigPin(GPIOD, 12, GPIO_OUT_PP_100MHz);
	/*RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	TIM1->PSC   = 8399; // 84MHz/8400=10000
	TIM1->ARR   = 9999; // 10000/10000=1
	TIM1->DIER |= TIM_DIER_UIE; // w³aczenie przerwania update
	TIM1->RCR = 9; // co ile przepelnien ma nastapic przerwanie
	TIM1->CR1  |= TIM_CR1_CEN; // w³aczenie zegara
	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);

	GPIO_ConfigPin(GPIOD, 12, GPIO_AF2_PP_100MHz);
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	TIM4->PSC = 83; // 84MHz/84=1MHz
	TIM4->ARR = 15; // 1MHz/16=62.5kHz
	TIM4->CCR1 = 8;
	TIM4->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
	TIM4->CR1 |= TIM_CR1_ARPE;
	TIM4->CCER |= TIM_CCER_CC1E;
	TIM4->CR1 |= TIM_CR1_CEN;*/

	while(1)
	{

		uint32_t t = readtemp();

			char buf[10];
			itoa(t, buf, 16);
			Serial_Puts(buf);

			delay_ms(100);


		/*delay_ms(100);

		acc_t acc;
		mems_read_axis(&acc);

		char x[4] = {0},
			 y[4] = {0},
			 z[4] = {0};

		itoa(acc.x, x, 10);
		Serial_Puts(x);

		Serial_Puts("\t");
		itoa(acc.y, y, 10);
		Serial_Puts(y);

		Serial_Puts("\t");
		itoa(acc.z, z, 10);
		Serial_Puts(z);

		Serial_Puts("\r\n");
*/
	}

	return 0;
}

void TIM1_UP_TIM10_IRQHandler(void)
{
	if (TIM1->SR & TIM_SR_UIF)
	{
		TIM1->SR &= ~TIM_SR_UIF;
		//TIM4->CCR1 += 1;
	}
}
