/*
 * pwm.h
 *
 */ 


#pragma once

/**
 * @brief Initialize pin C19 as PWM output.
 *
 * left aligned inverted signal.
 *
 */
void pwm_init();

/**
 * @brief
 *
 * @param duty_cycle in milliseconds.
 *
 * @note Accepts only values between 0.045 and 0.105. Values outside this range will be transformed to to closest valid value.
 */
void pwm_set_duty_cycle(double duty_cycle);
