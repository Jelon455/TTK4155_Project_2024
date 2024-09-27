/*
 * OLED_driver.c
 */

/* === Include area === */
#include "OLED_driver.h"
#include "Memory_driver.h"



/* === Function definition === */
void OLED_Write_Command(uint8_t data) {
	XMEM_Write(data, OLED_COMMAND_ADDRESS);
}

void OLED_Write_Data(uint8_t data) {
	XMEM_Write(data, OLED_DATA_ADDRESS);
}

void OLED_Init(void) {
	OLED_Write_Command(0xae); // display off
	OLED_Write_Command(0xa1); // segment remap
	OLED_Write_Command(0xda); // common pads hardware: alternative
	OLED_Write_Command(0x12);
	OLED_Write_Command(0xc8); // common output scan direction: com63~com0
	OLED_Write_Command(0xa8); // multiplex ratio mode: 63
	OLED_Write_Command(0x3f);
	OLED_Write_Command(0xd5); // display divide ratio/osc. freq. mode
	OLED_Write_Command(0x80);
	OLED_Write_Command(0x81); // contrast control
	OLED_Write_Command(0x50);
	OLED_Write_Command(0xd9); // set pre-charge period
	OLED_Write_Command(0x21);
	OLED_Write_Command(0x20); // Set Memory Addressing Mode
	OLED_Write_Command(0x02);
	OLED_Write_Command(0xdb); // VCOM deselect level mode
	OLED_Write_Command(0x30);
	OLED_Write_Command(0xad); // master configuration
	OLED_Write_Command(0x00);
	OLED_Write_Command(0xa4); // out follows RAM content
	OLED_Write_Command(0xa6); // set normal display
	OLED_Write_Command(0xaf); // display on
}
/*
void OLED_Home(void) {
	OLED_Write_Command(0x00); // Ustaw kolumnê na 0
	OLED_Write_Command(0x10); // Wyzeruj wy¿sze bity kolumny
}

void OLED_Goto_Line(uint16_t line) {
	if (line < 8) { 
		OLED_Write_Command(0xB0 | line);
			}
}

void OLED_Goto_Column(uint16_t column) {
	OLED_Write_Command(0x00 | (column & 0x0F));
	OLED_Write_Command(0x10 | ((column >> 4) & 0x0F));
}

|void OLED_Pos(uint16_t row, uint16_t column) {
	OLED_Goto_Line(row);
	OLED_Goto_Column(column);
}

void OLED_Clear_Line(uint16_t line) {
	OLED_Goto_Line(line);
	for (int i = 0; i < 128; i++) {
		OLED_Write_Data(0x00);
	}
}

void OLED_Print_Char(char c) {
	if (c < 32 || c > 126) {
		c = 32;
			}
	const unsigned char* font = font8[c - 32];
	for (int i = 0; i < 8; i++) {
		OLED_Write_Data(pgm_read_byte(&font[i]));
			}
}

void OLED_Print(char* str) {
	while (*str) {
		OLED_Print_Char(*str++);
	}
}*/