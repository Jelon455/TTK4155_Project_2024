/*
 * ADC_driver.c
 */

/* === Include area === */
#include <avr/io.h>
#include <stdint.h>

#include "ADC_driver.h"
#include "UART_driver.h"

/* === Global variables === */
JoystickCalibration joystick_calibration = {0, 0};


uint8_t ADC_Read(uint8_t channel) 
{
	volatile uint8_t* adc_ptr = (volatile uint8_t*)ADC_BASE_ADDRESS;
	_delay_ms(3); //time for the ADC to activate (maybe not necessary)
	*adc_ptr = channel;
	return *adc_ptr;
}

JoystickCalibration Calibrate_Joystick(void) 
{
	JoystickCalibration calibration = {0, 0};
	int32_t x_sum = 0, y_sum = 0;
	const uint16_t num_samples = 500;
	
	printf("Hold the joystic on neutral position!");
	for (uint16_t i = 0; i < num_samples; i++) 
	{
		uint8_t adc_x = ADC_Read(ADC_CHANNEL_X);
		uint8_t adc_y = ADC_Read(ADC_CHANNEL_Y);

		x_sum += adc_x;
		y_sum += adc_y;
	}
/*Calculate average values as the neutral position */
	calibration.x_offset = x_sum / num_samples;
	calibration.y_offset = y_sum / num_samples;
	return calibration;
}

JoystickPosition Get_Joystick_Position(JoystickCalibration calibration) 
{
	JoystickPosition pos;

	uint8_t adc_x = ADC_Read(ADC_CHANNEL_X) - calibration.x_offset;
	uint8_t adc_y = ADC_Read(ADC_CHANNEL_Y) - calibration.y_offset;

/*Convert ADC value (0-255) to a percentage (-100% to 100%)*/
    pos.x = ((int16_t)adc_x) * 100 / 128;
    pos.y = ((int16_t)adc_y) * 100 / 128;

	return pos;
}

JoystickDirection Get_Joystick_Direction(void) 
{
	JoystickPosition pos = Get_Joystick_Position(joystick_calibration);
	
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
/*Configure PD5 as output (to provide a clock to the ADC)*/
	DDRD |= (1 << ADC_CLOCK_PIN);
/**/
	TCCR1A &= ~(1 << WGM10) & ~(1 << WGM11);
	TCCR1A |= (1 << COM1A0);
/*Prescaler is 1*/
	TCCR1B |= (1 << CS10) | (1 << WGM12) ;
/*Half of duty cycle in 0-255 8bit timer register*/
	OCR1AL = 0;
	OCR1AH = 0;
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

