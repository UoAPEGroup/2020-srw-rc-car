/*
 * adc.c
 *
 * Created: 11/01/2021 10:04:51 am
 *  Author: npat692
 */ 

#include "global.h"

#include "adc.h"

// ADC Setup
void adc_init(){
	DDRC &=~(1 << PINC1); // Input
	ADMUX = 0x00;
	ADMUX |= (1 << REFS0); // AVCC
	ADMUX |= (1 << MUX0); // ADC Channel 1
	ADCSRA |= (1 << ADEN); // ADC Enable
	ADCSRA |= (1 << ADPS2)|(1 << ADPS0); // 250kHz Sampling time 
}

// Reads the ADC 
uint16_t adc_read(){
	ADCSRA |= (1 << ADSC); // ADC Start
	while ((ADCSRA & (1 << ADIF)) == 0){ 
		;
	}
	uint16_t adc_val = ((ADCL << 0) | (ADCH << 8));
	adc_val = ((uint32_t) adc_val * VREF)/(ADC_RES);
	return adc_val;
}