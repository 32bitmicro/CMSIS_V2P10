;-----------------------------------------------------------------------------
; Copyright (c) 2009 - 2010 Semiconductor Components Industries, LLC
; (d/b/a ON Semiconductor). All Rights Reserved.
;
; This code is the property of ON Semiconductor and may not be redistributed
; in any form without prior written permission from ON Semiconductor. The
; terms of use and warranty for this code are covered by contractual
; agreements between ON Semiconductor and the licensee.
;-----------------------------------------------------------------------------
; q32m200_cm3_asm.s
; - Assembly code routine supporting the ARM Cortex-M3 Processor
;-----------------------------------------------------------------------------
; $Revision: 1.2 $
; $Date: 2012/04/17 20:32:12 $
; -------------------------------------------------------------------------

;-----------------------------------------------------------------------------
; Place this code in the usual .text segment for code
;-----------------------------------------------------------------------------

    AREA    |.text|, CODE, READONLY
    THUMB

;-----------------------------------------------------------------------------
; Function      : void Sys_CM3_SoftReset()
;-----------------------------------------------------------------------------
; Description   : Perform a soft reset of the currently running application
; Inputs        : None
; Outputs       : return value - BOOTROM_ERR_NONE is written to the top of the
;                                restarted application's stack
; Assumptions   : The core is currently using the main stack pointer (MSP)
;-----------------------------------------------------------------------------
    export  Sys_CM3_SoftReset
Sys_CM3_SoftReset PROC

    ; Disable interrupts before performing a soft-reset to prevent interrupts
    ; from triggering before
    movs    r0, #+1
    msr     primask, r0

    ; A pointer to the application vector table is available in the
    ; NVIC_VTABLE_OFFSET register (0xE000ED08)
	;
    ldr.n   r0, =0xE000ED08
    ldr     r0, [r0]

    ; Load the stack pointer from the application's vector table
    ldr     r1, [r0]
    mov     sp, r1

    ; Push an application startup status of 0 (BOOTROM_ERR_NONE) onto the
    ; application stack
    movs    r1, #+0
    push    {r1}

    ; Load the initial PC from the application's vector table and branch to
    ; the application's entry point.
    ldr     r0, [r0, #4]
    bx      r0
	
	ENDP

	ALIGN

    END