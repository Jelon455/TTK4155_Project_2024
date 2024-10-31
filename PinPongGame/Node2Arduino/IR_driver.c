#include "sam.h"
#include <stdio.h>
#include "IR_driver.h"

#define FILTER_SIZE 10  // Define the size of the filter

int filterBuffer[FILTER_SIZE];  // Buffer to store the filter values
int filterIndex = 0;  // Index for the filter buffer

void IR_ADC_Init(void) {
	// Enable the ADC clock (28.7 - p.528 / ID_ADC: 43.5.2 - p.1319)
	PMC->PMC_PCER1 |= PMC_PCER1_PID37;

	// Reset the ADC (43.7.1 - p.1332)
	ADC->ADC_CR = ADC_CR_SWRST;

	// Set the ADC mode to free running (43.7.2 - p.1334)
	ADC->ADC_MR = ADC_MR_FREERUN_ON | ADC_MR_PRESCAL(7) | ADC_MR_STARTUP_SUT64; // prescale set to MCK/256 ; starting after 64 periods of ADCclock

	// Set the ADC channel (43.7.5 - p.1338) 
	ADC->ADC_CHER = ADC_CHER_CH10;
}

uint32_t IR_ADC_Read(void) {
	// Start the conversion (43.7.1 - p.1332)
	ADC->ADC_CR = ADC_CR_START;

	// Wait for the conversion to complete
	while (!(ADC->ADC_ISR & ADC_ISR_DRDY));

	// Return the ADC value
	return ADC->ADC_CDR[1];
}




int applyFilter(int rawValue) { //average filter
	// Update the filter buffer
	filterBuffer[filterIndex] = rawValue;
	filterIndex = (filterIndex + 1) % FILTER_SIZE; //increment position in the buffer for next value, when 10 go back to 0 and will replace value

	// Calculate the average
	long sum = 0;
	for (int i = 0; i < FILTER_SIZE; i++) {
		sum += filterBuffer[i];
	}
	int average = sum / FILTER_SIZE;

	return average;
}

uint8_t score(uint8_t value){
	int rawValue =  IR_ADC_Read();
	int filteredValue = applyFilter(rawValue);
	
	if (filteredValue > 1536) //score!
	{
		value += 1;
	}
	return value;
}