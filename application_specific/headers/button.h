/*
 * button.h
 *
 *  Created on: Nov 20, 2020
 *      Author: jgamm
 */

#ifndef APPLICATION_SPECIFIC_HEADERS_BUTTON_H_
#define APPLICATION_SPECIFIC_HEADERS_BUTTON_H_

#include "global.h"

typedef enum
{
    button1 = 0,
    button2 = 1
} BUTTON_Enum;

void BUTTON_initialize(void);

void BUTTON_block(BUTTON_Enum button);

void BUTTON_unblock(BUTTON_Enum button);

void BUTTON_setHandlers(BUTTON_Enum button, void (* rising_handler)(void), void (* falling_handler)(void));

#endif /* APPLICATION_SPECIFIC_HEADERS_BUTTON_H_ */
