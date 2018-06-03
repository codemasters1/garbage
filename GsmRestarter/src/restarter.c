#define BAUD_GSM 9600
#define BAUD_MAIN 57600

#include <stdlib.h>
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define LED_PIN 5
#define LED_PORT PORTB
#define LED_DDR DDRB

void led_init(void)
{
	LED_DDR |= (1 << LED_PIN);
}

void led_on(void)
{
	LED_PORT |= (1 << LED_PIN);
}

void led_off(void)
{
	LED_PORT &= ~(1 << LED_PIN);
}

void led_toggle(void)
{
	LED_PORT ^= (1 << LED_PIN);
}

#define UART_PRESCALER(baud) (F_CPU / (baud << 4) - 1)

void uart_main_init(void)
{
	uint16_t br = UART_PRESCALER(BAUD_MAIN);

 	UBRR0H = (unsigned char) (br >> 8);
	UBRR0L = (unsigned char) (br >> 0);

	UCSR0C = (1 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00); // 8 bits, 1 stop bit, no parity bit
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0); // TX and RX enabled, RX interrupt enabled
}

void uart_main_putc(char c)
{
	while (!( UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

void uart_main_puts(char *s)
{
	while (*s)
		uart_main_putc(*(s++));
}

void uart_modem_init(void)
{

}

void interrupts_enable(void)
{
	SREG |= (1 << 7); // Global Interrupts Enable
}


ISR(USART_RX_vect)
{
	switch(UDR0)
	{
		case '\r':
			uart_main_putc('R');
			break;
		case '\n':
			uart_main_putc('N');
			break;
		case '\t':
			uart_main_putc('T');
			break;
		default:
			uart_main_putc(UDR0);
	}

	led_toggle();	
}

int main(void)
{
	interrupts_enable();

	led_init();

	uart_main_init();
	uart_modem_init();

	while(1)
	{
		//uart_main_send_byte('.');
	}
}