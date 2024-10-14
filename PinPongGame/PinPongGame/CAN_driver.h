/*
 * CAN_driver.h
 */ 

#include "CAN_control.h"

#ifndef CAN_DRIVER_H_
#define CAN_DRIVER_H_

/* Define MCP2515 Register Addresses for CAN configuration */
#define MCP_CANCTRL     0x0F
#define MCP_CANSTAT     0x0E
#define MCP_TXB0SIDH    0x31
#define MCP_TXB0SIDL    0x32
#define MCP_TXB0DLC     0x35
#define MCP_TXB0D0      0x36
#define MCP_RXB0SIDH    0x61
#define MCP_RXB0SIDL    0x62
#define MCP_RXB0DLC     0x65
#define MCP_RXB0D0      0x66
#define MCP_CANINTE     0x2B
#define MCP_CANINTF     0x2C
#define MCP_RX0IF       0x01

// MCP2515 Command Bytes
#define MCP2515_RESET       0xC0
#define MCP2515_READ        0x03
#define MCP2515_WRITE       0x02
#define MCP2515_BIT_MODIFY  0x05
#define MCP_REQUEST         0x81

// MCP2515 Bit Masks
#define MODE_MASK           0xE0  // Mode mask (bits 5, 6, 7 of CANCTRL register)
#define MODE_NORMAL         0x00  // Normal mode (bits 5-7 of CANCTRL set to 000)
#define MODE_LOOPBACK		0x40  // Loopback mode (CANCTRL register bits 5-7 set to 010
#define MCP_RX0IF           0x01  // RX Buffer 0 Full Interrupt Flag

typedef struct
{
	/*Standard Identifier*/
	uint16_t id;
	/*Data length*/
	char length;
	/*Data array*/
	char data[8];
}
CAN_Message;

/* === Function declaration === */
void CAN_Init(void);
uint8_t CAN_Receive_Message(CAN_Message* msg);
void CAN_Send_Message(CAN_Message* msg);

#endif /* CAN_DRIVER_H_ */