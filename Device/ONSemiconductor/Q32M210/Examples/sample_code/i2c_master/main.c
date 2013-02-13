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
 * $Revision: 1.16 $
 * $Date: 2012/04/26 18:50:01 $
 * ------------------------------------------------------------------------- */

#include <q32.h>
#include "globals.h"

/* ----------------------------------------------------------------------------
 * Global Variables
 * ------------------------------------------------------------------------- */
unsigned int I2C_Control_State;
unsigned int I2C_Control_Status;
unsigned int I2C_Control_Cmd_Byte;
unsigned int I2C_Control_Num_RX_Bytes;
unsigned int I2C_Control_Num_TX_Bytes;
unsigned int I2C_RX_buffer_index;
unsigned int I2C_TX_buffer_index;
unsigned char I2C_RX_buffer[MAX_RX_BUFFER_SIZE] = {0};
unsigned char I2C_TX_buffer[MAX_TX_BUFFER_SIZE] = {0};
unsigned int I2C_ACK_received = 0;
unsigned int GPIO_event_flag = 0;
unsigned int Timer0_Running = 0;

/* The number of counter timeouts that have occurred */
unsigned int Timer0_Timeouts = 0;
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
    Sys_NVIC_Int_ClearPending(I2C_IRQn);
    Sys_NVIC_Int_ClearPending(GPIO_GP1_IRQn);
    Sys_NVIC_Int_ClearPending(TIMER0_IRQn);
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
    Sys_NVIC_Int_EnableInt(I2C_IRQn);
    Sys_NVIC_Int_EnableInt(GPIO_GP1_IRQn);
    Sys_NVIC_Int_EnableInt(TIMER0_IRQn);
}

/* ----------------------------------------------------------------------------
 * Function      : void I2C_Control_Initialize(void)
 * ----------------------------------------------------------------------------
 * Description   : Initialize the I2C control interface.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void I2C_Control_Initialize(void)
{
    /* Set initial state and status */
    I2C_Control_State = I2C_CONTROL_STATE_IDLE;
    I2C_Control_Status = I2C_CONTROL_TRANSACTION_NOT_STARTED;

    /* Initialize other state variables */
    I2C_Control_Cmd_Byte = 0;
    I2C_Control_Num_RX_Bytes = 0;
    I2C_Control_Num_TX_Bytes = 0;
    I2C_RX_buffer_index = 0;
    I2C_TX_buffer_index = 0;

}


/* ----------------------------------------------------------------------------
 * Function      : void Default_Command_Handler(void)
 * ----------------------------------------------------------------------------
 * Description   : Flash the LEDs in a pattern to indicate that an invalid
 *                 command has been received.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Default_Command_Handler(void)
{
    Sys_GPIO_Set_IF4(0x550);
}


/* ----------------------------------------------------------------------------
 * Function      : void I2C_Control_Call_Command_Handler(unsigned int cmd)
 * ----------------------------------------------------------------------------
 * Description   : Calls the command handler specified by 'cmd'
 * Inputs        : cmd - command number
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void I2C_Control_Call_Command_Handler(unsigned int cmd)
{
    /* Check which command has been specified, and then call the appropriate
     * command handler. */
    switch(cmd)
    {

        case I2C_CMD4:

            /* Display the data received from the slave on the LEDs */
            Display_Command_Handler();
            break;

        case I2C_CMD5:

            /* Display the data received from the slave on the LEDs */
            Display_Command_Handler();
            break;

        default:

            /* Call the default command handler */
            Default_Command_Handler();
            break;
    }
}


/* ----------------------------------------------------------------------------
 * Function      : void Display_Command_Handler(void)
 * ----------------------------------------------------------------------------
 * Description   : Display the values received in the I2C_RX_buffer on the
 *                 LEDs
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Display_Command_Handler(void)
{
    /* Set all LEDs */
    Sys_GPIO_Set_IF4(((I2C_RX_buffer[0] << 0) | 
                      (I2C_RX_buffer[1] << 1) | 
                      (I2C_RX_buffer[2] << 2) | 
                      (I2C_RX_buffer[3] << 3) | 
                      (I2C_RX_buffer[4] << 4) | 
                      (I2C_RX_buffer[5] << 5) | 
                      (I2C_RX_buffer[6] << 6) | 
                      (I2C_RX_buffer[7] << 7)));

}


/* ----------------------------------------------------------------------------
 * Function      : void Error_Handler(void)
 * ----------------------------------------------------------------------------
 * Description   : Reset the I2C state
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Error_Handler(void)
{
    /* Turn on the LEDs in an error pattern */
    Sys_GPIO_Set_IF4(0x810);

    /* Reset the state */
    I2C_Control_State = I2C_CONTROL_STATE_IDLE;

    /* Reset the status */
    I2C_Control_Status = I2C_CONTROL_TRANSACTION_INCOMPLETE;

    /* Reset the I2C interface */
    Sys_I2C_Reset();

}


