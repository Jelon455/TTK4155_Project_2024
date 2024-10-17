/* === F_CPU and Bound Rate === */ 
#define FOCS 4915200UL
#define BAUND 9600
#define F_CPU	FOCS

#define ADC_BASE_ADDRESS 0x1400

/* === Include area === */
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#include "UART_driver.h"
#include "Memory_driver.h"
#include "ADC_driver.h"
#include "OLED_driver.h"
#include "SPI_driver.h"
#include "CAN_control.h"
#include "CAN_driver.h"

/* === Define area === */

/* === Global definition area === */
JoystickCalibration calibration = {0,0,255,255,0,0};
int selected_page = 0;
/*Flag informing about if user is in subpage*/
int in_subpage = 0;


int main(void) 
{
	Init();
	USART_Init(UBBR);
	Init_ADC();

	FILE *uart_stream = fdevopen(USART_Transmit_Char, USART_Receive_Char);
	
	stdout = uart_stream;
	stdin = uart_stream;

	SPI_Init();
	CAN_Init();
	_delay_ms(20);
	
	/*making CAN message*/
	CanMsg test_message;
	test_message.id = 0x15;
	test_message.length = 8;
	test_message.byte[0] = 0x11;
	test_message.byte[1] = 0x22;
	test_message.byte[2] = 0x33;
	test_message.byte[3] = 0x44;
	test_message.byte[4] = 0x55;
	test_message.byte[5] = 0x66;
	test_message.byte[6] = 0x77;
	test_message.byte[7] = 0x88;
	
	while (1)
	{	
		printf("HELLO I am NODE 1! Sending CAN message...\n");
		CAN_Send_Message(&test_message);
		_delay_ms(100);
//		printf("TEXT MESSAGE %d\n",test_message.data[0]);
/*		_delay_ms(20);
		CanMsg received_message;
		CAN_Receive_Message(&received_message);
		printf("Received CAN message with ID: 0x%03X\n", received_message.id);
		printf("Data: ");
		for (uint8_t i = 0; i < received_message.length; i++)
		{
			printf("0x%02X ", received_message.byte[i]);
		}
		printf("\n");*/
	}

	return 0;
}




