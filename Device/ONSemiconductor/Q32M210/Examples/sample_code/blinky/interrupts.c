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
 * - Interrupts source file
 * ----------------------------------------------------------------------------
 * $Revision: 1.15 $
 * $Date: 2012/04/26 18:50:01 $
 * ------------------------------------------------------------------------- */

#include <q32.h>
#include "globals.h"

extern int timer0_int_count;
extern int led_pos;

/* ----------------------------------------------------------------------------
 * Function      : void TIMER0_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : Interrupt handler for Timer0.  Timer0 is used to control
 *                 when the LEDs are reset.  There are 6 different patterns
 *                 displayed on the LEDs. Each pattern is held for 1 second
 *                 (i.e., one SYSTICK period).  So, Timer0 is configured with
 *                 a 6 second period.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void TIMER0_IRQHandler(void) 
{
    /* Increment interrupt count */
    timer0_int_count++;

    /* Refresh watchdog */
    Sys_Watchdog_Refresh();

    /* Check if the full 6 seconds has passed */
    if (timer0_int_count == TIMER0_INTS_PER_TIMEOUT)
    {
        /* Reset the counter */
        timer0_int_count = 0;

        /* Set the flag */
        timer0_flag = 1;
   }
}
