#include <stdbool.h>
#include "stm32/i2c/i2c_master.h"
#include "ms5611.h"

static ms5611_osr_t osr;
static uint16_t C[6];

static uint8_t getConversionTime(void)
{
	static uint8_t conversion_times[] = {1, 2, 3, 5, 10};

	return conversion_times[osr << 1];
}

void MS5611_setResolution(ms5611_osr_t new_osr)
{
	osr = new_osr;
}

ms5611_osr_t MS5611_getResolution(void)
{
	return osr;
}

void MS5611_writeRegister(uint8_t reg)
{
	I2CMaster_write(MS5611_ADDRESS, reg);
}

void MS5611_readRegister(uint8_t reg, uint8_t * data, uint8_t length)
{
	I2CMaster_write(MS5611_ADDRESS, reg);
	I2CMaster_readBuffer(MS5611_ADDRESS, data, length);
}

uint32_t MS5611_readAdc(void)
{
	uint8_t data[3] = {0};
	MS5611_readRegister(MS5611_CMD_ADC_READ, data, 3);
	return data[0] << 16 | data[1] << 8 | data[2];
}

uint32_t MS5611_readRawTemperature(void)
{
	MS5611_writeRegister(MS5611_CMD_CONV_D2 | osr);
	delay(getConversionTime());

	return MS5611_readAdc();
}

uint32_t MS5611_readRawPressure(void)
{
	MS5611_writeRegister(MS5611_CMD_CONV_D1 | osr);
	delay(getConversionTime());

	return MS5611_readAdc();
}

uint16_t MS5611_readProm(uint8_t index)
{
	uint8_t data[2];
	MS5611_readRegister(MS5611_CMD_READ_PROM + index * 2, data, 2);

	return data[0] << 8 | data[1];
}

void MS5611_readCalibrationData(void)
{
	for (uint8_t index = 0; index < 6; index++)
		C[index] = MS5611_readProm(index + 1);
}

void MS5611_init(void)
{
	MS5611_readCalibrationData();
}

double MS5611_readTemperature(bool compensate)
{
    int32_t dT = MS5611_readRawTemperature() - (uint32_t)(C[4] << 8);
    int32_t T = 2000 + ((int64_t) dT * C[5]) / (1 << 24);

    if (compensate)
    {
    	int32_t T2 = 0;

    	if (T < 2000)
    	{
    		T2 = (dT * dT) / (1 << 31);
    	}

    	T -= T2;
    }

    return (double) T / 100.0f;
}

uint32_t MS5611_readPressure(bool compensate)
{
	uint32_t D1 = MS5611_readRawPressure();
	uint32_t D2 = MS5611_readRawTemperature();

	int32_t dT = D2 - (uint32_t)(C[4] << 8);

	int64_t OFF  = (int64_t) C[1] * 0x10000 + (int64_t) C[3] * dT / 0x080;
	int64_t SENS = (int64_t) C[0] * 0x08000 + (int64_t) C[2] * dT / 0x100;

	int32_t TEMP = 2000 + ((int64_t) dT * C[5]) / (1 << 24);

	if (compensate)
	{
		int64_t OFF2 = 0;
		int64_t SENS2 = 0;

		if (TEMP < 2000)
		{
			OFF2 = 5 * ((TEMP - 2000) * (TEMP - 2000)) / 2;
			SENS2 = 5 * ((TEMP - 2000) * (TEMP - 2000)) / 4;
		}

		if (TEMP < -1500)
		{
			OFF2 = OFF2 + 7 * ((TEMP + 1500) * (TEMP + 1500));
			SENS2 = SENS2 + 11 * ((TEMP + 1500) * (TEMP + 1500)) / 2;
		}

		OFF = OFF - OFF2;
		SENS = SENS - SENS2;
	}

	return (D1 * SENS / 0x200000 - OFF) / 0x8000;
}
