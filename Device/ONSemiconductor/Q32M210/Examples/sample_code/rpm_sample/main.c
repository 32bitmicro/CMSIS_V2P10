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
 *  - Main source file for the RPM sample application.
 * ----------------------------------------------------------------------------
 * $Revision: 1.5 $
 * $Date: 2012/04/26 18:20:39 $
 * ------------------------------------------------------------------------- */

#include <q32m210.h>
#include <q32m210_info_map.h>
#include <string.h>
#include "globals.h"
#include "usb.h"
#include "adc.h"
#include "rtc.h"

/* ----------------------------------------------------------------------------
 * Global variables
 * ------------------------------------------------------------------------- */
static int s_SendingADCData = 0;
static int s_ADCBufferOffset;

unsigned int g_SW1_Pressed;
unsigned int g_Num_RTC_Ticks;

/* ----------------------------------------------------------------------------
 * Kernel command constants
 * ------------------------------------------------------------------------- */
#define CMD_GET_VERSION_INFO        1
#define CMD_WRITE_MEM               3
#define CMD_READ_MEM                4
#define CMD_START_ADC_CAPTURE       5
#define CMD_READ_CAPTURE_DATA       9
#define CMD_SET_RC_CLOCK_FREQ       11

#define MAX_WRITE_WORDS             5
#define MAX_READ_WORDS              8

#define ERR_BUSY                    0xFF
#define ERR_INVALID_ARG             0xFE
#define ERR_INVALID_CMD             0xFD
#define ERR_UNSUPPORTED_CMD         0xFC

/* ----------------------------------------------------------------------------
 * LED pattern constants
 * ------------------------------------------------------------------------- */
#define LEDS_ONE_ON     0x10
#define LEDS_ALL_ON     0xF0

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
    Sys_NVIC_Int_ClearPending(USB_EP3IN_IRQn);
    Sys_NVIC_Int_ClearPending(USB_EP4OUT_IRQn);
    Sys_NVIC_Int_ClearPending(USB_EP5OUT_IRQn);   
    Sys_NVIC_Int_ClearPending(ADC_IRQn);
    Sys_NVIC_Int_ClearPending(DMA0_IRQn);
    Sys_NVIC_Int_ClearPending(DMA1_IRQn);
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
    Sys_NVIC_Int_EnableInt(USB_WAKEUP_IRQn);
    Sys_NVIC_Int_EnableInt(USB_SDAV_IRQn);
    Sys_NVIC_Int_EnableInt(USB_SOF_IRQn);
    Sys_NVIC_Int_EnableInt(USB_SUTOK_IRQn);
    Sys_NVIC_Int_EnableInt(USB_SUSPEND_IRQn);
    Sys_NVIC_Int_EnableInt(USB_RESET_IRQn);
    Sys_NVIC_Int_EnableInt(USB_EP0IN_IRQn);
    Sys_NVIC_Int_EnableInt(USB_EP0OUT_IRQn);
    Sys_NVIC_Int_EnableInt(USB_EP2IN_IRQn);
    Sys_NVIC_Int_DisableInt(USB_EP3IN_IRQn);
    Sys_NVIC_Int_EnableInt(USB_EP4OUT_IRQn);
    Sys_NVIC_Int_DisableInt(USB_EP5OUT_IRQn);   
    Sys_NVIC_Int_EnableInt(GPIO_GP1_IRQn);
    Sys_NVIC_Int_EnableInt(TIMER0_IRQn);
}

/* ----------------------------------------------------------------------------
 * Function      : void delay(int nticks)
 * ----------------------------------------------------------------------------
 * Description   : Waits until the SYSTICK timer count flag indicates that
 *                 the timer has expired.  Once the SYSTICK timer expires,
 *                 the counter is automatically reloaded, and the timer starts
 *                 counting again.  This is repeated the number of times 
 *                 specified via the nticks parameter.
 *
 *                 The reload count is configured so that the SYSTICK timer
 *                 has a period of approximately 250 ms (SYS_FREQ / 
 *                 SYSTICK_RELOAD_COUNT).  This function does not wait for 
 *                 the full 250 ms itself.  It simply waits until the timer 
 *                 expires.
 *
 * Inputs        : - nticks     - Number of SYSTICK ticks to wait.
 * Outputs       : None
 * Assumptions   : SYSTICK has already been enabled.
 * ------------------------------------------------------------------------- */
