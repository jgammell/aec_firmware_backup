/*
 * timera_hal.h
 *
 *  Created on: Nov 12, 2020
 *      Author: jgamm
 */

#ifndef HAL_HEADERS_TIMERA_HAL_H_
#define HAL_HEADERS_TIMERA_HAL_H_

#include "global.h"

typedef struct
{
    volatile uint16_t      CTL;
    volatile uint16_t      CCTL[7];
    volatile uint16_t      R;
    volatile uint16_t      CCR[7];
    volatile uint16_t      EX0;
    volatile const uint8_t RESERVED0[0xC];
    volatile uint16_t      IV;
} TIMERA_Registers_Struct;

#define TA0 ((TIMERA_Registers_Struct *) 0x0340)
#define TA1 ((TIMERA_Registers_Struct *) 0x0380)
#define TA2 ((TIMERA_Registers_Struct *) 0x0400)

#define TIMERA_CTL_IFG_SHIFT     (0)
#define TIMERA_CTL_IE_SHIFT      (1)
#define TIMERA_CTL_CLR_SHIFT     (2)
#define TIMERA_CTL_MC_SHIFT      (4)
#define TIMERA_CTL_ID_SHIFT      (6)
#define TIMERA_CTL_TASSEL_SHIFT  (8)
#define TIMERA_CCTL_IFG_SHIFT    (0)
#define TIMERA_CCTL_COV_SHIFT    (1)
#define TIMERA_CCTL_OUT_SHIFT    (2)
#define TIMERA_CCTL_CCI_SHIFT    (3)
#define TIMERA_CCTL_CCIE_SHIFT   (4)
#define TIMERA_CCTL_OUTMOD_SHIFT (5)
#define TIMERA_CCTL_CAP_SHIFT    (8)
#define TIMERA_CCTL_SCCI_SHIFT   (10)
#define TIMERA_CCTL_SCS_SHIFT    (11)
#define TIMERA_CCTL_CCIS_SHIFT   (12)
#define TIMERA_CCTL_CM_SHIFT     (14)
#define TIMERA_EX0_TAIDEX_SHIFT  (0)

#define TIMERA_CTL_IFG_MASK     (1<<TIMERA_CTL_IFG_SHIFT)
#define TIMERA_CTL_IE_MASK      (1<<TIMERA_CTL_IFG_SHIFT)
#define TIMERA_CTL_CLR_MASK     (1<<TIMERA_CTL_IFG_SHIFT)
#define TIMERA_CTL_MC_MASK      (3<<TIMERA_CTL_MC_SHIFT)
#define TIMERA_CTL_ID_MASK      (3<<TIMERA_CTL_ID_SHIFT)
#define TIMERA_CTL_TASSEL_MASK  (3<<TIMERA_CTL_TASSEL_SHIFT)
#define TIMERA_CCTL_IFG_MASK    (1<<TIMERA_CCTL_IFG_SHIFT)
#define TIMERA_CCTL_COV_MASK    (1<<TIMERA_CCTL_COV_SHIFT)
#define TIMERA_CCTL_OUT_MASK    (1<<TIMERA_CCTL_OUT_SHIFT)
#define TIMERA_CCTL_CCI_MASK    (1<<TIMERA_CCTL_CCI_SHIFT)
#define TIMERA_CCTL_CCIE_MASK   (1<<TIMERA_CCTL_CCIE_SHIFT)
#define TIMERA_CCTL_OUTMOD_MASK (7<<TIMERA_CCTL_OUTMOD_SHIFT)
#define TIMERA_CCTL_CAP_MASK    (1<<TIMERA_CCTL_CAP_SHIFT)
#define TIMERA_CCTL_SCCI_MASK   (1<<TIMERA_CCTL_SCCI_SHIFT)
#define TIMERA_CCTL_SCS_MASK    (1<<TIMERA_CCTL_SCS_SHIFT)
#define TIMERA_CCTL_CCIS_MASK   (3<<TIMERA_CCTL_CCIS_SHIFT)
#define TIMERA_CCTL_CM_MASK     (3<<TIMERA_CCTL_CM_SHIFT)
#define TIMERA_EX0_TAIDEX_MASK  (7<<TIMERA_EX0_TAIDEX_SHIFT)

