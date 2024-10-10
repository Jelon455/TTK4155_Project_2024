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
#include "CAN_connection.h"
#include "CAN_control.h"

/* === Define area === */

/* === Global definition area === */
JoystickCalibration calibration = {0,0,255,255,0,0};
int selected_page = 0;
/*Flag informing about if user is in subpage*/
int in_subpage = 0;


int main(void) {
	
	Init();
	USART_Init(UBBR);
	Init_ADC();

	FILE *uart_stream = fdevopen(USART_Transmit_Char, USART_Receive_Char);
	stdout = uart_stream;
	stdin = uart_stream;

	// Initialize MCP2515 (which also initializes SPI)
	SPI_Init();
	MCP2515_Init();
		
	_delay_ms(20);
	while (1) 
	{
		MCP2515_Write(0x2A, 0x01);
		_delay_ms(500);
		MCP2515_Read(0x2A);
		_delay_ms(500);
	}

	return 0;
}




