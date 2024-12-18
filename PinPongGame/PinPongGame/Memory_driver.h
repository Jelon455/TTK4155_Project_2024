/*
 * Memory_driver.h
 */

#ifndef MOMORY_DRIVER_H_
#define MOMORY_DRIVER_H_
/* === F_CPU and Bound Rate === */
#define FOCS 4915200UL
#define BAUND 9600
#define F_CPU	FOCS

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
uint8_t XMEM_Read(uint16_t addr);
void XMEM_Write(uint8_t data, uint16_t addr);
/* === End function declaration === */

#endif /* MOMORY_DRIVER_H_ */