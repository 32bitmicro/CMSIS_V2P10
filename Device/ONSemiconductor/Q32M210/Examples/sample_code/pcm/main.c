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
 * $Revision: 1.17 $
 * $Date: 2012/04/26 18:50:02 $
 * ------------------------------------------------------------------------- */

#include <q32.h>
#include "globals.h"

/* ----------------------------------------------------------------------------
 * Global Variables
 * ------------------------------------------------------------------------- */

/* Define buffer variables used by the DMA for transfers to/from the PCM
 * interface */
unsigned int TX_buffer[DMA_CH0_TRANSFER_LENGTH];
unsigned int TX_bufferCounter = 0;

unsigned int RX_buffer[DMA_CH1_TRANSFER_LENGTH];
unsigned int RX_bufferCounter = 0;

/* Amplitude of sine to be sent over PCM */
unsigned int sineAmplitude = 10;

/* Sine wave to be sent over PCM */
int sine[8] = {40, 47, 50, 47, 40, 33, 30, 33};

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
    Sys_NVIC_Int_ClearPending(DMA1_IRQn);
    Sys_NVIC_Int_ClearPending(DMA2_IRQn);
    Sys_NVIC_Int_ClearPending(PCM_ERROR_IRQn);
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
    Sys_NVIC_Int_EnableInt(DMA1_IRQn);
    Sys_NVIC_Int_EnableInt(DMA2_IRQn);
    Sys_NVIC_Int_EnableInt(PCM_ERROR_IRQn);
}

/* ----------------------------------------------------------------------------
 * Function      : void main(void)
 * ----------------------------------------------------------------------------
 * Description   : Transmit a sine wave over the PCM interface.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int main()
{
    int i;
    
    /* Call the system initialization function */
    initialize();

    /* Configure the DAC1 timer */
    Sys_DMA_Set_DAC1TimerConfig(DMA_DAC1_REQ_CFG);

    /* Initialize the values in the RX buffer */
    for (i = 0; i < DMA_CH0_TRANSFER_LENGTH; i++)
    {
        RX_buffer[i] = 0;
    }

    /* Turn off all LEDs */
    Sys_GPIO_Set_IF4(0x0);

    /* Enable the PCM interface */
    Sys_PCM_Enable();

    /* Enable interrupts */
    Sys_NVIC_MasterEnable();

#ifdef PCM_MASTER

    /* Fill the DMA TX buffer to initiate the PCM transmission */
    for (i = 0; i < DMA_CH1_COUNTER_CFG; i++)
    {
        TX_buffer[i] = sineAmplitude * sine[i];
        TX_buffer[i + DMA_CH1_COUNTER_CFG] = TX_buffer[i];
    }

#endif

    /* Enable DMA channel 0 to begin the DMA to PCM transfer */
    Sys_DMA_Channel0_Enable();

    /* Enable DMA channel 1 to begin the PCM to DMA transfer */
    Sys_DMA_Channel1_Enable();

    /* Enable DMA channel 2 for memory to DAC transfer */
    Sys_DMA_Channel2_Enable();

    /* Mainloop - execute an infinite loop while DMA to PCM and PCM to
     * DMA transfers occur */
    while(1)
    {
        /* Refresh watchdog */
        Sys_Watchdog_Refresh();
    }
}
