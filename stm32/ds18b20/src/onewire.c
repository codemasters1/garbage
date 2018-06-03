#include "include/stm32f4xx.h"
#include "include/gpio.h"
#include "include/delay.h"
#include "include/onewire.h"

inline void OW_Out(OWBus *bus)
{
	gpio_pin_cfg(bus->gpio, bus->pin, GPIO_OUT_PP_50MHz);
}

inline void OW_In(OWBus *bus)
{
	gpio_pin_cfg(bus->gpio, bus->pin, GPIO_IN_FLOATING);
}

inline void OW_Set(OWBus *bus)
{
	bus->gpio->ODR |= (1 << bus->pin);
}

inline void OW_Clear(OWBus *bus)
{
	bus->gpio->ODR &= ~(1 << bus->pin);
}

inline uint32_t OW_IsSet(OWBus *bus)
{
	return bus->gpio->IDR & (1 << bus->pin);
}

inline void OW_Delay(OWBus *bus, uint32_t us)
{
	(void) bus;
	delay_us(us);
}

uint8_t OW_Init(OWBus *bus)
{
	uint8_t presence = 0;

	OW_Out(bus);
	OW_Clear(bus);
	OW_Delay(bus, 500);

	OW_Set(bus);
	OW_Delay(bus, 70);

	OW_In(bus);
	if (! OW_IsSet(bus))
		presence = 1;

	OW_Delay(bus, 500);

    return presence && OW_IsSet(bus);
}

static inline void OW_Write0(OWBus * bus)
{
	OW_Clear(bus);
	OW_Delay(bus, 70);
	OW_Set(bus);
	OW_Delay(bus, 7);
}

static inline void OW_Write1(OWBus * bus)
{
	OW_Clear(bus);
	OW_Delay(bus, 7);
	OW_Set(bus);
	OW_Delay(bus, 70);
}

void OW_WriteByte(OWBus *bus, uint8_t byte)
{
	uint8_t i;

	OW_Out(bus);

	for (i = 0; i < 8; i++)
	{
		if (byte & 0x01)
			OW_Write1(bus);
		else
			OW_Write0(bus);

		byte >>= 1;
	}
}

uint8_t OW_ReadByte(OWBus *bus)
{
	uint8_t i, byte = 0;

	for (i = 0; i < 8; i++)
	{
		byte >>= 1;

		OW_Out(bus);
		OW_Clear(bus);
		OW_Delay(bus, 3);
		OW_Set(bus);
		OW_Delay(bus, 7);

		OW_In(bus);
    	if(OW_IsSet(bus))
    		byte |= 0x80;

    	OW_Delay(bus, 70);
	}

	return byte;
}
