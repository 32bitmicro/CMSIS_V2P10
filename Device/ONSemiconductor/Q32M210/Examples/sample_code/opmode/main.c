/* ----------------------------------------------------------------------------
 * Copyright (c) 2012 Semiconductor Components Industries, LLC
 * (d/b/a ON Semiconductor). All Rights Reserved.
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor. The
 * terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * main.c
 *  - Main source file
 * ----------------------------------------------------------------------------
 * $Revision: 1.17 $
 * $Date: 2012/04/26 20:45:54 $
 * ------------------------------------------------------------------------- */

#include <q32.h>
#include "globals.h"

/* ----------------------------------------------------------------------------
 * Global Variables
 * ------------------------------------------------------------------------- */

/* Flag used to indicate that the wakeup interrupt has been triggered */
int wakeup_flag = 0;

/* ----------------------------------------------------------------------------
 * Function      : void Clear_Pending_Interrupts(void)
 * ----------------------------------------------------------------------------
 * Description   : Clear pending interrupts.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Clear_Pending_Interrupts()
{
    Sys_NVIC_Int_ClearPending(WATCHDOG_IRQn);
    Sys_NVIC_Int_ClearPending(WAKEUP_IRQn);
}

/* ----------------------------------------------------------------------------
 * Function      : void Enable_Individual_Interrupts(void)
 * ----------------------------------------------------------------------------
 * Description   : Enable required interrupts.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Enable_Individual_Interrupts()
{
    Sys_NVIC_Int_EnableInt(WATCHDOG_IRQn);
    Sys_NVIC_Int_EnableInt(WAKEUP_IRQn);
}

/* ----------------------------------------------------------------------------
 * Function      : void delay(void)
 * ----------------------------------------------------------------------------
 * Description   : Waits until the SYSTICK timer count flag indicates that
 *                 the timer has expired.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : SYSTICK timer is enabled
 * ------------------------------------------------------------------------- */
void delay()
{
    while(!Sys_SYSTICK_Get_CountFlag())
    {
        /* Refresh watchdog */
        Sys_Watchdog_Refresh();
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void main(void)
 * ----------------------------------------------------------------------------
 * Description   : Enables the RTC alarm.  Sets the alarm to go off after
 *                 8 seconds.  Generates a pattern of blinking lights on
 *                 the LEDs.  Puts the system to sleep.
 *                 The system will awake when the RTC alarm goes off or when
 *                 the WAKEUP0 button is pressed.
 *                 A different pattern will appear on the LEDs when the
 *                 system wakes up.  The RTC alarm is re-enabled as soon
 *                 as the system wakes up.
 *                 If this code has already been downloaded, you may have some
 *                 difficultly downloading new code because the system is
 *                 periodically asleep.  Turn SW5 OFF then ON while pressing
 *                 the NMI button.  Continue to press the NMI button while
 *                 downloading.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int main()
{
    int led_pos;

    /* Call the system initialization function */
    initialize();

    /* Check the retention register */
    if (Sys_Analog_Get_RetentionReg() == 0)
    {
        /* The value in the retention register is 0, indicating that this
         * is the first time that this code is executed. */

        /* Configure the RTC */
        Sys_Analog_RTC_Config(SYS_FREQ, AFE_RTC_CTRL_RUN_CFG,
                              AFE_RTC_CLOCK_RESET, AFE_RTC_ALARM_CFG);

        /* Set the value in the retention register to 1, so we will know that
         * we have already visited this state. */
        Sys_Analog_Set_RetentionReg(1);
    }
    else
    {
        /* The value in the retention register is 1, indicating that the
         * system has just awoken from sleep. */

        /* Flash alternating LEDs */
        Sys_GPIO_Set_IF4(0xAA0);

        /* Enable the alarm (takes 2 seconds) */
        Sys_Analog_Set_RTCControl(SYS_FREQ, AFE_RTC_CTRL_RUN_CFG);
    }

    /* Enable interrupts now that initialization is complete */
    Sys_NVIC_MasterEnable();

    /* Turn off all LEDs */
    Sys_GPIO_Set_IF4(0x0);

    /* Enable the SYSTICK timer */
    Sys_SYSTICK_Enable();

    /* Create a sequence of toggling LEDs */
    for(led_pos = 0; led_pos <= 6; led_pos += 2)
    {
        /* Turn adjacent pairs of LEDs on */
        Sys_GPIO_Set_IF4(LEDS_TWO_ON << led_pos);

        /* Keep this pattern on for about 1 second */
        delay();
    }

    /* Refresh the watchdog */
    Sys_Watchdog_Refresh();

    /* Turn off all LEDs before going to sleep */
    Sys_GPIO_Set_IF4(0x0);

    /* Enable the IF5 pull-ups and pull-downs before going to sleep */
    Sys_GPIO_IF5_Config(GPIO_IF5_FUNCTION_SLEEPCFG);

    /* Enter sleep mode.
     * This function does not return.  When the RTC alarm goes off, the
     * system will awake and start executing the Boot ROM. WakeupInt_Handler()
     * will be run as soon as interrupts are re-enabled. */
    Sys_OperatingMode_Sleep();

    /* We should never get here.... */
    while (1)
    {
        Sys_Watchdog_Refresh();
    }

}
