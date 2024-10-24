#include "pwm.h"
#include "sam.h"

static const uint32_t CLK_PRESCALAR = 1024;
static const double PERIOD = 20.0 / 1000; // 20ms
static double F_CHANNEL_5_CLOCK;
static uint32_t CPRD;
static const double MIN_DUTY_CYCLE = 0.9 / 20;
static const double MAX_DUTY_CYCLE = 2.1 / 20;

void pwm_init()
{
	F_CHANNEL_5_CLOCK = CHIP_FREQ_CPU_MAX / CLK_PRESCALAR;
	CPRD = PERIOD * CHIP_FREQ_CPU_MAX / CLK_PRESCALAR;

	// enable power clock for pwm
	PMC->PMC_PCER1 |= PMC_PCER1_PID36;
	while ((PMC->PMC_PCSR1 & PMC_PCSR1_PID36) == 0)
	{
	};

	// Multiplex PIOC19 to peripheral
	PIOC->PIO_PDR |= PIO_PDR_P19;
	// set peripheral B as per page 973
	PIOC->PIO_ABSR |= PIO_ABSR_P19;

	// clock A prescaler X = 1024, use prescalar A
	PWM->PWM_CLK = (PWM_CLK_DIVA(1) | PWM_CLK_PREA(0b1010));

	// channel A prescaler, left alignment, inverse polarity
	PWM->PWM_CH_NUM[5].PWM_CMR = PWM_CMR_CPRE_CLKA | PWM_CMR_CPOL;

	// Set the waveform period
	PWM->PWM_CH_NUM[5].PWM_CPRD = PWM_CPRD_CPRD(CPRD);
	PWM->PWM_ENA = PWM_ENA_CHID5;

	return;
}

void pwm_set_duty_cycle(double duty_cycle)
{
	if (duty_cycle < MIN_DUTY_CYCLE)
	{
		duty_cycle = MIN_DUTY_CYCLE;
	}
	if (duty_cycle > MAX_DUTY_CYCLE)
	{
		duty_cycle = MAX_DUTY_CYCLE;
	}

	PWM->PWM_CH_NUM[5].PWM_CMR |= PWM_CMR_CPOL;
	PWM->PWM_CH_NUM[5].PWM_CDTY = -1 / (F_CHANNEL_5_CLOCK * (duty_cycle * PERIOD - PERIOD));
}
