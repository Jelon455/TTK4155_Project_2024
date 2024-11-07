#include "sam.h"
#include "Motor_driver.h"

void Encoder_Init(void)
{
	PMC->PMC_WPMR &= ~(PMC_WPMR_WPEN);
	/* Enable peripheral clock for TC2 (Peripheral ID for TC2 is 29) */
	PMC->PMC_PCER0 |= PMC_PCER0_PID29;
	/* Enable peripheral clock for PIOC (Peripheral ID for PIOC is 13) */
	PMC->PMC_PCER1 |= PMC_PCER1_PID33;
	
	/*PIO Controller Input Filter Enable Register manual p.639*/
	PIOC->PIO_IFER |=	PIO_PDR_P26 | 
						PIO_PDR_P25 |
						PIO_PDR_P29; 
	/*Configure PC26 and PC25 for quadrature decoding*/
	PIOC->PIO_PDR |=	PIO_PDR_P26 |
						PIO_PDR_P29 | 
						PIO_PDR_P25;  
	/*PIO Peripheral AB Select Register manual p.656*/			
	PIOC->PIO_ABSR |=	PIO_ABSR_P26 |
						PIO_PDR_P29 | 
						PIO_ABSR_P25;
	/*PIO Controller Output Disable Register manual p.637*/				
	PIOC->PIO_ODR |=	PIO_ODR_P25 |
						PIO_PDR_P29 | 
						PIO_PDR_P26;
	/* xC0 as a clock and rising edge */
	TC2->TC_CHANNEL[0].TC_CMR =		TC_CMR_TCCLKS_XC0 |
									/*‘Rising edge’ must be selected as the External Trigger Edge manual p.876*/
									TC_CMR_ETRGEDG_RISING |
									/*TIOA must be selected as the External Trigger manual p.876*/
									TC_CMR_ABETRG;
	/*Disable wave form*/
	TC2->TC_CHANNEL[0].TC_CMR &= ~(TC_CMR_WAVE);
    /*ENABLE TC2 Channel 0 before configuring*/
//    TC2->TC_CHANNEL[0].TC_CCR = 0b001;

	TC2->TC_CHANNEL[0].TC_CCR |= TC_CCR_CLKEN;	
	TC2->TC_WPMR = TC_WPMR_WPKEY_PASSWD & ~(TC_WPMR_WPEN);
	/* Enable Quadrature Decoder Mode and Position Tracking Mode and Use edges of Phase A for position counting in the BMR */	
	TC2->TC_BMR |=	TC_BMR_QDEN |	
					TC_BMR_POSEN |
					TC_BMR_MAXFILT(1) |
					TC_BMR_EDGPHA;

	/* Reset the counter and enable it */
	TC2->TC_CHANNEL[0].TC_CCR |=		TC_CCR_CLKEN |
										TC_CCR_SWTRG;
}

uint32_t Get_Encoder_Position(void) 
{
	return TC2->TC_CHANNEL[0].TC_CV;
}

void Reset_Encoder_Position(void) 
{
	TC2->TC_CHANNEL[0].TC_CCR |= TC_CCR_SWTRG;
}
