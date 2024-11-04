#include "sam.h"
#include <stdio.h>
#include "IR_driver.h"

#define FILTER_SIZE 10  // Define the size of the filter

int filterBuffer[FILTER_SIZE];  // Buffer to store the filter values
int filterIndex = 0;  // Index for the filter buffer

void IR_ADC_Init(void) {
	// Enable ADC clock
	PMC->PMC_PCER1 |= PMC_PCER1_PID37;  // Enable the ADC peripheral clock

	// Configure the ADC settings
	ADC->ADC_CR = ADC_CR_SWRST;         // Reset the ADC
	ADC->ADC_MR |= ADC_MR_TRGEN_DIS     // Disable hardware trigger mode
	|  ADC_MR_FREERUN_OFF   // Disable free-running mode
	|  ADC_MR_PRESCAL(10)   // Set prescaler (ADC clock = MCK / (2 * (PRESCAL + 1)))
	|  ADC_MR_SETTLING_AST9 // Set settling time to 9 ADC clock cycles
	|  ADC_MR_TRACKTIM(1);  // Set tracking time

	// Enable channel 1
	ADC->ADC_CHER |= ADC_CHER_CH1;
	
}

uint32_t  IR_ADC_Read(void) {
    // Start conversion
    ADC->ADC_CR = ADC_CR_START;
    
    // Wait for conversion to complete
    while (!(ADC->ADC_ISR & ADC_ISR_EOC1)) {
	    // Wait until End of Conversion (EOC) for channel 1
    }
    
    // Read the value from channel 1
    return ADC->ADC_CDR[1];  // ADC_CDR is the conversion data register array
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