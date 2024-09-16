/*
 * ADC_driver.h
 */


#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

/* === Define area === */
#define ADC_BASE_ADDRESS 0x1400
#define ADC_CLOCK_PIN PD5
#define ADC_CHANNEL_X 0x00
#define ADC_CHANNEL_Y 0x01
#define ADC_CHANNEL_SLIDER 0x02
#define JOYSTICK_NEUTRAL_THRESHOLD 20

/* === Object area === */
typedef enum
{
	NEUTRAL,
	LEFT,
	RIGHT,
	UP,
	DOWN
} JoystickDirection;

typedef struct
{
	/*values x and y are form -100% to 100%*/
	int8_t x;
	int8_t y;
} JoystickPosition;


/* === Function declaration === */
void Init_ADC(void);
void ADC_test(void);
uint8_t ADC_Read(uint8_t channel);
JoystickPosition Get_Joystick_Position(void);
JoystickDirection Get_Joystick_Direction(void);

#endif /* ADC_DRIVER_H_ */