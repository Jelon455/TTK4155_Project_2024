/*
 * Encoder_driver.h
 *
 */ 
#include "sam.h"



int32_t PI_controller(int32_t ref, uint32_t pos);

int32_t Motor_position(uint8_t joystick_position, int32_t ref);

void Motor_driving(int32_t u);

void PWM_Motor_Init();