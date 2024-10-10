/*
 * CAN_connection.c
 *
 * Created: 07.10.2024 12:51:28
 *  Author: mazan
 */ 
#include "CAN_connection.h"

/* Define Chip Select Pin for MCP2515 */
#define CS_MCP2515   PB4


/* Initialize SPI as Master */
void SPI_Init(void) {
	// Set MOSI (PB5), SCK (PB7) as output, and SS (PB4) as output
	DDRB |= (1 << PB5) | (1 << PB7) | (1 << CS_MCP2515);
	// Set MISO (PB6) as input
	DDRB &= ~(1 << PB6);

	// Enable SPI, set Master mode, clock rate fosc/16
	SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

/* SPI Transmit and Receive Function */
uint8_t SPI_Transmit(uint8_t data) {
	// Load data into the SPI data register
	SPDR = data;
	// Wait for transmission to complete
	while (!(SPSR & (1 << SPIF)));
	// Return received data from SPI data register
	return SPDR;
}

/* MCP2515 Select (CS Low) */
void SPI_Select(void) {
	PORTB &= ~(1 << CS_MCP2515);  // Pull CS low
}

/* MCP2515 Deselect (CS High) */
void SPI_Deselect(void) {
	PORTB |= (1 << CS_MCP2515);   // Pull CS high
}
