/*
 * timer2.c
 *
 * Created: 29/01/2021 3:42:50 pm
 *  Author: npat692
 */ 

#include <avr/interrupt.h>

#include "global.h"
#include "timer2.h"
#include "pi_controller.h"
#include "uart.h"

// 1ms Interrupt
ISR(TIMER2_COMPA_vect){
	//check_timeout(); // Disabled for Testing
	// PI controller function used for steering motor 
	pi_controller(); 
}

// Sets up the 1ms timer used for triggering the PI Controller
void timer2_init(){
	TCCR2A = 0x00;
	TCCR2A |= (1 << WGM21); // CTC
	TCCR2B |= (1 << CS22); // 64 Prescaler
	OCR2A = 62; // 1ms Interrupt
	TIMSK2 |= (1 << OCIE2A); // Output Compare A
}