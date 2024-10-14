/*
 * CAN_control.h
 *
 * Created: 07.10.2024 13:15:08
 *  Author: mazan
 */ 


#ifndef CAN_CONTROL_H_
#define CAN_CONTROL_H_

#include <avr/io.h>

/* === Define area === */
#define MCP2515_RESET			0xC0
#define MCP2515_READ			0x03
#define MCP2515_WRITE			0x02
#define MCP2515_READ_STATUS		0xA0
#define MCP2515_BIT_MODIFY		0x05
#define MCP_REQUEST				0x81
#define MCP2515_RTS				0x80
#define MCP_CANINTE				0x2B  // Interrupt Enable Register
#define MCP_RX0IE				0x01  // Enable receive interrupt for RXB0

#define MCP2515_CNF1 0x04  // BRP = 4, SJW = 0 (SJW-1) PROP (10)
#define MCP2515_CNF2 0xC6  // BTLMODE = 1, PHSEG1 = 5-1, PRSEG = 3-1
#define MCP2515_CNF3 0x07  // PHSEG2 = 5-1

#define MCP_CNF1 0x2A  // Configuration Register 1
#define MCP_CNF2 0x29  // Configuration Register 2
#define MCP_CNF3 0x28  // Configuration Register 3

/* === Function declaration === */
void MCP2515_Init(void);
void MCP2515_Reset(void);
uint8_t MCP2515_Read(uint8_t address);
void MCP2515_Write(uint8_t address, uint8_t data);
uint8_t MCP2515_Read_Status(void);
void MCP2515_Bit_Modify(uint8_t address, uint8_t mask, uint8_t data);
void MCP2515_Request_Send(uint8_t txBuffer);




#endif /* CAN_CONTROL_H_ */