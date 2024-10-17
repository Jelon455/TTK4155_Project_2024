/*
 * CAN_driver.c
 */

#include "CAN_driver.h"

/* CAN Initialization in Loop-back mode */
void CAN_Init(void) 
{
    MCP2515_Init();
    /*Set loopback mode (CANCTRL Mode bits = 010)*/
    MCP2515_Bit_Modify(MCP_CANCTRL, 0xE0, MODE_NORMAL);
    /*Enable interrupts for receiving messages in RXB0*/
    MCP2515_Write(MCP_CANINTE, 0x01); 
}

/* CAN Message Sending Function */
void CAN_Send_Message(CanMsg* msg)
{
    /*Set the standard ID (11-bit) in TXB0SIDH and TXB0SIDL*/
    MCP2515_Write(MCP_TXB0SIDH, (msg->id >> 3) & 0xFF);
    MCP2515_Write(MCP_TXB0SIDL, (msg->id << 5) & 0xE0);
    
    /*Set data length in TXB0DLC*/
    MCP2515_Write(MCP_TXB0DLC, msg->length & 0x0F);
    /*Write the data bytes to TXB0*/
    for (uint8_t i = 0; i < msg->length; i++) 
	{
        MCP2515_Write(MCP_TXB0D0 + i, msg->byte[i]);
    }
    /*Request to send the message via TXB0*/
    MCP2515_Request_Send(0x01);
}

uint8_t CAN_Receive_Message(CanMsg* msg)
{
    /*Check if there's a message in RXB0*/
    uint8_t canintf = MCP2515_Read(MCP_CANINTF);
    if (!(canintf & MCP_RX0IF)) 
	{
        return 0;
    }

    /*Read the standard ID from RXB0SIDH and RXB0SIDL*/
    msg->id = (MCP2515_Read(MCP_RXB0SIDH) << 3);            // Higher 8 bits of ID
    msg->id |= (MCP2515_Read(MCP_RXB0SIDL) >> 5);           // Lower 3 bits of ID

    /*Read the data length from RXB0DLC*/
    msg->length = MCP2515_Read(MCP_RXB0DLC) & 0x0F; 
    /*Read the data bytes from RXB0*/
    for (uint8_t i = 0; i < msg->length; i++) 
	{
        msg->byte[i] = MCP2515_Read(MCP_RXB0D0 + i);
    }
    /*Clear the RX0IF flag to indicate the message was received*/
    MCP2515_Bit_Modify(MCP_CANINTF, MCP_RX0IF, 0);

    return 1;
}
