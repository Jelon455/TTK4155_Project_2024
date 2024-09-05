#define FOCS 4915200
#define BAUND 9600

#include <avr/io.h>
#include <string.h>

void USART_Init()
{
	/*Baund rate*/
	unsigned int ubrr  = (FOCS /(16 * BAUND)) - 1;
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	UCSR0A &= ~(1 << U2X0);
	/*transmission settings*/
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	/*Set frame format USBS0 - 1 stop bit  ; UCSZ00 - 8data*/
	UCSR0C = (1<<URSEL0) | (0<<USBS0) | (3<<UCSZ00);
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1 << UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}

char USART_Receive(void)
{
	while (!(UCSR0A & (1<<RXC0)));
	
	return UDR0;
}
void UART_Send_String(const char *str)
{
	USART_Transmit(*str);
	str++;
}
int main(void)
{
	USART_Init();
	while(1)
	{
		UART_Send_String("HELL\n");
		for(volatile long i = 0; i < 5000; i++);
	}
	return 0;
}