#include "gpio.h"

void GPIO_setMode(GPIO_TypeDef * gpio, int32_t pin, int32_t conf)
{
   gpio->MODER &= ~(3 << (pin << 1));
   gpio->MODER |= conf << (pin << 1);
}

void GPIO_setSpeed(GPIO_TypeDef * gpio, int32_t pin, int32_t conf)
{
   gpio->OSPEEDR &= ~(3 << (pin << 1));
   gpio->OSPEEDR |= conf << (pin << 1);
}

void GPIO_setPUPD(GPIO_TypeDef * gpio, int32_t pin, int32_t conf)
{
   gpio->PUPDR &= ~(3 << (pin << 1));
   gpio->PUPDR |= conf << (pin << 1);
}

void GPIO_setAF(GPIO_TypeDef * gpio, int32_t pin, int32_t conf)
{
   if (pin > 7)
   {
      gpio->AFR[1] &= ~(0xF << ((pin - 8) << 2));
      gpio->AFR[1] |= conf << ((pin - 8) << 2);
   }
   else
   {
      gpio->AFR[0] &= ~(0xF << (pin << 2));
      gpio->AFR[0] |= conf << (pin << 2);
   }
}
