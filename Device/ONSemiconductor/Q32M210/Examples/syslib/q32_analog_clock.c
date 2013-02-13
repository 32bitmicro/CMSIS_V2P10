/* ----------------------------------------------------------------------------
 * Copyright (c) 2008-2012 Semiconductor Components Industries, LLC (d/b/a ON
 * Semiconductor), All Rights Reserved
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor.
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * q32_analog_clock.c
 * - Analog hardware support code source file for clock support
 * ----------------------------------------------------------------------------
 * $Revision: 1.7 $
 * $Date: 2012/05/10 16:08:32 $
 * ------------------------------------------------------------------------- */

#include <q32.h>
#include <q32m210_info_map.h>

/* External variable defined in system_Q32M210.c. Used to track the current
 * RC oscillator frequency set using Sys_Analog_Set_RCFreq below. 
 * Declared here rather than in system_Q32M210.h because it's not meant 
 * to be used by application code. */
extern uint32_t __Q32M210_Current_RCFreq;

/* ----------------------------------------------------------------------------
 * Function      : FlashStatus Sys_Analog_Set_RCFreq(uint32_t freq)
 * ----------------------------------------------------------------------------
 * Description   : Set the CCR trim value for the RC oscillator to achieve the
 *                 specified frequency.
 * Inputs        : freq         - RC oscillator frequency (resolution of
 *                                100 kHz)
 * Outputs       : return value - Status code indicating if the requested
 *                                frequency was set. FLASH_ERR_GENERAL_FAILURE
 *                                is returned if the requested value was not
 *                                found in the flash information page memory
 * Assumptions   : The data in the information page at the specified location
 *                 is either unwritten flash memory or valid data
 * ------------------------------------------------------------------------- */
FlashStatus Sys_Analog_Set_RCFreq(uint32_t freq)
{
    uint32_t rc_mask;
    uint32_t *setting;
    uint32_t actual_freq;
        
    freq = (freq / 100000);
    actual_freq = freq * 100000;
    freq <<= Q32M210_CAL_RC_CCR_CTRL_INFO_Pos;
    
    rc_mask = (AFE_RC_CCR_CTRL_FINE_CTRL_Mask |
               AFE_RC_CCR_CTRL_RANGE_SEL_Mask |
               AFE_RC_CCR_CTRL_COARSE_CTRL_Mask);

    /* There are a maximum of 32 possible RC oscillator calibration settings
     * stored in the calibration table. Loop through them as long as we have
     * not found the requested frequency. */
    for (setting = (uint32_t *)Q32M210_CAL_RC_CCR_CTRL_BASE;
         setting < ((uint32_t *)Q32M210_CAL_RC_CCR_CTRL_BASE + 32);
         setting++)
    {
        if ((*setting != 0xFFFFFFFF) && ((*setting & ~rc_mask) == freq))
        {
             AFE->RC_CCR_CTRL = (*setting & rc_mask);
             
             /* Update the global variable that tracks the current
              * RC oscillator frequency.  This is used by the CMSIS
              * function SystemCoreClockUpdate to update the current
              * clock rate rate (SystemCoreClock). */
             __Q32M210_Current_RCFreq = actual_freq; 

             return FLASH_ERR_NONE;
        }
    }

    /* Requested frequency was not found when reading through the flash info
     * page calibration table */
    return FLASH_ERR_GENERAL_FAILURE;
}
