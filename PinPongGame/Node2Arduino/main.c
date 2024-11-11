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
#include "Motor_driver.h"
#include "Encoder_driver.h"

/*Defining the  value for CAN_BR DATASHEET: page 1193*/
#define BR_BRP		20   
#define BR_SJW		3   
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

#define MIN_DUTY_CYCLE  (0.9 / 20) //0,045
#define MAX_DUTY_CYCLE  (2.1 / 20) //0,105
#define RANGE_DUTY_CYCLE (MAX_DUTY_CYCLE - MIN_DUTY_CYCLE) //0,06

#define ERROR_SIZE 10  // Define the size of the error buffer


void SysTick_Init(void);
void SysTick_Handler(void);
double map_joystick_to_duty_cycle(uint8_t joystick_value);
void Pin_PD10_Init(void);
void init_pin_pd9_as_input(void);
int read_pin_pd9(void);
void Pin_PC18_Init(void);

static double duty_cycle = MIN_DUTY_CYCLE;

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
	
	int32_t errorBuffer[ERROR_SIZE];  // Buffer to store the filter values
	uint8_t errorIndex = 0;  // Index for the filter buffer
	int32_t ref = 0 ;
	int32_t u = 0 ;
	
	uint8_t joystick_x;
	double duty_cycle_motor = 0.0;
	
		
	while (1) 
	{
		current_state = read_pin_pd9();
		CanMsg joystick_message;
		can_rx(&joystick_message);
		//printf("Message id 0x%x\r\n",joystick_message.id);
		//printf("Message: %d\r\n",joystick_message.byte[0]);
		for (volatile int i = 0; i<500000;i++);

		if ((current_state != last_state) && current_state == 0) 
		{
			bounce_count++;
			for (volatile int i = 0; i<100;i++);
		}
		last_state = current_state;
		//printf("!!!!!!!!!!!!!!! SCORE:    %d    !!!!!!!!!!!!!!!!!\n", bounce_count);
		
		
		can_printmsg(joystick_message);
		if (joystick_message.id == 0x21)
		{
			uint8_t joystick_button = joystick_message.byte[0];
			joystick_x = joystick_message.byte[1];
			uint8_t joystick_y = joystick_message.byte[2];
			
			duty_cycle = map_joystick_to_duty_cycle(joystick_y);
			//printf("duty_cycle: %f! \r\n", duty_cycle);
			
			if (joystick_button == 0)
			{
				PIOC->PIO_SODR = PIO_SODR_P18;
				//printf("Solenoid activated\n");
			}
			else
			{
				PIOC->PIO_CODR = PIO_CODR_P18;
				//printf("Solenoid deactivated\n");
			}
		}
		//printf("Hello I am node 2! \r\n");
		encoder_value = Get_Encoder_Position();
		printf("ENCODER POSITION %lu \r\n", encoder_value);
		
		SimpleMotor(joystick_x, duty_cycle_motor);
		
		//ref = Motor_position(joystick_x,ref);
		//printf("Reference %d \r\n", ref);
		//u = PI_controller(ref, encoder_value);
		//printf("PI %d \r\n\n", u);
		//Motor_driving(u);
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

void SysTick_Init(void)
{
	uint32_t reload_value = 1679999; // 20 ms przy 84 MHz

	SysTick->CTRL = 0;                        /* Wy³¹cz SysTick podczas konfiguracji */
	SysTick->LOAD = reload_value;             /* Ustaw wartoœæ prze³adowania */
	SysTick->VAL = 0;                         /* Reset wartoœci licznika */
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk // U¿yj zegara procesora
	| SysTick_CTRL_TICKINT_Msk   // W³¹cz przerwanie SysTick
	| SysTick_CTRL_ENABLE_Msk;   // W³¹cz SysTick
}


// Interrupt handler for SysTick
void SysTick_Handler(void)
{
	PWM_Set_Duty_Cycle(duty_cycle); 	
}

void init_pin_pd9_as_input(void) 
{
	// W³¹czamy zegar dla PIOD
	PMC->PMC_PCER0 |= PMC_PCER0_PID14;

	// Ustawiamy PD9 jako wejœcie
	PIOD->PIO_PER |= PIO_PER_P9;   // W³¹czamy kontrolê portu PIO na pinie PD9
	PIOD->PIO_ODR |= PIO_ODR_P9;   // Ustawiamy PD9 jako wejœcie
	PIOD->PIO_PUER |= PIO_PUER_P9; // W³¹czamy rezystor pull-up na PD9 (opcjonalnie)
}

void Pin_PC18_Init(void)
 {
	// W³¹cz zegar dla portu D
	/*PMC->PMC_PCER0 |= PMC_PCER0_PID14; // ID_PIOD to identyfikator PIOD

	// Ustaw PD10 jako wyjœcie
	PIOD->PIO_PER = PIO_PER_P3;      // W³¹cz kontrolê nad PD10
	PIOD->PIO_OER = PIO_OER_P3;      // Ustaw PD10 jako wyjœcie
	
	
	PIOD->PIO_CODR = PIO_CODR_P3;    // Ustaw pocz¹tkowo na niski stan
*/	
	PMC->PMC_PCER0 |= (1 << ID_PIOC);
	PIOC->PIO_PER |= PIO_CODR_P18;
	PIOC->PIO_OER |= PIO_CODR_P18;
	
}



