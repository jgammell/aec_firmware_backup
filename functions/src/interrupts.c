/*
 * interrupts.c
 *
 *  Created on: Nov 20, 2020
 *      Author: jgamm
 */

#include "interrupts.h"
#include "interrupts_hal.h"
#include "errors.h"

#define NUM_SOURCES (64)

static uint16_t num_blocks = 0;

INTERRUPT void INT_defaultHandler(void)
{
    ASSERT_perm(false);
}

void INT_initialize(void)
{
    INTERRUPTS_writeSysctlAddress(true);
    for(uint8_t i=0; i<NUM_SOURCES; ++i)
        INTERRUPTS_writeHandler(i, &INT_defaultHandler);
}

void INT_setHandler(INT_Source_Enum source, void (* handler)(void))
{
    //ASSERT_debug(INTERRUPTS_readHandler(source) == &INT_defaultHandler);
    INTERRUPTS_writeHandler(source, handler);
}

void INT_removeHandler(INT_Source_Enum source)
{
    //ASSERT_debug(INTERRUPTS_readHandler(source) != &INT_defaultHandler);
    INTERRUPTS_writeHandler(source, &INT_defaultHandler);
}

void INT_blockInterrupts(void)
{
    if(num_blocks == 0)
        INTERRUPTS_clearGie();
    num_blocks += 1;
}

void INT_unblockInterrupts(void)
{
    if(num_blocks == 1)
        INTERRUPTS_setGie();
    num_blocks -= 1;
}
