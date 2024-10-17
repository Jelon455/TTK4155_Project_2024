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
#include "can.h"

/*Defining the  value for CAN_BR DATASHEET: page 1193*/
#define BR_BRP	20   
#define BR_SJW	1   
#define BR_PROP		1   
#define BR_PHASE1	1  
#define BR_PHASE2	1 


/*Defining the starts point on CAN Baudrate Register CAN_BR*/
#define BR_BRP_Pos		16
#define BR_SJW_Pos		12
#define BR_PROPAG_Pos	8
#define BR_PHASE1_Pos	4
#define BR_PHASE2_Pos	0

int main(void)
{
    SystemInit();
	uart_init(FOCS, BAUND);
	CanInit canInit;

	canInit.phase2 = BR_PHASE2;
	canInit.propag = BR_PROP;  
	canInit.phase1 = BR_PHASE1;
	canInit.sjw = BR_SJW;      
	canInit.brp = BR_BRP;      
	canInit.smp = 0;
	
    /* Initialize CAN with predefined timing */
    can_init(canInit, 0);

    while (1) 
    {
		printf("HELLO I am NODE 2!\n\r");
		CanMsg received_message;
		can_rx(&received_message);
		can_printmsg(received_message);
    }
}
