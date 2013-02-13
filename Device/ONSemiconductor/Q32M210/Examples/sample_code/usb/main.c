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
#include "usb.h"

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
    Sys_NVIC_Int_ClearPending(USB_WAKEUP_IRQn);
    Sys_NVIC_Int_ClearPending(USB_SDAV_IRQn);
    Sys_NVIC_Int_ClearPending(USB_SOF_IRQn);
    Sys_NVIC_Int_ClearPending(USB_SUTOK_IRQn);
    Sys_NVIC_Int_ClearPending(USB_SUSPEND_IRQn);
    Sys_NVIC_Int_ClearPending(USB_RESET_IRQn);
    Sys_NVIC_Int_ClearPending(USB_EP0IN_IRQn);
    Sys_NVIC_Int_ClearPending(USB_EP0OUT_IRQn);
    Sys_NVIC_Int_ClearPending(USB_EP2IN_IRQn);
    Sys_NVIC_Int_ClearPending(USB_EP4OUT_IRQn);
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
    Sys_NVIC_Int_EnableInt(USB_WAKEUP_IRQn);
    Sys_NVIC_Int_EnableInt(USB_SDAV_IRQn);
    Sys_NVIC_Int_EnableInt(USB_SOF_IRQn);
    Sys_NVIC_Int_EnableInt(USB_SUTOK_IRQn);
    Sys_NVIC_Int_EnableInt(USB_SUSPEND_IRQn);
    Sys_NVIC_Int_EnableInt(USB_RESET_IRQn);
    Sys_NVIC_Int_EnableInt(USB_EP0IN_IRQn);
    Sys_NVIC_Int_EnableInt(USB_EP0OUT_IRQn);
    Sys_NVIC_Int_EnableInt(USB_EP2IN_IRQn);
    Sys_NVIC_Int_EnableInt(USB_EP4OUT_IRQn);
}

/* ----------------------------------------------------------------------------
 * Function      : void delay(void)
 * ----------------------------------------------------------------------------
 * Description   : Waits until the SYSTICK timer count flag indicates that
 *                 the timer has expired.  Once the SYSTICK timer expires,
 *                 the counter is automatically reloaded, and the timer starts
 *                 counting again.
 *
 *                 The reload count is configured so that the SYSTICK timer
 *                 has a period of approximately 1 s.  This function does
 *                 not wait for the full 1 s itself.  It simply waits until
 *                 the timer expires.
 *
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void delay()
{
    while(!Sys_SYSTICK_Get_CountFlag())
    {
        /* Refresh watchdog */
        Sys_Watchdog_Refresh();
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void main(void)
 * ----------------------------------------------------------------------------
 * Description   : Initialize system and USB core. Ready and wait for endpoint 4
 *                 to receive transfer information.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int main()
{
    unsigned char report_data[HID_REPORT_SIZE];
    int i;

    /* Call the system initialization function */
    initialize();

    /* Turn off all LEDs. */
    Sys_GPIO_Set_IF4(0x0);

    /* Configure the USB interface controller into typical operation */
    Sys_USB_Initialize();

    /* Configure USB controller to use CM3 for data transfer. */
    Sys_USB_Config(USB_ENABLE | USB_CONTROLLER_CM3);

    /* Enable interrupts now that initialization is complete */
    Sys_NVIC_MasterEnable();

    /* Start the SYSTICK timer */
    Sys_SYSTICK_Enable();

    /* Initialize report data */
    for( i=0; i < HID_REPORT_SIZE; i ++ ) {
        report_data[i] = i;
    }

    /* Main loop - execute an infinite loop */
    while(1)
    {
        /* Refresh the watchdog timer to prevent it restarting the core */
        Sys_Watchdog_Refresh();

        delay();

        /* Update HID report */
        report_data[0] ++;
        report_data[1] = (Sys_GPIO_Get_IF4() & 0x00003000) >> 12;

        if( USB_HID_IsEnumerated() ) {
            USB_HID_ReportSend( report_data );
        }
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void USB_HID_ReportReceived(unsigned char *data)
 * ----------------------------------------------------------------------------
 * Description   : Called when a USB HID output report is received
 * Inputs        : data     - Content of HID output report.
 * Outputs       : None
 * Assumptions   : data is exactly HID_REPORT_SIZE bytes long
 * ------------------------------------------------------------------------- */
void USB_HID_ReportReceived( unsigned char *data )
{
    Sys_GPIO_Set_IF4(data[0] << 4);
}
