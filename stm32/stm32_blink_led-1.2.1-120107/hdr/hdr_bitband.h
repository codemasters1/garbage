#ifndef HDR_BITBAND_H_
#define HDR_BITBAND_H_

#define BITBAND_SRAM_REF   					0x20000000
#define BITBAND_SRAM_BASE  					0x22000000

#define BITBAND_PERIPH_REF   				0x40000000
#define BITBAND_PERIPH_BASE  				0x42000000

#define bitband_t *(volatile unsigned long*)

#define m_BITBAND_SRAM(address, bit)		(BITBAND_SRAM_BASE + (((unsigned long)address) - BITBAND_SRAM_REF) * 32 + (bit) * 4)
#define m_BITBAND_PERIPH(address, bit)	(BITBAND_PERIPH_BASE + (((unsigned long)address) - BITBAND_PERIPH_REF) * 32 + (bit) * 4)

#endif /* HDR_BITBAND_H_ */
