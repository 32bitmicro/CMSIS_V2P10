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
 * $Date: 2012/04/26 18:59:28 $
 * ------------------------------------------------------------------------- */

#include <q32.h>
#include "globals.h"

/* ----------------------------------------------------------------------------
 * Function      : void DMA1_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : DMA channel 1 interrupt handler.
 *                 Triggered on PCM TX.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void DMA1_IRQHandler(void)
{
#ifdef PCM_MASTER
    uint32_t i;
#endif

    /* Get the DMA channel 1 status to see which DMA interrupt has occurred. */
    uint32_t status = Sys_DMA_Get_Channel1_Status();

    /* Check the DMA channel 1 status */
    if ((status & DMA_CH1_ERROR_INT_BYTE) != 0)
    {
        /* A DMA channel 1 error interrupt has been received.
         * Remain in an infinite loop for debugging purposes. */
        while(1)
        {
        }
    }

#ifdef PCM_MASTER

    else if ((status & DMA_CH1_COUNTER_INT_BYTE) != 0)
    {
        /* The first 8 bytes in the transmit buffer have been transmitted.
         * Write the new data to be transmitted.  This is not
         * necessary in this example, but is done for illustrative purposes. */
        for (i = 0; i < DMA_CH1_COUNTER_CFG; i++)
        {
            TX_buffer[i] = sineAmplitude * sine[i];
        }
    }
    else if ((status & DMA_CH1_COMPLETE_INT_BYTE) != 0)
    {
        /* The next 8 bytes in the transmit buffer have been transmitted.
         * Write the new data to be transmitted.  This is not
         * necessary in this example, but is done for illustrative purposes. */
        for (i = 0; i < DMA_CH1_COUNTER_CFG; i++)
        {
            TX_buffer[i + DMA_CH1_COUNTER_CFG] = sineAmplitude * sine[i];
        }
    }

#endif

    /* Reset the DMA channel 1 status */
    Sys_DMA_Clear_Channel1_Status();
}


/* ----------------------------------------------------------------------------
 * Function      : void DMA0_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : DMA channel 0 interrupt handler.
 *                 Triggered on PCM RX.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void DMA0_IRQHandler(void)
{
#ifdef PCM_SLAVE
    uint32_t i;
#endif

    uint32_t status;
    
    /* Disable interrupts */
    Sys_NVIC_MasterDisable();

    /* Get the DMA channel 0 status to see which DMA interrupt has occurred. */
    status = Sys_DMA_Get_Channel0_Status();

    /* Check the DMA channel 0 status */
    if ((status & DMA_CH0_ERROR_INT_BYTE) != 0)
    {
        /* A DMA channel 0 error interrupt has been received.
         * Remain in an infinite loop for debugging purposes. */
        while(1)
        {
        }
    }

#ifdef PCM_SLAVE

    else if ((status & DMA_CH0_COUNTER_INT_BYTE) != 0)
    {
        /* A DMA channel 0 counter interrupt has been received, indicating that
         * 8 words have been received by the PCM interface and transferred to
         * DMA 1. */

        for (i = 0; i < DMA_CH0_COUNTER_CFG; i++)
        {
            /* Copy the words that have been received to TX_buffer, so that
             * they may be transmitted via the PCM interface */
            TX_buffer[i] = RX_buffer[i];
        }

    }
    else if ((status & DMA_CH0_COMPLETE_INT_BYTE) != 0)
    {
        /* A DMA channel 0 counter interrupt has been received, indicating that
         * another 8 words have been received by the PCM interface and 
         * transferred to DMA 1. */

        for (i = DMA_CH0_COUNTER_CFG; i < DMA_CH0_TRANSFER_LENGTH; i++)
        {
            /* Copy the words that has been received to TX_buffer, so that they 
             * may be transmitted via the PCM interface */
            TX_buffer[i] = RX_buffer[i];
        }
    }

#endif /*   #ifdef PCM_SLAVE */

    /* Reset the DMA channel 0 status */
    Sys_DMA_Clear_Channel0_Status();

    /* Re-enable interrupts */
    Sys_NVIC_MasterEnable();
}


/* ----------------------------------------------------------------------------
 * Function      : void DMA2_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : DMA channel 2 interrupt handler.
 *                 DMA channel 2 handles memory-to-DAC1 transfers.
 *                 This interrupt handler is used for debugging purposes only.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void DMA2_IRQHandler(void)
{
    /* Get the DMA channel 2 status to see which DMA interrupt has occurred. */
    uint32_t status = Sys_DMA_Get_Channel2_Status();

    /* Check the DMA channel 2 status */
    if ((status & DMA_CH2_ERROR_INT_BYTE) != 0)
    {
        /* A DMA channel 2 error interrupt has been received.
         * Remain in an infinite loop for debugging purposes. */
        while(1)
        {
        }
    }
    else if ((status & DMA_CH2_COMPLETE_INT_BYTE) != 0)
    {
        __NOP();
    }

    /* Reset the DMA channel 2 status */
    Sys_DMA_Clear_Channel2_Status();
}

/* ----------------------------------------------------------------------------
 * Function      : void PCM_ERROR_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : Interrupt handler that is called when there is a PCM
 *                 overrun or underrun error.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void PCM_ERROR_IRQHandler(void)
{
    /* Insert a assembly 'nop' so that a breakpoint can be used to detect
     * PCM errors. */
    __NOP();
}
