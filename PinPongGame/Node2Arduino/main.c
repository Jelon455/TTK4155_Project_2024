/*
 * Node2Arduino.c
 *
 * Created: 14.10.2024 12:12:36
 * Author : Julia
 */ 
/* === F_CPU and Bound Rate === */
#define FOCS 4915200UL
#define BAUND 9600
#define F_CPU	FOCS

#include "sam.h"
#include "uart.h"

int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	uart_init(FOCS, BAUND);
    /* Replace with your application code */
    while (1) 
    {
		printf("HELLO\n\r");
    }
}
