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

int main(void)
{
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS;        // Disable Watchdog Timer
	uart_init(FOCS, BAUND);


    double duty_cycle = 0.045;
	CanInit canInit;
	canInit.phase2 = BR_PHASE2;
	canInit.propag = BR_PROP;  
	canInit.phase1 = BR_PHASE1;
	canInit.sjw = BR_SJW;      
	canInit.brp = BR_BRP;      
	canInit.smp = 0;
    /* Initialize CAN with predefined timing */
    can_init(canInit, 0);
	
	CanMsg test_message;
	test_message.id = 0x15;
	test_message.length = 8;
	test_message.byte[0] = 0x11;
	test_message.byte[1] = 0x22;
	test_message.byte[2] = 0x33;
	test_message.byte[3] = 0x44;
	test_message.byte[4] = 0x55;
	test_message.byte[5] = 0x66;
	test_message.byte[6] = 0x77;
	test_message.byte[7] = 0x88;
	PWM_Init();
    while (1) 
    {
//		printf("HELLO I am NODE 2!\n\r");
//		can_rx(&test_message);
//		can_printmsg(test_message);
  		if (duty_cycle >= 0.105)
        {
	        duty_cycle = 0.045;
        }
		PWM_Set_Duty_Cycle(duty_cycle);
		/*increase the duty cycle*/
		duty_cycle = duty_cycle + 0.01;
		/*delay*/
		for(volatile int i = 0; i < 1000000; i++);
		printf("duty cycle = %lu\n\r", CPRD);
		
    }
}
