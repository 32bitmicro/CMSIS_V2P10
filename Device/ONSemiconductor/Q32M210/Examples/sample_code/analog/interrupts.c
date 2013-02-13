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
 * $Revision: 1.14 $
 * $Date: 2012/04/26 18:50:00 $
 * ------------------------------------------------------------------------- */

#include <q32.h>
#include "globals.h"

/* ----------------------------------------------------------------------------
 * Function      : void DMA0_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : Read latest ADC reading and update a meter style display.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void DMA0_IRQHandler(void)
{
    int i;

    /* Read the most recent data from ADC0. The result is shifted down 16 bits
     * and then ANDed with the mask. This is because the bottom 16 bits of the
     * ADC0 data is filled with zeroes and only the top 16 bits contain the
     * data collected by the ADC. */

    int temp = (Sys_Analog_Get_ADC0Data() >> 16) & ADC_DATA_MASK;

    /* The number of LEDs to display is calculated by dividing the ADC
     * output range. 0xFFFF (16 bits) divided by 8 LEDs is approximately 0x2000
     * per LED. */
    i = (int)(temp / 0x2000);

    /* Set all bits up to and including bit position i.  Add an offset of 5
     * to account for the fact that the LEDs are connected to IF4.4 through
     * IF4.11. */
    Sys_GPIO_Set_IF4(((0x1 << i + 5) - 0x1));

    /* Clear the interrupt status */
    Sys_DMA_Clear_Channel0_Status();

    /* Refresh the watchdog */
    Sys_Watchdog_Refresh();
}
