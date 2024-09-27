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

/* === Define area === */

/* === Global variable === */
char tx_data;

int main(void) {
	Init();
	USART_Init(UBBR);
	//Init_ADC();
	OLED_Init();
	
	FILE *uart_stream = fdevopen(USART_Transmit_Char, USART_Receive_Char);
	
	stdout = uart_stream;
	stdin = uart_stream;
	
	printf("I am printf!\n\r");
	
//	SRAM_test();
	
//	JoystickPosition pos = {0,0};
	
//	int8_t poss = {0};
//	int8_t poss_right = {0};	
	
	//JoystickCalibration joystick_calibration = Calibrate_Joystick();

	while(1)
	{	
		//OLED_Write_Command(0xa5);
		//OLED_Write_Data(0x10);
		//OLED_Write_Data(0x1100);
		/*Right touch button*/
		//DDRE &= ~(1 << PE0);
		//PORTE |= (1 << PE0);
		/*Left touch button*/
		//DDRE &= ~(1 << PE2);
		//PORTE |= (1 << PE2);
		/*joystick touch button, inverted logic*/
		//DDRB &= ~(1 << PB2);
		//OLED_Print_Char('A');
/*		uint8_t pin_valueE0 = PINE & (1 << PE0);
		uint8_t pin_valueE2 = PINE & (1 << PE2);
		uint8_t pin_valueB2 = PINB & (1 << PB2);
		printf("PIN value right touch button: %d\n\r", pin_valueE0);
		printf("PIN value left touch button: %d\n\r", pin_valueE2);
		printf("PIN value joystick touch button: %d\n\r", pin_valueB2);
		_delay_ms(800);*/
//		pos = Get_Joystick_Position(joystick_calibration);
//		printf("Position x: %d , Position y: %d \n\r",pos.x, pos.y);
//		printf("Position of joystick: %s\n\r", Get_Joystick_Direction(pos));
//		_delay_ms(800);
//		poss = Get_Slider_Position(ADC_CHANNEL_SLIDER_LEFT);
//		printf("Position LEFT : %d %%\n\r",100-poss);	
//		poss_right = Get_Slider_Position(ADC_CHANNEL_SLIDER_RIGHT);
//		printf("Position RIGHT : %d %%\n\r",poss_right);
		_delay_ms(1000);
		OLED_Write_Command(0xae);
		OLED_Write_Command(0xaf);
		_delay_ms(1000);
		        OLED_Write_Command(0xB0);    // Sélectionner la page (0xB0 à 0xB7 pour les 8 pages)
		        OLED_Write_Command(0x00);           // Colonne basse (0x00)
		        OLED_Write_Command(0x10);
		OLED_Write_Data(0x11111001);
		_delay_ms(1000);
	}
	return 0;
}





