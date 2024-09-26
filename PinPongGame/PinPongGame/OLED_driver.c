/*
 * OLED_driver.c
 */

/* === Include area === */
#include "OLED_driver.h"

/* === Function definition === */
void OLED_Init(void)
{
	 XMEM_Write(0xae, 0x1000); // display off
	 XMEM_Write(0xa1, 0x1000); //segment remap
	 XMEM_Write(0xda, 0x1000); //common pads hardware: alternative
	 XMEM_Write(0x12, 0x1000);
	 XMEM_Write(0xc8, 0x1000); //common output scan direction:com63~com0
	 XMEM_Write(0xa8, 0x1000); //multiplex ration mode:63
	 XMEM_Write(0x3f, 0x1000);
	 XMEM_Write(0xd5, 0x1000); //display divide ratio/osc. freq. mode
	 XMEM_Write(0x80, 0x1000);
	 XMEM_Write(0x81, 0x1000); //contrast control
	 XMEM_Write(0x50, 0x1000);
	 XMEM_Write(0xd9, 0x1000); //set pre-charge period
	 XMEM_Write(0x21, 0x1000);
	 XMEM_Write(0x20, 0x1000); //Set Memory Addressing Mode
	 XMEM_Write(0x02, 0x1000);
	 XMEM_Write(0xdb, 0x1000); //VCOM deselect level mode
	 XMEM_Write(0x30, 0x1000);
	 XMEM_Write(0xad, 0x1000); //master configuration
	 XMEM_Write(0x00, 0x1000);
	 XMEM_Write(0xa4, 0x1000); //out follows RAM content
	 XMEM_Write(0xa6, 0x1000); //set normal display
	 XMEM_Write(0xaf, 0x1000); // display on
}
