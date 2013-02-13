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
 * $Date: 2012/04/26 18:50:01 $
 * ------------------------------------------------------------------------- */

#include <q32.h>
#include <q32_eeprom.h>
#include "globals.h"

/* ----------------------------------------------------------------------------
 * Global Variables
 * ------------------------------------------------------------------------- */

/* Address to write to in EEPROM - initialize it to address 0 */
unsigned short EEPROM_write_addr = 0;

/* Address to read from in EEPROM */
unsigned short EEPROM_read_addr;

/* Maximum EEPROM address to read to/write from
 * Allows 100 100-word buffers */
unsigned short max_EEPROM_addr = 10000;

/* DMA ch0 destination buffer */
unsigned int DMA_Ch0_Dest_Buffer[DMA_CH0_TRANSFER_LENGTH];

/* Flag used to indicate if timer0 is running - used in button debouncing */
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
    Sys_NVIC_Int_ClearPending(DMA0_IRQn);
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
    Sys_NVIC_Int_EnableInt(DMA0_IRQn);
    Sys_NVIC_Int_EnableInt(GPIO_GP1_IRQn);
    Sys_NVIC_Int_EnableInt(TIMER0_IRQn);
}


/* ----------------------------------------------------------------------------
 * Function      : void main()
 * ----------------------------------------------------------------------------
 * Description   : Configures the system, EEPROM, and Timer0.  Executes
 *                 an infinite loop while interrupt handlers do all the work.
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

    /* Clear the LEDs */
    Sys_GPIO_Set_IF4(0x0);

    /* Call the EEPROM initialization function */
    EEPROM_Init(SPI_INTERFACE, SPI_PRESCALER, SYSTEM_CLK_FREQ);

    /* Disable write protection, disable write-enable, and set EEPROM to
     * ready.  Enable write-enable immediately before performing an EEPROM
     * write. */
    EEPROM_Write_StatusReg(SPI_INTERFACE, EEPROM_CONFIG);

    /* Enable DMA0 to start data collection from ADC0 */
    Sys_DMA_Channel0_Enable();

    while(1)
    {
        /* Refresh watchdog */
        Sys_Watchdog_Refresh();
    }
}
