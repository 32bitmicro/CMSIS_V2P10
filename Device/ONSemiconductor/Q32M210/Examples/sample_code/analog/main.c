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
 * $Date: 2012/04/26 18:50:00 $
 * ------------------------------------------------------------------------- */

#include <q32.h>
#include "globals.h"

/* ----------------------------------------------------------------------------
 * Global variables
 * ------------------------------------------------------------------------- */

/* Data received from the ADC */
unsigned int DMA_Ch0_Dest_Buffer[DMA_CH0_TRANSFER_LENGTH];

/* Source data for the DAC */
unsigned int DMA_Ch1_Src_Buffer[DMA_CH1_TRANSFER_LENGTH];

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
}


/* ----------------------------------------------------------------------------
 * Function      : void main(void)
 * ----------------------------------------------------------------------------
 * Description   : Displays a meter style reading of the most recent ADC sample
 *                 on the LEDs.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int main()
{
    int i;
    
    /* Call the system initialization function */
    initialize();

    /* Configure the DAC0 reference voltage */
    Sys_Analog_Set_DAC0VoltageRef(AFE_DAC_VREF_CFG);

    /* Configure the DAC0 timer */
    Sys_DMA_Set_DAC0TimerConfig(DMA_DAC0_REQ_CFG);

    /* Initialize the ADC destination buffer */
    for (i = 0; i < DMA_CH0_TRANSFER_LENGTH; i++)
    {
        DMA_Ch0_Dest_Buffer[i] = 0;
    }

    /* Load a square wave into the DAC data buffer */
    for (i = 0; i < DMA_CH1_TRANSFER_LENGTH; i++)
    {
        if (i < (DMA_CH1_TRANSFER_LENGTH / 2))
        {
            DMA_Ch1_Src_Buffer[i] = 0x3FF;
        }
        else
        {
            DMA_Ch1_Src_Buffer[i] = 0x0;
        }
    }

    /* Enable the DMAs */
    Sys_DMA_Channel0_Enable();
    Sys_DMA_Channel1_Enable();

    /* Enable interrupts now that initialization is complete */
    Sys_NVIC_MasterEnable();

    /* Mainloop - executes an infinite loop */
    while(1)
    {
        /* Refresh watchdog */
        Sys_Watchdog_Refresh();
    }
}
