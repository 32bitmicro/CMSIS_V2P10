/* ----------------------------------------------------------------------------
 * Copyright (c) 2009 - 2012 Semiconductor Components Industries, LLC (d/b/a ON
 * Semiconductor), All Rights Reserved
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor.
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * q32_uart.c
 * - Source code for system support functions used to configure behaviour of
 * the UART interfaces and write/read data using the UART
 * ----------------------------------------------------------------------------
 * $Revision: 1.5 $
 * $Date: 2012/04/25 21:56:14 $
 * ------------------------------------------------------------------------- */

#include <q32.h>

/* ----------------------------------------------------------------------------
 * Function      : void Sys_UART0_Set_Speed(uint32_t baud_rate,
 *                                          uint32_t freq_in_hz)
 * ----------------------------------------------------------------------------
 * Description   : Set the baud rate of UART0. Calculates the required
 *                 UART0_SPEED_CTRL for the specified baud rate with the
 *                 system clock speed provided.
 * Inputs        : - baud_rate  - Baud rate to use for UART0
 *                 - freq_in_hz - Speed of the UART0 divided clock in hertz
 * Outputs       : None
 * Assumptions   : - baud_rate is valid for the specified freq_in_hz
 * ------------------------------------------------------------------------- */
void Sys_UART0_Set_Speed(uint32_t baud_rate, uint32_t freq_in_hz)
{
    /* Enable the prescalar if appropriate */
    if (freq_in_hz / baud_rate > 1024)
    {
        UART0_CTRL->PRESCALE_ENABLE_ALIAS = 1;
    }
    else
    {
        UART0_CTRL->PRESCALE_ENABLE_ALIAS = 0;
    }

    /* Calculate the speed based using a multiplier of 2^18, reduced
     * appropriately to extend the valid range of baud_rate */
    if (baud_rate < 16384)
    {
        UART0->SPEED_CTRL = ((baud_rate << 14)
                        * (1 + 11 * UART0_CTRL->PRESCALE_ENABLE_ALIAS))
                        / (freq_in_hz >> (18 - 14));
    }
    else
    {
        UART0->SPEED_CTRL = ((baud_rate << 8)
                        * (1 + 11 * UART0_CTRL->PRESCALE_ENABLE_ALIAS))
                        / (freq_in_hz >> (18 - 8));
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void Sys_UART1_Set_Speed(uint32_t baud_rate,
 *                                          uint32_t freq_in_hz)
 * ----------------------------------------------------------------------------
 * Description   : Set the baud rate of UART1. Calculates the required
 *                 UART1_SPEED_CTRL for the specified baud rate with the
 *                 system clock speed provided.
 * Inputs        : - baud_rate  - Baud rate to use for UART1
 *                 - freq_in_hz - Speed of the UART1 divided clock in hertz
 * Outputs       : None
 * Assumptions   :  - baud_rate is valid for the specified freq_in_hz
 * ------------------------------------------------------------------------- */
void Sys_UART1_Set_Speed(uint32_t baud_rate, uint32_t freq_in_hz)
{
    /* Enable the prescalar if appropriate */
    if (freq_in_hz / baud_rate > 1024)
    {
        UART1_CTRL->PRESCALE_ENABLE_ALIAS = 1;
    }
    else
    {
        UART1_CTRL->PRESCALE_ENABLE_ALIAS = 0;
    }

    /* Calculate the speed based using a multiplier of 2^18, reduced
     * appropriately to extend the valid range of baud_rate */
    if (baud_rate < 16384)
    {
        UART1->SPEED_CTRL = ((baud_rate << 14)
                        * (1 + 11 * UART1_CTRL->PRESCALE_ENABLE_ALIAS))
                        / (freq_in_hz >> (18 - 14));
    }
    else
    {
        UART1->SPEED_CTRL = ((baud_rate << 8)
                        * (1 + 11 * UART1_CTRL->PRESCALE_ENABLE_ALIAS))
                        / (freq_in_hz >> (18 - 8));
    }
}

/* ----------------------------------------------------------------------------
 * Function      : uint32_t Sys_UART0_Get_Speed(uint32_t freq_in_hz)
 * ----------------------------------------------------------------------------
 * Description   : Return the baud rate of UART0
 * Inputs        : freq_in_hz - Speed of the UART0 divided clock in hertz
 * Outputs       : return value - Baud rate of UART0
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
uint32_t Sys_UART0_Get_Speed(uint32_t freq_in_hz)
{
    uint32_t temp;

    /* Calculate the scaling factor needed to get the baud rate from the speed
     * control setting */
    temp = ((1U << 18) * (1 + 11 * UART0_CTRL->PRESCALE_ENABLE_ALIAS));
    return ((UART0->SPEED_CTRL * freq_in_hz) / temp);
}

/* ----------------------------------------------------------------------------
 * Function      : uint32_t Sys_UART1_Get_Speed(uint32_t freq_in_hz)
 * ----------------------------------------------------------------------------
 * Description   : Return the baud rate of UART1
 * Inputs        : freq_in_hz - Speed of the UART1 divided clock in hertz
 * Outputs       : return value - Baud rate of UART1
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
uint32_t Sys_UART1_Get_Speed(uint32_t freq_in_hz)
{
    uint32_t temp;

    /* Calculate the scaling factor needed to get the baud rate from the speed
     * control setting */
    temp = ((1U << 18) * (1 + 11 * UART1_CTRL->PRESCALE_ENABLE_ALIAS));

    return ((UART1->SPEED_CTRL * freq_in_hz) / temp);
}
