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
	_delay_ms(20);
	FILE *uart_stream = fdevopen(USART_Transmit_Char, USART_Receive_Char);
	stdout = uart_stream;
	stdin = uart_stream;

	SPI_Init();
	CAN_Init();

	calibration = Calibrate_Joystick();
	CanMsg joystick_message;
	
	joystick_message.id = 0x21;
	joystick_message.length = 3;
	joystick_message.byte[0] = 0x00;
	joystick_message.byte[1] = 0xFF;
	joystick_message.byte[2] = 0xFF;
		
	printf("Hello I am node 1! \r\n");
	while (1) 
	{
		JoystickPosition joystick_pos = Get_Joystick_Position(calibration);
		
		/*Create message 1: button pressed, 2: x position joystick, 3: y position joystick*/
		joystick_message.byte[0] = Joystick_Pushed();
		joystick_message.byte[1] = ADC_Read(ADC_CHANNEL_X);
		_delay_ms(50);
		joystick_message.byte[2] = ADC_Read(ADC_CHANNEL_Y);
		_delay_ms(50);
		
		printf("Button State: %d\n\r", joystick_message.byte[0]);
		printf("Joystick position x: %d\n\r", joystick_message.byte[1]);
		printf("Joystick position y: %d\n\r", joystick_message.byte[2]);
				
		CAN_Send_Message(&joystick_message);

		_delay_ms(100);
		
	}

	return 0;
}