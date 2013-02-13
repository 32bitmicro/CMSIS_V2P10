/* ----------------------------------------------------------------------------
 * Copyright (c) 2012 Semiconductor Components Industries, LLC
 * (d/b/a ON Semiconductor). All Rights Reserved.
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor. The
 * terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * interrupts.c
 * - Interrupts source file for the RPM sample application.
 * ----------------------------------------------------------------------------
 * $Revision: 1.3 $
 * $Date: 2012/04/18 15:14:19 $
 * ------------------------------------------------------------------------- */

#include <q32m210.h>
#include "globals.h"
#include "usb.h"

/* ----------------------------------------------------------------------------
 * Global Variables
 * ------------------------------------------------------------------------- */
static unsigned int Timer0_Running = 0;

/* The number of counter timeouts that have occurred */
static unsigned int Timer0_Timeouts = 0;

/* ----------------------------------------------------------------------------
 * Function      : void GPIO_GP1_IntHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : Start timer0 if it isn't already started, in order to
 *                 perform some basic button debouncing.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void GPIO_GP1_IRQHandler(void)
{    
    if (Timer0_Running == 0)
    {
        Timer0_Running = 1;
        
        /* Start timer0 */
        Sys_Timers_Start(SELECT_TIMER0);

    }
}

/* ----------------------------------------------------------------------------
 * Function      : void Timer0_IntHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : Set the GPIO event flag
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void TIMER0_IRQHandler(void)
{
    Timer0_Timeouts = Timer0_Timeouts + 1;
    
    /* Make sure that the button is still held down. */
    if ((Sys_GPIO_Get_IF5() & 0x1) == 0)
    {
        if (Timer0_Timeouts == TIMER0_NUM_TIMEOUTS)
        {
            /* Indicate to the main loop that SW1 has been pressed. */
            g_SW1_Pressed = 1;
            
            /* Clear the timer0 flag */
            Timer0_Running = 0;
            
            /* Clear the timeouts flags */
            Timer0_Timeouts = 0;
        }
        else
        {
            /* Start the timer again */
            Sys_Timers_Start(SELECT_TIMER0);
        }
    }
    else
    {   
        /* Clear the timer0 flag */
        Timer0_Running = 0;
        
        /* Clear the timeouts flags */
        Timer0_Timeouts = 0;
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void RTC_CLKIntHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : Interrupt handler for RTC clock.
 *                 Store current SYSTICK counter and clear interrupt flag
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void RTC_CLOCK_IRQHandler(void)
{
    g_Num_RTC_Ticks++;
    AFE->INTERRUPT_STATUS = RTC_CLOCK_CLEAR;
}
