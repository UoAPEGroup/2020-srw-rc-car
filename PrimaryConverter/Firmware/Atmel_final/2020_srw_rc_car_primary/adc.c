/*
 * adc.c
 *
 * Created: 12/10/2020 9:55:45 AM
 * Author: Kimsong Lor
 */ 

#include "adc.h"
#include "calc.h"
#include "usart0.h"
#include "led.h"

//ADC read counters
volatile uint8_t voltage_counter = 0;
volatile uint8_t current_counter = 0;
volatile uint8_t temp1_counter = 0;
volatile uint8_t temp2_counter = 0;
volatile uint8_t temp3_counter = 0;


//ADC transfer booleans
bool adc_full = false;														//set all booleans to false 

//ADC read arrays for raw ADC values
uint32_t adc_voltage[SAMPLING_SIZE];										//raw ADC value
uint32_t adc_current[SAMPLING_SIZE];										//raw ADC value
uint32_t adc_temp1[SAMPLING_SIZE];											//raw ADC value
uint32_t adc_temp2[SAMPLING_SIZE];											//raw ADC value
uint32_t adc_temp3[SAMPLING_SIZE];											//raw ADC value
uint32_t adc_averages[ADC_used] = {0};										//voltage[0], current[1], temp1[2], temp2[3], temp3[4]

//ADC arrays for converted values
uint32_t adc_voltage_conv[SAMPLING_SIZE];										
uint32_t adc_current_conv[SAMPLING_SIZE];										
uint32_t adc_temp1_conv[SAMPLING_SIZE];											
uint32_t adc_temp2_conv[SAMPLING_SIZE];											
uint32_t adc_temp3_conv[SAMPLING_SIZE];										

//amended by Andrey
ISR(ADC_vect) {
	if (voltage_counter < SAMPLING_SIZE) {
		TIMER1_COMPB_CLR;
		adc_voltage[voltage_counter] = ADC;
		voltage_counter++;
	} else if (voltage_counter == SAMPLING_SIZE) {
		TIMER1_COMPB_CLR;
		ADC_CH_CLR;
		ADC_CH_ISENS;
		voltage_counter++;
	} else if (current_counter < SAMPLING_SIZE) {
		TIMER1_COMPB_CLR;
		adc_current[current_counter] = ADC;
		current_counter++;
	} else if (current_counter == SAMPLING_SIZE) {
		TIMER1_COMPB_CLR;
		ADC_CH_CLR;
		ADC_CH_TEMP1;
		current_counter++;
	} else if (temp1_counter < SAMPLING_SIZE) {
		TIMER1_COMPB_CLR;
		adc_temp1[temp1_counter] = ADC;
		temp1_counter++;
	} else if (temp1_counter == SAMPLING_SIZE) {
		TIMER1_COMPB_CLR;
		ADC_CH_CLR;
		ADC_CH_TEMP2;
		temp1_counter++;
	} else if (temp2_counter < SAMPLING_SIZE) {
		TIMER1_COMPB_CLR;
		adc_temp2[temp2_counter] = ADC;
		temp2_counter++;
	} else if (temp2_counter == SAMPLING_SIZE) {
		TIMER1_COMPB_CLR;
		ADC_CH_CLR;
		ADC_CH_TEMP3;
		temp2_counter++;
	} else if (temp3_counter < SAMPLING_SIZE) {
		TIMER1_COMPB_CLR;
		adc_temp3[temp3_counter] = ADC;
		temp3_counter++;
	} else if (temp3_counter == SAMPLING_SIZE) {
		TIMER1_COMPB_CLR;
		ADC_CH_CLR;
		ADC_CH_VSENS;
		adc_full = true;
		adc_reset_counters();
	}
}

void adc_init() {
	ADMUX |= (1<<REFS0);													//Reference voltage selected to be AVCC
	ADCSRA |= (1<<ADEN);													//ADC enabled
	ADC_CH_VSENS;															//set start channel to vsens
	ADCSRA |= (1<<ADATE);													//ADC auto trigger enabled
	ADCSRA |= (1<<ADIE);													//ADC interrupt enabled
	ADCSRA |= (1<<ADPS2) | (1<<ADPS0);										//ADC prescaler = 32, system clock = 8Mhz, adc clock = 250kHz
	ADCSRB |= (1<<ADTS2) | (1<<ADTS0);										//ADC auto trigger source = Timer/Counter1 compare match B
}

uint16_t adc_read(uint8_t channel) {
	ADMUX &= ~( (1<<MUX3) | (1<<MUX2) | (1<<MUX1) | (1<<MUX0) );			//clearing MUX register
	ADMUX |= channel;														//select the adc channel
	
	return ADC;																//return the ADC value from the ADCL and ADCH registers
}

//convert the raw adc value to original value by multiplying with Vstep in mV
//amended by Andrey
void adc_convert_all() {  
	for (uint8_t i = 0; i < SAMPLING_SIZE; i++) {
		adc_voltage_conv[i] = (adc_voltage[i] * V_ref) / A_int;
		adc_current_conv[i] = (adc_current[i] * V_ref) / A_int;
		adc_temp1_conv[i] = (adc_temp1[i] * V_ref) / A_int;
		adc_temp2_conv[i] = (adc_temp2[i] * V_ref) / A_int;
		adc_temp3_conv[i] = (adc_temp3[i] * V_ref) / A_int;
	}							
}

void adc_reset_counters() {
	//reset all counters to 0
	voltage_counter = 0;
	current_counter = 0;
	temp1_counter = 0;
	temp2_counter = 0;
	temp3_counter = 0;
}

//amended by Andrey
void adc_make_averages() {
	uint32_t sum_vol = 0;
	uint32_t sum_cur = 0;
	uint32_t sum_t1 = 0;
	uint32_t sum_t2 = 0;
	uint32_t sum_t3 = 0;
	
	for (uint8_t i = 0; i < SAMPLING_SIZE; i++) {
		sum_vol += adc_voltage_conv[i];
		sum_cur += adc_current_conv[i];
		sum_t1 += adc_temp1_conv[i];
		sum_t2 += adc_temp2_conv[i];
		sum_t3 += adc_temp3_conv[i];
	}
	
	adc_averages[VOLTAGE_POS] = sum_vol / SAMPLING_SIZE;
	adc_averages[CURRENT_POS] = sum_cur / SAMPLING_SIZE;
	adc_averages[TEMP1_POS] = sum_t1 / SAMPLING_SIZE;
	adc_averages[TEMP2_POS] = sum_t2 / SAMPLING_SIZE;
	adc_averages[TEMP3_POS] = sum_t3 / SAMPLING_SIZE;
}

//amended by Andrey
uint32_t adc_get_avg_voltage()
{
	return adc_averages[VOLTAGE_POS];
}	

uint32_t adc_get_avg_current()
{
	return adc_averages[CURRENT_POS];
}								
					
uint32_t adc_get_avg_temp1()
{
	return adc_averages[TEMP1_POS];
}

uint32_t adc_get_avg_temp2()
{
	return adc_averages[TEMP2_POS];
}

uint32_t adc_get_avg_temp3()
{
	return adc_averages[TEMP3_POS];	
}

//get and clear adc_full flag
bool adc_get_full_flag()
{
	return adc_full;
}

void adc_clr_full_flag()
{
	adc_full = false;
}