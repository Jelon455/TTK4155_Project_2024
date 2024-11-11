/*
 * Encoder_driver.c
 *
 */ 
#include "Encoder_driver.h"

#define ERROR_SIZE 10  // Define the size of the error buffer
#define Max_encoder 4294961674UL //Maximum position from encoder
#define F_CHANNEL_1_CLOCK  (CHIP_FREQ_CPU_MAX / 1024) //clock frequency for the PWM signal
#define CPRD  ((uint32_t)(0.001 * F_CHANNEL_1_CLOCK))//frequency of the PWM signal is 1 kHz

int32_t errorBuffer[ERROR_SIZE];  // Buffer to store the filter values
uint8_t errorIndex = 0;  // Index for the filter buffer

int32_t PI_controller(int32_t ref, int32_t pos)
{
	double T = 0.5; //sampling period
	uint8_t Kp = 1 ; //1 to test it first, change value when working
	uint8_t Ki = 5 ;
	int32_t e = ref - pos ;
	errorBuffer[errorIndex] = e;
	errorIndex = (errorIndex + 1) % ERROR_SIZE; //increment position in the buffer for next value, when 10 go back to 0 and will replace value
	long error_sum = 0;
	for (int i = 0; i < ERROR_SIZE; i++) {
		error_sum += errorBuffer[i];
	}
	int32_t u = Kp*e ; // + T*Ki*error_sum ; -> should try only P first to see how it works
	return u ;
}


int32_t Motor_position(uint8_t joystick_position, int32_t ref)
{
	int32_t step ;
	if(joystick_position <= 115) //left
	{
		step = (115-joystick_position)*(Max_encoder/10)/115 ; // linear step from 0% to 10% of the motor range of motion according to the joystick position
		if (step > ref)
		ref = 0 ; //maximum left side
		else
		ref = ref - step ;
	}
	else if(joystick_position <= 140) //right
	{
		step = (joystick_position-140)*(Max_encoder/10)/115 ; // linear step from 0% to 10% of the motor range of motion according to the joystick position
		if (step+ref > Max_encoder)
		ref = Max_encoder ; //maximum right side
		else
		ref = ref + step ;
	}
	else
	// no change in reference position
	return ref ;
}



void Init_gpio_motor(void) {

	// Enable the clock for the GPIO port
	PMC->PMC_PCER0 = (1 << ID_PIOC);
	PMC->PMC_PCER0 = (1 << ID_PIOB);

	// Configure pin D7(on Arduino, PC23 on SAM) as an output -> PHASE/DIR
	PIOD->PIO_PER = PIO_PC23;  // Enable PIO controller
	PIOD->PIO_OER = PIO_PC23;  // Set as output

	// Configure pin D20(on Arduino, PB12 on SAM) as an output -> ENABLE/PWM
	PIOD->PIO_PER = PIO_PB12;  // Enable PIO controller
	PIOD->PIO_OER = PIO_PB12;  // Set as output
}


void Motor_driving(int32_t u)
{
	int8_t duty_cycle ;

	if (u < 0)
	{ // Set the phase pin high (outA is high)
	PIOD->PIO_SODR = PIO_PC23; }
	else
	{ // Set the phase pin low (outB is high)
	PIOD->PIO_CODR = PIO_PC23; }
	u = abs(u) ;
	// maximum speed (define as duty cycle = 50% -> just to test the speed, might change to 100%) is obtain when the error is bigger than half of the motor's range of motion
	if (u > (Max_encoder/2))
	{ duty_cycle = 0.5; }
	else
	{
		duty_cycle = u*0.5 / (Max_encoder/2) ;
	}
	PWM->PWM_CH_NUM[1].PWM_CDTY = (uint32_t)(duty_cycle * CPRD);
}



void PWM_Motor_Init()
 {
	/*PMC enable*/
	PMC->PMC_WPMR &= ~(PMC_WPMR_WPEN);
	/*enable power clock for pwm 
	to get PID number refer to the ATSAM3X8E manual table 9.1 p.39 */
	PMC->PMC_PCER1 |= PMC_PCER1_PID36;	
	/*atsam3x8e manual p.563 
	checking for peripherial clock status register*/
	while ((PMC->PMC_PCSR1 & PMC_PCSR1_PID36) == 0)
	{
	
	};
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
	PWM->PWM_CH_NUM[2].PWM_CMR = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;
	/*the waveform period*/
	PWM->PWM_CH_NUM[2].PWM_CPRD = PWM_CPRD_CPRD(CPRD); 
	PWM->PWM_ENA |= PWM_ENA_CHID1;
}