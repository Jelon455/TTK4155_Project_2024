/*
 * Memory_driver.h
 */

#ifndef MOMORY_DRIVER_H_
#define MOMORY_DRIVER_H_

/* === Include area === */
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

/* === Define area === */
#define LATCH_ADDRESS 0x8000
#define ADC_BASE_ADDRESS 0x1400

/* === Function declaration === */
void write_to_latch(uint8_t data);
void Init(void);
void SRAM_test(void);
void ADC_test(void);

#endif /* MOMORY_DRIVER_H_ */