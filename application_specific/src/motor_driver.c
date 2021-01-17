/*
 * motor_driver.c
 *
 *  Created on: Nov 20, 2020
 *      Author: jgamm
 */

#include "motor_driver.h"
#include "button.h"
#include "led.h"
#include "timer.h"
#include "errors.h"
#include "interrupts.h"

#define ABS(x) ((x)>=0? (x) : -1*(x))

static bool in_use = false;
static void (* handler)(void) = NULLPTR;

static void _button0RisingHandler(void)
{
    //TIMER_stopPulseTrain(MOTORA_TIMER);
    //ASSERT_perm(in_use == true);
    //in_use = false;
    //LED_write(led1, false);
    //BUTTON_unblock(button2);
}
static void _button0FallingHandler(void)
{
    //ASSERT_perm(in_use == false);
    //in_use = true;
    BUTTON_block(button2);
    GPIO_writePin(MOTORA_DIR_PORT, MOTORA_DIR_PIN, mdDirectionCW);
    TIMER_startPulseTrain(MOTORA_TIMER, 0, NULLPTR);
    LED_write(led1, true);
    while(GPIO_readPin(SW2_PORT, SW2_PIN) == 0);
    TIMER_stopPulseTrain(MOTORA_TIMER);
    LED_write(led1, false);
    BUTTON_unblock(button2);
}
static void _button1RisingHandler(void)
{
    //TIMER_stopPulseTrain(MOTORA_TIMER);
    //ASSERT_perm(in_use == true);
    //in_use = false;
    //LED_write(led2, false);
    //BUTTON_unblock(button1);
}
static void _button1FallingHandler(void)
{
    //ASSERT_perm(in_use == false);
    //in_use = true;
    BUTTON_block(button1);
    GPIO_writePin(MOTORA_DIR_PORT, MOTORA_DIR_PIN, mdDirectionCCW);
    TIMER_startPulseTrain(MOTORA_TIMER, 0, NULLPTR);
    LED_write(led2, true);
    while(GPIO_readPin(SW3_PORT, SW3_PIN) == 0);
    TIMER_stopPulseTrain(MOTORA_TIMER);
    LED_write(led2, false);
    BUTTON_unblock(button1);
}
static void _doneSteppingHandler(void)
{
    if(handler != NULLPTR)
        handler();
    LED_write(led1, false);
    LED_write(led2, false);
    BUTTON_unblock(button1);
    BUTTON_unblock(button2);
    in_use = false;
    handler = NULLPTR;
}

void MD_initialize(void)
{
    GPIO_PinConfig_Struct output_settings =
    {
     .initial_output = false,
     .direction = gpioDirectionOutput,
     .resistor = gpioResistorNone,
     .drive_strength = gpioDrivestrengthReduced
    };
    GPIO_configurePin(MOTORA_STEP_PORT, MOTORA_STEP_PIN, &output_settings);
    GPIO_configurePin(MOTORA_DIR_PORT, MOTORA_DIR_PIN, &output_settings);
    GPIO_configurePin(MOTORA_RES_PORT, MOTORA_RES_PIN, &output_settings);
    GPIO_configurePin(MOTORA_SD_PORT, MOTORA_SD_PIN, &output_settings);
    GPIO_PinConfig_Struct input_settings =
    {
     .initial_output = false,
     .direction = gpioDirectionInput,
     .resistor = gpioResistorNone,
     .drive_strength = gpioDrivestrengthReduced
    };
    GPIO_configurePin(MOTORA_ESNO_PORT, MOTORA_ESNO_PIN, &input_settings);
    GPIO_configurePin(MOTORA_ESNC_PORT, MOTORA_ESNC_PIN, &input_settings);
    TIMER_PulseTrainSettings_Struct step_settings =
    {
     .ticks_on = SMCLK_FREQ_HZ/(2*MOTORA_DEFAULT_FREQ),
     .ticks_off = SMCLK_FREQ_HZ/(2*MOTORA_DEFAULT_FREQ),
     .output = MOTORA_TIMER_OUTPUT
    };
    TIMER_configurePulseTrain(MOTORA_TIMER, &step_settings);
    TIMER_configureDelay(DELAY_TIMER);
    BUTTON_setHandlers(button1, &_button0RisingHandler, &_button0FallingHandler);
    BUTTON_setHandlers(button2, &_button1RisingHandler, &_button1FallingHandler);
}

