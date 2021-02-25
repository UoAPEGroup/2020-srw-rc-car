/*
 * pwm.c
 *
 * Created: 29/01/2021 1:33:43 pm
 *  Author: npat692
 */ 

#include "global.h"

#include "pwm.h"

// DEBUGGING
#include "uart.h" //
#include <stdio.h>//
#include <string.h>//
char input_buffer[20];//

// IN1 PWM Initialization (33kHz)
void pwm1_init(){
	DDRB |= (1 << PORTB1); // Output PB1
	TCCR1A |= (1 << WGM10);  // Fast PWM
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS10); // ~33kHz
	OCR1A = 0; // Sets Duty Cycle to 0%
	OCR1B = 255;
	IN_1_OFF;
	//IN_1_ON; //  Toggle OC1A on compare match
}

// IN2 PWM Initialization (33kHz)
void pwm0_init(){
	DDRD |= (1 << PORTD6); // Output PD6
	TCCR0A |= (1 << WGM01)|(1 << WGM00); // Fast PWM
	TCCR0B |= (1 << CS00); // ~33kHz
	OCR0A = 0; // Sets Duty Cycle to 0%
	OCR0B = 255;
	IN_2_OFF; //  Toggle OC0A on compare match
	//IN_2_ON;
}

// Turns OFF IN1 and turns ON IN2 (Goes RIGHT)
// Then sets the duty cycle
 void set_duty_cycle_IN2(uint16_t value){
 	IN_1_OFF;
 	
 	calculate_t_on(value);
 	
	//sprintf(input_buffer, "Set Output IN2:	%i \n\r", t_on);
	//usart0_transmit_string(input_buffer);
	
	OCR0A = t_on; // Sets Duty Cycle
	OCR1A = 0;
	 
 	IN_2_ON;
 }
 
// Turns OFF IN2 and turns ON IN1 (Goes LEFT)
// Then sets the duty cycle
 void set_duty_cycle_IN1(uint16_t value){
	IN_2_OFF; // Turns OFF IN2
	 
	calculate_t_on(value); // Calculates the required duty cycle
	 
	//sprintf(input_buffer, "Set Output IN1:	%i \n\r", t_on);
	//usart0_transmit_string(input_buffer);

	OCR1A = t_on; // Sets Duty Cycle
	OCR0A = 0;
	 
	IN_1_ON; // Turns ON IN1
 }
 
 // Calculates the duty cycle 
 void calculate_t_on(uint16_t value){
// 	sprintf(input_buffer, "Value:	%i \n\r", value);
// 	usart0_transmit_string(input_buffer);
	
 	t_on =	(((uint32_t)value * 255)/VREF); // Calculates duty cycle
	 
//  	sprintf(input_buffer, "T_On:	%i \n\r", t_on);
//   	usart0_transmit_string(input_buffer);
 	
 	if (t_on > 255){ // Prevent overflow
 		t_on = 255;
	} 
	
 }
