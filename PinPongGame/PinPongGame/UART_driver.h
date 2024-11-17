/*
 * UART_driver.h
 */

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
#define ADC_BASE_ADDRESS 0x1400

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

/* === Function declaration === */
void USART_Init(unsigned int ubbr);
void USART_Transmit( unsigned char data );
void USART_Msg(char *str);
char USART_Receive(void);
int USART_Transmit_Char(char c, FILE *stream);
int USART_Receive_Char(FILE *stream);
/* === End function declaration === */

#endif /* UART_DRIVER_H_ */