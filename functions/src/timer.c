/*
 * timer.c
 *
 *  Created on: Nov 13, 2020
 *      Author: jgamm
 */

#include "timer.h"
#include "timera_hal.h"
#include "errors.h"
#include "interrupts.h"
#include "io_hal.h"

typedef enum
{
    idle,
    delay,
    pulse_train
} _TimerOperations_Enum;

typedef struct
{
    TIMERA_Registers_Struct * base;
    INT_Source_Enum           interrupt_source;
    _TimerOperations_Enum     operation;
    uint32_t                  frequency;
    void                      (* handler)(void);
    bool                      running;
    uint32_t                  pulses_to_go;
} _TimerState_Struct;

#define _NUM_TIMERS (3)
#define _NUM_CCR(TIMER) (TIMER==TA0? 5 : TIMER==TA1? 3 : TIMER==TA2? 3 : 0)

static bool _initialized = false;
static _TimerState_Struct _timers[_NUM_TIMERS] =
{
 {TA0, intSourceInvalid, idle, 0, NULLPTR, false, 0},
 {TA1, intSourceInvalid, idle, 0, NULLPTR, false, 0},
 {TA2, intSourceInvalid, idle, 0, NULLPTR, false, 0}
};

INTERRUPT void _defaultHandler(void)
{
    ASSERT_perm(false);
}
INTERRUPT void timerA0_Delay_IRQHandler(void)
{
    INT_blockInterrupts();
    _timers[timerA0].running = false;
    TIMERA_writeCctlCcifg(_timers[timerA0].base, 0, false);
    TIMERA_writeCctlCcie(_timers[timerA0].base, 0, false);
    TIMERA_writeCtlMc(_timers[timerA0].base, timeraCtlMcStop);
    TIMERA_writeR(_timers[timerA0].base, 0);
    if(_timers[timerA0].handler != NULLPTR)
        _timers[timerA0].handler();
    INT_unblockInterrupts();
}
INTERRUPT void timerA1_Delay_IRQHandler(void)
{
    INT_blockInterrupts();
    _timers[timerA1].running = false;
    TIMERA_writeCctlCcifg(_timers[timerA1].base, 0, false);
    TIMERA_writeCctlCcie(_timers[timerA1].base, 0, false);
    TIMERA_writeCtlMc(_timers[timerA1].base, timeraCtlMcStop);
    TIMERA_writeR(_timers[timerA1].base, 0);
    if(_timers[timerA1].handler != NULLPTR)
        _timers[timerA1].handler();
    INT_unblockInterrupts();
}
INTERRUPT void timerA2_Delay_IRQHandler(void)
{
    INT_blockInterrupts();
    _timers[timerA2].running = false;
    TIMERA_writeCctlCcifg(_timers[timerA2].base, 0, false);
    TIMERA_writeCctlCcie(_timers[timerA2].base, 0, false);
    TIMERA_writeCtlMc(_timers[timerA2].base, timeraCtlMcStop);
    TIMERA_writeR(_timers[timerA2].base, 0);
    if(_timers[timerA2].handler != NULLPTR)
        _timers[timerA2].handler();
    INT_unblockInterrupts();
}
INTERRUPT void timerA0_PT_IRQHandler(void)
{
    INT_blockInterrupts();
    if(_timers[timerA0].pulses_to_go == 1)
    {
        TIMERA_writeCtlMc(_timers[timerA0].base, timeraCtlMcStop);
        if(_timers[timerA0].handler != NULLPTR)
            _timers[timerA0].handler();
        TIMERA_writeCctlCcie(_timers[timerA0].base, 0, false);
        TIMERA_writeR(_timers[timerA0].base, 0);
        _timers[timerA0].running = false;
    }
    _timers[timerA0].pulses_to_go -= 1;
    TIMERA_writeCctlCcifg(_timers[timerA0].base, 0, false);
    INT_unblockInterrupts();
}
INTERRUPT void timerA1_PT_IRQHandler(void)
{
    INT_blockInterrupts();
    if(_timers[timerA1].pulses_to_go == 1)
    {
        TIMERA_writeCtlMc(_timers[timerA1].base, timeraCtlMcStop);
        if(_timers[timerA1].handler != NULLPTR)
            _timers[timerA1].handler();
        TIMERA_writeCctlCcie(_timers[timerA1].base, 0, false);
        TIMERA_writeR(_timers[timerA1].base, 0);
        _timers[timerA1].running = false;
    }
    _timers[timerA1].pulses_to_go -= 1;
    TIMERA_writeCctlCcifg(_timers[timerA1].base, 0, false);
    INT_unblockInterrupts();
}
INTERRUPT void timerA2_PT_IRQHandler(void)
{
    INT_blockInterrupts();
    if(_timers[timerA2].pulses_to_go == 1)
    {
        TIMERA_writeCtlMc(_timers[timerA2].base, timeraCtlMcStop);
        if(_timers[timerA2].handler != NULLPTR)
            _timers[timerA2].handler();
        TIMERA_writeCctlCcie(_timers[timerA2].base, 0, false);
        TIMERA_writeR(_timers[timerA2].base, 0);
        _timers[timerA2].running = false;
    }
    _timers[timerA2].pulses_to_go -= 1;
    TIMERA_writeCctlCcifg(_timers[timerA2].base, 0, false);
    INT_unblockInterrupts();
}

