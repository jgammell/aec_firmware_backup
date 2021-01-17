/*
 * io.h
 *
 *  Created on: Nov 20, 2020
 *      Author: jgamm
 */

#ifndef HAL_HEADERS_IO_HAL_H_
#define HAL_HEADERS_IO_HAL_H_

#include "global.h"

typedef struct
{
    const volatile uint8_t IN;
    const volatile uint8_t empty0;
    volatile       uint8_t OUT;
    const volatile uint8_t empty1;
    volatile       uint8_t DIR;
    const volatile uint8_t empty2;
    volatile       uint8_t RES;
    const volatile uint8_t empty3;
    volatile       uint8_t DS;
    const volatile uint8_t empty4;
    volatile       uint8_t SEL;
    const volatile uint8_t empty5[13];
    volatile       uint8_t IES;
    const volatile uint8_t empty6;
    volatile       uint8_t IEN;
    const volatile uint8_t empty7;
    volatile       uint8_t IFG;
    const volatile uint8_t empty8;
} IO_Registers_Struct;

#define PORT1 ((IO_Registers_Struct *) 0x0200)
#define PORT2 ((IO_Registers_Struct *) 0x0201)
#define PORT3 ((IO_Registers_Struct *) 0x0220)
#define PORT4 ((IO_Registers_Struct *) 0x0221)
#define PORT5 ((IO_Registers_Struct *) 0x0240)
#define PORT6 ((IO_Registers_Struct *) 0x0241)
#define PORT7 ((IO_Registers_Struct *) 0x0260)
#define PORT8 ((IO_Registers_Struct *) 0x0261)
#define PORTJ ((IO_Registers_Struct *) 0x0320)

typedef enum
{
    ioDirInput  = false,
    ioDirOutput = true
} IO_Dir_Enum;
typedef enum
{
    ioDsReduced = false,
    ioDsFull    = true
} IO_Ds_Enum;
typedef enum
{
    ioSelGpio       = false,
    ioSelPeripheral = true
} IO_Sel_Enum;
typedef enum
{
    ioIesRising  = false,
    ioIesFalling = true
} IO_Ies_Enum;

