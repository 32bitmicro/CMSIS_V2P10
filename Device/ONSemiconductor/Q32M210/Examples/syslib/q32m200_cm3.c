/* ----------------------------------------------------------------------------
 * Copyright (c) 2009 - 2012 Semiconductor Components Industries, LLC (d/b/a ON
 * Semiconductor), All Rights Reserved
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor.
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * q32m200_cm3.c
 * - Support functions for the ARM Cortex-M3 processor
 * ----------------------------------------------------------------------------
 * $Revision: 1.4 $
 * $Date: 2012/04/26 17:58:46 $
 * ------------------------------------------------------------------------- */

#include <q32.h>

#define AIRCR_VECTOR_KEY                0x5FA

/* ----------------------------------------------------------------------------
 * Function      : void Sys_CM3_CoreReset()
 * ----------------------------------------------------------------------------
 * Description   : Perform a core reset of only the ARM Cortex-M3 processor
 * Inputs        : None
 * Outputs       : N/A  - This function should not return
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Sys_CM3_CoreReset()
{
    /* Disable interrupts while waiting for the core reset to occur */
    Sys_NVIC_MasterDisable();
    
    SCB->AIRCR  = ((AIRCR_VECTOR_KEY << SCB_AIRCR_VECTKEY_Pos)      | 
                  SCB_AIRCR_VECTRESET_Msk);
    
    /* Loop while waiting for the reset to occur (will reset due to a watchdog
     * timeout if the core reset doesn't trigger in time). */
    while(1);
}

/* ----------------------------------------------------------------------------
 * Function      : void Sys_CM3_SystemReset()
 * ----------------------------------------------------------------------------
 * Description   : Perform a system reset
 * Inputs        : None
 * Outputs       : N/A  - This function should not return
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Sys_CM3_SystemReset()
{
    /* Disable interrupts while waiting for the system reset to occur */
    Sys_NVIC_MasterDisable();
   
    SCB->AIRCR  = ((AIRCR_VECTOR_KEY << SCB_AIRCR_VECTKEY_Pos)      | 
                  SCB_AIRCR_SYSRESETREQ_Msk); 

    /* Loop while waiting for the reset to occur (will reset due to a watchdog
     * timeout if the system reset doesn't trigger in time). */
    while(1);
}
