/* ----------------------------------------------------------------------------
 * Copyright (c) 2008 - 2012 Semiconductor Components Industries, LLC (d/b/a ON
 * Semiconductor), All Rights Reserved
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor.
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * q32_analog_rtc.c
 * - Analog hardware support code source file for the real-time clock
 * ----------------------------------------------------------------------------
 * $Revision: 1.8 $
 * $Date: 2012/04/26 20:54:38 $
 * ------------------------------------------------------------------------- */

#include <q32.h>

/* ----------------------------------------------------------------------------
 * Function      : void Sys_Analog_Set_RTCControl(uint32_t freq_in_hz,
 *                                                uint32_t config)
 * ----------------------------------------------------------------------------
 * Description   : Select the real-time clock counter and alarm mode
 *                 configuration
 * Inputs        : - freq_in_hz - System clock frequency in hertz
 *                 - config     - RTC operation and alarm modes; use
 *                                RTC_DISABLE/RTC_ENABLE,
 *                                ALARM_DISABLE/ALARM_ENABLE and
 *                                RTC_RUN_MODE/RTC_SET_MODE
 * Outputs       : None
 * Assumptions   : This function can block until the RTC configuration load is
 *                 completed
 * ------------------------------------------------------------------------- */
void Sys_Analog_Set_RTCControl(uint32_t freq_in_hz, uint32_t config)
{
    uint32_t i;
    
    /* Load the RTC configuration settings that need to be synchronized (if any
     * changes need to be made) */
    if ((config | RTC_ENABLE) != (AFE->RTC_CTRL | RTC_ENABLE))
    {
        AFE->RTC_CTRL = ((config | RTC_LOAD | RTC_ENABLE)
                        & ((1U << AFE_RTC_CTRL_RTC_LOAD_Pos)
                           | (1U << AFE_RTC_CTRL_RTC_BIAS_ENABLE_Pos)
                           | (1U << AFE_RTC_CTRL_ALARM_ENABLE_Pos)
                           | (1U << AFE_RTC_CTRL_RTC_MODE_CFG_Pos)));

        /* Pause for two seconds to allow the RTC loading to complete.
         * The watchdog needs to be refreshed before the minimum watchdog
         * timeout occurs, which is 2E+11 = 2048 cycles. To ensure that
         * the watchdog is refreshed on time, it will be refreshed after
         * (2048/2) - 15 = 1009 cycles. The 15 fewer cycles comes from the
         * overhead due to processor instructions when running with a system
         * clock of 1 MHz */
        for (i = 0; i < ((freq_in_hz * 2) / 1024); i++)
        {
            Sys_Delay_ProgramROM(1009);
            Sys_Watchdog_Refresh();
        }
    }

    /* Load the actual configuration for the RTC bias enable */
    AFE->RTC_CTRL = (config & ((1U << AFE_RTC_CTRL_RTC_BIAS_ENABLE_Pos)
                              | (1U << AFE_RTC_CTRL_ALARM_ENABLE_Pos)
                              | (1U << AFE_RTC_CTRL_RTC_MODE_CFG_Pos)));
}


/* ----------------------------------------------------------------------------
 * Function      : void Sys_Analog_Set_RTCCount(uint32_t freq_in_hz,
 *                                              uint32_t config)
 * ----------------------------------------------------------------------------
 * Description   : Set the current time for the real-time clock
 * Inputs        : - freq_in_hz - System clock frequency in hertz
 *                 - config     - Real-time clock setting
 * Outputs       : None
 * Assumptions   : This function can block until the RTC count load is completed
 * ------------------------------------------------------------------------- */
void Sys_Analog_Set_RTCCount(uint32_t freq_in_hz, uint32_t config)
{
    uint32_t i;
    uint32_t tempSetting;

    /* Temporarily enable RTC_ENABLE if it wasn't previously enabled */
    tempSetting = (AFE->RTC_CTRL & ~RTC_LOAD);
    AFE->RTC_CTRL |= RTC_ENABLE;

    /* Switch the RTC clock configuration to set mode and set the clock */
    AFE->RTC_COUNT = (config & AFE_RTC_COUNT_RTC_COUNT_Mask);
    AFE->RTC_CTRL = (tempSetting | RTC_ENABLE | RTC_SET_MODE | RTC_LOAD);

    /* Pause for two seconds waiting for the RTC count to complete loading.
     * The watchdog needs to be refreshed before the minimum watchdog
     * timeout occurs, which is 2E+11 = 2048 cycles. To ensure that
     * the watchdog is refreshed on time, it will be refreshed after
     * (2048/2) - 15 = 1009 cycles. The 15 fewer cycles comes from the
     * overhead due to processor instructions when running with a system
     * clock of 1 MHz */
    for (i = 0; i < ((freq_in_hz * 2) / 1024); i++)
    {
        Sys_Delay_ProgramROM(1009);
        Sys_Watchdog_Refresh();
    }

    /* Restore the RTC count mode and again wait for this load to complete */
    AFE->RTC_CTRL = (tempSetting | RTC_ENABLE | RTC_LOAD);

    /* Pause for two seconds to allow the RTC loading to complete. */
    for (i = 0; i < ((freq_in_hz * 2) / 1024); i++)
    {
        Sys_Delay_ProgramROM(1009);
        Sys_Watchdog_Refresh();
    }

    /* Restore the original AFE_RTC_CTRL setting */
    AFE->RTC_CTRL = tempSetting;
}


