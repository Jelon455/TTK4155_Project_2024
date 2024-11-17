/*
 * pwm.h
 *
 */ 

#pragma once

/* === Include area === */
#include "sam.h"

/* === Define area === */
#define F_CHANNEL_1_CLOCK  (CHIP_FREQ_CPU_MAX / 1024)
#define PERIOD (20.0 / 1000)
#define CPRD  ((uint32_t)(PERIOD * F_CHANNEL_1_CLOCK))

#define MIN_DUTY_CYCLE  (0.9 / 20)
#define MAX_DUTY_CYCLE  (2.1 / 20)

/* === Function declaration === */
void PWM_Init();
void PWM_Set_Duty_Cycle(double duty_cycle);
/* === End function declaration === */