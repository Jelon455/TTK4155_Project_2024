/*
 * SPI_driver.h
 *
 * Created: 07.10.2024 13:42:36
 *  Author: Julia
 */ 


#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_

/* === Include area === */
#include <avr/io.h>

/* === Define area === */
/* PB4 is used as Slave Select (SS)*/
#define SS_PIN     PB4

/*Macro to enable the slave*/
#define SET_SLAVE()  (PORTB &= ~(1 << SS_PIN))

/*Macro to disable the slave (set SS high)*/
#define CLEAR_SLAVE() (PORTB |= (1 << SS_PIN))

/* === Function declaration === */
void SPI_Master_Init(void);
void SPI_Master_Transmit(char cData);
void SPI_Slave_Init(void);
char SPI_Slave_Receive(void);



#endif /* SPI_DRIVER_H_ */