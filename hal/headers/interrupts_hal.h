/*
 * interrupts_hal.h
 *
 *  Created on: Nov 20, 2020
 *      Author: jgamm
 */

#ifndef HAL_HEADERS_INTERRUPTS_HAL_H_
#define HAL_HEADERS_INTERRUPTS_HAL_H_

#include "global.h"

typedef enum
{
    interruptsSourceInvalid     = 23,
    interruptsSourceRTC_A       = 22,
    interruptsSourceIOP2        = 21,
    interruptsSourceTA2_CCR1    = 20,
    interruptsSourceTA2_CCR2    = 20,
    interruptsSourceTA2_IFG     = 20,
    interruptsSourceTA2_CCR0    = 19,
    interruptsSourceUSCI_B1     = 18,
    interruptsSourceUSCI_A1     = 17,
    interruptsSourceIOP1        = 16,
    interruptsSourceTA1CCR1     = 15,
    interruptsSourceTA1CCR2     = 15,
    interruptsSourceTA1CCR3     = 15,
    interruptsSourceTA1CCR4     = 15,
    interruptsSourceTA1_IFG     = 15,
    interruptsSourceTA1_CCR0    = 14,
    interruptsSourceDMA         = 13,
    interruptsSourceUSB_UBM     = 12,
    interruptsSourceTA0_CCR1    = 11,
    interruptsSourceTA0_CCR2    = 11,
    interruptsSourceTA0_CCR3    = 11,
    interruptsSourceTA0_CCR4    = 11,
    interruptsSourceTA0_IFG     = 11,
    interruptsSourceTA0_CCR0    = 10,
    interruptsSourceADC12_A     = 9,
    interruptsSourceUSCI_B0     = 8,
    interruptsSourceUSCI_A0     = 7,
    interruptsSourceWDTA        = 6,
    interruptsSourceTB0_CCR1    = 5,
    interruptsSourceTB0_CCR2    = 5,
    interruptsSourceTB0_CCR3    = 5,
    interruptsSourceTB0_CCR4    = 5,
    interruptsSourceTB0_CCR5    = 5,
    interruptsSourceTB0_CCR6    = 5,
    interruptsSourceTB0_IFG     = 5,
    interruptsSourceTB0_CCR0    = 4,
    interruptsSourceCOMP_B      = 3,
    interruptsSourceUserNMI     = 2,
    interruptsSourceSystemNMI   = 1,
    interruptsSourceSystemReset = 0
} INTERRUPTS_Sources_Enum;

#define SYSCTL ((volatile uint16_t *) 0x0180)

FORCED_INLINE void INTERRUPTS_writeSysctlAddress(bool val)
{
    *SYSCTL &= ~(!val<<0);
    *SYSCTL |= val;
}
FORCED_INLINE bool INTERRUPTS_readSysctlAddress(void)
{
    return (*SYSCTL & 1) != 0;
}
FORCED_INLINE void INTERRUPTS_setGie(void)
{
    __asm__ __volatile__ ("nop { nop { eint { nop { nop");
}
FORCED_INLINE void INTERRUPTS_clearGie(void)
{
    __asm__ __volatile__ ("dint { nop { nop");
}
FORCED_INLINE void INTERRUPTS_writeHandler(INTERRUPTS_Sources_Enum source, void * handler)
{
    *((uint16_t *)(uintptr_t)(TOP_OF_RAM - (2*source) - 2)) = (uint16_t)(uintptr_t) handler;
}
FORCED_INLINE void * INTERRUPTS_readHandler(INTERRUPTS_Sources_Enum source)
{
    return (void *)(uintptr_t)(uint16_t)(uintptr_t)(TOP_OF_RAM - (2*source) - 2);
}

#endif /* HAL_HEADERS_INTERRUPTS_HAL_H_ */
