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
 * $Revision: 1.12 $
 * $Date: 2012/04/26 18:50:01 $
 * ------------------------------------------------------------------------- */

#include <q32.h>
#include "globals.h"

/* ----------------------------------------------------------------------------
 * Global Variables
 * ------------------------------------------------------------------------- */
unsigned int I2C_Control_State;
unsigned int I2C_Control_Cmd_Byte;
unsigned int I2C_Control_Num_RX_Bytes;
unsigned int I2C_Control_Num_TX_Bytes;
unsigned int I2C_RX_buffer_index;
unsigned int I2C_TX_buffer_index;
unsigned char I2C_RX_buffer[MAX_RX_BUFFER_SIZE] = {0};
unsigned char I2C_TX_buffer[MAX_TX_BUFFER_SIZE] = {0};

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
    /* Set default state */
    I2C_Control_State = I2C_CONTROL_STATE_GET_CMD;

    /* Initialize other state variables */
    I2C_Control_Cmd_Byte = 0;
    I2C_Control_Num_RX_Bytes = 0;
    I2C_Control_Num_TX_Bytes = 0;
    I2C_RX_buffer_index = 0;
    I2C_TX_buffer_index = 0;

}

/* ----------------------------------------------------------------------------
 * Function      : void I2C_Control_Process_Cmd_Byte(void)
 * ----------------------------------------------------------------------------
 * Description   : Read the latest value in the I2C data register and determine
 *                 which command has been specified by the master.  Set
 *                 up the necessary variables based on the command number.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void I2C_Control_Process_Cmd_Byte(void)
{
    uint32_t cmd;

    /* Clear all variables */
    I2C_Control_Num_RX_Bytes = 0;
    I2C_Control_Num_TX_Bytes = 0;
    I2C_RX_buffer_index = 0;
    I2C_TX_buffer_index = 0;

    /* Get the latest command */
    cmd = Sys_I2C_Read();

    /* Store it */
    I2C_Control_Cmd_Byte = cmd;

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
            I2C_Control_Num_RX_Bytes = 8;

            /* Use default state for all other variables */
            break;

        case I2C_CMD4:

            /* Get the current state of the LEDs - read command */
            I2C_Control_Num_TX_Bytes = 8;

            /* Use the default state for all other variables */
            break;

        case I2C_CMD5:

            /* Return the state of the LEDs, and then set new values for
             * the LEDs - read and write command */
            I2C_Control_Num_TX_Bytes = 8;
            I2C_Control_Num_RX_Bytes = 8;

            /* Use the default state for all other variables */
            break;

        case I2C_CMD6:

            /* Set all LEDs command - simple command
             * Use default state for all variables */
            break;

        default:

            /* Use the default state for all variables */
            break;
    }
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
        case I2C_CMD2:

            Clear_LEDs_Command_Handler();
            break;

        case I2C_CMD3:

            Set_LEDs_Command_Handler();
            break;

        case I2C_CMD4:

            Get_LEDs_Command_Handler();
            break;

        case I2C_CMD5:

            Get_and_Set_LEDs_Command_Handler();
            break;

        case I2C_CMD6:

            Turn_On_All_LEDs_Command_Handler();
            break;

        default:

            Default_Command_Handler();
            break;
    }
}


/* ----------------------------------------------------------------------------
 * Function      : void Clear_LEDs_Command_Handler()
 * ----------------------------------------------------------------------------
 * Description   : Clears the LEDs.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Clear_LEDs_Command_Handler(void)
{
    Sys_GPIO_Set_IF4(0x0);
}


/* ----------------------------------------------------------------------------
 * Function      : void Turn_On_All_LEDs_Command_Handler()
 * ----------------------------------------------------------------------------
 * Description   : Turns on all of the LEDs.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Turn_On_All_LEDs_Command_Handler(void)
{
    Sys_GPIO_Set_IF4(0xFF0);
}

/* ----------------------------------------------------------------------------
 * Function      : void Set_LEDs_Command_Handler()
 * ----------------------------------------------------------------------------
 * Description   : Set the LEDs based on the values received via the I2C
 *                 interface.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Set_LEDs_Command_Handler(void)
{
    /* Set all LEDs at once */
    Sys_GPIO_Set_IF4(((I2C_RX_buffer[0] << 4) |
                      (I2C_RX_buffer[1] << 5) |
                      (I2C_RX_buffer[2] << 6) |
                      (I2C_RX_buffer[3] << 7) |
                      (I2C_RX_buffer[4] << 8) |
                      (I2C_RX_buffer[5] << 9) |
                      (I2C_RX_buffer[6] << 10) |
                      (I2C_RX_buffer[7] << 11)));
}


/* ----------------------------------------------------------------------------
 * Function      : void Get_LEDs_Command_Handler()
 * ----------------------------------------------------------------------------
 * Description   : Get the LED values and put them in the I2C TX buffer.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Get_LEDs_Command_Handler(void)
{
    int i;
    
    for (i = 0; i < 8; i++)
    {
        /* Get the ith LED */
        I2C_TX_buffer[i] = ((GPIO->IF4_LCD_OUT & (0x1 << i + 4)) >> i);
    }
}


/* ----------------------------------------------------------------------------
 * Function      : void Get_and_Set_LEDs_Command_Handler()
 * ----------------------------------------------------------------------------
 * Description   : Get the LED values and put them in the I2C TX buffer.
 *                 Set the LEDs based on the values in the I2C RX buffer.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Get_and_Set_LEDs_Command_Handler(void)
{
    /* Set the LEDs */
    Set_LEDs_Command_Handler();

    /* Get the LEDs */
    Get_LEDs_Command_Handler();

}


/* ----------------------------------------------------------------------------
 * Function      : void Error_Handler(void)
 * ----------------------------------------------------------------------------
 * Description   : Reset the I2C state.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Error_Handler(void)
{
    /* Reset the state to the get command state */
    I2C_Control_State = I2C_CONTROL_STATE_GET_CMD;

    /* Set the LEDs into an error pattern */
    Sys_GPIO_Set_IF4(0x550);

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
 * Function      : void main(void)
 * ----------------------------------------------------------------------------
 * Description   : Executes an infinite loop.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int main()
{
    /* Call the system initialization function */
    initialize();

    /* Enable interrupts now that initialization is complete */
    Sys_NVIC_MasterEnable();

    /* Initialize the I2C interface */
    I2C_Control_Initialize();

    /* Turn on all LEDs */
    Sys_GPIO_Set_IF4(0xFF0);

    /* Execute an infinite loop to blink adjacent pairs of LEDs */
    while(1)
    {
        Sys_Watchdog_Refresh();
    }
}
