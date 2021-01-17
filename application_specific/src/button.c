/*
 * button.c
 *
 *  Created on: Nov 20, 2020
 *      Author: jgamm
 */

#include "button.h"
#include "gpio.h"
#include "errors.h"

typedef struct
{
    void (* rising_handler)(void);
    void (* falling_handler)(void);
    uint16_t num_blocks;
} _State_Struct;

_State_Struct button_states[2] = {{NULLPTR, NULLPTR, 0}, {NULLPTR, NULLPTR, 0}};

static void _button0Handler(void)
{
    if(GPIO_readPin(SW2_PORT, SW2_PIN))
    {
        ASSERT_perm(GPIO_getPinInterrupts(SW2_PORT, SW2_PIN) == gpioInterruptsRising);
        GPIO_configurePinInterrupts(SW2_PORT, SW2_PIN, gpioInterruptsFalling, button_states[0].falling_handler);
        button_states[0].rising_handler();
    }
    else
    {
        ASSERT_perm(GPIO_getPinInterrupts(SW2_PORT, SW2_PIN) == gpioInterruptsFalling);
        GPIO_configurePinInterrupts(SW2_PORT, SW2_PIN, gpioInterruptsRising, button_states[0].rising_handler);
        button_states[0].falling_handler();
    }
}
static void _button1Handler(void)
{
    if(GPIO_readPin(SW3_PORT, SW3_PIN))
    {
        ASSERT_perm(GPIO_getPinInterrupts(SW3_PORT, SW3_PIN) == gpioInterruptsRising);
        GPIO_configurePinInterrupts(SW3_PORT, SW3_PIN, gpioInterruptsFalling, button_states[1].falling_handler);
        button_states[1].rising_handler();
    }
    else
    {
        ASSERT_perm(GPIO_getPinInterrupts(SW3_PORT, SW3_PIN) == gpioInterruptsFalling);
        GPIO_configurePinInterrupts(SW3_PORT, SW3_PIN, gpioInterruptsRising, button_states[1].rising_handler);
        button_states[1].falling_handler();
    }
}

void BUTTON_initialize(void)
{
    GPIO_PinConfig_Struct settings =
    {
     .initial_output = false,
     .direction = gpioDirectionInput,
     .resistor = gpioResistorPullup,
     .drive_strength = gpioDrivestrengthReduced
    };
    GPIO_configurePin(SW2_PORT, SW2_PIN, &settings);
    GPIO_configurePin(SW3_PORT, SW3_PIN, &settings);
}

void BUTTON_block(BUTTON_Enum button)
{
    switch(button)
    {
    case button1:
        if(button_states[0].num_blocks == 0)
            GPIO_configurePinInterrupts(SW2_PORT, SW2_PIN, gpioInterruptsNone, NULLPTR);
        button_states[0].num_blocks += 1;
        break;
    case button2:
        if(button_states[1].num_blocks == 0)
            GPIO_configurePinInterrupts(SW3_PORT, SW3_PIN, gpioInterruptsNone, NULLPTR);
        button_states[1].num_blocks += 1;
        break;
    default:
        ASSERT_perm(false);
    }
}

void BUTTON_unblock(BUTTON_Enum button)
{
    switch(button)
    {
    case button1:
        if(button_states[0].num_blocks == 1)
        {
            if(GPIO_readPin(SW2_PORT, SW2_PIN))
                GPIO_configurePinInterrupts(SW2_PORT, SW2_PIN, gpioInterruptsFalling, &_button0Handler);
            else
                GPIO_configurePinInterrupts(SW2_PORT, SW2_PIN, gpioInterruptsRising, &_button0Handler);
        }
        button_states[0].num_blocks -= 1;
        break;
    case button2:
        if(button_states[1].num_blocks == 1)
        {
            if(GPIO_readPin(SW3_PORT, SW3_PIN))
                GPIO_configurePinInterrupts(SW3_PORT, SW3_PIN, gpioInterruptsFalling, &_button1Handler);
            else
                GPIO_configurePinInterrupts(SW3_PORT, SW3_PIN, gpioInterruptsRising, &_button1Handler);
        }
        button_states[1].num_blocks -= 1;
        break;
    default:
        ASSERT_perm(false);
    }
}

void BUTTON_setHandlers(BUTTON_Enum button, void (* rising_handler)(void), void (* falling_handler)(void))
{
    switch(button)
    {
    case button1:
        button_states[0].rising_handler = rising_handler;
        button_states[0].falling_handler = falling_handler;
        if(GPIO_readPin(SW2_PORT, SW2_PIN))
            GPIO_configurePinInterrupts(SW2_PORT, SW2_PIN, gpioInterruptsFalling, &_button0Handler);
        else
            GPIO_configurePinInterrupts(SW2_PORT, SW2_PIN, gpioInterruptsRising, &_button0Handler);
        break;
    case button2:
        button_states[1].rising_handler = rising_handler;
        button_states[1].falling_handler = falling_handler;
        if(GPIO_readPin(SW3_PORT, SW3_PIN))
            GPIO_configurePinInterrupts(SW3_PORT, SW3_PIN, gpioInterruptsFalling, &_button1Handler);
        else
            GPIO_configurePinInterrupts(SW3_PORT, SW3_PIN, gpioInterruptsRising, &_button1Handler);
        break;
    default:
        ASSERT_perm(false);
    }
}

