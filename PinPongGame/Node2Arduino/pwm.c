#include "pwm.h"
#include "sam.h"

static const double PERIOD = 20.0 / 1000;
static double F_CHANNEL_1_CLOCK;
static uint32_t CPRD;
static const double MIN_DUTY_CYCLE = 0.9 / 20;
static const double MAX_DUTY_CYCLE = 2.1 / 20;

void PWM_Init()
{
	F_CHANNEL_1_CLOCK = CHIP_FREQ_CPU_MAX / 1024;
	CPRD = PERIOD * F_CHANNEL_1_CLOCK;
	/*enable power clock for pwm to get PID numbmer refer to the ATSAM3X8E manual table 9.1 p.36 */
	PMC->PMC_PCER1 |= PMC_PCER1_PID36;
	/*atsam3x8e manual p.528 chcecking for peripherial clock status register*/
	while ((PMC->PMC_PCSR1 & PMC_PCSR1_PID36) == 0)
	{
	
	};
	/*PIOB to peripheral*/
	PIOB->PIO_PDR |= PIO_PDR_P13;
	/*peripheral B*/
	PIOB->PIO_ABSR |= PIO_ABSR_P13;
	/*clock A prescaler 1024, prescalar A ATSAM3xe8 manual p.1006*/
	PWM->PWM_CLK = (PWM_CLK_DIVA(1) | PWM_CLK_PREA(0b1010));
	/*channel A prescaler, left alignment, inverse polarity*/
	PWM->PWM_CH_NUM[1].PWM_CMR = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;
	/*the waveform period*/
	PWM->PWM_CH_NUM[1].PWM_CPRD = PWM_CPRD_CPRD(CPRD);
	PWM->PWM_ENA = PWM_ENA_CHID1;
	return;
}

void PWM_Set_Duty_Cycle(double duty_cycle)
{
	if (duty_cycle < MIN_DUTY_CYCLE)
	{
		duty_cycle = MIN_DUTY_CYCLE;
	}
	if (duty_cycle > MAX_DUTY_CYCLE)
	{
		duty_cycle = MAX_DUTY_CYCLE;
	}

	PWM->PWM_CH_NUM[1].PWM_CMR |= PWM_CMR_CPOL;
	PWM->PWM_CH_NUM[1].PWM_CDTY = -1 / (F_CHANNEL_1_CLOCK * (duty_cycle * PERIOD - PERIOD));
	printf("PWM_CH_NUM[1].PWM_CDTY: %h\n\r", PWM->PWM_CH_NUM[1].PWM_CDTY);
}
