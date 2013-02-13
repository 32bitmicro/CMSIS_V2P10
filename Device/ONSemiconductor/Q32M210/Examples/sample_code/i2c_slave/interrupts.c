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
 * $Revision: 1.10 $
 * $Date: 2012/04/26 20:45:54 $
 * ------------------------------------------------------------------------- */

#include <q32.h>
#include "globals.h"

/* ----------------------------------------------------------------------------
 * Function      : void I2C_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : Set GPIO LEDs to output the content of the I2C Data Buffer
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void I2C_IRQHandler(void)
{
    uint32_t status;

    /* Disable interrupts */
    Sys_NVIC_MasterDisable();

    /* Get the I2C status */
    status = Sys_I2C_Get_Status();

    /* Check for any I2C error conditions */
    if (((status & I2C_ERROR) != 0) || ((status & I2C_BUS_ERROR) != 0))
    {
        /* An I2C error has occurred */
        Error_Handler();
        Cleanup();
        return;
    }

    /* Check if we have received a stop interrupt */
    if ((status & I2C_STOP_DETECTED) != 0)
    {
        /* Stop bit detected
         * Exit interrupt handler */
        Cleanup();
        return;
    }

    /* No error has occurred, so check our current state */
    if (I2C_Control_State == I2C_CONTROL_STATE_GET_CMD)
    {
        /* Make sure this isn't a read */
        if ((status & I2C_IS_READ) != 0)
        {
            /* The I2C command is a read; our internal state is out of sync
             * with what we have just received. */
            Error_Handler();
            Cleanup();
            return;
        }

        /* The I2C command is a write, as expected.
         * Read the command byte and process it. */
        I2C_Control_Process_Cmd_Byte();

        /* The cmd byte processing function will have extracted the necessary
         * information about the cmd into the I2C control variables.
         * Now, check what state to enter:
         *      rx_bytes  = 0 && tx_bytes == 0   -> simple cmd
         *      rx_bytes != 0                    -> write (and possibly read)cmd
         *      tx_bytes != 0                    -> read cmd */

       if ((I2C_Control_Num_RX_Bytes == 0) && (I2C_Control_Num_TX_Bytes == 0))
       {
            /* Simple command. */

            /* There are no further bytes to send or receive, so call the
             * necessary command handler */
            I2C_Control_Call_Command_Handler(I2C_Control_Cmd_Byte);

            /* The command is complete - reset the state */
            I2C_Control_State = I2C_CONTROL_STATE_GET_CMD;
            Cleanup();
            return;
       }
       else if ((I2C_Control_Num_RX_Bytes == 0) &&
                (I2C_Control_Num_TX_Bytes != 0))
       {
            /* There are some bytes to send - this must be a read command */

            /* Call the command handler to fill the TX buffer */
            I2C_Control_Call_Command_Handler(I2C_Control_Cmd_Byte);

            /* Set the next state and exit */
            I2C_Control_State = I2C_CONTROL_STATE_SEND_DATA;
            Cleanup();
            return;
       }
       else if ((I2C_Control_Num_RX_Bytes != 0) &&
               ((I2C_Control_Num_TX_Bytes == 0) ||
                (I2C_Control_Num_TX_Bytes != 0)))
       {
            /* There are some bytes to receive - this must be a write command */

            /* Set the next state and exit */
            I2C_Control_State = I2C_CONTROL_STATE_GET_DATA;
            Cleanup();
            return;
       }

    }
    else if (I2C_Control_State == I2C_CONTROL_STATE_GET_DATA)
    {
        /* Make sure this isn't a read */
        if ((status & I2C_IS_READ) != 0)
        {
            /* The I2C command is a read */
            Error_Handler();
            Cleanup();
            return;
        }

        /* Read the I2C data and store it into the RX buffer */
        I2C_RX_buffer[I2C_RX_buffer_index++] = Sys_I2C_Read();

        /* Decrement the RX byte count */
        I2C_Control_Num_RX_Bytes--;

        if ((I2C_Control_Num_RX_Bytes == 0) && (I2C_Control_Num_TX_Bytes == 0))
        {
            /* Call the correct command handler since all data has been
             * received */
            I2C_Control_Call_Command_Handler(I2C_Control_Cmd_Byte);

            /* The command is complete.  Reset the state and exit */
            I2C_Control_State = I2C_CONTROL_STATE_GET_CMD;
            Cleanup();
            return;
        }
        else if ((I2C_Control_Num_RX_Bytes == 0) &&
                 (I2C_Control_Num_TX_Bytes != 0))
        {
             /* Call the correct command handler since all data has been
              * received.  This command handler should fill the TX buffer as 
              * well. */
             I2C_Control_Call_Command_Handler(I2C_Control_Cmd_Byte);

             /* There are some TX bytes - set the next state */
             I2C_Control_State = I2C_CONTROL_STATE_SEND_DATA;
             Cleanup();
             return;
        }
        else
        {
            /* Just return - no change of state necessary */
            Cleanup();
            return;
        }

    }
    else if (I2C_Control_State == I2C_CONTROL_STATE_SEND_DATA)
    {
        /* Make sure this isn't a write */
        if ((status & I2C_IS_WRITE) != 0)
        {
            /* The I2C command is a write */
            Error_Handler();
            Cleanup();
            return;
        }

        /* Check if there are any more bytes to send */
        if (I2C_Control_Num_TX_Bytes == 0)
        {
            /* There are no more bytes to send.  This command is complete.
             * Go back to our initial state. */
            I2C_Control_State = I2C_CONTROL_STATE_GET_CMD;
            Cleanup();
            return;
        }

        /* Write the data to the I2C buffer */
        Sys_I2C_Write(I2C_TX_buffer[I2C_TX_buffer_index++]);

        /* Decrement the byte count */
        I2C_Control_Num_TX_Bytes--;
        Cleanup();
        return;
    }
    else
    {
        /* We shouldn't get here */
        Error_Handler();
        Cleanup();
        return;
    }

    /* We shouldn't get here either */
    Error_Handler();
    Cleanup();
    return;
}


