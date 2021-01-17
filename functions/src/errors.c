/*
 * errors.c
 *
 *  Created on: Nov 13, 2020
 *      Author: jgamm
 */

#include "errors.h"
#include "interrupts_hal.h"
#include "led.h"

void __attribute__ ((optimize("O0"))) _assert_failure(char * expression, char * file, uint16_t line)
{
    LED_initialize();
    INTERRUPTS_clearGie();
    while(1)
    {
        LED_write(led2, true);
        LED_write(led1, false);
        for(uint16_t i=0; i<0xFFF; ++i);
        LED_write(led2, false);
        LED_write(led1, true);
        for(uint16_t i=0; i<0xFFF; ++i);
    }
}


