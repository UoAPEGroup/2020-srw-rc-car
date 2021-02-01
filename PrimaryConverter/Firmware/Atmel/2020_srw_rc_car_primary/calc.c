/*
 * calc.c
 *
 * Created: 11/01/2021 11:25:03 am
 *  Author: Andrey Chukhraev & Kimsong Lor
 */ 

#include <stdint.h>
#include <string.h>
#include <math.h>

#include "calc.h"
#include "timer_control.h"

#define ASCII_OFF		48
#define ASCII_MAX		57
#define ASCII_MIN		48
#define ASCII_1			49
#define PERCENT			37
#define DIGIT_1			1
#define DIGIT_2			2
#define DIGIT_3			3
#define DIGIT_4			4
#define SAMPLING_SIZE	10		

#define DIGIT_1_OK		((buffer[DIGIT_1] >= ASCII_MIN) && (buffer[DIGIT_1] <= ASCII_MAX))									//check if ASCII char entered is a number								
#define DIGIT_2_OK		((buffer[DIGIT_2] >= ASCII_MIN) && (buffer[DIGIT_2] <= ASCII_MAX))
#define DIGIT_3_OK		((buffer[DIGIT_3] >= ASCII_MIN) && (buffer[DIGIT_3] <= ASCII_MAX))

//calculate duty cycle from maximum of 3 digits received from user
uint8_t calc_make_duty_cycle(uint8_t *buffer) 
{		
	if (buffer[DIGIT_2] == PERCENT) {																						//user entered a single digit number
		if (DIGIT_1_OK) {																																
			return (buffer[DIGIT_1] - ASCII_OFF);
		}																							
	} else if (buffer[DIGIT_3] == PERCENT) {																				//user entered a double digit number
		if (DIGIT_1_OK && DIGIT_2_OK) {																				
			return ((buffer[DIGIT_1] - ASCII_OFF) * 10 + (buffer[DIGIT_2] - ASCII_OFF));
		}
	} else if (buffer[DIGIT_4] == PERCENT) {																				//user entered a triple digit number
		if (DIGIT_1_OK && DIGIT_2_OK && DIGIT_3_OK) {
			if (buffer[DIGIT_1] <= ASCII_1) {																				//cap max possible duty cycle at 199% to prevent overflow
				return ((buffer[DIGIT_1] - ASCII_OFF) * 100 + (buffer[DIGIT_2] - ASCII_OFF) * 10 + (buffer[DIGIT_3] - ASCII_OFF));			
			}
		}
	}
	
	return timer_control_get_current_duty();																				//if non true, return current duty cycle
}

//calculate OCRnB value to be set for timer0 & timer2
uint8_t calc_make_OCRnB(uint8_t period, uint8_t duty_cycle) 
{	
	if (duty_cycle >= 100) {
		return 52;																											//cap at 99% duty cycle
	} else if (duty_cycle == 0) {
		return 0;
	}
	
	return (((uint16_t)period * 100) / (10000 / (uint16_t)duty_cycle));
}

//convert an ASCII character to int
uint8_t calc_ascii_to_num(uint8_t ascii)
{
	return (ascii - ASCII_OFF);
}  

/*from Kimsong's functions.c below this line*/
uint32_t calc_make_average(uint32_t *arr) 
{
	uint32_t temp = 0;
	for(uint8_t i = 0; i < SAMPLING_SIZE; i++) {
		temp = temp + arr[i];
	}
	uint32_t result = (temp/SAMPLING_SIZE);
	return result;
}

uint32_t calc_make_mA(uint32_t mV)																							//needs update
{ 
	return 0;
}

uint32_t calc_make_temp(uint32_t mV)																						//needs update
{ 
	return 0;
}