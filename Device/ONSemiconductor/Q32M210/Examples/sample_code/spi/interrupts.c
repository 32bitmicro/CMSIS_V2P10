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
 * $Revision: 1.11 $
 * $Date: 2012/04/26 18:50:02 $
 * ------------------------------------------------------------------------- */

#include <q32.h>
#include "globals.h"

/* ----------------------------------------------------------------------------
 * Function      : void SPI0_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : SPI0 interrupt handler. Transmits data over SPI0 when the
 *                 internal SPI0_DATA_S register is empty.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void SPI0_IRQHandler(void)
{
    /* If SPI_Write_buffer is not empty, send the next character. */
    if (write_buffer_start != write_buffer_end)
    {
        /* Send data over SPI0 */
        Sys_SPI0_Write(SPI_Write_buffer[write_buffer_start++]);

        /* Initiate the SPIO0 transfer.  This step is necessary since SPIO0
         * is configured in manual mode.  It is not necessary if SPIO0 is
         * configured in auto mode. */
        Sys_SPI0_MasterInit();

        /* Reset the index when the end of the buffer is reached */
        if (write_buffer_start == BUFFER_SIZE - 1)
        {
            write_buffer_start = 0;
        }
    }
}
