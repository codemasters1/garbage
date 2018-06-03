#include "stm32/i2c/i2c_master.h"
#include "sensors/hmc5883l.h"

float mg_factor = 0.92f;

uint8_t HMC5883L_writeRegister(uint8_t reg)
{
	uint8_t value;
	I2CMaster_write(HMC5883L_ADDRESS, reg);
	value = I2CMaster_read(HMC5883L_ADDRESS);

	return value;
}

bool HMC5883L_init(void)
{
	if ((hmc5883l_write_register(HMC5883L_REG_IDENT_A) != 0x48)
	 || (hmc5883l_write_register(HMC5883L_REG_IDENT_B) != 0x34)
	 || (hmc5883l_write_register(HMC5883L_REG_IDENT_C) != 0x33))
	{
		return false;
	}

	return true;

	HMC5883L_setRange(HMC5883L_RANGE_1_3GA);
	HMC5883L_setMode(HMC5883L_CONTINOUS);
	HMC5883L_setDataRate(HMC5883L_DATARATE_15HZ);
	HMC5883L_setSamples(HMC5883L_SAMPLES_1);
	mg_factor = 0.92f;
}


void HMC5883L_setRange(hmc5883l_range_t range)
{
	mg_factor = mg_factors[range];

    writeRegister8(HMC5883L_REG_CONFIG_B, range << 5);
}

void HMC5883L_setMode(hmc5883l_mode_t mode)
{
    uint8_t value;

    value = readRegister8(HMC5883L_REG_MODE);
    value &= 0b11111100;
    value |= mode;

    writeRegister8(HMC5883L_REG_MODE, value);
}

void HMC5883L_setDataRate(hmc5883l_data_rate_t data_rate)
{
    uint8_t value;

    value = readRegister8(HMC5883L_REG_CONFIG_A);
    value &= 0b11100011;
    value |= (data_rate << 2);

    writeRegister8(HMC5883L_REG_CONFIG_A, value);
}


void HMC5883L_setSamples(hmc5883l_samples_t samples)
{
    uint8_t value;

    value = readRegister8(HMC5883L_REG_CONFIG_A);
    value &= 0b10011111;
    value |= (samples << 5);

    writeRegister8(HMC5883L_REG_CONFIG_A, value);
}
