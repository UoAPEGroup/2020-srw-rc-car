/*
 * localEmbedded.c
 *
 * Created: 9/12/2020 2:54:10 pm
 * Author : Earlene
 */ 

#include <avr/io.h>
#include "timer.h"
#include "Calculations.h"
#include "adc.h"
#include "uart.h"
#include "interrupt.h"
#include "global.h"
#include "watchdog.h"
#include "validate.h"

#define F_CPU 800000UL
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>


int main(void)
{
// 	watchdog_init(); //initialize the watchdog timer
// 	adc_init(); //initialize the adc
// 	interrupt_init(); //initialize the interrupt
	uart_init(9600); //initialize the uart
		
// 	sei(); //turn on global interrupts
// 	uint8_t offset = 0;
// 	
	//set output pins
// 	DDRD |= (1 << DDD3)|(1 << DDD5);
// 	DDRB |= (1 << DDB1);

	//stop all the timers
	//GTCCR = ((1<<TSM)|(1<<PSRASY)|(1<<PSRSYNC)); 
	
// 	setSpeedGrade(STOP);
// 	setInputV(10000);
// 	setDirection(true); // true = forward, false = backward
// 	updateDutyCycle();
// 	
// 	//initialize timers
// 	timer0_init(returnFinalPeriod(),returnLeftOnTime());  // PWm that controls the left FET driver
// 	timer2_init(returnFinalPeriod(),returnRightOnTime());  // PWM that controls the right FET driver
	//initialize timers
	//timer0_init(132, 62);  // PWm that controls the left FET driver
	//timer2_init(132, 20); // PWM that controls the right FET driver
	//timer1_init();
	
// 	TCNT0 = 0;   // setting offset
// 	TCNT2 = offset;  // setting offset(set to period to)
// 
// 	GTCCR = 0;   // start all timers
// 	
	char hello[100] = "Hello, hi there this is so frustrating \n\r";
	
    while (1)
    {
		
		//_delay_ms(500);
		//send_data(hello);
		
		uart_transmit_string(hello);
		
		
//  		if (arrayFull) {//every 10.5 ms
// // 			
//  			TCCR1B &= ~(1 << CS10) | ~(1 << CS11) | ~(1 << CS12); //turn off adc sampling
//  			//send_data(hello);
//  			convertVoltageAndCurrent();
// // 			
//  			//checkADC();
// 			 
// 			 	char transmitVoltage[10];
// 			 	char transmitCurrent[10];
// 			 	
// 			 	uint16_t voltageValue = returnInputV(); //PC1, ADC1
// 			 	uint16_t currentValue = returnInputI(); //PC0, ADC0
// 
// 			 	//sprintf(transmitCurrent, "%u", currentValue);
// 			 	uart_transmit_string("Hello, Hi there, UoA, AUT\n\r");
// 				 
// 			 	//sprintf(transmitCurrent, "%u", voltageValue);
// 			 	//uart_transmit_string("Hi\n\r");
// // 			
//  			arrayFull = false;
//  			TCCR1B |= (1 << CS10) | (1 << CS11); //turn on adc sampling
// // 			
//  		}
		
		
// 		if (arrayFull) {//if 10 adc readings of current and voltage have been taken (i.e., roughly 10.5 ms)
// 			
// 			//check that the remote is still in contact and sending valid signals 
// 			if (interruptCount >= REQUIRED_INTERRUPT_COUNT) {
// 				interruptCount = 0;
// 				lostRemoteConnection = false;
// 			}
// 			else { //if contact is lost, stop the car
// 				lostRemoteConnection = true; //set lost remote connection flag to true
// 				setSpeedGrade(STOP);
// 				interruptCount = 0;
// 			}
// 			
// 			TCCR1B &= ~(1 << CS10) | ~(1 << CS11) | ~(1 << CS12); //turn off adc by turning off timer1
// 			 
// 			convertVoltageAndCurrent(); //convert adc readings into original values 
// 		
// 			//print on terminal to verify
// 			//uint16_t inputV = returnInputV();
// 			//sprintf(hello, "%u", inputV);
// 			//send_data(hello);
// 			GTCCR = ((1<<TSM)|(1<<PSRASY)|(1<<PSRSYNC)); //stop timers
// 			//updateDutyCycle(); //update duty cycle
// 			
// 			timer0_init(132, 52);  // PWm that controls the left FET driver
// 			timer2_init(132, 98);  // PWM that controls the right FET driver
// 			//timer1_init();
// 			
// 			TCNT0 = 0;   // setting offset
// 			TCNT2 = offset;  
// 
// 			GTCCR = 0; //start the timers
// 			
// 			//uint16_t inputI = returnInputI();
// 			//sprintf(hello, "%u", inputI);
// 			//send_data(hello);
// 		
// 			//turn on timer1 and set flag
// 			arrayFull = false;
// 			TCCR1B |= (1 << CS10) | (1 << CS11);
// 		
// 		}
// 		
// 		if (sendData) {
// 			
// 			averageVoltageAndCurrent();
// 			
// // 			uint16_t inputV = returnAvgV();
// // 			sprintf(hello, "%u", inputV);
// // 			send_data(hello);
// // 					
// // 			uint16_t inputI = returnAvgI();
// // 			sprintf(hello, "%u", inputI);
//  			send_data(hello);
// 
// 			
// 			sendData = false;
// 		}
		
    }
}

