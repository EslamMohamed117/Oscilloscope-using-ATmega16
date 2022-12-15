/*
 * ADC.c
 *
 * Created: 12/15/2022 12:14:11 PM
 *  Author: Eslam M.Ashour
 */ 
#include "ADC.h"


void ADC_Init()
{
	ADMUX = 0b01100000;    // PA0 -> ADC0, ADLAR=1 (8-bit)
	ADCSRA |= ((1<<ADEN) | (1<<ADSC) | (1<<ADPS1)); // ADC prescaler at 4
}

uint8_t getADCSample(void)
{
	uint8_t ADCvalue = 0;
	ADCSRA |= (1 << ADSC);
	while(!(ADCSRA & (1<<ADIF))); // waiting for ADIF, conversion complete
	ADCvalue = ADCH;
	return ADCvalue;
}