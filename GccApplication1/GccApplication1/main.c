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
#define UBBR (FOCS /(16UL * BAUND)) - 1
#define LATCH_ADDRESS 0x8000

/* === Function declaration === */
void USART_Init(unsigned int ubbr);
void USART_Transmit( unsigned char data );
void USART_Msg(char *str);
char USART_Receive(void);
int USART_Transmit_Char(char c, FILE *stream);
int USART_Receive_Char(FILE *stream);

void write_to_latch(uint8_t data);
void init_external_memory_interface();
void SRAM_test(void);

/* === Global variable === */
char tx_data;

/*int main(void)
{
	USART_Init(UBBR);
	
    FILE *uart_stream = fdevopen(USART_Transmit_Char, USART_Receive_Char);
	
    stdout = uart_stream;
    stdin = uart_stream;
	
	printf("I am printf!\n\r");
	
    while (1)
	{
		tx_data = USART_Receive();
		USART_Transmit(tx_data);
		USART_Transmit(0x0d);
    }
}*/

int main(void) {
	// Initialize the external memory interface
	init_external_memory_interface();
	// Main loop

		// Example: Blink LEDs by writing different values to the latch
/*while(1){
		// Turn on LEDs in pattern 1
		write_to_latch(0b10101010);  // Set alternate LEDs on/off
		_delay_ms(500);  // Delay for 500 milliseconds

		// Turn on LEDs in pattern 2
		write_to_latch(0b01010101);  // Invert the LED pattern
		_delay_ms(500);  // Delay for 500 milliseconds

		// Repeat the pattern with different values
		write_to_latch(0xFF);  // Turn all LEDs on
		_delay_ms(500);  // Delay for 500 milliseconds

		write_to_latch(0x00);  // Turn all LEDs off
		_delay_ms(500);  // Delay for 500 milliseconds
		
}*/
	USART_Init(UBBR);
	
	FILE *uart_stream = fdevopen(USART_Transmit_Char, USART_Receive_Char);
	
	stdout = uart_stream;
	stdin = uart_stream;
	
	printf("I am printf!\n\r");
	
	SRAM_test();
	
	while(1){;}
		
	return 0;  // This line is never reached
}
/* === Prototype Declaration === */
void USART_Init(unsigned int ubbr)
{
	/*Baund rate*/
	UBRR0H = (unsigned char)(ubbr>>8);
	UBRR0L = (unsigned char)ubbr;
	UCSR0A &= ~(1 << U2X0);
	/*Transmission settings*/
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	/*Set frame format USBS0 - 1 stop bit  ; UCSZ00 - 8data*/
	UCSR0C = (1<<URSEL0) | (0<<USBS0) | (3<<UCSZ00);
}
void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1 << UDRE0)) )
	;
	/* Put data into buffer, sends the data */
	UDR0 = data;
}
void USART_Msg(char *str)
{
	while(*str)
	{
		USART_Transmit(*str);
		str++;
	}
}
char USART_Receive(void)
{
	/*Do nothing until data have been recieved and is ready to be read from UDR*/
	while (!(UCSR0A & (1<<RXC0)));
	return UDR0;
}
int USART_Transmit_Char(char c, FILE *stream)
{
	if (c == '\n')
	{
		USART_Transmit('\r');
	}
	USART_Transmit(c);
	return 0;
}
int USART_Receive_Char(FILE *stream)
{
	/* Wait until data is received */
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}


void init_external_memory_interface() {
    // Set up the external memory interface
    MCUCR |= (1 << SRE);  // Enable the external memory interface
	/*Disabled JTag pins on port C*/
	SFIOR |= (1 << XMM2);
}

void write_to_latch(uint8_t data) {
	// Write data to the specified address in external memory space
	*(volatile uint8_t *)LATCH_ADDRESS = data;
}

void SRAM_test(void)
{
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint16_t ext_ram_size = 0x800;
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf("Starting SRAM test...\n");
	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			write_errors++;
		}
	}
	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed);
	// reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n", i, retreived_value, some_value);
			retrieval_errors++;
		}
	}
	printf("SRAM test completed with \n%4d errors in write phase and \n%4d errors in retrieval phase\n\n", write_errors, retrieval_errors);
}
/* === End od Prototype declaration === */
