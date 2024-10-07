/* === F_CPU and Bound Rate === */ 
#define FOCS 4915200UL
#define BAUND 9600
#define F_CPU	FOCS

#define ADC_BASE_ADDRESS 0x1400

/* === Include area === */
#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>

#include "UART_driver.h"
#include "Memory_driver.h"
#include "ADC_driver.h"
#include "OLED_driver.h"

/* === Define area === */

/* === Global definition area === */
JoystickCalibration calibration = {0,0,255,255,0,0};
int selected_page = 0;
/*Flag informing about if user is in subpage*/
int in_subpage = 0;


//////////////// SPI /////////////////////////


#define MCP_RESET      0xC0
#define MCP_READ       0x03
#define MCP_WRITE      0x02
#define MCP_BITMOD     0x05
#define MCP_READ_STATUS 0xA0



#define CS_MCP2515     PB4  // Chip Select for MCP2515 on Port B





void SPI_init(void) {
	// Set MOSI, SCK, and SS as output, MISO as input
	DDRB = (1<<PB5) | (1<<PB7) | (1<<PB4);  // MOSI: PB5, SCK: PB7, SS: PB4
	DDRB &= ~(1<<PB6);                      // MISO: PB6 as input
	// Enable SPI, set as Master, set clock rate (Fosc/16)
	SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0) ;
}


void SPI_transmit(char cData)
{
	PORTB &= ~(1<<CS_MCP2515);  // Set CS to low (select the MCP2515)
	/* Start transmission */
	SPDR = cData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))
	;
	PORTB |= (1<<CS_MCP2515);  // Set CS to high (deselect the MCP2515)
}

#define MCP_CANINTE  0x2B  // Interrupt Enable Register
#define MCP_RX0IE    0x01  // Enable receive interrupt for RXB0

void MCP2515_init_with_interrupt(void) {
	// Initialize SPI interface
	SPI_init();
	
	// Reset MCP2515 to put it in a known state
	MCP2515_reset();

	// Enable receive interrupt for RXB0 (Receive Buffer 0)
	MCP2515_write(MCP_CANINTE, MCP_RX0IE);

	// Set up ATmega162 external interrupt on INT0 (PD2 pin) for falling edge
	MCUCR |= (1 << ISC01);  // Configure INT0 to trigger on falling edge
	GICR |= (1 << INT0);    // Enable external interrupt INT0

	// Enable global interrupts
	sei();
}

// Function to reset the MCP2515
void MCP2515_reset(void) {
	SPI_transmit(MCP_RESET);       // Send reset command
}

// Function to write a byte to a specific MCP2515 register
void MCP2515_write(uint8_t address, uint8_t data) {
	SPI_transmit(MCP_WRITE);       // Send write command
	SPI_transmit(address);         // Send register address
	SPI_transmit(data);            // Send data byte
}

// Function to read a byte from a specific MCP2515 register
uint8_t MCP2515_read(uint8_t address) {
	uint8_t result;
	SPI_transmit(MCP_READ);        // Send read command
	SPI_transmit(address); 
	SPI_transmit(0x00);        // Send register address
	result = SPDR; 
	return result;                 // Return the read result
}

// Function to modify bits in a specific register (using bit-modify command)
void MCP2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
	SPI_transmit(MCP_BITMOD);      // Send bit-modify command
	SPI_transmit(address);         // Send register address
	SPI_transmit(mask);            // Send mask (bits to modify)
	SPI_transmit(data);            // Send data (new values for the bits)
}

uint8_t MCP2515_read_status(void) {
	SPI_transmit(MCP_READ_STATUS) ;
	uint8_t result = SPDR;
	return result; 
}





//////////////////////////////////////////////

int main(void) {
	Init();
	USART_Init(UBBR);
	FILE *uart_stream = fdevopen(USART_Transmit_Char, USART_Receive_Char);
	stdout = uart_stream;
	stdin = uart_stream;
	printf("Hreloo");
	
    // Initialize SPI
    SPI_init();

    while(1) {
		    // Reset MCP2515
		    MCP2515_reset();

		    // Write a value to a register (for example, to the CANCTRL register)
		    MCP2515_write(0x0F, 0xCC); 
		    uint8_t value = MCP2515_read(0x0F);
			printf("%d \n",value);
			_delay_ms(500);
	}
	return 0 ;
}




