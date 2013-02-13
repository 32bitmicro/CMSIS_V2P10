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
 * $Revision: 1.10 $
 * $Date: 2012/04/26 18:50:02 $
 * ------------------------------------------------------------------------- */

#include <q32.h>
#include "globals.h"

/* ----------------------------------------------------------------------------
 * Function      : void Wakeup_IntHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : Clear the wakeup status and the interrupt status.
 *                 Set the wakeup flag to indicate that the system has awoken.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void WAKEUP_IRQHandler(void)
{
    /* Clear the interrupt status */
    Sys_Analog_Clear_InterruptStatus(AFE_CLR_INT_STATUS_RESET);
}
