/*
 * timer0.c
 *
 * Created: 12/8/2022 12:52:23 PM
 *  Author: Eslam M.Ashour
 */ 
#include "timer1.h"

void TIMER_Init()
{
	// Setting Prescaler to 64
	SET_BIT(TCCR1B,CS10); //CS10
	SET_BIT(TCCR1B,CS11); //CS11
	CLR_BIT(TCCR1B,CS12); //CS12
	
	// Starting Timer
	TIMER_Reset();
	
	// Setting Timer1 to Normal Mode
	CLR_BIT(TCCR1A,WGM10); //WGM10
	CLR_BIT(TCCR1A,WGM11); //WGM11
	CLR_BIT(TCCR1B,WGM12); //WGM12
	CLR_BIT(TCCR1B,WGM13); //WGM13
	
	
	// Setting Timer ov interrupt on
	SET_BIT(TIMSK,TOIE1);
}

void TIMER_Reset()
{
	TCNT1 = 0;
	timerOVFs = 0;
}

static uint32_t getTime(void)
{
	// TODO: Place your code here, to compute the elapsed time.
	// 0xFFFF -> 2^16 =65536 -> Clks= 65535
	// Prescaler 8 -> Ft= 1 MHz / Tt = 8x10^-6 second
	// Time = timerOVFs * 65535 * 8 (usec)
	// TCNT1L & TCNT1H
	uint32_t timerValue = 0;
	timerValue = TCNT1;
	return (((timerOVFs*clks_number)+timerValue)*clk_time);
}

ISR(TIMER1_OVF_vect)
{
	cli();
	timerOVFs+=1;	
	sei();
}