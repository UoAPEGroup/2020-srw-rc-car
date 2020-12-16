/*
 * timer.c
 *
 * Created: 9/12/2020 2:54:23 pm
 *  Author: Earlene
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>

timer0_init(uint8_t period, uint8_t duty_cycle) {
	
	//Phase Correct PWM, set OC0A on compare match
	TCCR0A |= (1 << COM0B1)|(1 << COM0B0)|(1 << WGM00);
	TCCR0B |= (1 << WGM02)|(1 << CS00);
	
	OCR0A = period;
	OCR0B = duty_cycle;
	
}

timer2_init(uint8_t period, uint8_t duty_cycle) {
	//Phase Correct PWM, set OC2A on compare match
	TCCR2A |= (1 << COM2B1)|(1 << COM2B0)|(1 << WGM20);
	TCCR2B |= (1 << WGM22)|(1 << CS20);
	
	OCR2A = period;
	OCR2B = duty_cycle;
}
