/*
 * led.c
 *
 *  Created on: Nov 20, 2020
 *      Author: jgamm
 */

#include "led.h"
#include "gpio.h"
#include "errors.h"

void LED_initialize(void)
{
    GPIO_PinConfig_Struct settings =
    {
     .initial_output = 0,
     .direction = gpioDirectionOutput,
     .resistor = gpioResistorNone,
     .drive_strength = gpioDrivestrengthReduced
    };
    GPIO_configurePin(LED5_PORT, LED5_PIN, &settings);
    GPIO_configurePin(LED6_PORT, LED6_PIN, &settings);
}

void LED_write(LED_Enum led, bool val)
{
    switch(led)
    {
    case led1:
        GPIO_writePin(LED5_PORT, LED5_PIN, val);
        break;
    case led2:
        GPIO_writePin(LED6_PORT, LED6_PIN, val);
        break;
    default:
        ASSERT_perm(false);
    }
}


