/*
 * SPI_driver.h
 */ 

/* === Include area === */
#include <avr/io.h>

#ifndef CAN_CONNECTION_H_
#define CAN_CONNECTION_H_

/* === Function declaration === */
void SPI_Init(void);
void SPI_Write(char data);
void SPI_Select(void);
void SPI_Deselect(void);
char SPI_Read(void);

#endif /* CAN_CONNECTION_H_ */