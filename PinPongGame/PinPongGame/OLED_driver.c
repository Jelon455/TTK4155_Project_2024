/*
 * OLED_driver.c
 */

/* === Include area === */
#include "OLED_driver.h"
#include "Memory_driver.h"
#include "fonts.h"
#include "ADC_driver.h"
#include "CAN_driver.h"
#include "CAN_control.h"

/* === Define area === */
#define NUM_PAGES 3
#define RETURN_OPTION 1

/* === Global definition area === */
static JoystickCalibration calibration = {0,0,255,255,0,0};
static int selected_page = 0;
/*Flag informing about if user is in subpage*/
static int in_subpage = 0; 

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
	OLED_Clear();
	OLED_Set_Page(3);
	OLED_Write_Command(0x03); 
	OLED_Write_Command(0x10);
	
	for (uint8_t i = 0; i < 128; i++) 
	{ 
		OLED_Write_Data(0xFF); 	
	}
		_delay_ms(1000);
}

void Display_Menu(int selected_page)
{
	
	OLED_Write_String("     MAIN MENU",0,0);
	OLED_Write_String(" Start Game",1,0);
	OLED_Write_String(" Rules",2,0);
	OLED_Write_String(" Info",3,0);

	if (selected_page == 0)
	{
		OLED_Write_String(">",1,0);
	}
	else if (selected_page == 1)
	{
		OLED_Write_String(">",2,0);
	}
	else if (selected_page == 2)
	{
		OLED_Write_String(">",3,0);
	}
}

void Display_Subpage(int page)
{
	OLED_Clear();

	if (page == 0)
	{
		OLED_Clear();
		Play_Game();
	}
	else if (page == 1)
	{
		OLED_Clear();
		OLED_Write_String("Rules:", 0, 0);
		OLED_Write_String(" ", 1, 0);
		OLED_Write_String("1)Do not let the", 2, 0);
		OLED_Write_String("ball through", 3, 0);
		OLED_Write_String(" ", 4, 0);
		OLED_Write_String("2)Survive as", 5, 0);
		OLED_Write_String("long as possible", 6, 0);
		OLED_Write_String("> Back", 8, 0);
	}
	else if (page == 2)
	{
		OLED_Clear();
		OLED_Write_String("The game is ", 0, 0);
		OLED_Write_String("created by ", 1, 0);
		OLED_Write_String("group 3 ", 2, 0);
		OLED_Write_String("For course", 3, 0);
		OLED_Write_String("TTK4155", 4, 0);	
		OLED_Write_String("> Back", 8, 0);
	}
	
}

void Go_To_Page(int page)
{
	in_subpage = 1;
	Display_Subpage(page);

	while (in_subpage)
	{
		JoystickPosition pos = Get_Joystick_Position(calibration);

		if (strcmp(Get_Joystick_Direction(pos), "UP") == 0 || strcmp(Get_Joystick_Direction(pos), "DOWN") == 0)
		{
			Display_Subpage(page);
			_delay_ms(10);
		}
		if ((PINB & (1 << PINB2)) == 0)
		{
			in_subpage = 0;
			OLED_Clear();
		}
	}
	Display_Menu(selected_page);
}

void Menu_Navigation() 
{
	JoystickPosition pos = Get_Joystick_Position(calibration);

	if (strcmp(Get_Joystick_Direction(pos), "UP") == 0)
	{
		selected_page = (selected_page - 1 + NUM_PAGES) % NUM_PAGES;
		Display_Menu(selected_page);
		_delay_ms(100);
	}
	else if (strcmp(Get_Joystick_Direction(pos), "DOWN") == 0)
	{
		selected_page = (selected_page + 1) % NUM_PAGES;
		Display_Menu(selected_page);
		_delay_ms(100);
	}

	if ((PINB & (1 << PINB2)) == 0)
	{
		Go_To_Page(selected_page);
	}
}

void Play_Game(void)
{
	uint16_t score = 0;
	uint16_t last_displayed_score = 0;
	uint8_t game_over = 0;

	/*timer 1 Set prescaler to 1024*/
	TCCR0 |= (1 << CS02) | (1 << CS00); 
	/*Resset*/
	TCNT0 = 0;
	uint16_t last_timer_value = TCNT0;

	OLED_Clear();
	OLED_Write_String("Your Score: ", 2, 10);

	while (!game_over) 
	{
		/*check timer for score increment*/
		uint16_t current_timer_value = TCNT0;
		if ((current_timer_value - last_timer_value) >= 240) 
		{ 
			/*more or less add 1 every 500ms*/
			last_timer_value = current_timer_value;
			score++;
		}
		if (score != last_displayed_score) 
		{
			char score_text[16];
			snprintf(score_text, sizeof(score_text), " %d", score);
			OLED_Write_String(score_text, 3, 30);
			last_displayed_score = score;
		}

		/*send joystick position and button state to Node 2*/
		CanMsg joystick_message;
		joystick_message.id = 0x21;
		joystick_message.length = 3;
		joystick_message.byte[0] = Joystick_Pushed();
		joystick_message.byte[1] = ADC_Read(ADC_CHANNEL_X);
		joystick_message.byte[2] = ADC_Read(ADC_CHANNEL_Y);
		
		CAN_Send_Message(&joystick_message);

		/*game over message*/
		CanMsg received_msg;
		CAN_Receive_Message(&received_msg);
		if (received_msg.id == 0x11 && received_msg.byte[0] == 1)
		{
			game_over = 1;
		}
	}

	/*Display Game Over screen*/
	OLED_Clear();
	char game_over_text[16];
	snprintf(game_over_text, sizeof(game_over_text), "SCORE: %d", score);
	OLED_Write_String("GAME OVER", 3, 0);
	OLED_Write_String(game_over_text, 4, 0);
	OLED_Write_String("> Back", 8, 0);

	// Wait for user input to return to menu
	while (1) 
	{
		if ((PINB & (1 << PINB2)) == 0) 
		{
			OLED_Clear();
			Display_Menu(0);
			break;
		}
	}
}
