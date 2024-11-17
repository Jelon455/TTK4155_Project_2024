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
	
	printf("Hello I am node 1! \r\n");
	

	OLED_Init();
	OLED_Clear();
	/*setting ir as an PB1*/
	DDRB &= ~(1 < IR_PIN);
	PORTB |= (1 < IR_PIN);
	SPI_Init();
	CAN_Init();
	
	calibration = Calibrate_Joystick();
	_delay_ms(50);
	
	OLED_Clear();
	Display_Menu(0);
	
	while (1) 
	{
		Menu_Navigation();
	}

	return 0;
}