/* ----------------------------------------------------------------------------
 * Copyright (c) 2011 - 2012 Semiconductor Components Industries, LLC (d/b/a
 * ON Semiconductor), All Rights Reserved
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor.
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * system_Q32M210.c
 * - CMSIS Cortex-M3 Device Peripheral Access Layer Source File for Q32M210
 *
 *   Provides the required system_<Device>.c implementation for CMSIS
 *   compatibility
 * ----------------------------------------------------------------------------
 * $Revision: 1.2 $
 * $Date: 2012/05/10 23:38:06 $
 * ------------------------------------------------------------------------- */

#include <stdint.h>
#include <q32m210.h>

/* ----------------------------------------------------------------------------
 * Defines
 * ------------------------------------------------------------------------- */
#define __DEFAULT_FREQ      (3000000UL)  /* Default oscillator frequency     */

/* ----------------------------------------------------------------------------
 * Variables
 * ------------------------------------------------------------------------- */
uint32_t SystemCoreClock = __DEFAULT_FREQ;  /* Default frequency */
uint32_t __Q32M210_Current_RCFreq = __DEFAULT_FREQ;

/* ----------------------------------------------------------------------------
 * Function      : SystemCoreClockUpdate
 * ----------------------------------------------------------------------------
 * Description   : Updates the variable SystemCoreClock and must be called
 *                 whenever the core clock is changed during program
 *                 execution.  This function sets SystemCoreClock to the
 *                 frequency that was last passed to Sys_Analog_Set_RCFreq.
 * Inputs        : None
 * Outputs       : None
 * Register mods : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void SystemCoreClockUpdate (void)            /* Get Core Clock Frequency      */
{
    SystemCoreClock = __Q32M210_Current_RCFreq;
}

/* ----------------------------------------------------------------------------
 * Function      : SystemInit
 * ----------------------------------------------------------------------------
 * Description   : Setup the Cortex-M3 co-processor sub-system. Current
 *                 implementation re-initializes the system core frequency to
 *                 the default value.
 * Inputs        : None
 * Outputs       : None
 * Register mods : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void SystemInit (void)
{
    SystemCoreClock = __Q32M210_Current_RCFreq;
}
