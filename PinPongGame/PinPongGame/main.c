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


int main(void) {
	Init();
	USART_Init(UBBR);
	Init_ADC();
	FILE *uart_stream = fdevopen(USART_Transmit_Char, USART_Receive_Char);
	stdout = uart_stream;
	stdin = uart_stream;

	SPI_Init();
	CAN_Init();
	_delay_ms(20);

	// Initialize joystick calibration
	calibration = Calibrate_Joystick();
	CanMsg joystick_message;
	joystick_message.id = 0x22; // Set CAN ID for joystick message
	joystick_message.length = 1; // Only sending x-axis
	joystick_message.byte[0] = 0x00;
	
	while (1) {
		// Get joystick position
		printf("Hello I am node 1! \r\n");
		JoystickPosition joystick_pos = Get_Joystick_Position(calibration);

		// Prepare CAN message based on joystick x-axis data

		// raw joystic pos
		joystick_message.byte[0] = ADC_Read(ADC_CHANNEL_X);

		printf("Sending joystick x position: %d\n", joystick_message.byte[0]);
		CAN_Send_Message(&joystick_message);

		_delay_ms(200); // Send updates every 100 ms
		
	}

	return 0;
}