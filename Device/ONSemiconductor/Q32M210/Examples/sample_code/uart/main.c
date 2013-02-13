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
 * $Revision: 1.20 $
 * $Date: 2012/04/26 20:45:55 $
 * ------------------------------------------------------------------------- */

#include <q32.h>
#include "globals.h"

/* ----------------------------------------------------------------------------
 * Global Variables
 * ------------------------------------------------------------------------- */

/* Buffer to store data to be written to the UART */
char UART_Write_buffer[BUFFER_SIZE];

/* Buffer to store data read from the UART */
char UART_Read_buffer[BUFFER_SIZE];

/* Initialize the start index for UART_Write_buffer at 0 */
int write_buffer_start = 0;

/* Initialize the end index for UART_Write_buffer at 0 */
int write_buffer_end = 0;

/* Initialize the start index for UART_Read_buffer at 0 */
int read_buffer_start = 0;

/* Initialize the end index for UART_Read_buffer at 0 */
int read_buffer_end = 0;

/* Used to detect when a '\' is received. Initialize to 0. */
int output_stored_content_flag_UART0 = 0;

#if (BUILD_TYPE==UART_LOOPBACK)
    /* 16-point sine wave + null character */
    char sine[SINE_SIZE] = {169, 185, 196, 200, 196, 185, 169, 150, 130, 114,
                            103, 100, 103, 114, 130, 150, 0};

    /* Amplitude of the sine wave */
    int sine_amplitude = 2;

    /* Used to index the data that is being written to DAC2 */
    int idx = 0;

#endif


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
    Sys_NVIC_Int_ClearPending(UART0_RX_IRQn);
    Sys_NVIC_Int_ClearPending(UART0_TX_IRQn);
#if (BUILD_TYPE==UART_LOOPBACK)
    Sys_NVIC_Int_ClearPending(TIMER0_IRQn);
#endif
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
    Sys_NVIC_Int_EnableInt(UART0_RX_IRQn);
    Sys_NVIC_Int_EnableInt(UART0_TX_IRQn);
#if (BUILD_TYPE==UART_LOOPBACK)
    Sys_NVIC_Int_EnableInt(TIMER0_IRQn);
#endif
}


/* ----------------------------------------------------------------------------
 * Function      : void UART_Print(char * outString)
 * ----------------------------------------------------------------------------
 * Description   : Adds characters to UART_Write_buffer.
 * Inputs        : outString - string to transmit over UART0
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void UART_Print(char * outString)
{
    int i = 0;
    int tempflag = 0;

    /* Disable UART0 TX interrupt */
    Sys_NVIC_Int_DisableInt(UART0_TX_IRQn);

    /* Set tempflag if the UART interface is not currently transmitting */
    if (write_buffer_end == write_buffer_start)
    {
        tempflag = 1;
    }

    /* As long as current character is not null for input string, copy
     * that character to UART_Write_buffer */
    while(outString[i] != '\0')
    {
        /* Copy to UART_Write_buffer */
        UART_Write_buffer[write_buffer_end++] = outString[i];

        /* Increment input string character index */
        i++;

        /* If the end of the buffer has been reached, reset the pointer */
        if (write_buffer_end == BUFFER_SIZE)
        {
            write_buffer_end = 0;
        }
    }

    /* Set the UART0_TX interrupt to pending in order to trigger the interrupt
     * and start writing characters to UART0 */
    if (tempflag == 1)
    {
        NVIC_SetPendingIRQ(UART0_TX_IRQn);
    }

    /* Enable interrupts */
    Sys_NVIC_Int_EnableInt(UART0_TX_IRQn);

}

/* ----------------------------------------------------------------------------
 * Function      : void UART_Read(unsigned char data)
 * ----------------------------------------------------------------------------
 * Description   : Adds a character to UART_Read_buffer
 * Inputs        : data - character to append to UART_Read_buffer
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void UART_Read(unsigned char data)
{
    /* Add input data to end of UART Read buffer */
    UART_Read_buffer[read_buffer_end++] = data;

    /* If the end of the buffer has been reached, reset the pointer */
    if (read_buffer_end == BUFFER_SIZE)
    {
        read_buffer_end = 0;
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void main(void)
 * ----------------------------------------------------------------------------
 * Description   : Main routine for UART sample.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int main()
{
#if (BUILD_TYPE==UART_TO_PC)
    int i;
#endif

    /* Call the system initialization function */
    initialize();

    /* Initialize first element of UART Write buffer to null. This is to prevent
     * error when printing an empty buffer. */
    UART_Write_buffer[0] = '\0';

    /* Initialize first element of UART Read buffer to null. This is to prevent
     * error when printing an empty buffer. */
    UART_Read_buffer[0] = '\0';

    /* Enable interrupts now that initialization is complete */
    Sys_NVIC_MasterEnable();

    /* Turn off all LEDs */
    Sys_GPIO_Set_IF4(0x0);

#if (BUILD_TYPE==UART_TO_PC)

    /* Transmit information and instructions over UART0 */
    UART_Print("\n\n------------------------------------\n");
    UART_Print("UART Sample Code\n");
    UART_Print("Copyright (c) 2010 ON Semiconductor.\n");
    UART_Print("All Rights Reserved.\n");
    UART_Print("------------------------------------\n");
    UART_Print("\nPlease input data, type '\\' to print and clear data.\n");
    UART_Print("\nInput Data: ");

    /* Poll for a request to print stored content */
    while(1)
    {
        if(output_stored_content_flag_UART0 == 1)
        {
            /* Temporary variable used to output each element of
             * UART_Read_buffer. This allows the printing of individual
             * characters since the print function only accepts char * input 
             * parameters. */
            char stored_data[2];

            /* Reset the flag */
            output_stored_content_flag_UART0 = 0;

            /* Write the following to UART0 */
            UART_Print("\nStored Data: ");

            /* Initialize second element as null to terminate string */
            stored_data[1] = '\0';

            /* Print the elements of UART_Read_buffer from the start index to
             * the end index. */
            for (i = read_buffer_start; i < read_buffer_end; ++i)
            {
              stored_data[0] = UART_Read_buffer[i];
              UART_Print(stored_data);
            }

            /* Set end index of UART_Read_buffer to 0, new data will overwrite 
             * the previous. */
            read_buffer_end = 0;

            /* Write the following to UART0 */
            UART_Print("\nCleared Stored Data.\nInput Data: ");
        }

        /* Refresh the watchdog */
        Sys_Watchdog_Refresh();
    }

#elif (BUILD_TYPE==UART_LOOPBACK)

    /* Start timer 0 */
    Sys_Timers_Start(SELECT_TIMER0);

    while (1)
    {
        /* Write the sine wave to UART0 */
        UART_Print(sine);

        /* Refresh the watchdog */
        Sys_Watchdog_Refresh();
     }
#else
    #error Unsupported build type.
#endif /* #if BUILD_TYPE == UART_TO_PC */

}
