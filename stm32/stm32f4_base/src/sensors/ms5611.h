#ifndef MS5611_H_
#define MS5611_H_

#define MS5611_ADDRESS       0x77

#define MS5611_CMD_ADC_READ  0x00
#define MS5611_CMD_RESET     0x1E
#define MS5611_CMD_CONV_D1   0x40
#define MS5611_CMD_CONV_D2   0x50
#define MS5611_CMD_READ_PROM 0xA0

typedef enum {
	MS5611_OSR_256 = 0,
	MS5611_OSR_512 = 2,
	MS5611_OSR_1024 = 4,
	MS5611_OSR_2048 = 6,
	MS5611_OSR_4096 = 8
} ms5611_osr_t;

void MS5611_init(void);
void MS5611_setResolution(ms5611_osr_t);
ms5611_osr_t MS5611_getResolution(void);
void MS5611_writeRegister(uint8_t);
void MS5611_readRegister(uint8_t, uint8_t *, uint8_t);
uint32_t MS5611_readAdc(void);
uint32_t MS5611_readRawTemperature(void);
uint32_t MS5611_readRawPressure(void);
uint16_t MS5611_readProm(uint8_t);
void MS5611_readCalibrationData(void);
double MS5611_readTemperature(bool);
uint32_t MS5611_readPressure(bool);

#endif
