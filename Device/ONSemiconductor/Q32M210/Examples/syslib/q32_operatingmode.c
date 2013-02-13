/* ----------------------------------------------------------------------------
 * Copyright (c) 2009 - 2012 Semiconductor Components Industries, LLC (d/b/a ON
 * Semiconductor), All Rights Reserved
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor.
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * q32_operatingmode.c
 * - Low-power operating mode system support functions
 * ----------------------------------------------------------------------------
 * $Revision: 1.6 $
 * $Date: 2012/05/10 15:52:39 $
 * ------------------------------------------------------------------------- */

#include <q32.h>

/* ----------------------------------------------------------------------------
 * Function      : void Sys_OperatingMode_Sleep()
 * ----------------------------------------------------------------------------
 * Description   : Switch the system to sleep mode. This minimum power
 *                 consumption mode waits for a wakeup event to re-boot the
 *                 system using the Boot ROM.
 * Inputs        : None
 * Outputs       : N/A  - This function does not return
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Sys_OperatingMode_Sleep()
{
    /* Disable all clock divisors in the system */
    CLK->CTRL3 = (MCLK_CLK_DISABLE);
    CLK->CTRL4 = EXT_CLK_DISABLE;
    CLK->CTRL5 = (LCD_CLK_DISABLE
                 | USR_CLK0_DISABLE
                 | USR_CLK1_DISABLE
                 | USR_CLK2_DISABLE);
    CLK->CTRL6 = (PWM0_CLK_DISABLE
                 | PWM1_CLK_DISABLE
                 | PWM2_CLK_DISABLE
                 | PWM3_CLK_DISABLE);

    /* Power down the charge pump and LCD driver */
    AFE->PSU_CTRL = (VDBL_DISABLE
                    | LCDDRIVER_DISABLE
                    | VCP_DISABLE
                    | LCDBACKLIGHT_DISABLE);

    /* Set the XTAL BIAS to the minimum setting to achieve the specified sleep
     * mode current consumption */
    REG32_POINTER(0x40000014) = 0x0;

    /* Power down everything not needed to wakeup the system from sleep mode */
    AFE->OPMODE_CTRL = (SLEEP_MODE_ENABLE
                       | STANDBY_MODE_DISABLE
                       | VADC_DISABLE
                       | RC_OSC_ENABLE);

    /* The following loop should not be needed, as the preceeding code should
     * switch the system into sleep mode, with execution restarting after a
     * wakeup event from the base of ROM. However, to prevent extraneous clock
     * cycles from returning from this function/causing code to execute when not
     * expected, go into a spin-loop and wait for a watchdog reset or wakeup
     * event. */
    while(1);
}

/* ----------------------------------------------------------------------------
 * Function      : void Sys_OperatingMode_Standby()
 * ----------------------------------------------------------------------------
 * Description   : Switch the system to standby mode and wait for a wakeup
 *                 event before returning.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Sys_OperatingMode_Standby()
{
    uint32_t OpModeSetting;

    /* Save the current low-power mode settings to be restored after being woken
     * up */
    OpModeSetting = AFE->OPMODE_CTRL;

    /* Power down anything not needed in standby mode */
    AFE->OPMODE_CTRL = (OpModeSetting
                       & ((1U << AFE_OPMODE_CTRL_RC_OSC_ENABLE_Pos))
                          | (SLEEP_MODE_DISABLE
                          | STANDBY_MODE_ENABLE
                          | VADC_DISABLE));

    /* Put the ARM Cortex-M3 core into a wait mode waiting for an event to
     * occur */
    __WFE();

    /* Restore the previous low-power mode setting before returning */
    AFE->OPMODE_CTRL = OpModeSetting;
}