static void _turnMotor(uint32_t num_steps, MD_Direction_Enum direction, void (*_handler)(void))
{
    BUTTON_block(button1);
    BUTTON_block(button2);
    handler = _handler;
    GPIO_writePin(MOTORA_DIR_PORT, MOTORA_DIR_PIN, direction);
    TIMER_startPulseTrain(MOTORA_TIMER, num_steps, _doneSteppingHandler);
    LED_write(direction == mdDirectionCW? led1 : led2, true);
}

void MD_turnMotor(uint32_t num_steps, MD_Direction_Enum direction, void (* _handler)(void))
{
    ASSERT_perm(in_use == false);
    in_use = true;
    _turnMotor(num_steps, direction, _handler);
}


typedef enum
{
    findCwBoundPreDelay,
    findCwBoundPostDelay,
    findCcwBoundPreDelay,
    findCcwBoundPostDelay,
    bisectionPreDelay,
    bisectionPostDelay,
    done
} _FesStates_Enum;
static _FesStates_Enum fes_state;
static uint32_t current_freq;
static uint32_t dist;
static void (* fes_handler)(void);

static void _fesProgressState(void)
{
    INT_blockInterrupts();
    switch(fes_state)
    {
    case findCwBoundPreDelay:
        fes_state = findCwBoundPostDelay;
        TIMER_startDelay(DELAY_TIMER, MOTORA_FES_DELAY_MS, &_fesProgressState);
        break;
    case findCwBoundPostDelay:
        if(!GPIO_readPin(MOTORA_ESNO_PORT, MOTORA_ESNO_PIN))
        {
            fes_state = findCwBoundPreDelay;
            _turnMotor(dist, mdDirectionCW, &_fesProgressState);
        }
        else
        {
            fes_state = bisectionPreDelay;
            _fesProgressState();
        }
        break;
    case bisectionPreDelay:
        fes_state = bisectionPostDelay;
        TIMER_startDelay(DELAY_TIMER, MOTORA_FES_DELAY_MS, &_fesProgressState);
        break;
    case bisectionPostDelay:
        if(GPIO_readPin(MOTORA_ESNO_PORT, MOTORA_ESNO_PIN))
        {
            fes_state = bisectionPreDelay;
            _turnMotor(100, mdDirectionCCW, &_fesProgressState);
        }
        else
        {
            fes_state = done;
            _fesProgressState();
        }
        break;
    /*case bisectionPreDelay:
        fes_state = bisectionPostDelay;
        TIMER_startDelay(DELAY_TIMER, MOTORA_FES_DELAY_MS, &_fesProgressState);
        break;
    case bisectionPostDelay:
        dist *= MOTORA_FES_ATTF;
        if(GPIO_readPin(MOTORA_ESNO_PORT, MOTORA_ESNO_PIN))
        {
            fes_state = bisectionPreDelay;
            if(dist == 0)
            {
                fes_state = done;
                _fesProgressState();
                break;
            }
            _turnMotor(dist, mdDirectionCCW, &_fesProgressState);
        }
        else
        {
            fes_state = bisectionPreDelay;
            if(dist == 0)
            {
                fes_state = done;
                _fesProgressState();
                break;
            }
            _turnMotor(dist, mdDirectionCW, &_fesProgressState);
        }
        break;*/
    case done:
        handler = fes_handler;
        _doneSteppingHandler();
        break;
    default:
        ASSERT_perm(false);
    }
    INT_unblockInterrupts();
}

static void _fesEsEdgeHandler(void)
{
    TIMER_stopPulseTrain(MOTORA_TIMER);
    GPIO_configurePinInterrupts(MOTORA_ESNO_PORT, MOTORA_ESNO_PIN, gpioInterruptsNone, NULLPTR);
    _fesProgressState();
}

void MD_findEndSwitch(void (* _handler)(void))
{
    BUTTON_block(button1);
    BUTTON_block(button2);
    ASSERT_perm(in_use == false);
    in_use = true;
    fes_handler = _handler;
    fes_state = findCwBoundPreDelay;
    current_freq = MOTORA_DEFAULT_FREQ;
    dist = MOTORA_FES_IDIST;
    GPIO_writePin(MOTORA_DIR_PORT, MOTORA_DIR_PIN, mdDirectionCW);
    GPIO_configurePinInterrupts(MOTORA_ESNO_PORT, MOTORA_ESNO_PIN, gpioInterruptsRising, &_fesEsEdgeHandler);
    TIMER_startPulseTrain(MOTORA_TIMER, 0, NULLPTR);
}

bool MD_inUse(void)
{
    return in_use;
}

