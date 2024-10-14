/*
 * Node2Arduino.c
 *
 */ 
/* === F_CPU and Bound Rate === */
#define FOCS 84000000UL
#define BAUND 9600
#define F_CPU	FOCS

#include <stdio.h> 

#include "sam.h"
#include "uart.h"
#include "can_controller.h"
//#include "can_interrupt.h"


/*Defining the  value for CAN_BR*/
#define CAN_BR_BRP		3   
#define CAN_BR_SJW		1   
#define CAN_BR_PROP		1   
#define CAN_BR_PHASE1	5  
#define CAN_BR_PHASE2	5 


/*Defining the starts point on CAN Baudrate Register CAN_BR*/
#define CAN_BR_BRP_Pos		16
#define CAN_BR_SJW_Pos		12
#define CAN_BR_PROPAG_Pos	8
#define CAN_BR_PHASE1_Pos	4
#define CAN_BR_PHASE2_Pos	0

	 
int main(void)
{
    SystemInit();
	uart_init(FOCS, BAUND);
	/* Set the CAN bit timing using the CAN_BR register */
	uint32_t can_br =	(CAN_BR_BRP << CAN_BR_BRP_Pos) |
						(CAN_BR_SJW << CAN_BR_SJW_Pos) |
						(CAN_BR_PROP << CAN_BR_PROPAG_Pos) |
						(CAN_BR_PHASE1 << CAN_BR_PHASE1_Pos) |
						(CAN_BR_PHASE2 << CAN_BR_PHASE2_Pos);
    
    /* Initialize CAN with predefined timing */
    can_init_def_tx_rx_mb(can_br);

    while (1) 
    {
		printf("HELLO\n\r");
		CAN_MESSAGE received_message;
		can_receive(&received_message,2);
		printf("Received CAN message with ID: 0x%03X\n", received_message.id);
		printf("Data: ");
		for (uint8_t i = 0; i < received_message.data_length; i++)
		{
			printf("0x%02X ", received_message.data[i]);
		}
		printf("\n");
    }
}
