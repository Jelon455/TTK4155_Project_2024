#include "sam.h"
#include "Motor_driver.h"

void Encoder_Init(void)
{
	/* Enable peripheral clock for TC2 (Peripheral ID for TC2 is 29) */
	PMC->PMC_PCER0 |= PMC_PCER0_PID29;

	/* Configure PC26 and PC25 for quadrature decoding */
	PIOC->PIO_PER |= PIO_PER_P26 | PIO_PER_P25;  
	PIOC->PIO_ABSR |= PIO_ABSR_P26 | PIO_ABSR_P25;
	PIOC->PIO_ODR |= PIO_ODR_P25 | PIO_PDR_P26;
	/* xC0 as a clock and rising edge */
	TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_XC0 |
	TC_CMR_ETRGEDG_RISING;

	/* Enable Quadrature Decoder Mode and Position Tracking Mode and Use edges of Phase A for position counting in the BMR */
	TC2->TC_BMR = TC_BMR_QDEN |
	TC_BMR_SPEEDEN | 	
	TC_BMR_POSEN |
	TC_BMR_EDGPHA;

	/* Reset the counter and enable it */
	TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN |
	TC_CCR_SWTRG;
}

int32_t Get_Encoder_Position(void) {
	return TC2->TC_CHANNEL[0].TC_CV;
}

void Reset_Encoder_Position(void) {
	TC2->TC_CHANNEL[0].TC_CCR |= TC_CCR_SWTRG;
}
