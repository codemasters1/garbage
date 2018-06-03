#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> 

#define WS2812 (1 << PB0) // d8


void uart_init(unsigned int baudrate)
{
	UBRR0H = (unsigned char)(baudrate >> 8);
	UBRR0L = (unsigned char)(baudrate >> 0);

	UCSR0B = (1 << TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1 << USBS0) | (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_send(unsigned char data)
{
	while (!( UCSR0A & (1<<UDRE0))) ;
	UDR0 = data;
}

void uart_send_buffer(unsigned int length, char * buffer)
{
	while(*buffer++)
	{
		uart_send(*buffer);
	}
}

void send_reset(void)
{
	PORTB &= ~WS2812;
	_delay_us(100);
}

void send_data(uint32_t data)
{
	for (uint8_t i = 0; i < 24; ++i)
	{
		if (data & 1)
		{
			PORTB |= WS2812;
			_delay_us(0.9f);
			PORTB &= ~WS2812;
			_delay_us(0.35f);
		}
		else
		{
			PORTB |= WS2812;
			_delay_us(0.35f);
			PORTB &= ~WS2812;
			_delay_us(0.9f);
		}

		data >>= 1;
	}
}


int main(void) 
{
	//uart_init(F_CPU / (57600 << 4) - 1);

    DDRB |= WS2812; // wyjscie na B5

    uint8_t i = 0;

    while(1) 
    {
    	send_data((i << 16) | (~(i) << 8));
    	send_data(0xFF0000);
    	send_data(0x00FF00);
    	send_data(0x0000FF);
    	send_data(0x000000);
    	send_reset();
    	_delay_ms(100);
    	i += 10;
    }
} 