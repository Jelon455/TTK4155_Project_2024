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
#include "IR_driver.h"

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
#define RANGE_DUTY_CYCLE (MAX_DUTY_CYCLE - MIN_DUTY_CYCLE) //1,2ms

void SysTick_Init(void);
void SysTick_Handler(void);
double map_joystick_to_duty_cycle(uint8_t joystick_value);
void Pin_PD10_Init(void);
void init_pin_pd9_as_input(void);
int read_pin_pd9(void);

static double duty_cycle = MIN_DUTY_CYCLE;
volatile uint8_t toggle_flag = 0;

int main(void) 
{
	SystemInit();
	WDT->WDT_MR = WDT_MR_WDDIS; // Disable Watchdog Timer
	uart_init(FOCS, BAUND);

	CanInit canInit = { .phase2 = BR_PHASE2, .propag = BR_PROP, .phase1 = BR_PHASE1, .sjw = BR_SJW, .brp = BR_BRP, .smp = 0 };
	can_init(canInit, 0);

	PWM_Init();
	Pin_PD10_Init();
	init_pin_pd9_as_input();
	SysTick_Init();
	IR_ADC_Init();
	uint32_t  value =0;
	uint8_t last_state = 0;       // Poprzedni stan wej�cia
	uint8_t stable_state = 0;     // Stabilny stan po debouncowaniu
	uint32_t point_count = 0; // Licznik punkt�w (zmian z 0 na 1)
	uint8_t bounce_count = 0; // Licznik odbi� (wszystkich zmian stanu)
	uint8_t current_state = 0;
	while (1) 
	{
		current_state = read_pin_pd9();
		CanMsg joystick_message;
		can_rx(&joystick_message);
//		printf("Message id 0x%x\r\n",joystick_message.id);
//		printf("Message: %d\r\n",joystick_message.byte[0]);
		for (volatile int i = 0; i<1000000;i++);
		// Sprawdzamy, czy stan pinu si� zmieni�

		// Sprawd�, czy stan pinu si� zmieni�
		if ((current_state != last_state) && current_state == 0) 
		{
			bounce_count++;
			for (volatile int i = 0; i<100;i++);
		}
		last_state = current_state;
		printf("SCORE: %d\n", bounce_count);
		
		if ((joystick_message.id == 0x22)) 
		{
			uint8_t joystick_x = joystick_message.byte[0];
			duty_cycle = map_joystick_to_duty_cycle(joystick_x);
		}
		value = IR_ADC_Read();
		printf("Hello I am node 2! \r\n");
		printf("ADC: %lu\n\r", value);
	}

}

int read_pin_pd9(void) 
{
	// Odczytujemy stan pinu PD9
	return (PIOD->PIO_PDSR & PIO_PDSR_P9) ? 1 : 0;
}
/*Function to map joystick data to duty cycle*/
double map_joystick_to_duty_cycle(uint8_t normalized_value)
{
    // Map normalized value to the duty cycle range
    return MIN_DUTY_CYCLE + (RANGE_DUTY_CYCLE * normalized_value);
}

void SysTick_Init(void)
{
	uint32_t reload_value = 1679999; // 20 ms przy 84 MHz

	SysTick->CTRL = 0;                        /* Wy��cz SysTick podczas konfiguracji */
	SysTick->LOAD = reload_value;             /* Ustaw warto�� prze�adowania */
	SysTick->VAL = 0;                         /* Reset warto�ci licznika */
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk // U�yj zegara procesora
	| SysTick_CTRL_TICKINT_Msk   // W��cz przerwanie SysTick
	| SysTick_CTRL_ENABLE_Msk;   // W��cz SysTick
}


// Interrupt handler for SysTick
void SysTick_Handler(void)
{
	//printf("\n\r Setting duty cycle based on joystick: %f\n\r", duty_cycle);
	PWM_Set_Duty_Cycle(duty_cycle);   
	if (toggle_flag) 
	{
		PIOD->PIO_SODR = PIO_SODR_P10; // Ustaw PD10 na wysoki
	} 
	else 
	{
		PIOD->PIO_CODR = PIO_CODR_P10; // Ustaw PD10 na niski
	}
	toggle_flag = !toggle_flag; // Prze��cz flag�		
}


void Pin_PD10_Init(void)
 {
	// W��cz zegar dla portu D
	PMC->PMC_PCER0 |= PMC_PCER0_PID14; // ID_PIOD to identyfikator PIOD

	// Ustaw PD10 jako wyj�cie
	PIOD->PIO_PER = PIO_PER_P10;      // W��cz kontrol� nad PD10
	PIOD->PIO_OER = PIO_OER_P10;      // Ustaw PD10 jako wyj�cie
	PIOD->PIO_CODR = PIO_CODR_P10;    // Ustaw pocz�tkowo na niski stan
}

void init_pin_pd9_as_input(void) 
{
	// W��czamy zegar dla PIOD
	PMC->PMC_PCER0 |= PMC_PCER0_PID14;

	// Ustawiamy PD9 jako wej�cie
	PIOD->PIO_PER |= PIO_PER_P9;   // W��czamy kontrol� portu PIO na pinie PD9
	PIOD->PIO_ODR |= PIO_ODR_P9;   // Ustawiamy PD9 jako wej�cie
	PIOD->PIO_PUER |= PIO_PUER_P9; // W��czamy rezystor pull-up na PD9 (opcjonalnie)
}