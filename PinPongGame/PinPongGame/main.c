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

/* === Define area === */

/* === Global variable === */
char tx_data;

int main(void) {
	Init();
	USART_Init(UBBR);
	Init_ADC();
	
	FILE *uart_stream = fdevopen(USART_Transmit_Char, USART_Receive_Char);
	
	stdout = uart_stream;
	stdin = uart_stream;
	
	printf("I am printf!\n\r");
	
	//SRAM_test();
	
	JoystickPosition pos = {0,0};
	JoystickDirection dir = NEUTRAL;
	
	SliderPosition poss = {0};
	SliderDirection Sdir = SNEUTRAL;
	
	JoystickCalibration joystick_calibration = Calibrate_Joystick();

	SliderCalibration slider_calibration = Calibrate_Slider();
	while(1)
	{		
		pos = Get_Joystick_Position(joystick_calibration);
		printf("Position x: %d %%, Position y: %d %%\n\r",pos.x, pos.y);
		dir = Get_Joystick_Direction();
		printf("Position of joystick: %d\n\r", dir);
		
		poss = Get_Slider_Position(slider_calibration);
		printf("Position x: %d %%\n\r",poss.x);
		dir = Get_Slider_Direction();
		printf("Position of slider: %d\n\r", Sdir);
		_delay_ms(500);
	}
	return 0;
}