FORCED_INLINE uint8_t IO_readIn(IO_Registers_Struct * Px)
{
    return Px->IN;
}
FORCED_INLINE bool IO_readInPin(IO_Registers_Struct * Px, uint8_t pin)
{
    return (Px->IN & (1<<pin)) != 0;
}
FORCED_INLINE void IO_writeOut(IO_Registers_Struct * Px, uint8_t val)
{
    Px->OUT = val;
}
FORCED_INLINE uint8_t IO_readOut(IO_Registers_Struct * Px)
{
    return Px->OUT;
}
FORCED_INLINE void IO_writeOutPin(IO_Registers_Struct * Px, uint8_t pin, bool val)
{
    Px->OUT &= ~(!val<<pin);
    Px->OUT |= val<<pin;
}
FORCED_INLINE bool IO_readOutPin(IO_Registers_Struct * Px, uint8_t pin)
{
    return (Px->OUT & (1<<pin)) != 0;
}
FORCED_INLINE void IO_writeDir(IO_Registers_Struct * Px, uint8_t val)
{
    Px->DIR  = val;
}
FORCED_INLINE uint8_t IO_readDir(IO_Registers_Struct * Px)
{
    return Px->DIR;
}
FORCED_INLINE void IO_writeDirPin(IO_Registers_Struct * Px, uint8_t pin, IO_Dir_Enum val)
{
    Px->DIR &= ~(!val<<pin);
    Px->DIR |= val<<pin;
}
FORCED_INLINE IO_Dir_Enum IO_readDirPin(IO_Registers_Struct * Px, uint8_t pin)
{
    return (Px->DIR & (1<<pin)) != 0;
}
FORCED_INLINE void IO_writeRes(IO_Registers_Struct * Px, uint8_t val)
{
    Px->RES = val;
}
FORCED_INLINE uint8_t IO_readRes(IO_Registers_Struct * Px)
{
    return Px->RES;
}
FORCED_INLINE void IO_writeResPin(IO_Registers_Struct * Px, uint8_t pin, bool val)
{
    Px->RES &= ~(!val<<pin);
    Px->RES |= val<<pin;
}
FORCED_INLINE bool IO_readResPin(IO_Registers_Struct * Px, uint8_t pin)
{
    return (Px->RES & (1<<pin)) != 0;
}
FORCED_INLINE void IO_writeDs(IO_Registers_Struct * Px, uint8_t val)
{
    Px->DS = val;
}
FORCED_INLINE uint8_t IO_readDs(IO_Registers_Struct * Px)
{
    return Px->DS;
}
FORCED_INLINE void IO_writeDsPin(IO_Registers_Struct * Px, uint8_t pin, IO_Ds_Enum val)
{
    Px->DS &= ~(!val<<pin);
    Px->DS |= val<<pin;
}
FORCED_INLINE IO_Ds_Enum IO_readDsPin(IO_Registers_Struct * Px, uint8_t pin)
{
    return (Px->DS & (1<<pin)) != 0;
}
FORCED_INLINE void IO_writeSel(IO_Registers_Struct * Px, uint8_t val)
{
    Px->SEL = val;
}
FORCED_INLINE uint8_t IO_readSel(IO_Registers_Struct * Px)
{
    return Px->SEL;
}
FORCED_INLINE void IO_writeSelPin(IO_Registers_Struct * Px, uint8_t pin, IO_Sel_Enum val)
{
    Px->SEL &= ~(!val<<pin);
    Px->SEL |= val<<pin;
}
FORCED_INLINE IO_Sel_Enum IO_readSelPin(IO_Registers_Struct * Px, uint8_t pin)
{
    return (Px->SEL & (1<<pin)) != 0;
}
FORCED_INLINE void IO_writeIes(IO_Registers_Struct * Px, uint8_t val)
{
    Px->IES = val;
}
FORCED_INLINE uint8_t IO_readIes(IO_Registers_Struct * Px)
{
    return Px->IES;
}
FORCED_INLINE void IO_writeIesPin(IO_Registers_Struct * Px, uint8_t pin, IO_Ies_Enum val)
{
    Px->IES &= ~(!val<<pin);
    Px->IES |= val<<pin;
}
FORCED_INLINE IO_Ies_Enum IO_readIesPin(IO_Registers_Struct * Px, uint8_t pin)
{
    return (Px->IES & (1<<pin)) != 0;
}
FORCED_INLINE void IO_writeIen(IO_Registers_Struct * Px, uint8_t val)
{
    Px->IEN = val;
}
FORCED_INLINE uint8_t IO_readIen(IO_Registers_Struct * Px)
{
    return Px->IEN;
}
 FORCED_INLINE void IO_writeIenPin(IO_Registers_Struct * Px, uint8_t pin, bool val)
{
    Px->IEN &= ~(!val<<pin);
    Px->IEN |= val<<pin;
}
FORCED_INLINE bool IO_readIenPin(IO_Registers_Struct * Px, uint8_t pin)
{
    return (Px->IEN & (1<<pin)) != 0;
}
FORCED_INLINE void IO_writeIfg(IO_Registers_Struct * Px, uint8_t val)
{
    Px->IFG = val;
}
FORCED_INLINE uint8_t IO_readIfg(IO_Registers_Struct * Px)
{
    return Px->IFG;
}
FORCED_INLINE void IO_writeIfgPin(IO_Registers_Struct * Px, uint8_t pin, bool val)
{
    Px->IFG &= ~(!val<<pin);
    Px->IFG |= val<<pin;
}
FORCED_INLINE bool IO_readIfgPin(IO_Registers_Struct * Px, uint8_t pin)
{
    return (Px->IFG & (1<<pin)) != 0;
}


#endif /* HAL_HEADERS_IO_HAL_H_ */
