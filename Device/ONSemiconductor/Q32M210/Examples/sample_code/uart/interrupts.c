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
 * $Revision: 1.15 $
 * $Date: 2012/04/26 18:50:02 $
 * ------------------------------------------------------------------------- */

#include <q32.h>
#include "globals.h"

/* ----------------------------------------------------------------------------
 * Function      : void UART0_TX_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : UART0 transmit interrupt handler.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void UART0_TX_IRQHandler(void)
{
    /* When UART_Write_buffer is not empty, send the next character */
    if (write_buffer_start != write_buffer_end)
    {

#if (BUILD_TYPE==UART_LOOPBACK)

        /* Write the one word of the sine wave to DAC2 */
        Sys_Analog_Set_DAC2Data(sine_amplitude*sine[idx++]);

        /* Perform a wrap of the index if necessary */
        if (idx == (SINE_SIZE-1))
        {
            idx = 0;
        }
#endif

        /* Send data over UART0 */
        Sys_UART0_Write(UART_Write_buffer[write_buffer_start++]);

        /* Reset UART_Write_buffer start index to 0 when it reaches the end
         * of the buffer. */
        if (write_buffer_start == BUFFER_SIZE)
        {
            write_buffer_start = 0;
        }

    }
}

/* ----------------------------------------------------------------------------
 * Function      : void UART0_RX_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : UART0 receive interrupt handler.
 *                 Only used in UART_TO_PC mode.
 *                 Receives data from the PC over RS232.
 *                 Stores the data received in UART_Read_buffer and outputs
 *                 the stored data when a '\' character is received.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void UART0_RX_IRQHandler(void)
{
    /* Temporary variable to store received character */
    char input;

    /* Disable UART0 RX interrupt */
    Sys_NVIC_Int_DisableInt(UART0_RX_IRQn);

    /* Read data from UART0 */
    input = Sys_UART0_Read();

    /* When a '\' is received, output the contents of UART_Read_buffer */
    if (input == '\\')
    {
        output_stored_content_flag_UART0 = 1;
    }
    /* Other characters will be stored in UART_Read_buffer */
    else
    {
        UART_Read(input);
    }

    /* Re-enable interrupts */
    Sys_NVIC_Int_EnableInt(UART0_RX_IRQn);
}

#if (BUILD_TYPE==UART_LOOPBACK)

/* ----------------------------------------------------------------------------
 * Function      : void TIMER0_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : Timer 0 interrupt handler.
 *                 Read the UART1 RX buffer and write its value to DAC1.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void TIMER0_IRQHandler(void)
{
    /* Temporary variable to store received character */
    char input;

    /* Read the word just received */
    input = Sys_UART1_Read();

    /* Write the word to DAC1 */
    Sys_Analog_Set_DAC1Data(sine_amplitude*input);
}


#endif  /* #if BUILD_TYPE == UART_LOOPBACK */
