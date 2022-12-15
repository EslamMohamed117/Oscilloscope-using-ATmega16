#include <avr/io.h>
#include <stdlib.h>
 
#include <util/delay.h>

#include "../Lib/USART/uart.h"
#include "../Lib/TIMER/timer1.h"
#include "../LED/LED.h" 
#include "oscilloscope.h"
 
#define _CMD_START_CNT 1
#define _CMD_END_CNT   1
#define _CMD_SPACING   1
#define _CMD_PINS_ST   1
#define _CMD_TIME_SNAP 4
 
#define FULL_SAMPLE_CNT (_CMD_START_CNT + _CMD_PINS_ST +  _CMD_TIME_SNAP + _CMD_END_CNT)
 
#define _SAMPLE_PIN  (_CMD_START_CNT)
#define _SAMPLE_TIME (_CMD_START_CNT + _CMD_PINS_ST)
 
#define MARKER_END   (FULL_SAMPLE_CNT - 1)
#define MARKER_START (0)
 
// Send the following frame for each sample:
// @PIN TIME3 TIME2 TIME1 TIME0;
 
#define _SAMPLES_NUM 100
 
#define LOGIC_PORT PINA
 
typedef enum {SAMPLING, SENDING, IDLE} states_t;
 
static states_t currentState = SAMPLING;
static uint8_t  analog_samples[_SAMPLES_NUM];
static uint32_t time_snap[_SAMPLES_NUM];

 
void OSCI_Init(void)
{
    /* Start with analog sampling. */ 
    currentState = SAMPLING;    
}

void OSCI_MainFunction(void)
{    
    static volatile uint8_t samples_cnt = 0;
    static char _go_signal_buf = 'N';
    // Main function must have two states,
    // First state is command parsing and waveform selection.
    // second state is waveform executing.
    switch(currentState)
    {
        case SAMPLING:
        {
            // DO here sampling.
            analog_samples[samples_cnt] = getADCSample();
            time_snap[samples_cnt]      = getTime();
            
            // Increment sample count.
            samples_cnt++;
 
            // Start sending the collected _SAMPLES_NUM samples.
            currentState = (samples_cnt >= _SAMPLES_NUM) ? SENDING : SAMPLING;
            break;
        }
        case SENDING:
        {
            // For _SAMPLES_NUM samples send the construct the buffer.
            static uint8_t _sample_buf[FULL_SAMPLE_CNT];
            for(uint8_t i = 0; i < _SAMPLES_NUM; ++i)
            {
                // Construct the buffer.
                
                // Add buffer marker
                _sample_buf[MARKER_START] = '@';
 
                // Add pin value.
                _sample_buf[_SAMPLE_PIN]  = analog_samples[i];
 
                // Add time snap value.
                _sample_buf[_SAMPLE_TIME + 0] = ((time_snap[i] & 0xFF000000) >> 24);
                _sample_buf[_SAMPLE_TIME + 1] = ((time_snap[i] & 0x00FF0000) >> 16);
                _sample_buf[_SAMPLE_TIME + 2] = ((time_snap[i] & 0x0000FF00) >> 8);
                _sample_buf[_SAMPLE_TIME + 3] = ((time_snap[i] & 0x000000FF) >> 0);
 
                _sample_buf[MARKER_END]   = ';';
 
                // Send sample.
                UART_SendPayload(_sample_buf, FULL_SAMPLE_CNT);
                while (0 == UART_IsTxComplete());
            }
 
            // Trigger receiving for go signal.
            //UART_ReceivePayload((uint8_t *)_go_signal_buf, 1);   
        }
        case IDLE:
        {
            //currentState = ((1 == UART_IsRxComplete())&&(_go_signal_buf == 'G')) ? SAMPLING : IDLE;
            currentState = IDLE;
			if(currentState == SAMPLING)
            {
                _go_signal_buf = 'N';
                // TODO: Place your code here to reset the timer value.
            }
 
            break;
        }
        default: {/* Do nothing.*/}
    }
}
