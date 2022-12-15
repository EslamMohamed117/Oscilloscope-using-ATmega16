#include "Init.h"

void init()
{

	UART_Config();
	
	LED_Init();
	TIMER_Init();
	ADC_Init();
	OSCI_Init();
	sei();
}