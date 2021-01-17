/*
 * gpio.c
 *
 *  Created on: Nov 20, 2020
 *      Author: jgamm
 */

#include "gpio.h"
#include "io_hal.h"
#include "errors.h"
#include "interrupts.h"

#define NUM_PORTS (9)
#define NUM_INT_PORTS (2)
#define NUM_PINS (8)

static void _defaultHandler(void)
{
    ASSERT_perm(false);
}

static void (* _p1_handlers[NUM_PINS])(void) =
{
 &_defaultHandler,
 &_defaultHandler,
 &_defaultHandler,
 &_defaultHandler,
 &_defaultHandler,
 &_defaultHandler,
 &_defaultHandler,
 &_defaultHandler
};
static void (* _p2_handlers[NUM_PINS])(void) =
{
 &_defaultHandler,
 &_defaultHandler,
 &_defaultHandler,
 &_defaultHandler,
 &_defaultHandler,
 &_defaultHandler,
 &_defaultHandler,
 &_defaultHandler
};

FORCED_INLINE IO_Registers_Struct * _port(GPIO_Port_Enum port)
{
    switch(port)
    {
    case gpioPort1:
        return PORT1;
    case gpioPort2:
        return PORT2;
    case gpioPort3:
        return PORT3;
    case gpioPort4:
        return PORT4;
    case gpioPort5:
        return PORT5;
    case gpioPort6:
        return PORT6;
    case gpioPort7:
        return PORT7;
    case gpioPort8:
        return PORT8;
    case gpioPortJ:
        return PORTJ;
    default:
        ASSERT_perm(false);
    }
    return 0;
}
FORCED_INLINE IO_Ies_Enum _ies(GPIO_Interrupts_Enum interrupts)
{
    switch(interrupts)
    {
    case gpioInterruptsRising:
        return ioIesRising;
    case gpioInterruptsFalling:
    case gpioInterruptsNone:
        return ioIesFalling;
    default:
        ASSERT_debug(false);
    }
    return 0;
}
FORCED_INLINE bool _ien(GPIO_Interrupts_Enum interrupts)
{
    switch(interrupts)
    {
    case gpioInterruptsRising:
    case gpioInterruptsFalling:
        return true;
    case gpioInterruptsNone:
        return false;
    default:
        ASSERT_debug(false);
    }
    return 0;
}
FORCED_INLINE IO_Dir_Enum _dir(GPIO_Direction_Enum dir)
{
    switch(dir)
    {
    case gpioDirectionInput:
        return ioDirInput;
    case gpioDirectionOutput:
        return ioDirOutput;
    default:
        ASSERT_perm(false);
    }
    return 0;
}
FORCED_INLINE bool _res(GPIO_Resistor_Enum resistor)
{
    switch(resistor)
    {
    case gpioResistorPullup:
    case gpioResistorPulldown:
        return true;
    case gpioResistorNone:
        return false;
    default:
        ASSERT_debug(false);
    }
    return 0;
}
FORCED_INLINE bool _out(GPIO_Resistor_Enum resistor)
{
    switch(resistor)
    {
    case gpioResistorPullup:
        return true;
    case gpioResistorPulldown:
    case gpioResistorNone:
        return false;
    default:
        ASSERT_debug(false);
    }
    return 0;
}
FORCED_INLINE IO_Ds_Enum _ds(GPIO_Drivestrength_Enum drive_strength)
{
    switch(drive_strength)
    {
    case gpioDrivestrengthReduced:
        return ioDsReduced;
    case gpioDrivestrengthFull:
        return ioDsFull;
    default:
        ASSERT_debug(false);
    }
    return 0;
}

