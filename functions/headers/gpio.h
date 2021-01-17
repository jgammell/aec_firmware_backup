/*
 * gpio.h
 *
 *  Created on: Nov 20, 2020
 *      Author: jgamm
 */

#ifndef FUNCTIONS_HEADERS_GPIO_H_
#define FUNCTIONS_HEADERS_GPIO_H_

#include "global.h"

typedef enum
{
    gpioPort1,
    gpioPort2,
    gpioPort3,
    gpioPort4,
    gpioPort5,
    gpioPort6,
    gpioPort7,
    gpioPort8,
    gpioPortJ
} GPIO_Port_Enum;
typedef enum
{
    gpioInterruptsRising,
    gpioInterruptsFalling,
    gpioInterruptsNone
} GPIO_Interrupts_Enum;
typedef enum
{
    gpioDirectionInput,
    gpioDirectionOutput
} GPIO_Direction_Enum;
typedef enum
{
    gpioResistorPullup,
    gpioResistorPulldown,
    gpioResistorNone
} GPIO_Resistor_Enum;
typedef enum
{
    gpioDrivestrengthReduced,
    gpioDrivestrengthFull
} GPIO_Drivestrength_Enum;
typedef struct
{
    bool initial_output;
    GPIO_Direction_Enum direction;
    GPIO_Resistor_Enum resistor;
    GPIO_Drivestrength_Enum drive_strength;
} GPIO_PinConfig_Struct;


void GPIO_initialize(void);

void GPIO_configurePin(GPIO_Port_Enum port, uint8_t pin, GPIO_PinConfig_Struct * settings);

void GPIO_configurePinInterrupts(GPIO_Port_Enum port, uint8_t pin, GPIO_Interrupts_Enum setting, void (* handler)(void));

GPIO_Interrupts_Enum GPIO_getPinInterrupts(GPIO_Port_Enum port, uint8_t pin);

void GPIO_writePin(GPIO_Port_Enum port, uint8_t pin, bool val);

bool GPIO_readPin(GPIO_Port_Enum port, uint8_t pin);

#endif /* FUNCTIONS_HEADERS_GPIO_H_ */
