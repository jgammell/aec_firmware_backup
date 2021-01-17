#include <msp430.h> 
#include "gpio.h"
#include "interrupts.h"
#include "timer.h"
#include "button.h"
#include "led.h"
#include "motor_driver.h"
#include "errors.h"
#include "global.h"

#include "vcom.h"
#include "app_clocks.h"
#include "interface.h"

void setup(void)
{
    INT_blockInterrupts();
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    INT_initialize();
    TIMER_initialize();
    GPIO_initialize();
    LED_initialize();
    BUTTON_initialize();
    MD_initialize();
    INT_unblockInterrupts();
}


/**
 * main.c
 */
int main(void)
{
    setup();

    CLOCKS_initialize();
    bool command_in_waiting = false;
    VCOM_Init_Struct vcom_settings =
    {
     .parity = uartParityNone,
     .order = uartLsbFirst,
     .data_bits = uartDataBits8,
     .stop_bits = uartStopBits1,
     .baud_rate = uartBaudRate115200
    };
    VCOM_initialize(&vcom_settings, &command_in_waiting);

    while(1)
    {
        if(command_in_waiting)
        {
            INTERFACE_parseAndExecuteCommand();
            command_in_waiting = false;
        }
    }
	
	return 0;
}
