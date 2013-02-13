/* ----------------------------------------------------------------------------
 * Copyright (c) 2009 - 2012 Semiconductor Components Industries, LLC
 * (d/b/a ON Semiconductor). All Rights Reserved.
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor. The
 * terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 *-----------------------------------------------------------------------------
 * q32m200_sys_asm.s
 * - Provide fixed delays of the specified time
 *-----------------------------------------------------------------------------
 * $Revision: 1.4 $
 * $Date: 2012/05/08 15:09:11 $
 * ------------------------------------------------------------------------- */

#define SYS_DELAY_CALL_OVERHEAD         (15)
#define SYS_DELAY_LOOP_OVERHEAD         (4)

/* ----------------------------------------------------------------------------
 * Place this code in the usual .text segment for code
 * ------------------------------------------------------------------------- */
    rseg    .text:CODE(2)
    thumb

/* ----------------------------------------------------------------------------
 * Function      : void Sys_Delay(unsigned int cycles)
 * ----------------------------------------------------------------------------
 * Description   : Delay by approximately the specified number of clock cycles
 * Inputs        : cycles   - Count of the system clock cycles to delay
 * Outputs       : None
 * Assumptions   : - The requested delay is at least 15 cycles (15 us at 1 MHz)
 *                   and at most 0xFFFFFFFF cycles (approx 214.75 s at 20 MHz).
 *                 - A delay between cycles and (cycles + 3) provides a
 *                   sufficient delay resolution.
 *                 - The requested delay does not exceed the watchdog timeout.
 *                 - If the delay resolution is required to be exact, disable
 *                   interrupts.
 * ------------------------------------------------------------------------- */
    export  Sys_Delay
Sys_Delay:
    /* Add in the call overhead */
    MOVS     R1,#SYS_DELAY_CALL_OVERHEAD
    B.N      Sys_Delay_Check

Sys_Delay_Addition:
    ADDS     R1,R1,#SYS_DELAY_LOOP_OVERHEAD

Sys_Delay_Check:
    CMP      R1,R0
    BCC.N    Sys_Delay_Addition

    /* Return */
    BX       LR

/* ----------------------------------------------------------------------------
 * File end
 * ------------------------------------------------------------------------- */
    END