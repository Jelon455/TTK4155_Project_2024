/*
 * Node2Arduino.c
 *
 */ 
/* === F_CPU and Bound Rate === */
#define FOCS 84000000UL
#define BAUND 9600
#define F_CPU	FOCS

/* === Include area === */
#include <stdio.h> 

#include "sam.h"
#include "uart.h"
#include "can.h"
#include "pwm.h"
#include "Motor_driver.h"
#include "Encoder_driver.h"

/* === Define area === */
/*Defining the  value for CAN_BR DATASHEET: page 1193*/
#define BR_BRP		20   
#define BR_SJW		3   
#define BR_PROP		2   
#define BR_PHASE1	1  
#define BR_PHASE2	1 

#define F_CHANNEL_1_CLOCK  (CHIP_FREQ_CPU_MAX / 1024)
#define PERIOD (20.0 / 1000)
#define CPRD  ((uint32_t)(PERIOD * F_CHANNEL_1_CLOCK))

#define MIN_DUTY_CYCLE  (0.9 / 20) //0,045
#define MAX_DUTY_CYCLE  (2.1 / 20) //0,105
#define RANGE_DUTY_CYCLE (MAX_DUTY_CYCLE - MIN_DUTY_CYCLE) //0,06

/*Defining clock parameters for the PWM signal of the motor*/
#define F_CHANNEL_0_CLOCK  (CHIP_FREQ_CPU_MAX / 1024) //clock frequency for the PWM signal
#define CPRD_MOTOR  ((uint32_t)(0.00004 * F_CHANNEL_0_CLOCK))//frequency of the PWM signal is 25 kHz


void SysTick_Init(void);
void SysTick_Handler(void);
double map_joystick_to_duty_cycle(uint8_t joystick_value);
void Pin_PD10_Init(void);
void init_pin_pd9_as_input(void);
int read_pin_pd9(void);
void Pin_PC18_Init(void);

static double duty_cycle = MIN_DUTY_CYCLE;
static uint8_t joystick_x;
static int32_t ref = 0 ;

int main(void) 
{
	SystemInit();
	WDT->WDT_MR = WDT_MR_WDDIS;
	uart_init(FOCS, BAUND);

	CanInit canInit = { .phase2 = BR_PHASE2, .propag = BR_PROP, .phase1 = BR_PHASE1, .sjw = BR_SJW, .brp = BR_BRP, .smp = 0 };
	can_init(canInit, 0);

	PWM_Init();
	init_pin_pd9_as_input();
	Pin_PC18_Init();
	SysTick_Init();
	Encoder_Init();
	PWM_Motor_Init();

	uint8_t last_state = 0;
	uint8_t bounce_count = 0;
	uint8_t current_state = 0;
	uint32_t encoder_value = 1;
	
	PIOC->PIO_SODR = PIO_PC23;
	PWM->PWM_CH_NUM[0].PWM_CDTY = (uint32_t)(0.9 * CPRD_MOTOR);
	for (volatile int i = 0; i<500000;i++);
	PWM->PWM_CH_NUM[0].PWM_CDTY = (uint32_t)(0 * CPRD_MOTOR);
	Reset_Encoder_Position();
	encoder_value = Get_Encoder_Position();
	CanMsg IR_message;
	IR_message.id = 0x11;
	IR_message.length = 1;
	IR_message.byte[0] = 0;
	while (1) 
	{
		current_state = read_pin_pd9();
		CanMsg joystick_message;
		can_rx(&joystick_message);
		for (volatile int i = 0; i<30000;i++);

		if ((current_state != last_state) && current_state == 0) 
		{
			bounce_count++;
			for (volatile int i = 0; i<100;i++);
			IR_message.byte[0] = 1;
			can_tx(IR_message);
		}
		last_state = current_state;

		if (joystick_message.id == 0x21)
		{
			uint8_t joystick_button = joystick_message.byte[0];
			joystick_x = joystick_message.byte[1];
			uint8_t joystick_y = joystick_message.byte[2];
			duty_cycle = map_joystick_to_duty_cycle(joystick_y);
					
			if (joystick_button == 0)
			{
				PIOC->PIO_SODR = PIO_SODR_P18;
			}
			else
			{
				PIOC->PIO_CODR = PIO_CODR_P18;
			}
		}
	}

}

int read_pin_pd9(void) 
{
	return (PIOD->PIO_PDSR & PIO_PDSR_P9) ? 1 : 0;
}

/*Function to map joystick data to duty cycle*/
double map_joystick_to_duty_cycle(uint8_t adc_value)
{
    return MIN_DUTY_CYCLE + (RANGE_DUTY_CYCLE * ((double)adc_value/255.0));
}

/*Timer interrupt*/
void SysTick_Init(void)
{
	/*20 ms clock 84 MHz*/
	uint32_t reload_value = 1679999; 

	SysTick->CTRL = 0;
	SysTick->LOAD = reload_value;
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk
	| SysTick_CTRL_TICKINT_Msk 
	| SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler(void)
{
	PWM_Set_Duty_Cycle(duty_cycle);
	uint32_t encoder_value = Get_Encoder_Position();
	ref = Motor_position(joystick_x,ref);
	float u = PI_controller(ref, encoder_value);
	Motor_driving(u);
	
}

void init_pin_pd9_as_input(void) 
{
	PMC->PMC_PCER0 |= PMC_PCER0_PID14;
	/* PD9 as input*/
	PIOD->PIO_PER |= PIO_PER_P9;
	PIOD->PIO_ODR |= PIO_ODR_P9;
	PIOD->PIO_PUER |= PIO_PUER_P9;
}

void Pin_PC18_Init(void)
 {
	PMC->PMC_PCER0 |= (1 << ID_PIOC);
	PIOC->PIO_PER |= PIO_CODR_P18;
	PIOC->PIO_OER |= PIO_CODR_P18;
}



