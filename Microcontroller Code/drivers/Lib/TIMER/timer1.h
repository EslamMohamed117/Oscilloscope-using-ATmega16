/*
 * timer0.h
 *
 * Created: 12/8/2022 12:52:09 PM
 *  Author: Eslam M.Ashour
 */ 


#ifndef TIMER1_H_
#define TIMER1_H_


#include "../DIO/DIO.h"
#include <avr/io.h>
#include "../DIO/STD_MACROS.h"

volatile static uint8_t timerOVFs  = 0;

extern void TIMER_Init();
extern void TIMER_Reset();
extern static uint32_t getTime(void);

#endif /* TIMER1_H_ */