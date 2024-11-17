/*
 * CAN_control.c
 */ 

/* === Include area === */
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
	SPI_Select();
	SPI_Write(MCP2515_RESET); 
	SPI_Deselect();
}

void MCP2515_Write(uint8_t address, uint8_t data)
{
	SPI_Select();                   
	SPI_Write(MCP2515_WRITE);        
	SPI_Write(address);         
	SPI_Write(data);                 
	SPI_Deselect();       
}

uint8_t MCP2515_Read(uint8_t address)
{
	uint8_t result;
	SPI_Select();                   
	SPI_Write(MCP2515_READ);         
	SPI_Write(address);            
	result = SPI_Read();            
	SPI_Deselect();            
	return result;
}

void MCP2515_Request_Send(uint8_t txBuffer)
{
	SPI_Select();                    
	SPI_Write(MCP2515_RTS | txBuffer);  
	SPI_Deselect();          
}

uint8_t MCP2515_Read_Status(void)
{
	uint8_t status;
	SPI_Select();                    
	SPI_Write(MCP2515_READ_STATUS);  
	status = SPI_Read();             
	SPI_Deselect();                  
	return status;
}

void MCP2515_Bit_Modify(uint8_t address, uint8_t mask, uint8_t data)
{
	SPI_Select();                     
	SPI_Write(MCP2515_BIT_MODIFY);    
	SPI_Write(address);              
	SPI_Write(mask);                 
	SPI_Write(data);                 
	SPI_Deselect();               
}
/* === End of function definition === */