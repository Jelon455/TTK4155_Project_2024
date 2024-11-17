/*
 * Encoder_driver.c
 *
 */
 
/* === Include area === */
#include "Encoder_driver.h"

/* === Variables area === */
int32_t errorBuffer[ERROR_SIZE];]
uint8_t errorIndex = 0;

/* === Function definition === */
float PI_controller(int32_t ref, uint32_t pos)
/*compute the correction value (u) with a PI corrector and an antiwindup*/
{
	double T = 0.02; /*sampling period*/
	float Kp = 3 ; /*gain for P*/
	float Ki = 0.05 ; /*31.25 gain for I part*/
	/*get the motor position on a scale from 0 to 100*/
	int32_t motor_position = (int32_t)pos*100/MAX_ENCODER ; 
	float e = ref - motor_position;
	errorBuffer[errorIndex] = e;
	
	/*increment position in the buffer for next value, when 50 go back to 0 and will replace value*/
	errorIndex = (errorIndex + 1) % ERROR_SIZE; 
	long error_sum = 0;
	
	for (int i = 0; i < ERROR_SIZE; i++) 
	{
		error_sum += errorBuffer[i];
	}
	
	float u = Kp*e + T*Ki*error_sum;
	
	/*anti windup : add limits to correction value + erase error value from the list to not disturb next calculus*/ 
	if (u < -100 || u > 100) 
	{
		if (errorIndex == 0)
		{
			errorBuffer[ERROR_SIZE - 1] = 0;
		}
		else
		{
			errorBuffer[-1] = 0;
		}	
		if (u < -100)
		{
			u = -100;
		}
		else
		{
			u = 100;
		}
	}
	return u;
}


int32_t Motor_position(uint8_t joystick_position, int32_t position_ref)
/*give the new position reference value: it increases when the joystick is up and decreases when down*/
{
	int joystick_backward = CENTER_JOYSTICK_POSITION - 5;
	int joystick_forward = CENTER_JOYSTICK_POSITION + 5;
	/*value that will be add/remove from th reference*/
	int32_t step;
	 
	/*left and not close to center*/
	if (joystick_position <= joystick_backward)
	{
		/*mapping between 0% and 2% of the motor range of motion according to the joystick position*/
		step = (joystick_backward - joystick_position) * 2 / joystick_backward; 
		if (step > position_ref)
		{
			/*maximum left side*/
			position_ref = 0; 
		}
		else
		{
			position_ref = position_ref - step;		
		}
	}
	/*right and not close to center*/
	else if (joystick_position >= joystick_forward) 
	{
		/*mapping between 0% and 2% of the motor range of motion according to the joystick position*/
		step = (joystick_position - joystick_forward) * 2 / (255 - joystick_forward); 
		if (step+position_ref > 100)
		{
			/*maximum right side*/
			position_ref = 100; 
		}
		else
		{
			position_ref = position_ref + step;
		}
	}
	else 
	{
		/*no change in reference position if joystick close to center*/
	}
	
	return position_ref;
}

void Motor_driving(float u)
/*compute and send the duty cycle according to correction value (u)*/
{
	double duty_cycle_motor = 0.0 ;

	if (u < 0)
	{
		/*Set the phase pin high (outA is high)*/ 
		PIOC->PIO_SODR = PIO_PC23;
		u = -u;
	}
	else
	{
		/*Set the phase pin low (outB is high)*/ 
		PIOC->PIO_CODR = PIO_PC23;
	}
	if (u < MINIMAL_CORRECTION) 
	{
		/*if the correction value is close enough to 0 (it can't be exactly 0)*/
		duty_cycle_motor = 0;
	}
	else
	{
		/*0.6 is minimal speed, we increase it by a value from 0 to 0.4 depending on the error u (u is %)*/
		duty_cycle_motor = u*0.4 / 100.0 + MINIMAL_MOTOR_SPEED;	
	}
	PWM->PWM_CH_NUM[0].PWM_CDTY = (uint16_t)(duty_cycle_motor * CPRD_MOTOR);
}

void PWM_Motor_Init()
/*reusing the same PWM init as for the servo + init pins for PWM and PHASE signals*/
 {
	/*PMC enable*/
	PMC->PMC_WPMR &= ~(PMC_WPMR_WPEN);
	/*enable power clock for pwm to get PID number refer to the ATSAM3X8E manual table 9.1 p.39 */
	PMC->PMC_PCER1 |= PMC_PCER1_PID36;	
	/*atsam3x8e manual p.563 checking for peripherial clock status register*/
	while ((PMC->PMC_PCSR1 & PMC_PCSR1_PID36) == 0)
	{
	};
	
	/*Enable the clock for the GPIO port*/
	PMC->PMC_PCER0 = (1 << ID_PIOC);
	PMC->PMC_PCER0 = (1 << ID_PIOB);

	/*Configure pin D7(on Arduino, PC23 on SAM) as an output -> PHASE/DIR*/
	PIOC->PIO_PER = PIO_PC23;
	PIOC->PIO_OER = PIO_PC23;

	/*Configure pin D20(on Arduino, PB12 on SAM) as an output -> ENABLE/PWM*/
	PIOB->PIO_PER = PIO_PB12;
	PIOB->PIO_OER = PIO_PB12;
		
	/*PIOB to peripheral*/
	PIOB->PIO_PDR |= PIO_PDR_P12;
	/*peripheral B*/
	PIOB->PIO_ABSR |= PIO_ABSR_P12;
	/*ATSAM3xe8 manual p.1006
	PWM_CLK_DIVA(1): CLKA clock is clock selected by PREA
					 PWM_CLK_PREA(0b1010): Master clock/1024 */
	PWM->PWM_CLK = (PWM_CLK_DIVA(1) | PWM_CLK_PREA(0b1010));
	/*ATSAM3xe8 manual p.1044
	PWM_CMR_CPRE_CLKA: 0b1011 CLKA - clock A
	CALG: =0 Channel Alignment- left alignment (default so not set)
	PWM_CMR_CPOL: =1 output waveform (output from the comparator) starts at a high level
	*/
	PWM->PWM_CH_NUM[0].PWM_CMR = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;
	/*the waveform period*/
	PWM->PWM_CH_NUM[0].PWM_CPRD = PWM_CPRD_CPRD(CPRD_MOTOR); 
	PWM->PWM_ENA |= PWM_ENA_CHID0;
}
/* === End of function definition === */
