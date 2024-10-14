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
	CAN_Message test_message;
	test_message.id = 0x122;
	test_message.length = 8;
	test_message.data[0] = 0x11;
	test_message.data[1] = 0x22;
	test_message.data[2] = 0x33;
	test_message.data[3] = 0x44;
	test_message.data[4] = 0x55;
	test_message.data[5] = 0x66;
	test_message.data[6] = 0x77;
	test_message.data[7] = 0xBB;
	
	while (1)
	{	
		printf("Sending CAN message...\n");
		CAN_Send_Message(&test_message);
/*		printf("TEXT MESSAGE %d\n",test_message.data[0]);
		_delay_ms(20);
		CAN_Message received_message;
		CAN_Receive_Message(&received_message);
		printf("Received CAN message with ID: 0x%03X\n", received_message.id);
		printf("Data: ");
		for (uint8_t i = 0; i < received_message.length; i++)
		{
			printf("0x%02X ", received_message.data[i]);
		}
		printf("\n");
		*/
	}

	return 0;
}