typedef enum
{
    timeraCtlMcStop       = 0<<TIMERA_CTL_MC_SHIFT,
    timeraCtlMcUp         = 1<<TIMERA_CTL_MC_SHIFT,
    timeraCtlMcContinuous = 2<<TIMERA_CTL_MC_SHIFT,
    timeraCtlMcUpdown     = 3<<TIMERA_CTL_MC_SHIFT
} TIMERA_CtlMc_Enum;
typedef enum
{
    timeraCtlId1 = 0<<TIMERA_CTL_ID_SHIFT,
    timeraCtlId2 = 1<<TIMERA_CTL_ID_SHIFT,
    timeraCtlId4 = 2<<TIMERA_CTL_ID_SHIFT,
    timeraCtlId8 = 3<<TIMERA_CTL_ID_SHIFT
} TIMERA_CtlId_Enum;
typedef enum
{
    timeraCtlTasselTaclk = 0<<TIMERA_CTL_TASSEL_SHIFT,
    timeraCtlTasselAclk  = 1<<TIMERA_CTL_TASSEL_SHIFT,
    timeraCtlTasselSmclk = 2<<TIMERA_CTL_TASSEL_SHIFT,
    timeraCtlTasselInclk = 3<<TIMERA_CTL_TASSEL_SHIFT
} TIMERA_CtlTassel_Enum;
typedef enum
{
    timeraCctlOutmodOutput      = 0<<TIMERA_CCTL_OUTMOD_SHIFT,
    timeraCctlOutmodSet         = 1<<TIMERA_CCTL_OUTMOD_SHIFT,
    timeraCctlOutmodTogglereset = 2<<TIMERA_CCTL_OUTMOD_SHIFT,
    timeraCctlOutmodSetreset    = 3<<TIMERA_CCTL_OUTMOD_SHIFT,
    timeraCctlOutmodToggle      = 4<<TIMERA_CCTL_OUTMOD_SHIFT,
    timeraCctlOutmodReset       = 5<<TIMERA_CCTL_OUTMOD_SHIFT,
    timeraCctlOutmodToggleset   = 6<<TIMERA_CCTL_OUTMOD_SHIFT,
    timeraCctlOutmodResetset    = 7<<TIMERA_CCTL_OUTMOD_SHIFT
} TIMERA_CctlOutmod_Enum;
typedef enum
{
    timeraCctlCapCompare = 0x00<<TIMERA_CCTL_CAP_SHIFT,
    timeraCctlCapCapture = 0x01<<TIMERA_CCTL_CAP_SHIFT
} TIMERA_CctlCap_Enum;
typedef enum
{
    timeraCctlScsAsynchronous = 0x00<<TIMERA_CCTL_CAP_SHIFT,
    timeraCctlScsSynchronous  = 0x01<<TIMERA_CCTL_CAP_SHIFT
} TIMERA_CctlScs_Enum;
typedef enum
{
    timeraCctlCcisCcixa = 0<<TIMERA_CCTL_CCIS_SHIFT,
    timeraCctlCcisCcixb = 1<<TIMERA_CCTL_CCIS_SHIFT,
    timeraCctlCcisGnd   = 2<<TIMERA_CCTL_CCIS_SHIFT,
    timeraCctlCcisVcc   = 3<<TIMERA_CCTL_CCIS_SHIFT
} TIMERA_CctlCcis_Enum;
typedef enum
{
    timeraCctlCmDisabled = 0<<TIMERA_CCTL_CM_SHIFT,
    timeraCctlCmPos      = 1<<TIMERA_CCTL_CM_SHIFT,
    timeraCctlCmNeg      = 2<<TIMERA_CCTL_CM_SHIFT,
    timeraCctlCmBoth     = 3<<TIMERA_CCTL_CM_SHIFT
} TIMERA_CctlCm_Enum;
typedef enum
{
    timeraIvNone     = 0x00,
    timeraIvCc1      = 0x02,
    timeraIvCc2      = 0x04,
    timeraIvCc3      = 0x06,
    timeraIvCc4      = 0x08,
    timeraIvCc5      = 0x0A,
    timeraIvCc6      = 0x0C,
    timeraIvOverflow = 0x0E
} TIMERA_Iv_Enum;
typedef enum
{
    timeraEx0Taidex1 = 0<<TIMERA_EX0_TAIDEX_SHIFT,
    timeraEx0Taidex2 = 1<<TIMERA_EX0_TAIDEX_SHIFT,
    timeraEx0Taidex3 = 2<<TIMERA_EX0_TAIDEX_SHIFT,
    timeraEx0Taidex4 = 3<<TIMERA_EX0_TAIDEX_SHIFT,
    timeraEx0Taidex5 = 4<<TIMERA_EX0_TAIDEX_SHIFT,
    timeraEx0Taidex6 = 5<<TIMERA_EX0_TAIDEX_SHIFT,
    timeraEx0Taidex7 = 6<<TIMERA_EX0_TAIDEX_SHIFT,
    timeraEx0Taidex8 = 7<<TIMERA_EX0_TAIDEX_SHIFT
} TIMERA_Ex0Taidex_Enum;

