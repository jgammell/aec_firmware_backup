/*
 * interface.c
 *
 *  Created on: Oct 28, 2020
 *      Author: jgamm
 */

#include "interface.h"
#include "motor_driver.h"
#include "errors.h"
#include "vcom.h"

volatile bool command_in_progress = false;

static void indicate_done(void)
{
    ASSERT_perm(command_in_progress);
    command_in_progress = false;
    VCOM_transmitString("D", 1);
}

static void executeTurnCommand(bool dir, uint32_t num_steps)
{
    ASSERT_debug(!command_in_progress);
    command_in_progress = true;
    MD_turnMotor(num_steps, dir, &indicate_done);
}

static void executeEndSwitchCommand(void)
{
    ASSERT_debug(!command_in_progress);
    command_in_progress = true;
    MD_findEndSwitch(&indicate_done);
}

#define C2D(c) (c-'0')

static uint32_t s2d(char * s, uint8_t n)
{
    uint8_t i;
    uint32_t rv = 0, m;
    for(i=n-1, m=1; i<n; --i, m*=10)
        rv += C2D(s[i])*m;
    return rv;
}

void INTERFACE_parseAndExecuteCommand(void)
{
    ASSERT_debug(VCOM_stringAvailable('\n'));
    static char command[64];
    VCOM_getString(command, '\n', 64);
    if(INTERFACE_inProgress())
    {
        VCOM_transmitString("N", 1);
        return;
    }
    bool dir;
    uint32_t num_steps;
    switch(command[0])
    {
    case 't':
        if(MD_inUse())
        {
            VCOM_transmitString("N", 1);
            break;
        }
        VCOM_transmitString("A", 1);
        dir = (bool) C2D(command[1]);
        num_steps = s2d(command+2, 10);
        executeTurnCommand(dir, num_steps);
        break;
    case 'e':
        if(MD_inUse())
        {
            VCOM_transmitString("N", 1);
            break;
        }
        VCOM_transmitString("A", 1);
        executeEndSwitchCommand();
        break;
    case 'q':
        if(INTERFACE_inProgress())
            VCOM_transmitString("N", 1);
        else
            VCOM_transmitString("A", 1);
        break;
    default:
        VCOM_transmitString("N", 1);
        break;
    }
}

bool INTERFACE_inProgress(void)
{
    return command_in_progress;
}


