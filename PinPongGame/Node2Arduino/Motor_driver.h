/*
 * Motor_driver.h
 *
 */ 

/* === Include area === */
#include "sam.h"

#ifndef MOTOR_DRIVER_H_
#define MOTOR_DRIVER_H_

/* === Function declaration === */
void Encoder_Init(void);
uint32_t Get_Encoder_Position(void);
void Reset_Encoder_Position(void);
/* === End function declaration === */

#endif /* MOTOR_DRIVER_H_ */