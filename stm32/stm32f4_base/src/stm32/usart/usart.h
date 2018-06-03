/*
 * usart.h
 *
 *  Created on: 19 cze 2014
 *      Author: Gandalf
 */

#ifndef USART_H_
#define USART_H_

#include "config.h"

/**
 * @brief Inicjalizacja interfejsu USART do pracy.
 */
void USART_init(void);

/**
 * @brief Funkcja wysylajaca bufor przez USART.
 *
 * @param [in]buf                      Bufor do wyslania.
 * @param len                          Ilosc bajtow do wyslania.
 */
void USART_sendBuffer(uint8_t * buf, int32_t len);

#endif /* USART_H_ */
