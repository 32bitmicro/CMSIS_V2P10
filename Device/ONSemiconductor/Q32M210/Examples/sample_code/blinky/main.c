/* ----------------------------------------------------------------------------
 * Copyright (c) 2012 Semiconductor Components Industries, LLC
 * (d/b/a ON Semiconductor). All Rights Reserved.
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor. The
 * terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * main.c
 *  - Main source file
 * ----------------------------------------------------------------------------
 * $Revision: 1.19 $
 * $Date: 2012/04/26 18:50:01 $
 * ------------------------------------------------------------------------- */

#include <q32.h>
#include "globals.h"

/* ----------------------------------------------------------------------------
 * Global Variables
 * ------------------------------------------------------------------------- */

/* Timer0 interrupt count, used to count longer time intervals. This is because
 * the timeout value is only 12 bits which is not enough to represent a
 * half second interval */
int timer0_int_count = 0;

/* LED position variable used to shift LEDs */
int led_pos;

/* Flag used to indicate when Timer0 has expired */
int timer0_flag = 0;

/* ----------------------------------------------------------------------------
 * Function      : void Clear_Pending_Interrupts(void)
 * ----------------------------------------------------------------------------
 * Description   : Clear pending interrupts.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Clear_Pending_Interrupts()
{
    Sys_NVIC_Int_ClearPending(WATCHDOG_IRQn);
    Sys_NVIC_Int_ClearPending(TIMER0_IRQn);
}

/* ----------------------------------------------------------------------------
 * Function      : void Enable_Individual_Interrupts(void)
 * ----------------------------------------------------------------------------
 * Description   : Enable required interrupts.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Enable_Individual_Interrupts()
{
    Sys_NVIC_Int_EnableInt(WATCHDOG_IRQn);
    Sys_NVIC_Int_EnableInt(TIMER0_IRQn);
}

/* ----------------------------------------------------------------------------
 * Function      : void delay(void)
 * ----------------------------------------------------------------------------
 * Description   : Waits until the SYSTICK timer count flag indicates that
 *                 the timer has expired.  Once the SYSTICK timer expires,
 *                 the counter is automatically reloaded, and the timer starts
 *                 counting again.
 *
 *                 The reload count is configured so that the SYSTICK timer
 *                 has a period of approximately 1 s.  This function does
 *                 not wait for the full 1 s itself.  It simply waits until
 *                 the timer expires.
 *
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void delay()
{
    while(!Sys_SYSTICK_Get_CountFlag())
    {
        /* Refresh watchdog */
        Sys_Watchdog_Refresh();
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void main(void)
 * ----------------------------------------------------------------------------
 * Description   : Generates a pattern of blinking lights on the LEDs.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int main()
{
    /* Call the system initialization function */
    initialize();

    /* Initial position for the LEDs */
    led_pos = 0;

    /* Enable interrupts now that initialization is complete */
    Sys_NVIC_MasterEnable();

    /* Start the SYSTICK timer */
    Sys_SYSTICK_Enable();

    /* Start timer0 */
    Sys_Timers_Start(SELECT_TIMER0);

    /* Blink LEDs in checkerboard pattern */
    delay();
    Sys_GPIO_Set_IF4(0x550);
    delay();
    Sys_GPIO_Set_IF4(0xAA0);
    delay();

    /* Execute an infinite loop to blink adjacent pairs of LEDs */
    while(1)
    {
        /* Refresh watchdog */
        Sys_Watchdog_Refresh();

        /* Turn adjacent pairs of LEDs on */
        Sys_GPIO_Set_IF4((LEDS_TWO_ON << led_pos));

        /* Hold this pattern for one second */
        delay();

        /* Advance the starting led position */
        led_pos = led_pos + 2;

        /* Check if Timer0 has expired */
        if (timer0_flag == 1)
        {
            /* Blink LEDs in checkerboard pattern */
            Sys_GPIO_Set_IF4(0x550);
            delay();
            Sys_GPIO_Set_IF4(0xAA0);
            delay();

            /* Reset the flag */
            timer0_flag = 0;

            /* Reset the pattern on the LEDs */
            led_pos = 0;
        }
    }
}
