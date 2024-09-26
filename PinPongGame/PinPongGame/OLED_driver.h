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

/* === Function declaration === */
void OLED_Init(void);
void OLED_Reset(void);
void OLED_Home(void);
void OLED_Goto_Line(uint16_t line);
void OLED_Goto_Column(uint16_t column);
void OLED_Clear_Line(uint16_t line);
void OLED_Pos(uint16_t row, uint16_t column);
void OLED_Print(char*);

#endif /* OLED_DRIVER_H_ */