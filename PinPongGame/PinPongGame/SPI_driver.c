/*
 * CAN_connection.c
 */ 
#include "SPI_driver.h"

/* Define Chip Select Pin for MCP2515 */
#define CS_MCP2515   PB4

/* === Function definition === */
void SPI_Init(void) 
{
	/*Set MOSI (PB5), SCK (PB7) as output, and SS (PB4) as output*/
	DDRB |= (1 << PB5) | (1 << PB7) | (1 << CS_MCP2515);
	/*Set MISO (PB6) as input*/
	DDRB &= ~(1 << PB6);
	
	PORTB |= (1 << CS_MCP2515);

	/*Enable SPI, set Master mode, clock rate fosc/16*/
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void SPI_Write(char data)		/* SPI write data function */
{
//	char flush_buffer;
	SPDR = data;			/* Write data to SPI data register */
	while(!(SPSR & (1<<SPIF)));	/* Wait till transmission complete */
//	flush_buffer = SPDR;		/* Flush received data */
	/* Note: SPIF flag is cleared by first reading SPSR (with SPIF set) and then accessing SPDR hence flush buffer used here to access SPDR after SPSR read */
}

char SPI_Read(void)				/* SPI read data function */
{
	SPDR = 0xFF;
	while(!(SPSR & (1<<SPIF)));	/* Wait till reception complete */
	return(SPDR);			/* Return received data */
}

/* MCP2515 Select (CS Low) */
void SPI_Select(void) 
{
	PORTB &= ~(1 << CS_MCP2515);
}

/* MCP2515 Deselect (CS High) */
void SPI_Deselect(void) 
{
	PORTB |= (1 << CS_MCP2515);
}
