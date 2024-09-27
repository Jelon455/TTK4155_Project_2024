/*
 * OLED_driver.h
 */ 


#ifndef OLED_DRIVER_H_
#define OLED_DRIVER_H_

/* === Include area === */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#include "Memory_driver.h"
//#include "Font.h"	

/* === Define area === */
#define OLED_COMMAND_ADDRESS 0x1000
#define OLED_DATA_ADDRESS 0x1200

/* === Function declaration === */

void OLED_Init(void);
void OLED_Write_Command(uint8_t);
void OLED_Write_Data(uint8_t);

#endif /* OLED_DRIVER_H_ */