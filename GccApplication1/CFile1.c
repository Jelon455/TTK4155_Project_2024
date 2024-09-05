

#define FOCS 4915200
#define BAUND 9600

#define MYUBRR FOCS /16/ BAUND-1


#include <avr/io.h>
#include <util/delay.h>
#include <string.h>



void USART_Init(unsigned int ubrr )
{
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0A = (1<<RXEN0) | (1<<TXEN0);
	UCSR0C = (1<<URSEL0) | (1<<USBS0) | (3<<UCSZ00);
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1 << UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

void UART_Send_String(const char *str)
{
	USART_Transmit(*str);
	str++;
}
int main(void)
{
	USART_Init ( MYUBRR );
	while(1)
	{
		UART_Send_String("Hello");
		for(volatile long i = 0; i < 50000; i++);
	}
	return 0;
}

