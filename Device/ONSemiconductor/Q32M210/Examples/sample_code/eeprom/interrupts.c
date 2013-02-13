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
 * $Date: 2012/04/26 18:50:01 $
 * ------------------------------------------------------------------------- */

#include <q32.h>
#include <q32_eeprom.h>
#include "globals.h"

/* ----------------------------------------------------------------------------
 * Function      : void DMA0_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : DMA channel 0 interrupt handler.
 *                 Triggered on DMA error or DMA complete interrupt.
 *                 Write data from ADC to EEPROM.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void DMA0_IRQHandler(void)
{
    int i;
    
    /* Get the DMA channel 0 status to see which DMA interrupt has occurred. */
    unsigned int status = Sys_DMA_Get_Channel0_Status();

    /* Disable DMA channel 0 so that the ADC data does not get overwritten
     * while the EEPROM write is being performed. */
    Sys_DMA_Channel0_Disable();

    if ((status & DMA_CH0_ERROR_INT_BYTE) != 0)
    {
        /* A DMA channel 0 error interrupt has been received.
         * Remain in an infinite loop for debugging purposes. */
        while(1)
        {
        }
    }
    else if ((status & DMA_CH0_COMPLETE_INT_BYTE) != 0)
    {
        /* A DMA channel 0 complete interrupt has been received, meaning
         * that we have received 100 samples from ADC0. */

        /* The ADC is 16 bit, but the buffer DMA_Ch0_Dest_Buffer is 32-bit.
         * The ADC data is in the top 16 bits.  Shift the data by 16 bits
         * in order to get the correct value. */
        for (i = 0; i < DMA_CH0_TRANSFER_LENGTH; i++)
        {
            DMA_Ch0_Dest_Buffer[i] = DMA_Ch0_Dest_Buffer[i] >> 16;
        }

        /* Disable the Timer0 interrupt so that data does not get read from
         * the EEPROM during an EEPROM write. */
        Sys_NVIC_Int_DisableInt(TIMER0_IRQn);

        /* Set the write-enable opcode in EEPROM library */
        EEPROM_Set_WriteEnableOpcode(SPI_INTERFACE, EEPROM_OPCODE_WRITE_ENABLE);

        /* Write the data that has been received to EEPROM */
        EEPROM_Write(SPI_INTERFACE,
                     EEPROM_write_addr,
                     (unsigned char *) DMA_Ch0_Dest_Buffer,
                     (DMA_CH0_TRANSFER_LENGTH * 4));

        /* Refresh watchdog */
        Sys_Watchdog_Refresh();

        /* Clear the write-enable opcode in EEPROM library */
        EEPROM_Set_WriteEnableOpcode(SPI_INTERFACE, EEPROM_OPCODE_NOP);

        /* Set the EEPROM read address to the start of the buffer just 
         * written */
        EEPROM_read_addr = EEPROM_write_addr;

        /* Re-enable the Timer0 interrupt now that the EEPROM write is
         * complete */
        Sys_NVIC_Int_EnableInt(TIMER0_IRQn);

        /* Advance the write pointer by the size of the block just written */
        EEPROM_write_addr = EEPROM_write_addr + DMA_CH0_TRANSFER_LENGTH;

        /* Check if the write pointer is ready to be wrapped */
        if (EEPROM_write_addr == max_EEPROM_addr)
            EEPROM_write_addr = 0;

    }

    /* Clear the DMA channel 0 status */
    Sys_DMA_Clear_Channel0_Status();

    /* Re-enable DMA channel 0 */
    Sys_DMA_Channel0_Enable();
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
        /* Set the flag */
        Timer0_Running = 1;

        /* Start timer0 */
        Sys_Timers_Start(SELECT_TIMER0);
    }
}


/* ----------------------------------------------------------------------------
 * Function      : void TIMER0_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : Read data from the EEPROM
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void TIMER0_IRQHandler(void)
{
    int i;
    unsigned int sum = 0;
    unsigned int avg = 0;
    
    Timer0_Timeouts = Timer0_Timeouts + 1;
    
    if (Timer0_Timeouts == TIMER0_NUM_TIMEOUTS)
    {
        /* Create a buffer to store the data read from EEPROM */
        unsigned int read_buffer[DMA_CH0_TRANSFER_LENGTH];

        /* Do not read EEPROM unless there is data stored in the EEPROM */
        if (EEPROM_write_addr > EEPROM_read_addr)
        {
            /* Disable DMA0 interrupts so that the sample doesn't try to perform
             * an EEPROM write while we are doing an EEPROM read */
            Sys_NVIC_Int_DisableInt(DMA0_IRQn);
    
            /* Read the latest block of words written to the EEPROM */
            EEPROM_Read(SPI_INTERFACE,
                         EEPROM_read_addr,
                         (unsigned char *)read_buffer,
                         (DMA_CH0_TRANSFER_LENGTH * 4));
    
            /* Re-enable DMA0 interrupt */
            Sys_NVIC_Int_EnableInt(DMA0_IRQn);
    
            /* Refresh the watchdog */
            Sys_Watchdog_Refresh();
    
            /* Average the data that was read */
            for (i = 0; i < DMA_CH0_TRANSFER_LENGTH; i++)
                sum = sum + read_buffer[i];
    
            avg = sum/DMA_CH0_TRANSFER_LENGTH;
    
            /* Display the bottom 8-bits of 'avg' on the LEDs. */
            avg = (avg & 0xFF) << 4;
            Sys_GPIO_Set_IF4(avg);
        }

        /* Clear the timer0 flag */
        Timer0_Running = 0;
        
        /* Clear the timeouts flag as well */
        Timer0_Timeouts = 0;
    }
    else
    {
        /* Start the timer again */
        Sys_Timers_Start(SELECT_TIMER0);
    }
        

}
