/*
 * ADC_driver.h
 */
#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

/* === Define area === */
#define ADC_BASE_ADDRESS 0x1400
#define ADC_CLOCK_PIN DDD5
#define ADC_CHANNEL_X 0b00000000
#define ADC_CHANNEL_Y 0b00000001
#define ADC_CHANNEL_SLIDER_LEFT 0b00000010
#define ADC_CHANNEL_SLIDER_RIGHT 0b00000011
#define JOYSTICK_NEUTRAL_THRESHOLD 30

	
/* === Object area === */
typedef struct
{
/*values x and y are form -100% to 100%*/
	int8_t x;
	int8_t y;
} JoystickPosition;


typedef struct 
{
	int16_t x_offset;
	int16_t y_offset;
	int16_t x_max;
	int16_t y_max;
	int16_t x_min;
	int16_t y_min;
} JoystickCalibration;

typedef struct
{
	int16_t x_offset;
} SliderCalibration;


/* === Function declaration === */
void Init_ADC(void);
void ADC_test(void);
uint8_t ADC_Read(uint8_t channel);
char* Get_Joystick_Direction(JoystickPosition pos);
JoystickPosition Get_Joystick_Position(JoystickCalibration calibration);
JoystickCalibration Calibrate_Joystick(void);
int8_t Get_Joystick_Angle(JoystickPosition pos);
int8_t Get_Slider_Position(uint8_t slider);
#endif /* ADC_DRIVER_H_ */