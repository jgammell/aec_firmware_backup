/*
 * global.h
 *
 *  Created on: Nov 12, 2020
 *      Author: jgamm
 */

#ifndef HAL_HEADERS_GLOBAL_H_
#define HAL_HEADERS_GLOBAL_H_

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"

#define DEBUG_ASSERTS (true)
#define TWO_MOTORS    (false)

#define FORCED_INLINE inline __attribute__ ((always_inline))
#define INTERRUPT     static __attribute__ ((interrupt))
#define NULLPTR       ((void *) 0)
#define TOP_OF_RAM    (0x4400)

#define MCLK_FREQ_HZ  (4194304UL)
#define SMCLK_FREQ_HZ (4194304UL)
#define ACLK_FREQ_HZ  (32768UL)

#define SW2_PORT  (gpioPort1)
#define SW2_PIN   (1)
#define SW3_PORT  (gpioPort1)
#define SW3_PIN   (2)
#define LED5_PORT (gpioPort7)
#define LED5_PIN  (5)
#define LED6_PORT (gpioPort7)
#define LED6_PIN  (6)

#define BNC_TRIGGER_PORT (gpioPortJ)
#define BNC_TRIGGER_PIN  (2)

#define ALIGNMENT_POWER_PORT        (gpioPort6)
#define ALIGNMENT_POWER_PIN         (3)
#define ALIGNMENT_INTENSITY_PORT    (gpioPort6)
#define ALIGNMENT_INTENSITY_PIN     (1)
#define ALIGNMENT_INTENSITY_CHANNEL (1)

#define ACCELEROMETER_SDA_PORT (gpioPort3)
#define ACCELEROMETER_SDA_PIN  (0)
#define ACCELEROMETER_SCL_PORT (gpioPort3)
#define ACCELEROMETER_SCL_PIN  (1)
#define ACCELEROMETER_INT_PORT (gpioPort2)
#define ACCELEROMETER_INT_PIN  (7)
#define ACCELEROMETER_UCB      (UCB0)

#define DELAY_TIMER   (timerA1)

#define MOTORA_TIMER        (timerA2)
#define MOTORA_TIMER_OUTPUT (1)
#define MOTORA_STEP_PORT    (gpioPort2)
#define MOTORA_STEP_PIN     (4)
#define MOTORA_DIR_PORT     (gpioPort2)
#define MOTORA_DIR_PIN      (0)
#define MOTORA_RES_PORT     (gpioPort1)
#define MOTORA_RES_PIN      (7)
#define MOTORA_SD_PORT      (gpioPort1)
#define MOTORA_SD_PIN       (6)
#define MOTORA_ESNO_PORT    (gpioPort2)
#define MOTORA_ESNO_PIN     (6)
#define MOTORA_ESNC_PORT    (gpioPort2)
#define MOTORA_ESNC_PIN     (5)
#define MOTORA_DEFAULT_FREQ 1UL<<15
#define MOTORA_FES_DELAY_MS (50)
#define MOTORA_FES_IDIST    (10000UL)
#define MOTORA_FES_ATTF     (.6)

#if TWO_MOTORS == true
#define MOTORB_TIMER        (timerA0)
#define MOTORB_TIMER_OUTPUT (2)
#define MOTORB_STEP_PORT    (gpioPort1)
#define MOTORB_STEP_PIN     (3)
#define MOTORB_DIR_PORT     (gpioPort5)
#define MOTORB_DIR_PIN      (4)
#define MOTORB_RES_PORT     (gpioPort7)
#define MOTORB_RES_PIN      (3)
#define MOTORB_SD_PORT      (gpioPort7)
#define MOTORB_SD_PIN       (1)
#define MOTORB_ESNO_PORT    (gpioPort1)
#define MOTORB_ESNO_PIN     (6)
#define MOTORB_ESNC_PORT    (gpioPort2)
#define MOTORB_ESNC_PIN     (5)
#elif TWO_MOTORS != false
#error "Invalid value for TWO_MOTORS in global.h"
#endif

#endif /* HAL_HEADERS_GLOBAL_H_ */
