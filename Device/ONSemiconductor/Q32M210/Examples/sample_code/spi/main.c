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
 * $Revision: 1.14 $
 * $Date: 2012/04/26 18:50:02 $
 * ------------------------------------------------------------------------- */

#include <q32.h>
#include "globals.h"

/* ----------------------------------------------------------------------------
 * Global Variables
 * ------------------------------------------------------------------------- */

/* Initialize SPI Write buffer */
char SPI_Write_buffer[BUFFER_SIZE];

/* Initialize the start index for SPI Write buffer at 0 */
int write_buffer_start = 0;

/* Initialize the end index for SPI Write buffer at 0 */
int write_buffer_end = 0;

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
    Sys_NVIC_Int_ClearPending(SPI0_IRQn);
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
    Sys_NVIC_Int_EnableInt(SPI0_IRQn);
}


/* ----------------------------------------------------------------------------
 * Function      : void SPI_Print(char * outString)
 * ----------------------------------------------------------------------------
 * Description   : Adds a character to the SPI write buffer.
 * Inputs        : outString - string to transmit over SPI0
 * Outputs       : None
 * Assumptions   : SPI is in manual mode.
 * ------------------------------------------------------------------------- */
void SPI_Print(char * outString)
{
    int i = 0;
    int tempflag = 0;

    /* Disable the SPI0 interrupt */
    Sys_NVIC_Int_DisableInt(SPI0_IRQn);

    /* Set tempflag if the SPI interface is not currently transmitting */
    if (write_buffer_end == write_buffer_start)
    {
        tempflag = 1;
    }

    /* As long as current character is not null for input string, copy
     * that character to SPI write buffer */
    while (outString[i] != '\0')
    {
        /* Copy to buffer */
        SPI_Write_buffer[write_buffer_end++] = outString[i++];

        /* Wrap pointer, if necessary */
        if (write_buffer_end == BUFFER_SIZE - 1)
        {
            write_buffer_end = 0;
        }
    }

    /* Send the SPIO0 interrupt to pending */
    if (tempflag == 1)
    {
        NVIC_SetPendingIRQ(SPI0_IRQn);
    }

    /* Enable the SPI0 interrupt to start printing characters */
    Sys_NVIC_Int_EnableInt(SPI0_IRQn);
}

/* ----------------------------------------------------------------------------
 * Function      : void main(void)
 * ----------------------------------------------------------------------------
 * Description   : Sends data over the SPIO.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int main()
{
    /* Setup the system */
    initialize();

    /* Enable interrupts now that initialization is complete */
    Sys_NVIC_MasterEnable();

    /* Turn off all LEDs */
    Sys_GPIO_Set_IF4(0x0);

    /* Write the first word ("U") to the SPI interface */
    Sys_SPI0_Write((unsigned int)"U");

    /* Initiate the SPIO0 transfer.  This step is necessary since SPIO0
     * is configured in manual mode.  It is not necessary if SPIO0 is
     * configured in auto mode. */
    Sys_SPI0_MasterInit();

    /* Mainloop - Continually send the letter "U"(0x55) over SPI0 */
    while (1)
    {
        /* Print the letter "U" */
        SPI_Print("U");

        /* Refresh the watchdog */
        Sys_Watchdog_Refresh();
    }
}
