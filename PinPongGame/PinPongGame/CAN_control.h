/*
 * CAN_control.h
 *
 * Created: 07.10.2024 13:15:08
 *  Author: mazan
 */ 


#ifndef CAN_CONTROL_H_
#define CAN_CONTROL_H_

#include <avr/io.h>

/* MCP2515 Command Definitions */
#define MCP2515_RESET       0xC0
#define MCP2515_READ        0x03
#define MCP2515_WRITE       0x02
#define MCP2515_READ_STATUS 0xA0
#define MCP2515_BIT_MODIFY  0x05

/* MCP2515 Function Prototypes */
void MCP2515_Init(void);
void MCP2515_Reset(void);
uint8_t MCP2515_Read(uint8_t address);
void MCP2515_Write(uint8_t address, uint8_t data);
uint8_t MCP2515_Read_Status(void);
void MCP2515_Bit_Modify(uint8_t address, uint8_t mask, uint8_t data);



#endif /* CAN_CONTROL_H_ */