void delay(int nticks)
{
    int i;
    
    for (i = 0; i < nticks; i++)
    {
        while (!Sys_SYSTICK_Get_CountFlag())
        {
            /* Refresh watchdog */
            Sys_Watchdog_Refresh();
        }
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void main(void)
 * ----------------------------------------------------------------------------
 * Description   : Main loop that calls the init routine and then goes into an
 *                 infinite loop.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int main()
{
    int i;
    
    /* Call the system initialization function */
    initialize();
    
    /* Check the retention register */
    if (Sys_Analog_Get_RetentionReg() == 0)
    {
        /* The value in the retention register is 0, indicating that this
         * is the first time that this code is executed */
      
        /* Set the value in the retention register to 1, so we will know that
         * we have already visited this state */
        Sys_Analog_Set_RetentionReg(1);
        
        /* Assume that SW1 has not yet been pressed */
        g_SW1_Pressed = 0;
    }
    else
    {
        /* The value in the retention register is 1, indicating that the
         * system has just awoken from sleep */
      
        /* Assume that SW1 *has* been pressed */
        g_SW1_Pressed = 1;
    }
    
    /* Enable interrupts now that initialization is complete */
    Sys_NVIC_MasterEnable( );

    /* If we're rebooting after entering sleep mode, skip the ADC and USB
     * initialization */
    
    if (!g_SW1_Pressed)
    {
        /* Configure the ADC module */
        ADC_Init();
    
        /* Lower the priority of the USB EP4 out endpoint.  We need to
         * allow ADC interrupts to pre-empt the handler that receives
         * commands, because it waits in a loop while the ADC collects data. */
        Sys_NVIC_Set_InterruptPriority(USB_EP4OUT_IRQn, 0x40);
        
        /* Configure the USB interface controller into typical operation */
        Sys_USB_Initialize();
        
        /* Configure USB controller to use CM3 for data transfer */
        Sys_USB_Config(USB_ENABLE | USB_CONTROLLER_CM3);
    }

    /* Configure the RTC.  Takes 4 RTC ticks (up to 2 seconds), so we'll have
     * this run in parallel with the LED pattern */
    RTC_SetConfig(RTC_CLK_SELECT_16HZ, AFE_RTC_CTRL_SET_CFG,
                 AFE_RTC_CLOCK_RESET, AFE_RTC_ALARM_CFG, 0);

    /* Enable the RTC interrupt */
    Sys_NVIC_Int_EnableInt(RTC_CLOCK_IRQn);
    
    g_Num_RTC_Ticks = 0;
    
    /* Start the SYSTICK timer */
    Sys_SYSTICK_Enable();

    if (!g_SW1_Pressed)
    {
        /* Main loop - executes an infinite loop */
        while (!g_SW1_Pressed)
        {
            /* Refresh watchdog */
            Sys_Watchdog_Refresh();
            
            for (i = 0; i < 4 && !g_SW1_Pressed; i++)
            {
                /* Turn adjacent pairs of LEDs on */
                Sys_GPIO_Set_IF4(LEDS_ONE_ON << i);
                
                /* Hold the pattern for 250 us */
                delay(1);
            }
            
            /* Turn on all LEDs */
            Sys_GPIO_Set_IF4(LEDS_ALL_ON); 
            delay(1);
        }
    }

    /* Execute flash pattern 'B' */
            
    /* Turn all of the LEDs on, as part of the start of flash pattern
     * 'B' */
    Sys_GPIO_Set_IF4(LEDS_ALL_ON);
        
    /* Wait 1000 ms. */
    delay(4);
    
    for (i = 0; i < 5; i++)
    {                
        /* Blink LEDs in checkerboard pattern */
        Sys_GPIO_Set_IF4(0x50);
        delay(1);
        Sys_GPIO_Set_IF4(0xA0);
        delay(1);
    }
    
    /* Turn LEDs off */
    Sys_GPIO_Set_IF4(0);
    
    /* Enable the WAKEUP0 / SW1 pin as a wakeup pin */
    GPIO_IF5_FUNC_SEL->WAKEUP_ENABLE_PIN0_ALIAS = 
        GPIO_IF5_PIN0_WAKEUP_ENABLE_BITBAND;
    
    /* Make sure the RTC configuration from earlier completed */
    while (g_Num_RTC_Ticks < 4)
    {
        Sys_Watchdog_Refresh( );
    }
    
    /* Disable the RTC interrupts */
    Sys_NVIC_Int_DisableInt(RTC_CLOCK_IRQn);
    
    /* Enable the RTC.  This takes approximately 250 ms */
    RTC_SetControl(AFE_RTC_CTRL_RUN_CFG, 1);
    
    /* Enter sleep mode.
     * This function does not return.  When the RTC alarm goes off, the
     * system will awake and start executing the Boot ROM. */
    Sys_OperatingMode_Sleep();
}

/* ----------------------------------------------------------------------------
 * Function      : void USB_HID_ReportReceived(unsigned char *data)
 * ----------------------------------------------------------------------------
 * Description   : Callback indicating that a message was received from the PC.
 * Inputs        : data - Data packet that was received (of length
 *                        HID_REPORT_SIZE).
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void USB_HID_ReportReceived(unsigned char *data)
{
    unsigned char inReportData[HID_REPORT_SIZE];
    unsigned int baseAddress;
    unsigned int version;
    unsigned int numberOfWords;
    int word;
    int i;
    int adcChannel;
    int nsamples;
    int freq;
    int freqoffset;
    int frequency_setting;
    int val;
    int haveReturnData = 1;
    
    /* If we're receiving a new command, stop sending ADC data */
    s_SendingADCData = 0;
    
    /* Zero the return buffer */
    memset(inReportData, 0, HID_REPORT_SIZE);
    
    switch (data[0])
    {
    case CMD_GET_VERSION_INFO:
        inReportData[0] = Q32M210_VAR_SYSTEM_ID_FAMILY;
        inReportData[1] = Q32M210_VAR_SYSTEM_ID_VERSION;
        inReportData[2] = Q32M210_VAR_SYSTEM_ID_REVISION_MAJOR;
        inReportData[3] = Q32M210_VAR_SYSTEM_ID_REVISION_MINOR;
        version = READONLY_REG32_POINTER(0x0000001C);
        inReportData[4] = (version >> 12) & 0xf;
        inReportData[5] = (version >> 8) & 0xf;
        inReportData[6] = version & 0xff;
        inReportData[7] = (APP_VENDOR_ID >> 8) & 0xff;
        inReportData[8] = APP_VENDOR_ID & 0xff;
        inReportData[9] = (APP_PRODUCT_ID >> 8) & 0xff;
        inReportData[10] = APP_PRODUCT_ID & 0xff;
        inReportData[11] = APP_VERSION_MAJOR;
        inReportData[12] = APP_VERSION_MINOR;
        inReportData[13] = APP_VERSION_REVISION;
        break;
    case CMD_WRITE_MEM:
        /* Peripheral / SRAM / system control selection */
        switch (data[1] >> 6)
        {
        case 0:
            baseAddress = Q32M210_PERIPHERAL_BASE;
            break;
        case 1:
            baseAddress = Q32M210_RAM_BASE;
            break;
        case 2:
            baseAddress = Q32M210_PRIVATE_PERIPHERAL_BASE;
            break;
        }
        
        numberOfWords = data[1] & 0xf;
        
        if (numberOfWords <= MAX_WRITE_WORDS)
        {            
            for (i = 0; i < numberOfWords; i++)
            {
                *(int *)(baseAddress + (data[2 + i * 6] << 8 | data[3 + i * 6]))
                    = (data[4 + i * 6] << 24) |
                      (data[5 + i * 6] << 16) |
                      (data[6 + i * 6] << 8) |
                      data[7 + i * 6];
            }
        }
        
        haveReturnData = 0;
            
        break;
    case CMD_READ_MEM:
        /* Peripheral / SRAM / system control selection */
        switch (data[1] >> 6)
        {
        case 0:
            baseAddress = Q32M210_PERIPHERAL_BASE;
            break;
        case 1:
            baseAddress = Q32M210_RAM_BASE;
            break;
        case 2:
            baseAddress = Q32M210_PRIVATE_PERIPHERAL_BASE;
            break;
        }
        
        numberOfWords = data[1] & 0xf;

        if (numberOfWords <= MAX_READ_WORDS)
        {
            for (i = 0; i < numberOfWords; i++)
            {
                word = *(int *)(baseAddress + (data[2 + i * 2] << 8 | 
                                               data[3 + i * 2]));
                inReportData[i * 4] = (word >> 24) & 0xff;
                inReportData[1 + i * 4] = (word >> 16) & 0xff;
                inReportData[2 + i * 4] = (word >> 8) & 0xff;
                inReportData[3 + i * 4] = word & 0xff;
            }
        }
        
        break;
    case CMD_START_ADC_CAPTURE:
        adcChannel = data[1] & 0x3;
        nsamples = (data[2] << 8) | data[3];
        
        ADC_Enable(adcChannel, nsamples);
        
        ADC_Start();
        
        while( ADC_GetState( ) != ADC_STATE_DONE )
          Sys_Watchdog_Refresh();
        
        /* Calculate statistics */
        ADC_EvalMeasure();
        
        val = ADC_NumberOfSamples();    
        inReportData[0] = (val >> 8) & 0xff;
        inReportData[1] = val & 0xff;
        val = ADC_Min(0);
        inReportData[2] = (val >> 8) & 0xff;
        inReportData[3] = val & 0xff;
        val = ADC_Max(0);
        inReportData[4] = (val >> 8) & 0xff;
        inReportData[5] = val & 0xff;
        val = (int)(ADC_Average(0) * 1000.0);
        inReportData[6] = (val >> 24) & 0xff;
        inReportData[7] = (val >> 16) & 0xff;
        inReportData[8] = (val >> 8) & 0xff;
        inReportData[9] = val & 0xff;
        val = (int)(ADC_StandardDeviation(0) * 1000.0);
        inReportData[10] = (val >> 24) & 0xff;
        inReportData[11] = (val >> 16) & 0xff;
        inReportData[12] = (val >> 8) & 0xff;
        inReportData[13] = val & 0xff;
        
        if (ADC_NumberOfChannels() == 2)
        {
            val = ADC_Min(1);
            inReportData[14] = (val >> 8) & 0xff;
            inReportData[15] = val & 0xff;
            val = ADC_Max(1);
            inReportData[16] = (val >> 8) & 0xff;
            inReportData[17] = val & 0xff;
            val = (int)(ADC_Average(1) * 1000.0);
            inReportData[18] = (val >> 24) & 0xff;
            inReportData[19] = (val >> 16) & 0xff;
            inReportData[20] = (val >> 8) & 0xff;
            inReportData[21] = val & 0xff;
            val = (int)(ADC_StandardDeviation(1) * 1000.0);
            inReportData[22] = (val >> 24) & 0xff;
            inReportData[23] = (val >> 16) & 0xff;
            inReportData[24] = (val >> 8) & 0xff;
            inReportData[25] = val & 0xff;
        }
        else
        {
            /* Zero the channel 1 results */
            for (i = 14; i <= 25; i++)
            {
                inReportData[i] = 0;
            }
        }
        
        break;
    case CMD_READ_CAPTURE_DATA:
        nsamples = ADC_FillBuffer(0, inReportData, HID_REPORT_SIZE);
        if (nsamples > 0)
        {
            s_SendingADCData = 1;
            s_ADCBufferOffset = nsamples;
        }
        break;
    case CMD_SET_RC_CLOCK_FREQ:
        freq = data[1] & 0x1F;
        
        /* Handle parts with incorrect info pages */
        if (REG32_POINTER(Q32M210_CAL_RC_CCR_CTRL_BASE + 8 * 4) == 0xffffffff)
        {
            freqoffset = -40;
        }
        else
        {
            freqoffset = 0;
        }            
        
        frequency_setting = ( (REG32_POINTER(Q32M210_CAL_RC_CCR_CTRL_BASE 
                                             + freq * 4 + freqoffset ) )  
                             >> Q32M210_CAL_RC_CCR_CTRL_INFO_Pos ) & 0xFF;
        
        if (frequency_setting != 0xFF) /*invalid value */
        {
            AFE->RC_CCR_CTRL = (REG32_POINTER(Q32M210_CAL_RC_CCR_CTRL_BASE 
                               + freq * 4  + freqoffset )  ) & 0x3FFFFF;
            frequency_setting *= 100000;
        }
        
        /* Reset the systick period based on the frequency setting */
        Sys_SYSTICK_Set_ReloadCount(frequency_setting / 4);
        
        /* Reset the timer0 period also, to 5 ms */
        Sys_Timer0_Set_Control(TIMER0_CTRL_CFG_NO_TIMEOUT | 
                               (frequency_setting / 128 / 80));
    
        inReportData[0] = frequency_setting >> 24;
        inReportData[1] = (frequency_setting >> 16) & 0xff;
        inReportData[2] = (frequency_setting >> 8) & 0xff;
        inReportData[3] = (frequency_setting) & 0xff;
        break;    
    default:
        break;
    }
    
    if (haveReturnData)
    {
        USB_HID_ReportSend(inReportData);
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void USB_Report_Sent()
 * ----------------------------------------------------------------------------
 * Description   : Callback indicating that the last USB report has been sent to
 *                 the PC.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void USB_Report_Sent()
{
    if (s_SendingADCData)
    {
        unsigned char inReportData[HID_REPORT_SIZE];
        int nsamples;

        memset(inReportData, 0, HID_REPORT_SIZE);
        nsamples = ADC_FillBuffer(s_ADCBufferOffset, inReportData, 
                                  HID_REPORT_SIZE);
        
        if (nsamples > 0)
        {
            s_ADCBufferOffset += nsamples;
            USB_HID_ReportSend(inReportData);
        }
        else
        {      
            s_SendingADCData = 0;
        }
        
    }
}