void TIMER_initialize(void)
{
    ASSERT_debug(!_initialized);
    _initialized = true;
    for(uint8_t i=0; i<_NUM_TIMERS; ++i)
    {
        TIMER_reset(i);
    }
    INT_setHandler(intSourceTA0_CCR0, &_defaultHandler);
    INT_setHandler(intSourceTA0_IFG, &_defaultHandler);
    INT_setHandler(intSourceTA1_CCR0, &_defaultHandler);
    INT_setHandler(intSourceTA1_IFG, &_defaultHandler);
    INT_setHandler(intSourceTA2_CCR0, &_defaultHandler);
    INT_setHandler(intSourceTA2_IFG, &_defaultHandler);
}
void TIMER_configureDelay(TIMER_Timer_Enum timer)
{
    ASSERT_debug(_initialized);
    ASSERT_debug(_timers[timer].operation == idle);
    ASSERT_debug(_timers[timer].running == false);
    TIMER_reset(timer);
    _timers[timer].operation = delay;
#if (ACLK_FREQ_HZ>>3) >= 1000UL
    TIMERA_writeCtlId(_timers[timer].base, timeraCtlId8);
    _timers[timer].frequency = ACLK_FREQ_HZ>>3;
#elif (ACLK_FREQ_HZ>>2) >= 1000UL
    TIMERA_writeCtlId(_timers[timer].base, timeraCtlId4);
    _timers[timer].frequency = ACLK_FREQ_HZ>>2;
#elif (ACLK_FREQ_HZ>>1) >= 1000UL
    TIMERA_writeCtlId(_timers[timer].base, timeraCtlId2);
    _timers[timer].frequency = ACLK_FREQ_HZ>>1;
#elif ACLK_FREQ_HZ >= 1000UL
    TIMERA_writeCtlId(_timers[timer].base, timeraCtlId1);
    _timers[timer].frequency = ACLK_FREQ_HZ;
#else
#error "Invalid ACLK frequency"
#endif
    TIMERA_writeCtlTassel(_timers[timer].base, timeraCtlTasselAclk);
    TIMERA_writeCctlCov(_timers[timer].base, 0, false);
    TIMERA_writeCctlOut(_timers[timer].base, 0, false);
    TIMERA_writeCctlOutmod(_timers[timer].base, 0, timeraCctlOutmodOutput);
    TIMERA_writeCctlCap(_timers[timer].base, 0, timeraCctlCapCompare);
    TIMERA_writeCctlScs(_timers[timer].base, 0, timeraCctlScsAsynchronous);
    TIMERA_writeCctlCcis(_timers[timer].base, 0, timeraCctlCcisGnd);
    TIMERA_writeCctlCm(_timers[timer].base, 0, timeraCctlCmDisabled);
    TIMERA_writeEx0Taidex(_timers[timer].base, timeraEx0Taidex1);
    INT_Source_Enum interrupt_source = _timers[timer].base == TA0? intSourceTA0_CCR0 : \
            _timers[timer].base == TA1? intSourceTA1_CCR0 : \
            _timers[timer].base == TA2? intSourceTA2_CCR0 : ~0;
    _timers[timer].interrupt_source = interrupt_source;
    void (* IRQHandler)(void) = timer == timerA0? timerA0_Delay_IRQHandler : \
            timer == timerA1? timerA1_Delay_IRQHandler : \
            timer == timerA2? timerA2_Delay_IRQHandler : NULLPTR;
    INT_setHandler(interrupt_source, IRQHandler);
}
void TIMER_startDelay(TIMER_Timer_Enum timer, uint16_t delay_ms, void (* handler)(void))
{
    TIMERA_writeR(_timers[timer].base, 0);
    TIMERA_writeCcr(_timers[timer].base, 0, delay_ms * (_timers[timer].frequency / 1000));
    TIMERA_writeCtlMc(_timers[timer].base, timeraCtlMcUp);
    ASSERT_debug(_initialized);
    ASSERT_debug(_timers[timer].operation == delay);
    ASSERT_debug(_timers[timer].running == false);
    ASSERT_debug(TIMERA_readCctlCcifg(_timers[timer].base, 0) == false);
    ASSERT_debug(delay_ms * (_timers[timer].frequency / 1000) <= 0xFFFFUL);
    _timers[timer].handler = handler;
    _timers[timer].running = true;
    TIMERA_writeCctlCcie(_timers[timer].base, 0, true);
}
void TIMER_stopDelay(TIMER_Timer_Enum timer)
{
    TIMERA_writeCctlCcie(_timers[timer].base, 0, false);
    TIMERA_writeCtlMc(_timers[timer].base, timeraCtlMcStop);
    TIMERA_writeR(_timers[timer].base, 0);
    TIMERA_writeCcr(_timers[timer].base, 0, 0);
    ASSERT_debug(_initialized);
    ASSERT_debug(_timers[timer].operation == delay);
    ASSERT_debug(_timers[timer].running == true);
    _timers[timer].handler = NULLPTR;
    _timers[timer].running = false;
}
void TIMER_configurePulseTrain(TIMER_Timer_Enum timer, TIMER_PulseTrainSettings_Struct * settings)
{
    ASSERT_debug(_initialized);
    ASSERT_debug(_timers[timer].operation == idle);
    ASSERT_debug(_timers[timer].running == false);
    ASSERT_debug(settings->ticks_on+settings->ticks_off <= 0xFFFF);
    TIMER_reset(timer);
    _timers[timer].operation = pulse_train;
    TIMERA_writeCtlTassel(_timers[timer].base, timeraCtlTasselSmclk);
    TIMERA_writeCctlOutmod(_timers[timer].base, settings->output, timeraCctlOutmodSetreset);
    TIMERA_writeCcr(_timers[timer].base, 0, settings->ticks_on+settings->ticks_off);
    TIMERA_writeCcr(_timers[timer].base, settings->output, settings->ticks_on);
    switch(timer)
    {
    case timerA0:
        _timers[timer].interrupt_source = intSourceTA0_CCR0;
        INT_setHandler(intSourceTA0_CCR0, &timerA0_PT_IRQHandler);
        break;
    case timerA1:
        _timers[timer].interrupt_source = intSourceTA1_CCR0;
        INT_setHandler(intSourceTA1_CCR0, &timerA1_PT_IRQHandler);
        break;
    case timerA2:
        _timers[timer].interrupt_source = intSourceTA2_CCR0;
        INT_setHandler(intSourceTA2_CCR0, &timerA2_PT_IRQHandler);
        break;
    default:
        ASSERT_perm(false);
    }
    IO_Registers_Struct * port = 0;
    uint8_t pin = 0;
    switch(timer)
    {
    case timerA0:
        port = PORT1;
        pin = settings->output+1;
        break;
    case timerA1:
        port = PORT2;
        pin = settings->output-1;
        break;
    case timerA2:
        port = PORT2;
        pin = settings->output+3;
        break;
    default:
        ASSERT_perm(false);
    }
    IO_writeOutPin(port, pin, false);
    IO_writeDirPin(port, pin, ioDirOutput);
    IO_writeDsPin(port, pin, ioDsReduced);
    IO_writeSelPin(port, pin, ioSelPeripheral);
    IO_writeIenPin(port, pin, false);
}
void TIMER_startPulseTrain(TIMER_Timer_Enum timer, uint32_t num_pulses, void (* handler)(void))
{
    ASSERT_debug(_initialized);
    ASSERT_debug(_timers[timer].operation == pulse_train);
    ASSERT_debug(_timers[timer].running == false);
    TIMERA_writeR(_timers[timer].base, 0);
    TIMERA_writeCctlCcifg(_timers[timer].base, 0, false);
    if(num_pulses > 0)
        TIMERA_writeCctlCcie(_timers[timer].base, 0, true);
    TIMERA_writeCtlMc(_timers[timer].base, timeraCtlMcUp);
    _timers[timer].running = true;
    _timers[timer].handler = handler;
    _timers[timer].pulses_to_go = num_pulses;
}
void TIMER_stopPulseTrain(TIMER_Timer_Enum timer)
{
    ASSERT_debug(_initialized);
    ASSERT_debug(_timers[timer].operation == pulse_train);
    //ASSERT_debug(_timers[timer].running == true);
    TIMERA_writeCctlCcie(_timers[timer].base, 0, false);
    TIMERA_writeCtlMc(_timers[timer].base, timeraCtlMcStop);
    TIMERA_writeCctlCcifg(_timers[timer].base, 0, false);
    TIMERA_writeR(_timers[timer].base, 0);
    _timers[timer].running = false;
    _timers[timer].handler = NULLPTR;
    _timers[timer].pulses_to_go = 0;
}
void TIMER_reset(TIMER_Timer_Enum timer)
{
    ASSERT_debug(_initialized);
    TIMERA_writeCtl(_timers[timer].base, 0);
    ASSERT_debug(TIMERA_readCtl(_timers[timer].base) == 0);
    TIMERA_writeR(_timers[timer].base, 0);
    ASSERT_debug(TIMERA_readR(_timers[timer].base) == 0);
    TIMERA_writeEx0(_timers[timer].base, 0);
    ASSERT_debug(TIMERA_readEx0(_timers[timer].base) == 0);
    uint8_t num_ccr = _NUM_CCR(_timers[timer].base);
    ASSERT_debug(num_ccr != 0);
    for(uint8_t i=0; i<num_ccr; ++i)
    {
        TIMERA_writeCctl(_timers[timer].base, i, 0);
        ASSERT_debug(TIMERA_readCctl(_timers[timer].base, i) == 0);
    }
    for(uint8_t i=0; i<num_ccr; ++i)
    {
        TIMERA_writeCcr(_timers[timer].base, i, 0);
        ASSERT_debug(TIMERA_readCcr(_timers[timer].base, i) == 0);
    }
    INT_removeHandler(_timers[timer].interrupt_source);
    _timers[timer].interrupt_source = intSourceInvalid;
    _timers[timer].operation = idle;
    _timers[timer].frequency = 0;
    _timers[timer].handler = NULLPTR;
    _timers[timer].running = false;
}
bool TIMER_isAvailable(TIMER_Timer_Enum timer)
{
    ASSERT_debug(_initialized);
    return _timers[timer].operation == idle;
}
bool TIMER_isRunning(TIMER_Timer_Enum timer)
{
    ASSERT_debug(_initialized);
    ASSERT_debug(_timers[timer].operation != idle);
    return _timers[timer].running;
}
