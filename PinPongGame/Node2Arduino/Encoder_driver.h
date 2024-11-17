/*
 * Encoder_driver.h
 *
 */ 
#include <stdio.h> 
#include "sam.h"
#include "Motor_driver.h"


float PI_controller(int32_t ref, uint32_t pos);

int32_t Motor_position(uint8_t joystick_position, int32_t ref);

void Motor_driving(float u);

void PWM_Motor_Init();

