#include <stdint.h>
#include <stm32f4xx.h>
#include "include/config.h"
#include "include/gpio.h"
#include "include/serial.h"
#include "include/delay.h"
#include "include/i2c.h"
#include "include/ms5611.h"
#include "include/hmc5883l.h"

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

int main(void)
{
	FPU_Enable();
	PLL_Start(CRYSTAL, FREQUENCY);
	GPIO_InitAll();
	Delay_Config(168000000UL);
	Serial_Init();
	Serial_Puts("[ok] USART3\r\n");

	I2C_Init();
	HMC6883_init();

	//MS5611_readCalibrationData();

	while(1)
	{

		/*int32_t t = MS5611_readTemperature() * 100;
		int32_t p = MS5611_readPressure();

		char buf[10] = {0};
		itoa(t, buf, 10);
		char buf2[10] = {0};
		itoa(p, buf2, 10);

		Serial_Puts("\rT=");
		Serial_Puts(buf);
		Serial_Puts("*C p=");
		Serial_Puts(buf2);
		Serial_Puts("Pa");

		delay_ms(100);*/
	}

	return 0;
}
