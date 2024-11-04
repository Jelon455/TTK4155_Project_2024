/*
 * Motor_dirver.c
 *
 */ 
#include "sam.h"
#include "Motor_driver.h"

void Encoder_Init(void)
{
	/*Peripherial ID for TC2 is 29 manual SAM p. 542*/
    PMC->PMC_PCER0 |= PMC_PCER0_PID29;
	/*PIOC to peripheral for pin PC26*/
	PIOC->PIO_PDR |= PIO_PDR_P26;
	/*peripheral C*/
	PIOC->PIO_ABSR |= PIO_ABSR_P26;
	/*the same foe PC25*/
	PIOC->PIO_PDR |= PIO_PDR_P25;
	PIOC->PIO_ABSR |= PIO_ABSR_P25;
	
	/*Set up TC2, Channel 0 (TC2 channel 0 is at PC25 and PC26 for encoder input)*/
	TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_XC0 |   // Set clock source to XC0
	TC_CMR_ETRGEDG_RISING; // Enable rising edge trigger

	/*Enable quadrature decoder and position tracking mode manual p. 902*/
	TC2->TC_BMR = TC_BMR_QDEN | 
	TC_BMR_POSEN | 
	TC_BMR_EDGPHA;

	/*Enable the TC2 counter*/
	TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | 
	TC_CCR_SWTRG;
}

uint32_t Get_Encoder_Position(void) {
	// Read the encoder position directly from the TC_CV register of TC2 Channel 0
	return TC2->TC_CHANNEL[0].TC_CV;
}