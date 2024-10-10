/*
 * CAN_connection.h
 *
 * Created: 07.10.2024 12:51:45
 *  Author: mazan
 */ 


#ifndef CAN_CONNECTION_H_
#define CAN_CONNECTION_H_

#include <avr/io.h>

/* SPI Function Prototypes */
void SPI_Init(void);
uint8_t SPI_Transmit(uint8_t data);
void SPI_Select(void);
void SPI_Deselect(void);






#endif /* CAN_CONNECTION_H_ */