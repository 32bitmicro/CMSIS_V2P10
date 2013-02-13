/* -------------------------------------------------------------------------
 * Copyright (c) 2008 - 2012 Semiconductor Components Industries, LLC (d/b/a ON
 * Semiconductor), All Rights Reserved
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor.
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * -------------------------------------------------------------------------
 * q32_timers.c
 * - General-purpose system timer hardware support code source file
 * -------------------------------------------------------------------------
 * $Revision: 1.3 $
 * $Date: 2012/04/25 21:56:14 $
 * ------------------------------------------------------------------------- */

#include <q32.h>

/* ----------------------------------------------------------------------------
 * Function      : void Sys_Timers_Start( uint32_t config )
 * ----------------------------------------------------------------------------
 * Description   : Start the specified general-purpose system timers
 * Inputs        : config - Timers to start; use the SELECT_TIMER* settings or
 *                          SELECT_ALL_TIMERS/SELECT_NO_TIMERS to indicate which
 *                          timers to start
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Sys_Timers_Start( uint32_t config )
{
    /* Start the timers specified, leaving any unspecified timers in their
     * previous state. To avoid race conditions with a read-modify-write of the
     * TIMER_CTRL_STATUS register, write each of the bits independantly */

    if ((config & SELECT_TIMER0) != 0)
    {
        TIMER_CTRL_STATUS->TIMER0_STATUS_ALIAS = TIMER0_START_BITBAND;
    }

    if ((config & SELECT_TIMER1) != 0)
    {
        TIMER_CTRL_STATUS->TIMER1_STATUS_ALIAS = TIMER1_START_BITBAND;
    }

    if ((config & SELECT_TIMER2) != 0)
    {
        TIMER_CTRL_STATUS->TIMER2_STATUS_ALIAS = TIMER2_START_BITBAND;
    }

    if ((config & SELECT_TIMER3) != 0)
    {
        TIMER_CTRL_STATUS->TIMER3_STATUS_ALIAS = TIMER3_START_BITBAND;
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void Sys_Timers_Stop( uint32_t config )
 * ----------------------------------------------------------------------------
 * Description   : Stop the specified general-purpose system timers
 * Inputs        : config - Timers to stop; use the SELECT_TIMER* settings or
 *                          SELECT_ALL_TIMERS/SELECT_NO_TIMERS to indicate which
 *                          timers to stop
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Sys_Timers_Stop( uint32_t config )
{
    /* Stop only the timers specified, leaving any unspecified timers in their
     * previous state. To avoid race conditions with a read-modify-write of the
     * TIMER_CTRL_STATUS register, write each of the bits independantly */
    if ((config & SELECT_TIMER0) != 0)
    {
        TIMER_CTRL_STATUS->TIMER0_STATUS_ALIAS = TIMER0_STOP_BITBAND;
    }

    if ((config & SELECT_TIMER1) != 0)
    {
        TIMER_CTRL_STATUS->TIMER1_STATUS_ALIAS = TIMER1_STOP_BITBAND;
    }

    if ((config & SELECT_TIMER2) != 0)
    {
        TIMER_CTRL_STATUS->TIMER2_STATUS_ALIAS = TIMER2_STOP_BITBAND;
    }

    if ((config & SELECT_TIMER3) != 0)
    {
        TIMER_CTRL_STATUS->TIMER3_STATUS_ALIAS = TIMER3_STOP_BITBAND;
    }
}
