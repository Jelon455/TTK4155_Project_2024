/*
 * OLED_driver.c
 */

/* === Include area === */
#include "OLED_driver.h"
#include "Memory_driver.h"
#include "fonts.h"


/* === Function definition === */
void OLED_Write_Command(uint8_t data) 
{
	XMEM_Write(data, OLED_COMMAND_ADDRESS);
}

void OLED_Write_Data(uint8_t data) 
{
	XMEM_Write(data, OLED_DATA_ADDRESS);
}

void OLED_Init() 
{
 OLED_Write_Command(0xae); // display off
 OLED_Write_Command(0xa1); //segment remap
 OLED_Write_Command(0xda); //common pads hardware: alternative
 OLED_Write_Command(0x12);
 OLED_Write_Command(0xc8); //common output scan direction:com63~com0
 OLED_Write_Command(0xa8); //multiplex ration mode:63
 OLED_Write_Command(0x3f);
 OLED_Write_Command(0xd5); //display divide ratio/osc. freq. mode
 OLED_Write_Command(0x80);
 OLED_Write_Command(0x81); //contrast control
 OLED_Write_Command(0x50);
 OLED_Write_Command(0xd9); //set pre-charge period
 OLED_Write_Command(0x21);
 OLED_Write_Command(0x20); //Set Memory Addressing Mode
 OLED_Write_Command(0x02);
 OLED_Write_Command(0xdb); //VCOM deselect level mode
 OLED_Write_Command(0x30);
 OLED_Write_Command(0xad); //master configuration
 OLED_Write_Command(0x00);
 OLED_Write_Command(0xa4); //out follows RAM content
 OLED_Write_Command(0xa6); //set normal display
 OLED_Write_Command(0xaf); // display on
}
void OLED_Clear()
{
	for (uint8_t page = 0; page < 8; page++)
	{
		OLED_Write_Command(0xB0 + page);  // Set page start address
		OLED_Write_Command(0x00);         // Set lower column start address
		OLED_Write_Command(0x10);         // Set higher column start address

		for (uint8_t column = 0; column < 128; column++)
		{
			OLED_Write_Data(0x00);  // Clear all pixels on this page
		}
	}
}

void OLED_Set_Page(uint8_t page)
{
	OLED_Write_Command(0xB0 | page);
}

void OLED_Set_Column(uint8_t column)
{
	/*Masking lower bits*/
	OLED_Write_Command(0x00 | (column & 0x0F));
	/*Masking upper bits*/
	OLED_Write_Command(0x10 | (column >> 4));
}
void OLED_Set_Cursor(uint8_t page, uint8_t column) 
{
	OLED_Set_Page(page);
	OLED_Set_Column(column);
}

void OLED_Write_Char(char c, uint8_t page, uint8_t column) 
{
	uint8_t char_index = c - 32;

	OLED_Set_Cursor(page, column);

	for (uint8_t i = 0; i < 8; i++) 
	{
		OLED_Write_Data(pgm_read_byte(&font8[char_index][i]));
		_delay_ms(10);
	}
}
void OLED_Write_String(const char* str, uint8_t page, uint8_t column) 
{
	uint8_t currentPage = page;
	uint8_t currentCol = column;

	while (*str) 
	{
		OLED_Write_Char(*str++, currentPage, currentCol);
		currentCol += 8;

		if (currentCol > 120) 
		{
			currentCol = 0;
			currentPage++;
			
			if (currentPage > 7) 
			{
				currentPage = 0;
			}
		}
	}
}
int OLED_putchar(char c, FILE *stream) 
{
	static uint8_t page = 0;
	static uint8_t column = 0;

	if (c == '\n') 
	{
		page++;
		column = 0;
	} 
	else if (c == '\r') 
	{
			column = 0;
	} 
	else 
	{
			OLED_Write_Char(c, page, column);
			/*Assuming 8-pixel wide font*/
			column += 8;
	}
	return 0;
}

void OLED_Test_Pixel() 
{
	OLED_Clear();  // Clear the display first

	// Try writing some test data
	OLED_Set_Page(3);
	OLED_Write_Command(0x03);         // Set lower column start address
	OLED_Write_Command(0x10);         // Set higher column start address
	
	for (uint8_t i = 0; i < 128; i++) 
	{ 
		OLED_Write_Data(0xFF); 	
	}
		_delay_ms(1000);
}