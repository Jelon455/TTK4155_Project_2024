/*
 * ADC_driver.c
 */

#include "ADC_driver.h"

/* === Variables === */
JoystickCalibration joystick_calibration = {0,0,0,0,0};

/* === Function definition === */
uint8_t ADC_Read(uint8_t channel) 
{
	/*Single-ended unipolar and channel specified ADC mode*/
	uint8_t setup = (1 << 7) | channel;
	XMEM_Write(setup, ADC_BASE_ADDRESS);
	_delay_ms(3);
	return XMEM_Read(ADC_BASE_ADDRESS);
}

JoystickCalibration Calibrate_Joystick(void)
{
	JoystickCalibration calibration;
	int32_t x_sum = 0, y_sum = 0;
	uint16_t x_min = 255, x_max = 0, y_min = 255, y_max = 0;
	const uint16_t num_samples = 100;

	OLED_Clear();
	OLED_Write_String("CALIBRATION", 1, 30);
	OLED_Write_String("Do not move", 5, 30);

	for (uint16_t i = 0; i < num_samples; i++) 
	{
		uint8_t adc_x = ADC_Read(ADC_CHANNEL_X);
		uint8_t adc_y = ADC_Read(ADC_CHANNEL_Y);

		x_sum += adc_x;
		y_sum += adc_y;
		_delay_ms(20);
	}

	calibration.x_offset = (int16_t)(x_sum / num_samples);
	calibration.y_offset = (int16_t)(y_sum / num_samples);

	OLED_Clear();
	OLED_Write_String("CALIBRATION", 1, 30);
	OLED_Write_String("Move", 4, 40);
	OLED_Write_String("<--->", 6, 50);
	for (uint16_t i = 0; i < num_samples; i++) {
		uint8_t adc_x = ADC_Read(ADC_CHANNEL_X);

		if (adc_x > x_max) x_max = adc_x;
		if (adc_x < x_min) x_min = adc_x;
		_delay_ms(20);
	}

	OLED_Clear();
	OLED_Write_String("CALIBRATION", 1, 30);
	OLED_Write_String("Move ", 2, 40);
	OLED_Write_String(" ^ ", 3, 50);
	OLED_Write_String(" | ", 4, 50);
	OLED_Write_String(" v ", 5, 50);

	for (uint16_t i = 0; i < num_samples; i++) {
		uint8_t adc_y = ADC_Read(ADC_CHANNEL_Y);

		if (adc_y > y_max) y_max = adc_y;
		if (adc_y < y_min) y_min = adc_y;
		_delay_ms(20);
	}

	calibration.x_min = (int16_t)x_min;
	calibration.x_max = (int16_t)x_max;
	calibration.y_min = (int16_t)y_min;
	calibration.y_max = (int16_t)y_max;

	OLED_Clear();
	OLED_Write_String("     Done!", 3, 0);
	_delay_ms(1000);
	return calibration;
}

uint8_t Joystick_Pushed(void)
{
	/*Returns 1 if pushed reversed logic*/
	return !(PINB & (1 << JOYSTICK_PUSH_PIN));
}

JoystickPosition Get_Joystick_Position(JoystickCalibration calibration)
{
	JoystickPosition pos;
	int16_t adc_x = (int16_t)ADC_Read(ADC_CHANNEL_X);
	_delay_ms(2);
	int16_t adc_y = (int16_t)ADC_Read(ADC_CHANNEL_Y);
	_delay_ms(2);

	/*Calibrate the ADC readings*/
	int16_t adc_x_calibrated = adc_x - calibration.x_offset;
	int16_t adc_y_calibrated = adc_y - calibration.y_offset;

	/*scale X axis to 0-255*/
	if (adc_x_calibrated >= 0) 
	{
		pos.x = (adc_x_calibrated * 128) / (calibration.x_max - calibration.x_offset) + 127;
	} 
	else 
	{
		pos.x = (adc_x_calibrated * 127) / (calibration.x_offset - calibration.x_min) + 127;
	}

	/*Scale Y axis to 0-255*/
	if (adc_y_calibrated >= 0) 
	{
		pos.y = (adc_y_calibrated * 128) / (calibration.y_max - calibration.y_offset) + 127;
	} 
	else 
	{
		pos.y = (adc_y_calibrated * 127) / (calibration.y_offset - calibration.y_min) + 127;
	}

	if (pos.x > 255) 
	{
		 pos.x = 255;
	}
	if (pos.x < 0) 
	{
		pos.x = 0;
	}
	if (pos.y > 255)
	{
		pos.y = 255;
	}
	if (pos.y < 0) 
	{
		pos.y = 0;
	} 

	return pos;
}

char* Get_Joystick_Direction(JoystickPosition pos) 
{	
	/*Direction based on thresholds*/
	if (pos.x > JOYSTICK_NEUTRAL_THRESHOLD) 
	{
		return "RIGHT";
	} 
	else if (pos.x < JOYSTICK_NEUTRAL_THRESHOLD) 
	{
		return "LEFT";
	} 
	else if (pos.y > JOYSTICK_NEUTRAL_THRESHOLD) 
	{
		return "UP";
	} 
	else if (pos.y < JOYSTICK_NEUTRAL_THRESHOLD) 
	{
		return "DOWN";
	} 
	else 
	{
		return "NEUTRAL";
	}
}

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
	TCCR1A &= ~(1 << WGM10) & ~(1 << WGM11);
	TCCR1A |= (1 << COM1A0);
	/*Prescaler is 1*/
	TCCR1B |= (1 << CS10) | (1 << WGM12) ;
	/*Half of duty cycle in 0-255 8bit timer register*/
	OCR1AL = 0;
	OCR1AH = 0;
	/*setting the joystick as an PB5*/
	DDRB &= ~(1 < JOYSTICK_PUSH_PIN);
	PORTB |= (1 < JOYSTICK_PUSH_PIN);
}
/* === End of function definition === */

