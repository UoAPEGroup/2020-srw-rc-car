/*
 * PiControllerActivity.c
 *
 * Created: 18/01/2021 2:00:23 PM
 * Author : Manoj
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "main.h"

#define kp 1
#define ki 1
#define setPoint 5
#define clkfreq 8000000

static int32_t intError = 0;
static uint32_t overflowCount = 0;
static uint32_t previousTime = 0;
static int32_t output = 0;
static bool errorHasBeenCalc = false;

int main(void)
{
	adcInit();
	timerInit();
	pwmInit();
	sei();

    while (1) 
    {
		if(errorHasBeenCalc) {
			setPWM(output);
			errorHasBeenCalc = false;
		}
    }
}

ISR(ADC_vect){
	uint16_t measuredVolt = ADC;
	measuredVolt = (uint32_t)(measuredVolt) * 5/1024;
	calculatePiVals(measuredVolt);
}

void calculatePiVals(uint16_t measuredVolt) {
	uint32_t currentTime = overflowCount * 65536 / clkfreq + TCNT1 / clkfreq;
	uint32_t elapsedTime = currentTime - previousTime;

	
	int16_t error = setPoint - measuredVolt;
	intError += error * elapsedTime;
	
	output = kp * error + ki * intError;
	previousTime = currentTime;
	errorHasBeenCalc = true;
}

void adcInit(){
	DDRC &= ~(1<<PC0);
	ADMUX = 0b01000000;
	ADCSRA = 0b11111000;
	ADCSRB = 0;	
}

void timerInit(){
	TCCR1A = 0;
	TCCR1B = 0b00001000;
	TIMSK1 |= (1<<TOIE1);
	OCR1A = 65535;
}

ISR(TIMER1_OVF_vect) {
	overflowCount++;
}

void pwmInit(){
		
	TCCR0A |= (1<<COM0A1); //Clear OC0A on C match, set at Bottom. -Non-inverting.
	TCCR0A |= (1<<WGM01) | (1<<WGM00); //Fast PWM mode.
	TCCR0B |= (1<<CS00); //No prescaling.
	DDRD |= (1<<PD6);
	OCR0A = 255*0.5; //Set duty cycle to 50% to begin with
		
}


void setPWM(uint16_t error) {
	OCR0A = error/setPoint * 255;
}