/* ----------------------------------------------------------------------------
 * Function      : void Cleanup(void)
 * ----------------------------------------------------------------------------
 * Description   : Do anything that is required before exiting the interrupt
 *                 handler.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Cleanup(void)
{
    /* Re-enable interrupts */
    Sys_NVIC_MasterEnable();

}

/* ----------------------------------------------------------------------------
 * Function      : void I2C_Send_Command(unsigned int cmd)
 * ----------------------------------------------------------------------------
 * Description   : Send the specified command.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : I2C_TX_buffer preloaded if required by command
 * ------------------------------------------------------------------------- */
void I2C_Send_Command(unsigned int cmd)
{
    /* Clear all variables */
    I2C_Control_Num_RX_Bytes = 0;
    I2C_Control_Num_TX_Bytes = 0;
    I2C_RX_buffer_index = 0;
    I2C_TX_buffer_index = 0;

    /* Store the command */
    I2C_Control_Cmd_Byte = cmd;

    /* Disable interrupts */
    Sys_NVIC_MasterDisable();

    /* Start the write */
    Sys_I2C_StartWrite(I2C_SLAVE_ADDR);

    /* Update the state */
    I2C_Control_State = I2C_CONTROL_STATE_SEND_CMD;

    /* Indicate that a transaction has been started */
    I2C_Control_Status = I2C_CONTROL_TRANSACTION_PENDING;

    /* Check which command has been received, and fill all of the required
     * variables */
    switch(cmd)
    {
        case I2C_CMD2:

            /* Clear LEDs command - simple command
             * Use default state for all variables */
            break;

        case I2C_CMD3:

            /* Set 8 LEDs command - write command */
            I2C_Control_Num_TX_Bytes = 8;

            /* Setup the I2C_TX_buffer - this is the pattern that will
             * be displayed on the slave's LEDs. */
            I2C_TX_buffer[0] = 0;
            I2C_TX_buffer[1] = 0;
            I2C_TX_buffer[2] = 1;
            I2C_TX_buffer[3] = 1;
            I2C_TX_buffer[4] = 1;
            I2C_TX_buffer[5] = 1;
            I2C_TX_buffer[6] = 0;
            I2C_TX_buffer[7] = 0;

            /* Use default state for all other variables */
            break;

        case I2C_CMD4:

            /* Get the current state of the LEDs - read command */
            I2C_Control_Num_RX_Bytes = 8;

            /* Use the default state for all other variables */
            break;

        case I2C_CMD5:

            /* Return the state of the LEDs, and then set new values for
             * the LEDs - read and write command */
            I2C_Control_Num_TX_Bytes = 8;
            I2C_Control_Num_RX_Bytes = 8;

            /* Setup the I2C_TX_buffer - this is the pattern that will
             * be displayed on the slave's LEDs. */
            I2C_TX_buffer[0] = 1;
            I2C_TX_buffer[1] = 1;
            I2C_TX_buffer[2] = 0;
            I2C_TX_buffer[3] = 0;
            I2C_TX_buffer[4] = 0;
            I2C_TX_buffer[5] = 0;
            I2C_TX_buffer[6] = 1;
            I2C_TX_buffer[7] = 1;

            /* Use the default state for all other variables */
            break;

        case I2C_CMD6:

            /* Set all LEDs command - simple command
             * Use default state for all variables */
            break;


        default:

            /* Invalid command - transmisson not started */
            I2C_Control_Status = I2C_CONTROL_TRANSACTION_NOT_STARTED;
            I2C_Control_State = I2C_CONTROL_STATE_IDLE;

            /* Reset the I2C interface */
            Sys_I2C_Reset();
            break;
    }

    /* Enable interrupts */
    Sys_NVIC_MasterEnable();

}

/* ----------------------------------------------------------------------------
 * Function      : void main(void)
 * ----------------------------------------------------------------------------
 * Description   : Executes an infinite loop.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int main()
{
    unsigned int Current_Cmd = 2;

    /* Call the system initialization function */
    initialize();

    /* Enable interrupts now that initialization is complete */
    Sys_NVIC_MasterEnable();

    /* Initialize the I2C interface */
    I2C_Control_Initialize();

    /* Turn on all LEDs */
    Sys_GPIO_Set_IF4(0xFF0);

    while(1)
    {
        Sys_Watchdog_Refresh();

        /* Check if SW4 has been pressed */
        if (GPIO_event_flag == 1)
        {
            /* Check to see if the interface is available */
            while (I2C_Control_State != I2C_CONTROL_STATE_IDLE)
            {
                Sys_Watchdog_Refresh();
            }

            /* The interface is available.  Send the a command. */
            I2C_Send_Command(Current_Cmd);

            /* Advance the command number */
            Current_Cmd++;
            if (Current_Cmd == 7)
            {
                /* Wrap back to the first command number */
                Current_Cmd = 2;
            }

            /* Reset the GPIO event flag */
            GPIO_event_flag = 0;
        }
    }
}
