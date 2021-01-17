/*
 * led.h
 *
 *  Created on: Nov 20, 2020
 *      Author: jgamm
 */

#ifndef APPLICATION_SPECIFIC_HEADERS_LED_H_
#define APPLICATION_SPECIFIC_HEADERS_LED_H_

#include "global.h"

typedef enum
{
    led1,
    led2
} LED_Enum;

void LED_initialize(void);

void LED_write(LED_Enum led, bool val);

#endif /* APPLICATION_SPECIFIC_HEADERS_LED_H_ */
