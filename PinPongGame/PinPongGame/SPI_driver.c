/*
 * SPI_driver.c
 */

/* === Include area === */ 
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

void SPI_Write(char data)
{
	/* Write data to SPI data register */
	SPDR = data;			
	while(!(SPSR & (1<<SPIF)));
}

char SPI_Read(void)
{
	SPDR = 0xFF;
	/* Wait till reception complete */
	while(!(SPSR & (1<<SPIF)));
	return(SPDR);
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
/* === End of function definition === */