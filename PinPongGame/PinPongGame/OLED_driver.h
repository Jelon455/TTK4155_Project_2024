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


/* === Define area === */
#define OLED_COMMAND_ADDRESS 0x1000
#define OLED_DATA_ADDRESS 0x1200
#define IR_PIN PB1	
/* === Function declaration === */
void OLED_Write_Command(uint8_t data);
void OLED_Write_Data(uint8_t data);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Set_Page(uint8_t page);
void OLED_Set_Column(uint8_t column);
void OLED_Set_Cursor(uint8_t page, uint8_t column);
void OLED_Write_Char(char c, uint8_t page, uint8_t column);
void OLED_Write_String(const char* str, uint8_t page, uint8_t column);
int OLED_putchar(char c, FILE *stream);
void OLED_Test_Pixel(void);
void Display_Menu(int selected_page);
void Display_Subpage(int page);
void Go_To_Page(int page);
void Menu_Navigation(void);
void Play_Game(void);
#endif /* OLED_DRIVER_H_ */