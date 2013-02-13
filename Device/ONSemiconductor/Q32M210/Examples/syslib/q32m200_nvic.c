/* -------------------------------------------------------------------------
 * Copyright (c) 2008 - 2012 Semiconductor Components Industries, LLC (d/b/a ON
 * Semiconductor), All Rights Reserved
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor.
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * -------------------------------------------------------------------------
 * q32m200_nvic.c
 * - NVIC hardware support code source file
 * -------------------------------------------------------------------------
 * $Revision: 1.6 $
 * $Date: 2012/04/26 17:58:46 $
 * ------------------------------------------------------------------------- */

#include <q32.h>

#define NUM_EXTERNAL_INTERRUPT_REGS     2

/* ----------------------------------------------------------------------------
 * Function      : void Sys_NVIC_Fault_EnableInt(IRQn_Type intnum)
 * ----------------------------------------------------------------------------
 * Description   : Enable the specified fault exception.
 * Inputs        : intnum - Fault to enable; use MemoryManagement_IRQn, 
 *                          BusFault_IRQn or UsageFault_IRQn
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Sys_NVIC_Fault_EnableInt(IRQn_Type intnum)
{
   switch (intnum)
    {
        case MemoryManagement_IRQn:
            SCB->SHCSR = (SCB->SHCSR | (0x1UL << SCB_SHCSR_MEMFAULTENA_Pos));
            break;

        case BusFault_IRQn:
            SCB->SHCSR = (SCB->SHCSR | (0x1UL << SCB_SHCSR_BUSFAULTENA_Pos));
            break;

        case UsageFault_IRQn:
            SCB->SHCSR = (SCB->SHCSR | (0x1UL << SCB_SHCSR_USGFAULTENA_Pos));
            break;

        default:
            break;
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void Sys_NVIC_Fault_DisableInt(IRQn_Type intnum)
 * ----------------------------------------------------------------------------
 * Description   : Disable the specified fault exception.
 * Inputs        : intnum - Fault to disable; use MemoryManagement_IRQn, 
 *                          BusFault_IRQn or UsageFault_IRQn
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Sys_NVIC_Fault_DisableInt(IRQn_Type intnum)
{
    switch (intnum)
    {
        case MemoryManagement_IRQn:
            SCB->SHCSR = (SCB->SHCSR & ~(SCB_SHCSR_MEMFAULTENA_Msk));
            break;

        case BusFault_IRQn:
            SCB->SHCSR = (SCB->SHCSR & ~(SCB_SHCSR_BUSFAULTENA_Msk));
            break;

        case UsageFault_IRQn:
            SCB->SHCSR = (SCB->SHCSR & ~(SCB_SHCSR_USGFAULTENA_Msk));
            break;

        default:
            break;
    }
}

/* ----------------------------------------------------------------------------
 * Function      : uint32_t Sys_NVIC_Fault_IsActive(IRQn_Type intnum)
 * ----------------------------------------------------------------------------
 * Description   : Return whether the specified fault exception is active.
 * Inputs        : intnum - Fault to be checked for its active state; use 
 *                          MemoryManagement_IRQn, BusFault_IRQn or 
 *                          UsageFault_IRQn
 * Outputs       : return value - Active state of specified fault exception;
 *                                value loaded from SCB->SHCSR
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
uint32_t Sys_NVIC_Fault_IsActive(IRQn_Type intnum)
{
    switch (intnum)
    {
        case MemoryManagement_IRQn:
            return (SCB->SHCSR >> SCB_SHCSR_MEMFAULTACT_Pos) & 0x1;

        case BusFault_IRQn:
            return (SCB->SHCSR >> SCB_SHCSR_BUSFAULTACT_Pos) & 0x1;

        case UsageFault_IRQn:
            return (SCB->SHCSR >> SCB_SHCSR_USGFAULTACT_Pos) & 0x1;

        default:
            return 0;
    }
}

/* ----------------------------------------------------------------------------
 * Function      : uint32_t Sys_NVIC_Fault_IsPending(IRQn_Type intnum)
 * ----------------------------------------------------------------------------
 * Description   : Return whether the specified fault exception is pending.
 * Inputs        : intnum - Fault to be checked for its pending state; use 
 *                          MemoryManagement_IRQn, BusFault_IRQn or 
 *                          UsageFault_IRQn
 * Outputs       : return value - Pending state of specified fault exception;
 *                                value loaded from SCB->SHCSR
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
uint32_t Sys_NVIC_Fault_IsPending(IRQn_Type intnum)
{
    switch (intnum)
    {
        case MemoryManagement_IRQn:
            return (SCB->SHCSR >> SCB_SHCSR_MEMFAULTPENDED_Pos) & 0x1;

        case BusFault_IRQn:
            return (SCB->SHCSR >> SCB_SHCSR_BUSFAULTPENDED_Pos) & 0x1;

        case UsageFault_IRQn:
            return (SCB->SHCSR >> SCB_SHCSR_USGFAULTPENDED_Pos) & 0x1;

        default:
            return 0;
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void Sys_NVIC_Fault_SetPending(IRQn_Type intnum)
 * ----------------------------------------------------------------------------
 * Description   : Set the specified fault exception to pending.
 * Inputs        : intnum - Fault to be checked for its pending state; use 
 *                          MemoryManagement_IRQn, BusFault_IRQn or 
 *                          UsageFault_IRQn
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Sys_NVIC_Fault_SetPending(IRQn_Type intnum)
{
    switch (intnum)
    {
        case MemoryManagement_IRQn:
            SCB->SHCSR = (SCB->SHCSR | (0x1UL << SCB_SHCSR_MEMFAULTPENDED_Pos));
            return;

        case BusFault_IRQn:
            SCB->SHCSR = (SCB->SHCSR | (0x1UL << SCB_SHCSR_BUSFAULTPENDED_Pos));
            return;

        case UsageFault_IRQn:
            SCB->SHCSR = (SCB->SHCSR | (0x1UL << SCB_SHCSR_USGFAULTPENDED_Pos));
            return;

        default:
            return;
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void Sys_NVIC_Fault_ClearPending(IRQn_Type intnum)
 * ----------------------------------------------------------------------------
 * Description   : Clear the pending status of the specified fault exception.
 * Inputs        : intnum - Fault to be checked for its pending state; use 
 *                          MemoryManagement_IRQn, BusFault_IRQn or 
 *                          UsageFault_IRQn
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Sys_NVIC_Fault_ClearPending(IRQn_Type intnum)
{
     switch (intnum)
    {
        case MemoryManagement_IRQn:
            SCB->SHCSR = (SCB->SHCSR & ~(SCB_SHCSR_MEMFAULTPENDED_Msk));
            return;

        case BusFault_IRQn:
            SCB->SHCSR = (SCB->SHCSR & ~(SCB_SHCSR_BUSFAULTPENDED_Msk));
            return;

        case UsageFault_IRQn:
            SCB->SHCSR = (SCB->SHCSR & ~(SCB_SHCSR_USGFAULTPENDED_Msk));
            return;

        default:
            return;
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void Sys_NVIC_Int_DisableAllInt()
 * ----------------------------------------------------------------------------
 * Description   : Disable all of the external interrupts.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Sys_NVIC_Int_DisableAllInt()
{
    int i;
 
    for (i = 0; i < NUM_EXTERNAL_INTERRUPT_REGS; i++)
    {
        NVIC->ICER[i] = 0xFFFFFFFF;
    }
}


/* ----------------------------------------------------------------------------
 * Function      : void Sys_NVIC_Int_ClearAllPending()
 * ----------------------------------------------------------------------------
 * Description   : Clear all of the pending external interrupts.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Sys_NVIC_Int_ClearAllPending()
{
    int i;
     
    for (i = 0; i < NUM_EXTERNAL_INTERRUPT_REGS; i++)
    {
        NVIC->ICPR[i] = 0xFFFFFFFF;
    }
}