/* ----------------------------------------------------------------------------
 * Function      : void Sys_Analog_Set_RTCAlarm(uint32_t freq_in_hz,
 *                                              uint32_t config)
 * ----------------------------------------------------------------------------
 * Description   : Set the count and mode for the real-time clock alarm
 * Inputs        : - freq_in_hz - System clock frequency in hertz
 *                 - config     - RTC alarm count and mode; use
 *                                ALARM_ABSOLUTE_MODE/ALARM_RELATIVE_MODE and
 *                                the desired count setting
 * Outputs       : None
 * Assumptions   : This function can block until the RTC alarm load is completed
 * ------------------------------------------------------------------------- */
void Sys_Analog_Set_RTCAlarm(uint32_t freq_in_hz, uint32_t config)
{
    uint32_t i;
    uint32_t tempSetting;

    /* Temporarily enable RTC_ENABLE if it wasn't previously enabled */
    tempSetting = (AFE->RTC_CTRL & ~RTC_LOAD);
    AFE->RTC_CTRL |= RTC_ENABLE;

    /* Set the alarm */
    AFE->RTC_ALARM = (config
                     & ((uint32_t)(1U << AFE_RTC_ALARM_RTC_ALARM_MODE_Pos)
                        | AFE_RTC_ALARM_RTC_ALARM_Mask));
    AFE->RTC_CTRL |= RTC_LOAD;

    /* Wait for the load of the alarm setting to complete before restoring the
     * original value of AFE->RTC_CTRL.
     * The watchdog needs to be refreshed before the minimum watchdog
     * timeout occurs, which is 2E+11 = 2048 cycles. To ensure that
     * the watchdog is refreshed on time, it will be refreshed after
     * (2048/2) - 15 = 1009 cycles. The 15 fewer cycles comes from the
     * overhead due to processor instructions when running with a system
     * clock of 1 MHz */
    for (i = 0; i < ((freq_in_hz * 2) / 1024); i++)
    {
        Sys_Delay_ProgramROM(1009);
        Sys_Watchdog_Refresh();
    }
    AFE->RTC_CTRL = tempSetting;
}


/* ----------------------------------------------------------------------------
 * Function      : void Sys_Analog_RTC_Config(uint32_t freq_in_hz,
 *                                            uint32_t config,
 *                                            uint32_t count,
 *                                            uint32_t alarm)
 * ----------------------------------------------------------------------------
 * Description   : Configure all components of the RTC
 * Inputs        : - freq_in_hz - System clock frequency in hertz
 *                 - config     - RTC operation and alarm modes; use
 *                                RTC_DISABLE/RTC_ENABLE,
 *                                ALARM_DISABLE/ALARM_ENABLE and
 *                                RTC_RUN_MODE/RTC_SET_MODE
 *                 - count      - Real-time clock setting
 *                 - alarm      - RTC alarm count and mode; use
 *                                ALARM_ABSOLUTE_MODE/ALARM_RELATIVE_MODE and
 *                                the desired count setting
 * Outputs       : None
 * Assumptions   : This function can block until all RTCs are completed
 * ------------------------------------------------------------------------- */
void Sys_Analog_RTC_Config(uint32_t freq_in_hz, uint32_t config,
                           uint32_t count, uint32_t alarm)
{
    uint32_t i;
    
    /* Set the count and alarm */
    AFE->RTC_COUNT = count;
    AFE->RTC_ALARM = (alarm
                     & ((uint32_t)(1U << AFE_RTC_ALARM_RTC_ALARM_MODE_Pos)
                        | AFE_RTC_ALARM_RTC_ALARM_Mask));

    /* Temporarily set the RTC control configuration if the RTC configuration
     * doesn't indicate that the final configuration should leave this in set
     * mode */
    if ((config & (1U << AFE_RTC_CTRL_RTC_MODE_CFG_Pos)) != RTC_SET_MODE)
    {
        AFE->RTC_CTRL = (RTC_LOAD | RTC_ENABLE | ALARM_DISABLE | RTC_SET_MODE);
        /* Pause for two seconds to allow the RTC loading to complete.
         * The watchdog needs to be refreshed before the minimum watchdog
         * timeout occurs, which is 2E+11 = 2048 cycles. To ensure that
         * the watchdog is refreshed on time, it will be refreshed after
         * (2048/2) - 15 = 1009 cycles. The 15 fewer cycles comes from the
         * overhead due to processor instructions when running with a system
         * clock of 1 MHz */
        for (i = 0; i < ((freq_in_hz * 2) / 1024); i++)
        {
            Sys_Delay_ProgramROM(1009);
            Sys_Watchdog_Refresh();
        }
    }

    /* Wait for the load of the configuration (and possibly count and alarm
     * settings) to complete */
    AFE->RTC_CTRL = ((config | RTC_LOAD | RTC_ENABLE)
                    & ((1U << AFE_RTC_CTRL_RTC_LOAD_Pos)
                       | (1U << AFE_RTC_CTRL_RTC_BIAS_ENABLE_Pos)
                       | (1U << AFE_RTC_CTRL_ALARM_ENABLE_Pos)
                       | (1U << AFE_RTC_CTRL_RTC_MODE_CFG_Pos)));
    
    for (i = 0; i < ((freq_in_hz * 2) / 1024); i++)
    {
        Sys_Delay_ProgramROM(1009);
        Sys_Watchdog_Refresh();
    }

    /* Potentially disable the RTC bias now that the configuration load has
     * completed */
    AFE->RTC_CTRL = (config & ((1U << AFE_RTC_CTRL_RTC_BIAS_ENABLE_Pos)
                              | (1U << AFE_RTC_CTRL_ALARM_ENABLE_Pos)
                              | (1U << AFE_RTC_CTRL_RTC_MODE_CFG_Pos)));
}