FORCED_INLINE void TIMERA_writeCtl(TIMERA_Registers_Struct * TAx, uint16_t val)
{
    TAx->CTL = val;
}
FORCED_INLINE uint16_t TIMERA_readCtl(TIMERA_Registers_Struct * TAx)
{
    return TAx->CTL;
}
FORCED_INLINE void TIMERA_writeCtlIfg(TIMERA_Registers_Struct * TAx, bool val)
{
    TAx->CTL &= ~(!val<<TIMERA_CTL_IFG_SHIFT);
    TAx->CTL |= val<<TIMERA_CTL_IFG_SHIFT;
}
FORCED_INLINE bool TIMERA_readCtlIfg(TIMERA_Registers_Struct * TAx)
{
    return (TAx->CTL & TIMERA_CTL_IFG_MASK) >> TIMERA_CTL_IFG_SHIFT;
}
FORCED_INLINE void TIMERA_writeCtlIe(TIMERA_Registers_Struct * TAx, bool val)
{
    TAx->CTL &= ~(!val<<TIMERA_CTL_IE_SHIFT);
    TAx->CTL |= val<<TIMERA_CTL_IE_SHIFT;
}
FORCED_INLINE bool TIMERA_readCtlIe(TIMERA_Registers_Struct * TAx)
{
    return (TAx->CTL & TIMERA_CTL_IE_MASK) >> TIMERA_CTL_IE_SHIFT;
}
FORCED_INLINE void TIMERA_setCtlClr(TIMERA_Registers_Struct * TAx)
{
    TAx->CTL |= TIMERA_CTL_CLR_MASK;
}
FORCED_INLINE void TIMERA_writeCtlMc(TIMERA_Registers_Struct * TAx, TIMERA_CtlMc_Enum val)
{
    TAx->CTL &= ~TIMERA_CTL_MC_MASK | val;
    TAx->CTL |= val;
}
FORCED_INLINE TIMERA_CtlMc_Enum TIMERA_readCtlMc(TIMERA_Registers_Struct * TAx)
{
    return (TAx->CTL & TIMERA_CTL_MC_MASK) >> TIMERA_CTL_MC_SHIFT;
}
FORCED_INLINE void TIMERA_writeCtlId(TIMERA_Registers_Struct * TAx, TIMERA_CtlId_Enum val)
{
    TAx->CTL &= ~TIMERA_CTL_ID_MASK | val;
    TAx->CTL |= val;
}
FORCED_INLINE TIMERA_CtlId_Enum TIMERA_readCtlId(TIMERA_Registers_Struct * TAx)
{
    return (TAx->CTL & TIMERA_CTL_ID_MASK) >> TIMERA_CTL_ID_SHIFT;
}
FORCED_INLINE void TIMERA_writeCtlTassel(TIMERA_Registers_Struct * TAx, TIMERA_CtlTassel_Enum val)
{
    TAx->CTL &= ~TIMERA_CTL_TASSEL_MASK | val;
    TAx->CTL |= val;
}
FORCED_INLINE TIMERA_CtlTassel_Enum TIMERA_readCtlTassel(TIMERA_Registers_Struct * TAx)
{
    return (TAx->CTL & TIMERA_CTL_TASSEL_MASK) >> TIMERA_CTL_TASSEL_SHIFT;
}
FORCED_INLINE void TIMERA_writeCctl(TIMERA_Registers_Struct * TAx, uint8_t n, uint16_t val)
{
    TAx->CCTL[n] = val;
}
FORCED_INLINE uint16_t TIMERA_readCctl(TIMERA_Registers_Struct * TAx, uint8_t n)
{
    return TAx->CCTL[n];
}
FORCED_INLINE void TIMERA_writeCctlCcifg(TIMERA_Registers_Struct * TAx, uint8_t n, bool val)
{
    TAx->CCTL[n] &= ~(!val<<TIMERA_CCTL_IFG_SHIFT);
    TAx->CCTL[n] |= val<<TIMERA_CCTL_IFG_SHIFT;
}
FORCED_INLINE bool TIMERA_readCctlCcifg(TIMERA_Registers_Struct * TAx, uint8_t n)
{
    return (TAx->CCTL[n] & TIMERA_CCTL_IFG_MASK) >> TIMERA_CCTL_IFG_SHIFT;
}
FORCED_INLINE void TIMERA_writeCctlCov(TIMERA_Registers_Struct * TAx, uint8_t n, bool val)
{
    TAx->CCTL[n] &= ~(!val<<TIMERA_CCTL_COV_SHIFT);
    TAx->CCTL[n] |= val<<TIMERA_CCTL_COV_SHIFT;
}
FORCED_INLINE bool TIMERA_readCctlCov(TIMERA_Registers_Struct * TAx, uint8_t n)
{
    return (TAx->CCTL[n] & TIMERA_CCTL_COV_MASK) >> TIMERA_CCTL_COV_SHIFT;
}
FORCED_INLINE void TIMERA_writeCctlOut(TIMERA_Registers_Struct * TAx, uint8_t n, bool val)
{
    TAx->CCTL[n] &= ~(!val<<TIMERA_CCTL_OUT_SHIFT);
    TAx->CCTL[n] |= val<<TIMERA_CCTL_OUT_SHIFT;
}
FORCED_INLINE bool TIMERA_readCctlOut(TIMERA_Registers_Struct * TAx, uint8_t n)
{
    return (TAx->CCTL[n] & TIMERA_CCTL_OUT_MASK) >> TIMERA_CCTL_OUT_SHIFT;
}
FORCED_INLINE bool TIMERA_readCctlCci(TIMERA_Registers_Struct * TAx, uint8_t n)
{
    return (TAx->CCTL[n] & TIMERA_CCTL_CCI_MASK) >> TIMERA_CCTL_CCI_SHIFT;
}
FORCED_INLINE void TIMERA_writeCctlCcie(TIMERA_Registers_Struct * TAx, uint8_t n, bool val)
{
    TAx->CCTL[n] &= ~(!val<<TIMERA_CCTL_CCIE_SHIFT);
    TAx->CCTL[n] |= val<<TIMERA_CCTL_CCIE_SHIFT;
}
FORCED_INLINE bool TIMERA_readCctlCcie(TIMERA_Registers_Struct * TAx, uint8_t n)
{
    return (TAx->CCTL[n] & TIMERA_CCTL_CCIE_MASK) >> TIMERA_CCTL_CCIE_SHIFT;
}
FORCED_INLINE void TIMERA_writeCctlOutmod(TIMERA_Registers_Struct * TAx, uint8_t n, TIMERA_CctlOutmod_Enum val)
{
    TAx->CCTL[n] &= ~TIMERA_CCTL_OUTMOD_MASK | val;
    TAx->CCTL[n] |= val;
}
FORCED_INLINE TIMERA_CctlOutmod_Enum TIMERA_readCctlOutmod(TIMERA_Registers_Struct * TAx, uint8_t n)
{
    return (TAx->CCTL[n] & TIMERA_CCTL_OUTMOD_MASK) >> TIMERA_CCTL_OUTMOD_SHIFT;
}
FORCED_INLINE void TIMERA_writeCctlCap(TIMERA_Registers_Struct * TAx, uint8_t n, TIMERA_CctlCap_Enum val)
{
    TAx->CCTL[n] &= ~TIMERA_CCTL_CAP_MASK | val;
    TAx->CCTL[n] |= val;
}
FORCED_INLINE TIMERA_CctlCap_Enum TIMERA_readCctlCap(TIMERA_Registers_Struct * TAx, uint8_t n)
{
    return (TAx->CCTL[n] & TIMERA_CCTL_CAP_MASK) >> TIMERA_CCTL_CAP_SHIFT;
}
FORCED_INLINE void TIMERA_writeCctlScci(TIMERA_Registers_Struct * TAx, uint8_t n, bool val)
{
    TAx->CCTL[n] &= ~(!val<<TIMERA_CCTL_SCCI_SHIFT);
    TAx->CCTL[n] |= val<<TIMERA_CCTL_SCCI_SHIFT;
}
FORCED_INLINE bool TIMERA_readCctlScci(TIMERA_Registers_Struct * TAx, uint8_t n)
{
    return (TAx->CCTL[n] & TIMERA_CCTL_SCCI_MASK) >> TIMERA_CCTL_SCCI_SHIFT;
}
FORCED_INLINE void TIMERA_writeCctlScs(TIMERA_Registers_Struct * TAx, uint8_t n, TIMERA_CctlScs_Enum val)
{
    TAx->CCTL[n] &= ~TIMERA_CCTL_SCS_MASK | val;
    TAx->CCTL[n] |= val;
}
FORCED_INLINE TIMERA_CctlScs_Enum TIMERA_readCctlScs(TIMERA_Registers_Struct * TAx, uint8_t n)
{
    return (TAx->CCTL[n] & TIMERA_CCTL_SCS_MASK) >> TIMERA_CCTL_SCS_SHIFT;
}
FORCED_INLINE void TIMERA_writeCctlCcis(TIMERA_Registers_Struct * TAx, uint8_t n, TIMERA_CctlCcis_Enum val)
{
    TAx->CCTL[n] &= ~TIMERA_CCTL_CCIS_MASK | val;
    TAx->CCTL[n] |= val;
}
FORCED_INLINE TIMERA_CctlCcis_Enum TIMERA_readCctlCcis(TIMERA_Registers_Struct * TAx, uint8_t n)
{
    return (TAx->CCTL[n] & TIMERA_CCTL_CCIS_MASK) >> TIMERA_CCTL_CCIS_SHIFT;
}
FORCED_INLINE void TIMERA_writeCctlCm(TIMERA_Registers_Struct * TAx, uint8_t n, TIMERA_CctlCm_Enum val)
{
    TAx->CCTL[n] &= ~TIMERA_CCTL_CM_MASK | val;
    TAx->CCTL[n] |= val;
}
FORCED_INLINE TIMERA_CctlCm_Enum TIMERA_readCctlCm(TIMERA_Registers_Struct * TAx, uint8_t n)
{
    return (TAx->CCTL[n] & TIMERA_CCTL_CM_MASK) >> TIMERA_CCTL_CM_SHIFT;
}
FORCED_INLINE void TIMERA_writeR(TIMERA_Registers_Struct * TAx, uint16_t val)
{
    TAx->R = val;
}
FORCED_INLINE uint16_t TIMERA_readR(TIMERA_Registers_Struct * TAx)
{
    return TAx->R;
}
FORCED_INLINE void TIMERA_writeCcr(TIMERA_Registers_Struct * TAx, uint8_t n, uint16_t val)
{
    TAx->CCR[n] = val;
}
FORCED_INLINE uint16_t TIMERA_readCcr(TIMERA_Registers_Struct * TAx, uint8_t n)
{
    return TAx->CCR[n];
}
FORCED_INLINE TIMERA_Iv_Enum TIMERA_readIv(TIMERA_Registers_Struct * TAx)
{
    return TAx->IV;
}
FORCED_INLINE void TIMERA_writeEx0(TIMERA_Registers_Struct * TAx, uint16_t val)
{
    TAx->EX0 = val;
}
FORCED_INLINE uint16_t TIMERA_readEx0(TIMERA_Registers_Struct * TAx)
{
    return TAx->EX0;
}
FORCED_INLINE void TIMERA_writeEx0Taidex(TIMERA_Registers_Struct * TAx, TIMERA_Ex0Taidex_Enum val)
{
    TAx->EX0 &= ~TIMERA_EX0_TAIDEX_MASK | val;
    TAx->EX0 |= val;
}
FORCED_INLINE TIMERA_Ex0Taidex_Enum TIMERA_readEx0Taidex(TIMERA_Registers_Struct * TAx)
{
    return (TAx->EX0 & TIMERA_EX0_TAIDEX_MASK) >> TIMERA_EX0_TAIDEX_SHIFT;
}

#endif /* HAL_HEADERS_TIMERA_HAL_H_ */
