/*
 * ADC_driver.c
 */

/* === Include area === */
#include <avr/io.h>
#include <stdint.h>
#include <math.h>

#include "ADC_driver.h"
#include "UART_driver.h"
#include "Memory_driver.h"

/* === Global variables === */
JoystickCalibration joystick_calibration = {0,0,0,0,0};

uint8_t ADC_Read(uint8_t channel) 
{
/*Single-ended unipolar and channel specified ADC mode*/
	uint8_t setup = (1 << 7) | channel;
	XMEM_Write(setup, ADC_BASE_ADDRESS);
	_delay_ms(2);
	return XMEM_Read(ADC_BASE_ADDRESS);
}

JoystickCalibration Calibrate_Joystick(void)
{
	JoystickCalibration calibration;
	int32_t x_sum = 0, y_sum = 0;
	uint16_t x_min = 255, x_max = 0, y_min = 255, y_max = 0;
	const uint16_t num_samples = 500;

	printf("Step 1: Setting neutral position: Do not move the joystick.\n");
	_delay_ms(1000);

	for (uint16_t i = 0; i < num_samples; i++)
	{
		uint8_t adc_x = ADC_Read(ADC_CHANNEL_X);
		uint8_t adc_y = ADC_Read(ADC_CHANNEL_Y);

		x_sum += adc_x;
		y_sum += adc_y;

		_delay_ms(2);
	}

	calibration.x_offset = (int16_t)(x_sum / num_samples);
	calibration.y_offset = (int16_t)(y_sum / num_samples);

	printf("Neutral position set!: x_offset = %d, y_offset = %d\n", calibration.x_offset, calibration.y_offset);

	printf("Step 2: Calibration x axis: Move joystick in max left.\n");
	_delay_ms(1000);

	for (uint16_t i = 0; i < num_samples; i++)
	{
		uint8_t adc_x = ADC_Read(ADC_CHANNEL_X);
		if (adc_x < x_min) x_min = adc_x;
	}
	
	printf("Step 3: Calibration x axis: Move joystick in max right.\n");
	_delay_ms(2000);

	for (uint16_t i = 0; i < num_samples; i++)
	{
		uint8_t adc_x = ADC_Read(ADC_CHANNEL_X);
		if (adc_x > x_max) x_max = adc_x;
	}

	printf("Values X: x_min = %d, x_max = %d\n", x_min, x_max);

	printf("Step 4: Calibration y axis: Move joystick in max up.\n");
	_delay_ms(1000);
	for (uint16_t i = 0; i < num_samples; i++)
	{
		uint8_t adc_y = ADC_Read(ADC_CHANNEL_Y);
		if (adc_y > y_max) y_max = adc_y;
	}

	printf("Step 5: Calibration y axis: Move joystick in max down.\n");
	_delay_ms(1000);
	for (uint16_t i = 0; i < num_samples; i++)
	{
		uint8_t adc_y = ADC_Read(ADC_CHANNEL_Y);
		if (adc_y < y_min) y_min = adc_y;
	}

	printf("Values Y: y_min = %d, y_max = %d\n", y_min, y_max);
	_delay_ms(800);
	calibration.x_min = x_min;
	calibration.x_max = x_max;
	calibration.y_min = y_min;
	calibration.y_max = y_max;

	printf("Calibration ended!\n");
	
	return calibration;
}


JoystickPosition Get_Joystick_Position(JoystickCalibration calibration)
{
	JoystickPosition pos;
	
	int16_t adc_x = (int16_t)ADC_Read(ADC_CHANNEL_X);
	int16_t adc_y = (int16_t)ADC_Read(ADC_CHANNEL_Y);
	
	int16_t adc_x_calibrated = adc_x - calibration.x_offset;
	int16_t adc_y_calibrated = adc_y - calibration.y_offset;
	
	if (adc_x_calibrated > 0)
	{
		pos.x = (adc_x_calibrated * 100) / (calibration.x_max - calibration.x_offset);
	}
	else
	{
		pos.x = (adc_x_calibrated * 100) / (calibration.x_offset - calibration.x_min);
	}

	if (adc_y_calibrated > 0)
	{
		pos.y = (adc_y_calibrated * 100) / (calibration.y_max - calibration.y_offset);
	}
	else
	{
		pos.y = (adc_y_calibrated * 100) / (calibration.y_offset - calibration.y_min);
	}

	if (pos.x > 100) pos.x = 100;
	if (pos.x < -100) pos.x = -100;
	if (pos.y > 100) pos.y = 100;
	if (pos.y < -100) pos.y = -100;

	return pos;
}
char* Get_Joystick_Direction(JoystickPosition pos) 
{	
/*Direction based on thresholds*/
	if (pos.x > JOYSTICK_NEUTRAL_THRESHOLD) 
	{
		return "RIGHT";
	} 
	else if (pos.x < -JOYSTICK_NEUTRAL_THRESHOLD) 
	{
		return "LEFT";
	} 
	else if (pos.y > JOYSTICK_NEUTRAL_THRESHOLD) 
	{
		return "UP";
	} 
	else if (pos.y < -JOYSTICK_NEUTRAL_THRESHOLD) 
	{
		return "DOWN";
	} 
	else 
	{
		return "NEUTRAL";
	}
}

/*int8_t Get_Joystick_Angle(JoystickPosition pos)
{
	double result_radians = 0;
	int angle ;
	if (pos.x != 0)
	{
		if (pos.x >= 0 && pos.y >= 0)
			result_radians = atan(pos.y/pos.x); 
		if (pos.x < 0 && pos.y >= 0)
			result_radians = PI/2 + atan(-pos.x/pos.y);
		if (pos.x < 0 && pos.y < 0)
			result_radians = PI + atan(pos.y/pos.x);
		if (pos.x >= 0 && pos.y >= 0)
			result_radians = 3*PI/2 + atan(pos.x/-pos.y);
	}
	else
	{
		result_radians = 0;
	}             
	(int8_t) result_radians * 180 / PI ;
	return(angle);
}
*/
int8_t Get_Slider_Position(uint8_t slider)
{
	uint16_t adc_x = ADC_Read(slider);

	/*Convert ADC value (0-255) to a percentage (0% to 100%)*/
	return (adc_x * 100/255);
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

