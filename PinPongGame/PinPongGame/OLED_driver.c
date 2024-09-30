/*
 * OLED_driver.c
 */

/* === Include area === */
#include "OLED_driver.h"
#include "Memory_driver.h"
#include "fonts.h"


/* === Function definition === */
void OLED_Write_Command(uint8_t data) {
	XMEM_Write(data, OLED_COMMAND_ADDRESS);
}

void OLED_Write_Data(uint8_t data) {
	XMEM_Write(data, OLED_DATA_ADDRESS);
}
void OLED_Clear() {
	for (uint8_t page = 0; page < 8; page++) {
		OLED_Write_Command(0xB0 + page);  // Set page start address
		OLED_Write_Command(0x00);         // Set lower column start address
		OLED_Write_Command(0x10);         // Set higher column start address

		for (uint8_t column = 0; column < 128; column++) {
			OLED_Write_Data(0x00);  // Clear all pixels on this page
		}
	}
}
void OLED_Init() {
	// Step 1: Reset OLED (if there is a reset pin connected)
	// Set the Reset Pin (if applicable)
	// Example:
	// DDRD |= (1 << PD0); // Set PD0 as output (Reset pin)
	// PORTD &= ~(1 << PD0); // Set PD0 low to reset
	// _delay_ms(10);  // Delay for 10ms
	// PORTD |= (1 << PD0); // Set PD0 high to release from reset
	// _delay_ms(10);  // Delay for 10ms

	// Step 2: Initialization sequence for SSD1308 (or similar OLED driver)

	OLED_Write_Command(0xAE);  // Display OFF (sleep mode)

	OLED_Write_Command(0xD5);  // Set display clock divide ratio/oscillator frequency
	OLED_Write_Command(0x80);  // Suggested value (0x80)

	OLED_Write_Command(0xA8);  // Set multiplex ratio (1 to 64)
	OLED_Write_Command(0x3F);  // 1/64 duty

	OLED_Write_Command(0xD3);  // Set display offset
	OLED_Write_Command(0x00);  // No offset

	OLED_Write_Command(0x40);  // Set display start line (0x40 -> line 0)

	OLED_Write_Command(0x8D);  // Enable charge pump regulator
	OLED_Write_Command(0x14);  // Enable charge pump

	OLED_Write_Command(0xA1);  // Set segment re-map (A0 for normal, A1 for reversed)

	OLED_Write_Command(0xC8);  // Set COM output scan direction (C0 for normal, C8 for remapped)

	OLED_Write_Command(0xDA);  // Set COM pins hardware configuration
	OLED_Write_Command(0x12);  // Alternative COM pin configuration, disable left/right remap

	OLED_Write_Command(0x81);  // Set contrast control
	OLED_Write_Command(0xCF);  // Contrast value (can adjust for brightness)

	OLED_Write_Command(0xD9);  // Set pre-charge period
	OLED_Write_Command(0xF1);  // Pre-charge period

	OLED_Write_Command(0xDB);  // Set VCOMH deselect level
	OLED_Write_Command(0x40);  // VCOMH deselect level

	OLED_Write_Command(0xA4);  // Entire display ON (A4 - follow RAM content, A5 - ignore RAM content)

	OLED_Write_Command(0xA6);  // Set normal display (A6 - normal, A7 - inverse)

	OLED_Write_Command(0x20);  // Set Memory Addressing Mode
	OLED_Write_Command(0x02);  // Page Addressing Mode

	OLED_Write_Command(0xAF);  // Display ON in normal mode

	// Clear the display
	OLED_Clear();
}
void OLED_SetCursor(uint8_t page, uint8_t column) {
	// Set the page address (B0h to B7h for page 0 to 7)
	OLED_Write_Command(0xB0 | (page & 0x07));
	
	// Set the lower column address (00h to 0Fh)
	OLED_Write_Command(0x00 | (column & 0x0F));
	
	// Set the higher column address (10h to 1Fh)
	OLED_Write_Command(0x10 | ((column >> 4) & 0x0F));
}

void OLED_Write_Char(char c, uint8_t page, uint8_t column) {
	// Calculate the position of the character in the font8 array
	uint8_t char_index = c - 32;  // ' ' (space) starts at index 0

	// Set the cursor to the desired page and column
	OLED_SetCursor(page, column);

	// Write the character bitmap to the display (8 bytes per character)
	for (uint8_t i = 0; i < 8; i++) {
		OLED_Write_Data(pgm_read_byte(&font8[char_index][i]));
	}
}
// Function to clear the OLED display

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