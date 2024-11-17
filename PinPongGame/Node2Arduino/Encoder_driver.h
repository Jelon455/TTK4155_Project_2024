/*
 * Encoder_driver.h
 *
 */ 

/* === Include area === */
#include <stdio.h> 
#include "sam.h"
#include "Motor_driver.h"

/* === Define area === */
#define ERROR_SIZE 50  // Define the size of the error buffer (for integral calculation)
#define MAX_ENCODER 5633 //Maximum position from encoder
#define F_CHANNEL_0_CLOCK  (CHIP_FREQ_CPU_MAX / 1024) //clock frequency for the PWM signal
#define CPRD_MOTOR  ((uint32_t)(0.00004 * F_CHANNEL_0_CLOCK))//frequency of the PWM signal is 25 kHz
#define CENTER_JOYSTICK_POSITION 165
#define MINIMAL_MOTOR_SPEED 0.6
#define MINIMAL_CORRECTION 0.5

/* === Function declaration === */
float PI_controller(int32_t ref, uint32_t pos);
int32_t Motor_position(uint8_t joystick_position, int32_t ref);
void Motor_driving(float u);
void PWM_Motor_Init();
/* === End function declaration === */

