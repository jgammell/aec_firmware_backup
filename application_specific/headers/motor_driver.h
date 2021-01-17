/*
 * motor_driver.h
 *
 *  Created on: Nov 20, 2020
 *      Author: jgamm
 */

#ifndef APPLICATION_SPECIFIC_HEADERS_MOTOR_DRIVER_H_
#define APPLICATION_SPECIFIC_HEADERS_MOTOR_DRIVER_H_

#include "global.h"

typedef enum
{
    mdDirectionCW = false,
    mdDirectionCCW = true
} MD_Direction_Enum;

void MD_initialize(void);

void MD_turnMotor(uint32_t num_steps, MD_Direction_Enum direction, void (* handler)(void));

void MD_findEndSwitch(void (* handler)(void));

bool MD_inUse(void);


#endif /* APPLICATION_SPECIFIC_HEADERS_MOTOR_DRIVER_H_ */
