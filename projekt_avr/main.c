#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> 

#define PWM_A  (1 << PB1) 
#define PWM_B  (1 << PB2)
#define LED    (1 << PB0)
#define BUTTON (1 << PD0)


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


int main(void) 
{
	uart_init(F_CPU / (57600 << 4) - 1);

    DDRD  &= ~BUTTON; // wejscie
    PORTD |= BUTTON; // pull-up

    DDRB |= LED; // wyjscie na B5

    

    while(1) 
    {
    	
    }
} 