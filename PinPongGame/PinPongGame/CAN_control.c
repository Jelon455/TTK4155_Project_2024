/*
 * CAN_control.c
 *
 * Created: 07.10.2024 13:14:53
 *  Author: mazan
 */ 
#include "CAN_connection.h"
#include "CAN_control.h"
#include <util/delay.h>

/* MCP2515 Initialization */
void MCP2515_Init(void) {
	SPI_Init();           // Initialize SPI
	MCP2515_Reset();      // Reset MCP2515
	_delay_ms(50);        // Wait for MCP2515 to reset
}

/* MCP2515 Reset */
void MCP2515_Reset(void) {
	//SPI_Select();
	SPI_Transmit(MCP2515_RESET);  // Send reset command
	SPI_Deselect();
	_delay_ms(10);  // Wait for MCP2515 to reset
}

/* MCP2515 Read Register */
uint8_t MCP2515_Read(uint8_t address) {
	uint8_t result;
	//SPI_Select();
	SPI_Transmit(MCP2515_READ);   // Send read command
	SPI_Transmit(address);        // Send address to read
	result = SPI_Transmit(0xFF);  // Send dummy data to receive the data
	//SPI_Deselect();
	return result;
}

/* MCP2515 Write Register */
void MCP2515_Write(uint8_t address, uint8_t data) {
	//SPI_Select();
	SPI_Transmit(MCP2515_WRITE);  // Send write command
	SPI_Transmit(address);        // Send address to write
	SPI_Transmit(data);           // Send data to write
	//SPI_Deselect();
}

/* MCP2515 Read Status */
uint8_t MCP2515_Read_Status(void) {
	uint8_t status;
	//SPI_Select();
	SPI_Transmit(MCP2515_READ_STATUS);  // Send read status command
	status = SPI_Transmit(0xFF);        // Send dummy data to receive the status
	//SPI_Deselect();
	return status;
}

/* MCP2515 Bit Modify */
void MCP2515_Bit_Modify(uint8_t address, uint8_t mask, uint8_t data) {
	//SPI_Select();
	SPI_Transmit(MCP2515_BIT_MODIFY);  // Send bit modify command
	SPI_Transmit(address);             // Send address to modify
	SPI_Transmit(mask);                // Send mask
	SPI_Transmit(data);                // Send data to modify
	//SPI_Deselect();
}