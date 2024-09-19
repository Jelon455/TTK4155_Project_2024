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
#define ADC_CHANNEL_SLIDER 0b00000010
#define JOYSTICK_NEUTRAL_THRESHOLD 20
#define SLIDER_NEUTRAL_THRESHOLD 20
	
/* === Object area === */
typedef enum
{
	NEUTRAL,
	LEFT,
	RIGHT,
	UP,
	DOWN
} JoystickDirection;

typedef enum
{
	SNEUTRAL,
	SLEFT,
	SRIGHT
} SliderDirection;

typedef struct
{
/*values x and y are form -100% to 100%*/
	int8_t x;
	int8_t y;
} JoystickPosition;

typedef struct
{
	/*values x and y are form -100% to 100%*/
	int8_t x;
} SliderPosition;


typedef struct 
{
	int16_t x_offset;
	int16_t y_offset;
} JoystickCalibration;

typedef struct
{
	int16_t x_offset;
} SliderCalibration;


/* === Function declaration === */
void Init_ADC(void);
void ADC_test(void);
uint8_t ADC_Read(uint8_t channel);
JoystickDirection Get_Joystick_Direction(void);
JoystickPosition Get_Joystick_Position(JoystickCalibration calibration);
JoystickCalibration Calibrate_Joystick(void);

SliderDirection Get_Slider_Direction(void);
SliderPosition Get_Slider_Position(SliderCalibration calibration);
SliderCalibration Calibrate_Slider(void);
#endif /* ADC_DRIVER_H_ */