INTERRUPT void port1_IRQHandler(void)
{
    ASSERT_debug(IO_readIfg(PORT1) != 0);
    INT_blockInterrupts();
    for(uint8_t pin=0; pin<NUM_PINS; ++pin)
    {
        if(IO_readIenPin(PORT1, pin) && IO_readIfgPin(PORT1, pin))
            (_p1_handlers[pin])();
    }
    IO_writeIfg(PORT1, 0);
    INT_unblockInterrupts();
}
INTERRUPT void port2_IRQHandler(void)
{
    ASSERT_debug(IO_readIfg(PORT2) != 0);
    INT_blockInterrupts();
    for(uint8_t pin=0; pin<NUM_PINS; ++pin)
    {
        if(IO_readIenPin(PORT2, pin) && IO_readIfgPin(PORT2, pin))
            (_p2_handlers[pin])();
    }
    IO_writeIfg(PORT2, 0);
    INT_unblockInterrupts();
}

void GPIO_initialize(void)
{
    for(uint8_t i=0; i<NUM_PORTS; ++i)
    {
        IO_writeOut(_port(i), 0);
        IO_writeDir(_port(i), 0);
        IO_writeRes(_port(i), 0xFF);
        IO_writeDs(_port(i), 0);
        IO_writeSel(_port(i), 0);
    }
    for(uint8_t i=0; i<NUM_INT_PORTS; ++i)
    {
        IO_writeIes(_port(i), 0);
        IO_writeIen(_port(i), 0);
        IO_writeIfg(_port(i), 0);
    }
    INT_setHandler(intSourceIOP1, &port1_IRQHandler);
    INT_setHandler(intSourceIOP2, &port2_IRQHandler);
}

void GPIO_configurePin(GPIO_Port_Enum port, uint8_t pin, GPIO_PinConfig_Struct * settings)
{
    ASSERT_debug(port<NUM_PORTS);
    ASSERT_debug(pin<NUM_PINS);
    IO_writeOutPin(_port(port), pin, settings->direction == gpioDirectionOutput? settings->initial_output : _out(settings->resistor));
    IO_writeDirPin(_port(port), pin, _dir(settings->direction));
    IO_writeResPin(_port(port), pin, _res(settings->resistor));
    IO_writeDsPin(_port(port), pin, _ds(settings->drive_strength));
    IO_writeSelPin(_port(port), pin, ioSelGpio);
    if(port<NUM_INT_PORTS)
    {
        IO_writeIesPin(_port(port), pin, ioIesRising);
        IO_writeIenPin(_port(port), pin, false);
        IO_writeIfgPin(_port(port), pin, false);
    }
}

void GPIO_configurePinInterrupts(GPIO_Port_Enum port, uint8_t pin, GPIO_Interrupts_Enum setting, void (*handler)(void))
{
    ASSERT_debug(port<NUM_INT_PORTS);
    if(handler != NULLPTR)
        (port == gpioPort1? _p1_handlers : _p2_handlers)[pin] = handler;
    if(setting == gpioInterruptsNone)
        (port == gpioPort1? _p1_handlers : _p2_handlers)[pin] = &_defaultHandler;
    IO_writeIfgPin(_port(port), pin, false);
    IO_writeIenPin(_port(port), pin, _ien(setting));
    IO_writeIesPin(_port(port), pin, _ies(setting));
}

GPIO_Interrupts_Enum GPIO_getPinInterrupts(GPIO_Port_Enum port, uint8_t pin)
{
    return IO_readIenPin(_port(port), pin) == false? gpioInterruptsNone : \
            IO_readIesPin(_port(port), pin) == ioIesRising? gpioInterruptsRising : \
            gpioInterruptsFalling;
}

void GPIO_writePin(GPIO_Port_Enum port, uint8_t pin, bool val)
{
    IO_writeOutPin(_port(port), pin, val);
}

bool GPIO_readPin(GPIO_Port_Enum port, uint8_t pin)
{
    return IO_readDirPin(_port(port), pin) == ioDirOutput? IO_readOutPin(_port(port), pin) : \
            IO_readInPin(_port(port), pin);
}

