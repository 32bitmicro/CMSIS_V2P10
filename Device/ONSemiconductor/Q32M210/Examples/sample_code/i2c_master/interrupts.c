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
 * $Revision: 1.18 $
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
    if (((status & I2C_ERROR) != 0) ||
        ((status & I2C_BUS_ERROR) != 0))
    {
        /* An I2C error has occurred */
        Error_Handler();
        Cleanup();
        return;
    }

    /* No error has occurred, so check our current state */
    if (I2C_Control_State == I2C_CONTROL_STATE_SEND_CMD)
    {
        /* Send the command */
        Sys_I2C_Write(I2C_Control_Cmd_Byte);

        /* Check the command type */
        if ((I2C_Control_Num_RX_Bytes == 0) && (I2C_Control_Num_TX_Bytes == 0))
        {
            /* This is a simple command */
            I2C_Control_State = I2C_CONTROL_STATE_WAIT_FOR_STOP;

            /* Indicate that this is the last data */
            I2C_CTRL1->LAST_DATA_ALIAS = I2C_LAST_DATA_BITBAND;

            Cleanup();
            return;
        }
        else if (I2C_Control_Num_TX_Bytes != 0)
        {
            /* This is a write (and possibly read) command */
            I2C_Control_State = I2C_CONTROL_STATE_SEND_DATA;
            I2C_Control_Status = I2C_CONTROL_TRANSACTION_PENDING;

            Cleanup();
            return;
        }
        else if (I2C_Control_Num_RX_Bytes != 0)
        {
            /* This is a read command */
            I2C_Control_State = I2C_CONTROL_STATE_GET_DATA;
            I2C_Control_Status = I2C_CONTROL_TRANSACTION_PENDING;

            /* Start the read process */
            Sys_I2C_StartRead(I2C_SLAVE_ADDR);

            /* Indicate that we are waiting for an ACK */
            I2C_ACK_received = 0;

            Cleanup();
            return;
        }
    }
    else if (I2C_Control_State == I2C_CONTROL_STATE_WAIT_FOR_STOP)
    {
        /* Update the state and status */
        I2C_Control_Status = I2C_CONTROL_TRANSACTION_COMPLETE;
        I2C_Control_State = I2C_CONTROL_STATE_IDLE;

        Cleanup();
        return;
    }
    else if (I2C_Control_State == I2C_CONTROL_STATE_SEND_DATA)
    {
        /* Write the data to the I2C buffer */
        Sys_I2C_Write(I2C_TX_buffer[I2C_TX_buffer_index++]);

        /* Decrement the byte count */
        I2C_Control_Num_TX_Bytes--;

        /* Check if there are any more bytes to send */
        if ((I2C_Control_Num_TX_Bytes == 0) && (I2C_Control_Num_RX_Bytes == 0))
        {
            /* There are no more bytes to send.  This command is complete.
             * Go back to our initial state. */
            I2C_Control_State = I2C_CONTROL_STATE_WAIT_FOR_STOP;

            /* Indicate that this is the last data */
            I2C_CTRL1->LAST_DATA_ALIAS = I2C_LAST_DATA_BITBAND;

            Cleanup();
            return;
        }
        else if ((I2C_Control_Num_TX_Bytes == 0) &&
                 (I2C_Control_Num_RX_Bytes != 0))
        {
            /* We are done sending and are ready to receive */

            /* Start the read process */
            Sys_I2C_StartRead(I2C_SLAVE_ADDR);

             /* Indicate that we are waiting for an ACK */
            I2C_ACK_received = 0;

            /* Update the state */
            I2C_Control_State = I2C_CONTROL_STATE_GET_DATA;
            Cleanup();
            return;
        }
        else
        {
            /* Just return */
            Cleanup();
            return;
        }
    }
    else if (I2C_Control_State == I2C_CONTROL_STATE_GET_DATA)
    {
        /* The slave has acknowledged the start read command.
         * Send an acknowledge to tell the slave that we are ready to
         * read.  This will trigger a data event interrupt on the slave. */
        if (I2C_ACK_received == 0)
        {
            I2C_ACK_received = 1;

            /* Send the ack */
            Sys_I2C_ACK();
            Cleanup();
            return;
        }

        /* Read the I2C data and store it into the RX buffer */
        I2C_RX_buffer[I2C_RX_buffer_index++] = Sys_I2C_Read();

        /* Send the ack */
        Sys_I2C_ACK();

        /* Decrement the RX byte count */
        I2C_Control_Num_RX_Bytes--;

        if (I2C_Control_Num_RX_Bytes == 1)
        {
           I2C_CTRL1->LAST_DATA_ALIAS = I2C_LAST_DATA_BITBAND;
        }

        if (I2C_Control_Num_RX_Bytes == 0)
        {
            /* Call the correct command handler since all data has been
             * received */
            I2C_Control_Call_Command_Handler(I2C_Control_Cmd_Byte);

            /* The command is complete.  Reset the state and exit */
            I2C_Control_State = I2C_CONTROL_STATE_WAIT_FOR_STOP;
        }
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


/* ----------------------------------------------------------------------------
 * Function      : void GPIO_GP1_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : Start timer0 if it isn't already started, in order to
 *                 perform some basic button debouncing.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void GPIO_GP1_IRQHandler(void)
{    
    if (Timer0_Running == 0)
    {
        Timer0_Running = 1;
        
        /* Start timer0 */
        Sys_Timers_Start(SELECT_TIMER0);

    }
}


/* ----------------------------------------------------------------------------
 * Function      : void TIMER0_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : Set the GPIO event flag
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void TIMER0_IRQHandler(void)
{
    Timer0_Timeouts = Timer0_Timeouts + 1;
    
    if (Timer0_Timeouts == TIMER0_NUM_TIMEOUTS)
    {
      /* Set the GPIO event flag */
      GPIO_event_flag = 1;
  
      /* Clear the timer0 flag */
      Timer0_Running = 0;
      
      /* Clear the timeouts flags */
      Timer0_Timeouts = 0;
      
    }
    else
    {
        /* Start the timer again */
        Sys_Timers_Start(SELECT_TIMER0);
    }
}
