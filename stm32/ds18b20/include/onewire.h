#ifndef __ONEWIRE_H
#define __ONEWIRE_H

typedef struct {
	GPIO_TypeDef * gpio;
	uint32_t pin;
} OWBus;

uint8_t OW_Init(OWBus *bus);
void OW_WriteByte(OWBus *bus, uint8_t);
uint8_t OW_ReadByte(OWBus *bus);

void OW_Out(OWBus *bus);
void OW_In(OWBus *bus);
void OW_Set(OWBus *bus);
void OW_Clear(OWBus *bus);
uint32_t OW_IsSet(OWBus *bus);
void OW_Delay(OWBus *bus, uint32_t us);

#endif
