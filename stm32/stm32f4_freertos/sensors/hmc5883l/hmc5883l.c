#include "hardware/i2c/i2c_master.h"
#include "hmc5883l.h"

float mg_factor = 0.92f;

uint8_t hmc5883l_write_register(uint8_t reg)
{
	uint8_t value;
	i2c_master_write(HMC5883L_ADDRESS, &reg, 1);
	i2c_master_read(HMC5883L_ADDRESS, &value, 1);

	return value;
}

bool hmc5883l_init(void)
{
	if ((hmc5883l_write_register(HMC5883L_REG_IDENT_A) != 0x48)
	 || (hmc5883l_write_register(HMC5883L_REG_IDENT_B) != 0x34)
	 || (hmc5883l_write_register(HMC5883L_REG_IDENT_C) != 0x33))
	{
		return false;
	}

	return true;

	hmc5883l_set_range(HMC5883L_RANGE_1_3GA);
	hmc5883l_set_mode(HMC5883L_CONTINOUS);
	hmc5883l_set_data_rate(HMC5883L_DATARATE_15HZ);
	hmc5883l_set_samples(HMC5883L_SAMPLES_1);
	mg_factor = 0.92f;
}


void hmc5883l_set_range(hmc5883l_range_t range)
{
	mg_factor = mg_factors[range];

    writeRegister8(HMC5883L_REG_CONFIG_B, range << 5);
}

void hmc5883l_set_mode(hmc5883l_mode_t mode)
{
    uint8_t value;

    value = readRegister8(HMC5883L_REG_MODE);
    value &= 0b11111100;
    value |= mode;

    writeRegister8(HMC5883L_REG_MODE, value);
}

void hmc5883l_set_data_rate(hmc5883l_data_rate_t data_rate)
{
    uint8_t value;

    value = readRegister8(HMC5883L_REG_CONFIG_A);
    value &= 0b11100011;
    value |= (data_rate << 2);

    writeRegister8(HMC5883L_REG_CONFIG_A, value);
}


void hmc5883l_set_samples(hmc5883l_samples_t samples)
{
    uint8_t value;

    value = readRegister8(HMC5883L_REG_CONFIG_A);
    value &= 0b10011111;
    value |= (samples << 5);

    writeRegister8(HMC5883L_REG_CONFIG_A, value);
}


