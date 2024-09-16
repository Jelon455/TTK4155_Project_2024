/*
 * ADC_driver.c
 */

/* === Include area === */
#include <avr/io.h>
#include <stdint.h>
#include "ADC_driver.h"

/*Directions for joystick*/
typedef enum 
{
	NEUTRAL,
	LEFT,
	RIGHT,
	UP,
	DOWN
} JoystickDirection;

/*Position for joystick*/
typedef struct 
{
/*values x and y are form -100% to 100%*/
	int8_t x;
	int8_t y;
} JoystickPosition;

uint8_t ADC_Read(uint8_t channel) {
	volatile uint8_t* adc_ptr = (volatile uint8_t*)ADC_BASE_ADDRESS;

	adc_ptr[0] = channel;
	return adc_ptr[0];
}

JoystickPosition Get_Joystick_Position(void) 
{
	JoystickPosition pos;

	uint8_t adc_x = ADC_Read(ADC_CHANNEL_X);
	uint8_t adc_y = ADC_Read(ADC_CHANNEL_Y);

/*Convert ADC value (0-255) to a percentage (-100% to 100%)*/
	pos.x = ((int16_t)adc_x - 128) * 100 / 128;
	pos.y = ((int16_t)adc_y - 128) * 100 / 128;

	return pos;
}

JoystickDirection Get_Joystick_Direction(void) 
{
	JoystickPosition pos = Get_Joystick_Position();
	
/*Direction based on thresholds*/
	if (pos.x > JOYSTICK_NEUTRAL_THRESHOLD) 
	{
		return RIGHT;
	} 
	else if (pos.x < -JOYSTICK_NEUTRAL_THRESHOLD) 
	{
		return LEFT;
	} 
	else if (pos.y > JOYSTICK_NEUTRAL_THRESHOLD) 
	{
		return UP;
	} 
	else if (pos.y < -JOYSTICK_NEUTRAL_THRESHOLD) 
	{
		return DOWN;
	} 
	else 
	{
		return NEUTRAL;
	}
}

void Init_ADC()
{
/*AS2 = 0 Timer/Counter is clocked form the I/O clock*/
	ASSR &= ~(1 << AS2);
/*Configure PD5 as output (to provide a clock to the ADC)*/
	DDRD |= (1 << ADC_CLOCK_PIN);
/*
	// Set up Timer1 to generate a PWM signal
	TCCR1A = (1 << COM1A0) | (1 << WGM10);  // Toggle OC1A on compare match, fast PWM mode
	TCCR1B = (1 << WGM12) | (1 << CS10);    // No prescaling

	// Set the compare match value to generate a suitable clock signal
	OCR1A = 128; // Adjust this value to change the clock frequency
*/
}

void ADC_test(void)
{
	volatile char *ext_ram = (char *) 0x1400; // Start address for the ADC
	uint16_t ext_ram_size = 0x400;

	printf("Starting ADC test...\n");
	uint16_t seed = rand();
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++)
	{
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
	}
}
