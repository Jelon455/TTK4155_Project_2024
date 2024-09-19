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
JoystickCalibration joystick_calibration = {0, 0};
SliderCalibration slider_calibration = {0};
	


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
	JoystickCalibration calibration = {0, 0};
	int32_t x_sum = 0, y_sum = 0;
	const uint16_t num_samples = 500;
	
	printf("Hold the joystick on neutral position!");
	for (uint16_t i = 0; i < num_samples; i++) 
	{
		uint8_t adc_x = ADC_Read(ADC_CHANNEL_X);
		uint8_t adc_y = ADC_Read(ADC_CHANNEL_Y);

		x_sum += adc_x;
		y_sum += adc_y;
	}
/*Calculate average values as the neutral position */
	calibration.x_offset = (int16_t)(x_sum / num_samples) - 127;
	calibration.y_offset = (int16_t)(y_sum / num_samples) - 127;
	return calibration;
}

JoystickPosition Get_Joystick_Position(JoystickCalibration calibration) 
{
	JoystickPosition pos;
	
	int16_t  adc_x = (int16_t)ADC_Read(ADC_CHANNEL_X);//160
	int16_t  adc_y = (int16_t)ADC_Read(ADC_CHANNEL_Y);		
	
	int16_t adc_x_calibrated = (int16_t)ADC_Read(ADC_CHANNEL_X) - calibration.x_offset; //127
	int16_t adc_y_calibrated = (int16_t)ADC_Read(ADC_CHANNEL_X) - calibration.y_offset;

/*Convert ADC value (0-255) to a percentage (-100% to 100%)*/
	if (adc_x > adc_x_calibrated)
	{
		pos.x = (adc_x_calibrated * 200/(255 - calibration.x_offset)) - 100;
	}
	else if (adc_x < adc_x_calibrated)
	{
		pos.x = (adc_x_calibrated * 200/(255 + calibration.x_offset)) - 100;
	}
	else
	{
		pos.x = (adc_x_calibrated * 200/255) - 100;
	}
	
	if (adc_y > adc_y_calibrated)
	{
		pos.y = (adc_y_calibrated * 200/(255 - calibration.y_offset)) - 100;
	}
	else if (adc_y < adc_y_calibrated)
	{
		pos.y = (adc_y_calibrated * 200/(255 + calibration.y_offset)) - 100;
	}
	else
	{
		pos.y = (adc_y_calibrated * 200/255) - 100;
	}
	
	return pos;
}

char* Get_Joystick_Direction(void) 
{
	JoystickPosition pos = Get_Joystick_Position(joystick_calibration);
	
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

