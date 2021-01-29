/*
 * pwm.c
 *
 * Created: 29/01/2021 1:33:43 pm
 *  Author: npat692
 */ 

#include "pwm.h"
#include "global.h"

void pwm0_init(){
	DDRD |= (1 << PORTD6); // Output PD6
	TCCR0A |= (1 << COM0A1); //  Toggle OC0A on compare match
	TCCR0A |= (1 << WGM01)|(1 << WGM00); // Fast PWM
	TCCR0B |= (1 << CS00); // 8 Prescale = 4kHz
	OCR0A = 100; // Sets Duty Cycle to 0%
	OCR0B = 255;
}

void pwm1_init(){
	DDRB |= (1 << PORTB1); // Output PB1
	TCCR1A |= (1 << COM1A1);//  Toggle OC0A on compare match
	TCCR1A |= (1 << WGM10);  // Fast PWM
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS10); // 8 Prescale = 4kHz
	OCR1A = 100; // Sets Duty Cycle to 0%
	OCR1B = 255;
}