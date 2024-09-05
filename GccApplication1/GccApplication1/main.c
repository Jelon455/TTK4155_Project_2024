/* === F_CPU and Bound Rate === */ 
#define FOCS 4915200UL
#define BAUND 9600
#define F_CPU	FOCS

/* === Include area === */
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

/* === Define area === */
#define UBBR (FOCS /(16UL * BAUND)) - 1
/* === Function declaration === */
void USART_Init(unsigned int ubbr);
void USART_Transmit( unsigned char data );
void USART_Msg(char *str);
char USART_Receive(void);
int USART_Transmit_Char(char c, FILE *stream);
int USART_Receive_Char(FILE *stream);

/* === Global variable === */
char tx_data;

int main(void)
{
	USART_Init(UBBR);
	
    FILE *uart_stream = fdevopen(USART_Transmit_Char, USART_Receive_Char);
	
    stdout = uart_stream;
    stdin = uart_stream;
	
	printf("I am printf!\n\r");
	
    while (1)
	{
		tx_data = USART_Receive();
		USART_Transmit(tx_data);
		USART_Transmit(0x0d);
    }
}

/* === Prototype Declaration === */
void USART_Init(unsigned int ubbr)
{
	/*Baund rate*/
	UBRR0H = (unsigned char)(ubbr>>8);
	UBRR0L = (unsigned char)ubbr;
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
void USART_Msg(char *str)
{
	while(*str)
	{
		USART_Transmit(*str);
		str++;
	}
}
char USART_Receive(void)
{
	/*Do nothing until data have been recieved and is ready to be read from UDR*/
	while (!(UCSR0A & (1<<RXC0)));
	return UDR0;
}
int USART_Transmit_Char(char c, FILE *stream)
{
	if (c == '\n')
	{
		USART_Transmit('\r');
	}
	USART_Transmit(c);
	return 0;
}
int USART_Receive_Char(FILE *stream)
{
	/* Wait until data is received */
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}
/* === End od Prototype declaration === */
