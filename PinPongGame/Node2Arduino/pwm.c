#include "pwm.h"
#include "sam.h"

#define F_CHANNEL_1_CLOCK  (CHIP_FREQ_CPU_MAX / 1024)
#define PERIOD (20.0 / 1000)
#define CPRD  ((uint32_t)(PERIOD * F_CHANNEL_1_CLOCK))

#define MIN_DUTY_CYCLE  (0.9 / 20)
#define MAX_DUTY_CYCLE  (2.1 / 20)

void PWM_Init()
 {
	/*PMC enable*/
	PMC->PMC_WPMR &= ~(PMC_WPMR_WPEN);
	/*enable power clock for pwm 
	to get PID numbmer refer to the ATSAM3X8E manual table 9.1 p.39 */
	PMC->PMC_PCER1 |= PMC_PCER1_PID36;	
	/*atsam3x8e manual p.563 
	chcecking for peripherial clock status register*/
	while ((PMC->PMC_PCSR1 & PMC_PCSR1_PID36) == 0)
	{
	
	};
	/*PIOB to peripheral*/
	PIOB->PIO_PDR |= PIO_PDR_P13;
	/*peripheral B*/
	PIOB->PIO_ABSR |= PIO_ABSR_P13;
	/*ATSAM3xe8 manual p.1006
	PWM_CLK_DIVA(1): CLKA clock is clock selected by PREA
	PWM_CLK_PREA(0b1010): Master clock/1024 */
	PWM->PWM_CLK = (PWM_CLK_DIVA(1) | PWM_CLK_PREA(0b1010));
	/*ATSAM3xe8 manual p.1044
	PWM_CMR_CPRE_CLKA: 0b1011 CLKA - clock A
	CALG: =0 Channel Alignment- left alignment (default so not set)
	PWM_CMR_CPOL: =1 output waveform (output from the comparator) starts at a high level
	*/
	PWM->PWM_CH_NUM[1].PWM_CMR = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;
	/*the waveform period*/
	PWM->PWM_CH_NUM[1].PWM_CPRD = PWM_CPRD_CPRD(CPRD);
	PWM->PWM_CH_NUM[1].PWM_CDTY = (uint32_t)(MIN_DUTY_CYCLE * CPRD);
	PWM->PWM_ENA |= PWM_ENA_CHID1;
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
	PWM->PWM_CH_NUM[1].PWM_CDTY = (uint32_t)(duty_cycle * CPRD);
}
