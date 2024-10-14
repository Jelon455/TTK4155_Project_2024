/*
 * CAN_control.c
 */ 
#define FOCS 4915200UL
#define BAUND 9600
#define F_CPU	FOCS

#include <util/delay.h>
#include <avr/interrupt.h>

#include "SPI_driver.h"
#include "CAN_control.h"


void MCP2515_Init(void) 
{
	SPI_Init();
	/*Reset MCP2515 to put it in a known state*/
	MCP2515_Reset();
	/* Set CNF1, CNF2, CNF3 to configure bit timing */
	MCP2515_Write(MCP_CNF1, MCP2515_CNF1);
	MCP2515_Write(MCP_CNF2, MCP2515_CNF2);
	MCP2515_Write(MCP_CNF3, MCP2515_CNF3);
	/*Enable receive interrupt for RXB0 (Receive Buffer 0)*/
	MCP2515_Write(MCP_CANINTE, MCP_RX0IE);
	/*Set up ATmega162 external interrupt on INT0 (PD2 pin) for falling edge*/
	MCUCR |= (1 << ISC01);
	GICR |= (1 << INT0);
	/*Enable global interrupts*/
	sei();
}

void MCP2515_Reset(void)
{
	SPI_Select();                     // Select MCP2515 by pulling CS low
	SPI_Write(MCP2515_RESET);         // Send Reset instruction
	SPI_Deselect();                   // Deselect MCP2515 by pulling CS high
}

/* MCP2515 Write Function */
void MCP2515_Write(uint8_t address, uint8_t data)
{
	SPI_Select();                     // Select MCP2515
	SPI_Write(MCP2515_WRITE);         // Send Write instruction
	SPI_Write(address);               // Send the address where the data will be written
	SPI_Write(data);                  // Send the data
	SPI_Deselect();                   // Deselect MCP2515
}

/* MCP2515 Read Function */
uint8_t MCP2515_Read(uint8_t address)
{
	uint8_t result;
	SPI_Select();                     // Select MCP2515
	SPI_Write(MCP2515_READ);          // Send Read instruction
	SPI_Write(address);               // Send the address to read from
	result = SPI_Read();              // Read the data
	SPI_Deselect();                   // Deselect MCP2515
	return result;
}

/* MCP2515 Request to Send Function */
void MCP2515_Request_Send(uint8_t txBuffer)
{
	SPI_Select();                     // Select MCP2515
	SPI_Write(MCP2515_RTS | txBuffer);  // Send RTS command with specified buffer
	SPI_Deselect();                   // Deselect MCP2515
}

/* MCP2515 Read Status Function */
uint8_t MCP2515_Read_Status(void)
{
	uint8_t status;
	SPI_Select();                     // Select MCP2515
	SPI_Write(MCP2515_READ_STATUS);   // Send Read Status instruction
	status = SPI_Read();              // Read the status byte
	SPI_Deselect();                   // Deselect MCP2515
	return status;
}

/* MCP2515 Bit Modify Function */
void MCP2515_Bit_Modify(uint8_t address, uint8_t mask, uint8_t data)
{
	SPI_Select();                     // Select MCP2515
	SPI_Write(MCP2515_BIT_MODIFY);    // Send Bit Modify instruction
	SPI_Write(address);               // Send the address to modify
	SPI_Write(mask);                  // Send the mask byte
	SPI_Write(data);                  // Send the data to modify the bits
	SPI_Deselect();                   // Deselect MCP2515
}