/*
 * Node2Arduino.c
 *
 */ 
/* === F_CPU and Bound Rate === */
#define FOCS 84000000UL
#define BAUND 9600
#define F_CPU	FOCS

#include <stdio.h> 

#include "sam.h"
#include "uart.h"
#include "can.h"
#include "pwm.h"

/*Defining the  value for CAN_BR DATASHEET: page 1193*/
#define BR_BRP	20   
#define BR_SJW	3   
#define BR_PROP		2   
#define BR_PHASE1	1  
#define BR_PHASE2	1 


/*Defining the starts point on CAN Baudrate Register CAN_BR*/
#define BR_BRP_Pos		16
#define BR_SJW_Pos		12
#define BR_PROPAG_Pos	8
#define BR_PHASE1_Pos	4
#define BR_PHASE2_Pos	0

#define F_CHANNEL_1_CLOCK  (CHIP_FREQ_CPU_MAX / 1024)
#define PERIOD (20.0 / 1000)
#define CPRD  ((uint32_t)(PERIOD * F_CHANNEL_1_CLOCK))

#define MIN_DUTY_CYCLE  (0.9 / 20)
#define MAX_DUTY_CYCLE  (2.1 / 20)
#define RANGE_DUTY_CYCLE (MAX_DUTY_CYCLE - MIN_DUTY_CYCLE)

/*Function to map joystick data to duty cycle*/
double map_joystick_to_duty_cycle(uint8_t joystick_value) 
{
	return MIN_DUTY_CYCLE + (RANGE_DUTY_CYCLE * (joystick_value / 255.0));
}

int main(void) {
	SystemInit();
	WDT->WDT_MR = WDT_MR_WDDIS; // Disable Watchdog Timer
	uart_init(FOCS, BAUND);

	CanInit canInit = { .phase2 = BR_PHASE2, .propag = BR_PROP, .phase1 = BR_PHASE1, .sjw = BR_SJW, .brp = BR_BRP, .smp = 0 };
	can_init(canInit, 0);

	PWM_Init();
	while (1) {
		printf("Hello I am node 2! \r\n");
		// Create a structure to store the received CAN message
		CanMsg joystick_message;
		can_rx(&joystick_message);
		can_printmsg(joystick_message);
		if (can_rx(&joystick_message) && joystick_message.id == 0x22) {
			uint8_t joystick_x = joystick_message.byte[0];  // Read x-axis position from CAN message
			double duty_cycle = map_joystick_to_duty_cycle(joystick_x);

			printf("Setting duty cycle based on joystick: %f\n", duty_cycle);
			PWM_Set_Duty_Cycle(duty_cycle);
			for(volatile int i = 0; i < 100000; i++);
		}
	}
}
