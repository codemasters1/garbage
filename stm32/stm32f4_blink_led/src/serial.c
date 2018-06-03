#include <stdint.h>
#include <stm32f4xx.h>
#include "include/config.h"
#include "include/gpio.h"
#include "include/serial.h"
#include "include/delay.h"

#define TX_BUFFER_SIZE 128
#define RX_BUFFER_SIZE 512

static char rxQueueData[RX_BUFFER_SIZE];
volatile uint32_t rxQueueHead = 0;
volatile uint32_t rxQueueTail = 0;

static char txQueueData[TX_BUFFER_SIZE];
volatile uint32_t txQueueHead = 0;
volatile uint32_t txQueueTail = 0;

void USART3_IRQHandler(void)
{
	if ((USART3->SR & USART_SR_RXNE) && (USART3->CR1 & USART_CR1_RXNEIE))
	{
		if (rxQueueHead - rxQueueTail == RX_BUFFER_SIZE - 1)
		{
			USART3->CR1 &= ~USART_CR1_RXNEIE;
			GPIOD->ODR ^= (1 << 15);
		}
		else
		{
			rxQueueData[++rxQueueHead % RX_BUFFER_SIZE] = USART3->DR;
			GPIOD->ODR ^= (1 << 12);
		}

	}

	if ((USART3->SR & USART_SR_TXE) && (USART3->CR1 & USART_CR1_TXEIE))
	{
		if (txQueueHead == txQueueTail)
		{
			USART3->CR1 &= ~USART_CR1_TXEIE;
		}
		else
		{
			USART3->DR = txQueueData[++txQueueTail % TX_BUFFER_SIZE];
			GPIOD->ODR ^= (1 << 13);
		}
	}
}

void Serial_Init(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;

	GPIO_ConfigPin(GPIOD, 8, GPIO_AF7_PP_50MHz_PULL_UP ); // TX
	GPIO_ConfigPin(GPIOD, 9, GPIO_AF7_PP_50MHz_PULL_UP ); // RX

	USART3->BRR = 42000000 / 115200; // APB1 speed / baudrate
	USART3->CR1 |= USART_CR1_TE | USART_CR1_RE | USART_CR1_TXEIE | USART_CR1_RXNEIE;
	USART3->CR1 |= USART_CR1_UE;
	USART3->CR3 |= USART_CR3_DMAT;

	NVIC_EnableIRQ(USART3_IRQn);
}

uint16_t Serial_SendBuffer(char * buffer, uint16_t length)
{
	DMA1_Stream3->CR = 0;
	DMA1_Stream3->M0AR = (uint32_t) buffer;
	DMA1_Stream3->PAR = (uint32_t) &USART3->DR;
	DMA1_Stream3->NDTR = length;
	DMA1_Stream3->CR = DMA_SxCR_DIR_0 | DMA_SxCR_MINC | DMA_SxCR_CHSEL_2;
	DMA1_Stream3->CR |= DMA_SxCR_EN;
	return 0;
}

char Serial_Getc(void)
{
	if (rxQueueHead == rxQueueTail)
	{
		return '\0';
	}

	char ch = rxQueueData[++rxQueueTail % RX_BUFFER_SIZE];
	USART3->CR1 |= USART_CR1_RXNEIE;

	return ch;
}

void Serial_Putc(char c)
{
	while(txQueueTail - txQueueHead == TX_BUFFER_SIZE - 1);

	txQueueData[++txQueueHead % TX_BUFFER_SIZE] = c;
	USART3->CR1 |= USART_CR1_TXEIE;
}

void Serial_Puts(char *s)
{
	while (*s)
		Serial_Putc(*s++);
}
