#include <stdbool.h>
#include "include/i2c.h"
#include "include/hmc5883l.h"

uint8_t HMC6883L_writeRegister(uint8_t reg)
{
	uint8_t value;
	I2C_Write(HMC5883L_ADDRESS, &reg, 1);
	I2C_Read(HMC5883L_ADDRESS, &value, 1);

	return value;
}

bool HMC6883_init(void)
{
	if ((HMC6883L_writeRegister(HMC5883L_REG_IDENT_A) != 0x48)
	 || (HMC6883L_writeRegister(HMC5883L_REG_IDENT_B) != 0x34)
	 || (HMC6883L_writeRegister(HMC5883L_REG_IDENT_C) != 0x33))
	{
		return false;
	}

	/*setRange(HMC5883L_RANGE_1_3GA);
	setMeasurementMode(HMC5883L_CONTINOUS);
	setDataRate(HMC5883L_DATARATE_15HZ);
	setSamples(HMC5883L_SAMPLES_1);
	mgPerDigit = 0.92f;*/
}




