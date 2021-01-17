/*
 * timer.h
 *
 *  Created on: Nov 13, 2020
 *      Author: jgamm
 */

#ifndef FUNCTIONS_HEADERS_TIMER_H_
#define FUNCTIONS_HEADERS_TIMER_H_

#include "global.h"

typedef enum
{
    timerA0 = 0,
    timerA1 = 1,
    timerA2 = 2
} TIMER_Timer_Enum;

typedef struct
{
    uint16_t ticks_on;
    uint16_t ticks_off;
    uint8_t  output;
} TIMER_PulseTrainSettings_Struct;

void TIMER_initialize(void);
void TIMER_configureDelay(TIMER_Timer_Enum timer);
void TIMER_configurePulseTrain(TIMER_Timer_Enum timer, TIMER_PulseTrainSettings_Struct * settings);
void TIMER_startDelay(TIMER_Timer_Enum timer, uint16_t delay_ms, void (* handler)(void));
void TIMER_stopDelay(TIMER_Timer_Enum timer);
void TIMER_startPulseTrain(TIMER_Timer_Enum timer, uint32_t num_pulses, void (* handler)(void));
void TIMER_stopPulseTrain(TIMER_Timer_Enum timer);
void TIMER_reset(TIMER_Timer_Enum timer);
bool TIMER_isAvailable(TIMER_Timer_Enum timer);
bool TIMER_isRunning(TIMER_Timer_Enum timer);

#endif /* FUNCTIONS_HEADERS_TIMER_H_ */
