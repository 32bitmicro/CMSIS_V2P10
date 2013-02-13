/* ----------------------------------------------------------------------------
 * Copyright (c) 2008 - 2012 Semiconductor Components Industries, LLC (d/b/a ON
 * Semiconductor), All Rights Reserved
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor.
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * q32m210_romvect.c
 * - Wrapper functions for function vectors embedded in the Program ROM
 * ----------------------------------------------------------------------------
 * $Revision: 1.4 $
 * $Date: 2012/05/10 17:19:55 $
 * ------------------------------------------------------------------------- */

#include <q32.h>

/* ----------------------------------------------------------------------------
 * Function      : BootROMStatus
 *                 Sys_BootROM_StrictStartApp(uint32_t* vect_table)
 * ----------------------------------------------------------------------------
 * Description   : Validate and start up an application using the Boot ROM.
 *                 Only start the application if application validation returns
 *                 BOOTROM_ERR_NONE.
 * Inputs        : vect_table   - Pointer to the vector table at the start of an
 *                                application that will be validated and then
 *                                run.
 * Outputs       : return value - Status code indicating application validation
 *                                error if application cannot be started. If not
 *                                returning, the status code is written to the
 *                                top of the started application's stack to
 *                                capture non-fatal validation issues.
 * Assumptions   : - A bus fault handler is installed that can be used to handle
 *                   any detected uncorrectable ECC errors that are found during
 *                   validation of the application.
 *                 - If a transient, non-fatal error is detected after the
 *                   application has been completely validated, this error can
 *                   be reported through the new application's stack
 * ------------------------------------------------------------------------- */
BootROMStatus Sys_BootROM_StrictStartApp(uint32_t* vect_table)
{
    BootROMStatus result;
    result = (*((BootROMStatus (**)(uint32_t*))
                ROMVECT_BOOTROM_VALIDATE_APP))(vect_table);

    /* Check to ensure that the BootROM error code indicated no errors */
    if (result != BOOTROM_ERR_NONE)
    {
        return result;
    }
    else
    {
        /* No errors were observed, use the regular start app function to boot
         * the validated application. */
        result = (*((BootROMStatus (**)(uint32_t*))
                    ROMVECT_BOOTROM_START_APP))(vect_table);

        /* If returning here, an error occurred that was unexpected. Return the
         * error code, but if no error was detected return
         * BOOTROM_ERR_FAILED_START_APP. */
        if (result == BOOTROM_ERR_NONE)
        {
            return BOOTROM_ERR_FAILED_START_APP;
        }
        else
        {
            return result;
        }
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void Sys_Delay_ProgramROM(uint32_t cycles)
 * ----------------------------------------------------------------------------
 * Description   : Delay by at least the specified number of clock cycles
 * Inputs        : cycles   - Count of the system clock cycles to delay
 * Outputs       : None
 * Assumptions   : - The requested delay is at least 32 cycles (32 us at 1 MHz)
 *                   and at most 0xFFFFFFFF cycles (approx 214.75 s at 20 MHz).
 *                 - A delay between cycles and (cycles + 3) provides a
 *                   sufficient delay resolution.
 *                 - The requested delay does not exceed the watchdog timeout.
 *                 - If the delay resolution is required to be exact, disable
 *                   interrupts.
 * ------------------------------------------------------------------------- */
void Sys_Delay_ProgramROM(uint32_t cycles)
{
    /* The overhead for this version of the function is greater then that of a
     * direct call to Sys_Delay. Decrement the cycle count before calling the
     * underlying function to account for this added delay. */
    if (cycles >  17)
    {
        (*((uint32_t (**)(uint32_t))
           ROMVECT_PROGRAMROM_SYS_DELAY))(cycles - 17);
    }

}
