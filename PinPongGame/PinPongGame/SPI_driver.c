/*
 * SPI_driver.c
 */

/* === Include area === */
#include "SPI_driver.h"

void SPI_Master_Init(void)
{
	/*PB4 PB5 and PB7 set as output*/
	DDRB |= (1 << DDB4) | (1 << DDB5) | (1 << DDB7);
	/*PB6 as input*/
	DDRB &= ~(1 << DDB6);
	
	/*SPE: Enable SPI, MSTR: Master mode, SPR0: Set clock rate to 16MHz*/
	SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
	
	/*SPI2X:Double SPI Speed Bit is disabled*/
	SPSR &= ~(1 << SPI2X);
}

void SPI_Master_Transmit(char cData)
{
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1 << SPIF)))
	;
}
void SPI_Slave_Init(void)
{
	/* Set MISO output, all others input */
	DDRB = (1 << DDB6);
	/* Enable SPI */
	SPCR = (1 << SPE);
}
char SPI_Slave_Receive(void)
{
	/* Wait for reception complete */
	while(!(SPSR & (1 << SPIF)))
	;
	/* Return data register */
	return SPDR;
}