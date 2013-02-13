/* ----------------------------------------------------------------------------
 * Copyright (c) 2012 Semiconductor Components Industries, LLC
 * (d/b/a ON Semiconductor). All Rights Reserved.
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor. The
 * terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * adc.c
 * - RTC functions for the RPM sample application.
 * ----------------------------------------------------------------------------
 * $Revision: 1.2 $
 * $Date: 2012/04/18 15:14:19 $
 * ------------------------------------------------------------------------- */
#include <q32m210.h>
#include <q32m210_hw.h>
#include "rtc.h"
#include "globals.h"

/* ----------------------------------------------------------------------------
 * Function      : void RTC_SetControl(unsigned int config, int wait)
 * ----------------------------------------------------------------------------
 * Description   : Select the real-time clock counter and alarm mode
 *                 configuration.  This differs from the syslib function in
 *                 that it precisely counts 4 RTC ticks for the changes to 
 *                 take effect, rather than 2 seconds.
 *                 Requires the RTC interrupt to increment the g_num_rtc_ticks
 *                 global variable.
 * Inputs        : - config     - RTC operation and alarm modes; use
 *                                RTC_DISABLE/RTC_ENABLE,
 *                                ALARM_DISABLE/ALARM_ENABLE and
 *                                RTC_RUN_MODE/RTC_SET_MODE
 *                 - wait       - 1 = wait for completion, 0 = don't wait
 * Outputs       : None
 * Assumptions   : This function can block until the RTC configuration load is
 *                 completed
 * ------------------------------------------------------------------------- */
void RTC_SetControl(unsigned int config, int wait)
{
    /* Load the RTC configuration settings that need to be synchronized (if any
     * changes need to be made) */
    if ((config | RTC_ENABLE) != (AFE->RTC_CTRL | RTC_ENABLE))
    {
        AFE->RTC_CTRL = ((config | RTC_LOAD | RTC_ENABLE)
                        & ((1 << AFE_RTC_CTRL_RTC_LOAD_Pos)
                           | (1 << AFE_RTC_CTRL_RTC_BIAS_ENABLE_Pos)
                           | (1 << AFE_RTC_CTRL_ALARM_ENABLE_Pos)
                           | (1 << AFE_RTC_CTRL_RTC_MODE_CFG_Pos)));
    
            
        if (wait)
        {
            /* Enable the RTC interrupts */
            Sys_NVIC_Int_EnableInt(RTC_CLOCK_IRQn);
        
            /* Wait for 4 ticks for the configuration change to take effect */
            g_Num_RTC_Ticks = 0;
        
            while (g_Num_RTC_Ticks < 4)
            {
                Sys_Watchdog_Refresh( );
            }
                
            /* Disable the RTC interrupts */
            Sys_NVIC_Int_DisableInt(RTC_CLOCK_IRQn);
            
        }
        
    }

    /* Load the actual configuration for the RTC bias enable */
    AFE->RTC_CTRL = (config & ((1 << AFE_RTC_CTRL_RTC_BIAS_ENABLE_Pos)
                              | (1 << AFE_RTC_CTRL_ALARM_ENABLE_Pos)
                              | (1 << AFE_RTC_CTRL_RTC_MODE_CFG_Pos)));
}

/* ----------------------------------------------------------------------------
 * Function      : void RTC_SetConfig(unsigned int rate,
 *                                   unsigned int config,
 *                                   unsigned int count,
 *                                   unsigned int alarm,
 *                                   int wait)
 * ----------------------------------------------------------------------------
 * Description   : Configure all components of the RTC.  This differs from
 *                 the syslib function in that it precisely counts 4 RTC
 *                 ticks for the changes to take effect, rather than 2 seconds.
 *                 Requires the RTC interrupt to increment the g_num_rtc_ticks
 *                 global variable.
 * Inputs        : - rate       - One of the RTC_CLK_SELECT_* values
 *                 - config     - RTC operation and alarm modes; use
 *                                RTC_DISABLE/RTC_ENABLE,
 *                                ALARM_DISABLE/ALARM_ENABLE and
 *                                RTC_RUN_MODE/RTC_SET_MODE
 *                 - count      - Real-time clock setting
 *                 - alarm      - RTC alarm count and mode; use
 *                                ALARM_ABSOLUTE_MODE/ALARM_RELATIVE_MODE and
 *                                the desired count setting
 *                 - wait       - 1 = wait for completion, 0 = don't wait
 * Outputs       : None
 * Assumptions   : This function can block until all RTCs are completed
 * ------------------------------------------------------------------------- */
void RTC_SetConfig(unsigned int rate, unsigned int config, unsigned int count, 
                  unsigned int alarm, int wait)
{
    /* Select the RTC rate, keeping the root clock selection the same */
    CLK->CTRL0 = ( CLK->CTRL0 & CLK_CTRL0_RCLK_SELECT_Mask ) | rate;
    
    /* Set the count and alarm */
    AFE->RTC_COUNT = count;
    AFE->RTC_ALARM = alarm;

    /* Load the AFE RTC configuration */
    RTC_SetControl(config, wait);
}
