/* ----------------------------------------------------------------------------
 * Copyright (c) 2012 Semiconductor Components Industries, LLC
 * (d/b/a ON Semiconductor). All Rights Reserved.
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor. The
 * terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * adc.c
 * - ADC functions for the RPM sample application.
 * ----------------------------------------------------------------------------
 * $Revision: 1.3 $
 * $Date: 2012/04/18 15:14:19 $
 * ------------------------------------------------------------------------- */
#include <q32m210.h>
#include <q32m210_hw.h>
#include "adc.h"
#include "globals.h"
#include <math.h>

/* Globals */
int   s_ADC_IntCount;

short s_ADC_Buffer[ADC_BUF_SIZE];


/* Waveform Generation Variables */
static int s_ADC_ChannelToSave;
static int s_ADC_NewSampleReady;
static int s_ADC_Buf_CurIndex;
static int s_ADC_Buf_MarkIndex;

static int s_ADC_State;

static int s_ADC_NumSamples;

/* Measurements */
static int s_ADC_min[2];
static int s_ADC_max[2];
static int s_ADC_sum[2];
static double s_ADC_avg[2];
static double s_ADC_var[2];
static double s_ADC_stddev[2];
   
/* ----------------------------------------------------------------------------
 * Function      : void ADC_Init()
 * ----------------------------------------------------------------------------
 * Description   : Initializes the analog front end
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void ADC_Init()
{     
    /* Arbitrary default value -- overwritten by ADC_Enable below. */
    s_ADC_NumSamples = 100;

    /* ------------------------------------
     * Clocking Configuration
     * ------------------------------------ */

    Sys_Clk_Config_MCLK(MCLK_CLK_DISABLE_BYTE | MCLK_CLK_DIV_2_BYTE);  

    Sys_Analog_Set_OpModeControl(RC_OSC_ENABLE | VADC_ENABLE);
        
    /* --------------------------------------
     * ADC and Pre-amplifier Configuration
     * -------------------------------------- */

    /* Configure pre-amplifier */
    Sys_Analog_Set_PGA0Control(PGA0_DISABLE);

    Sys_Analog_Set_PGA1Control(PGA1_DISABLE);

    Sys_Analog_Set_PGAGainControl(PGA_CUT_OFF_LOW |
                                    PGA0_GAIN_0DB |
                                    PGA1_GAIN_0DB);

    /* Configure ADCs */
    Sys_Analog_Set_ADCControl(ADC0_DISABLE               |
                               ADC0_FORMAT_UNSIGNED_INT  |
                               ADC1_DISABLE              |
                               ADC1_FORMAT_UNSIGNED_INT);

    /* Configure sample rate
     * - Sampling frequency calculation is as follows:
     *
     *   SYS_CLK = 3.00 MHz
     *   MCLK    = 3.00 MHz (configure MCLK to divide by 1)
     *   ADC_CLK = 1.50 MHz (automatic divide by 2 for 50% duty cycle)
     *
     *   ORSD cycles  = 2^(0xA) = 1024
     *   DUMMY cycles = 475
     *
     *   Total cycles = 1024 + 475 + 1 = 1500
     *
     *   SF = MCLK / (Total Cycles) = 1.5M / 1500 = 1000 Hz
     * */
    AFE->DATARATE_CFG = (unsigned int)(ADC_DUMMY_CYCLES | (ADC_DECIMATION << 
        AFE_DATARATE_CFG_DECIMATION_FACTOR_Pos));

    /* --------------------------------------
     * Initialize globals
     * -------------------------------------- */
    s_ADC_ChannelToSave     = 0;
    s_ADC_IntCount          = 0;
    s_ADC_Buf_CurIndex      = 0;
    s_ADC_NewSampleReady    = 0;
    s_ADC_Buf_MarkIndex     = 0;
    
    s_ADC_State          = ADC_STATE_IDLE;
}

/* ----------------------------------------------------------------------------
 * Function      : void ADC_Enable(int channel, int nsamples)
 * ----------------------------------------------------------------------------
 * Description   : Enable the ADC peripherals and reset the buffer pointers
 * Inputs        : channel - Channel to save (0 - ch0, 1 - ch1, 2 - both)
 *                 nsamples - Number of samples
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void ADC_Enable(int channel, int nsamples)
{   
    /* Disable ADC interrupt */
    Sys_NVIC_Int_DisableInt(ADC_IRQn);

    /* Reset globals */
    s_ADC_Buf_CurIndex   = 0; 
    s_ADC_NewSampleReady = 0;
    s_ADC_Buf_MarkIndex  = 0;         
    s_ADC_State        = ADC_STATE_IDLE;    

    if (nsamples > ADC_BUF_SIZE)
        nsamples = ADC_BUF_SIZE;
        
    /* Configuration */
    s_ADC_ChannelToSave  = channel;  
    s_ADC_NumSamples = nsamples;   
}

/* ----------------------------------------------------------------------------
 * Function      : void ADC_Start()
 * ----------------------------------------------------------------------------
 * Description   : Configures ADC interrupt handler to fill buffer
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void ADC_Start()
{
    /* Setup parameters to fill buffer */
    s_ADC_State        = ADC_STATE_FILL;

    /* Enable ADC interrupt */
    Sys_NVIC_Int_EnableInt(ADC_IRQn);
}

/* ----------------------------------------------------------------------------
 * Function      : void ADC_EvalMeasure()
 * ----------------------------------------------------------------------------
 * Description   : Perform measurements on ADC buffer. Results set in global
 *                 variables
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void ADC_EvalMeasure()
{
    int nchannels, nsamples;
    int ch, i;
    
    /* Reset calculated parameters */
    s_ADC_min[0] = s_ADC_min[1] = 65535;
    s_ADC_max[0] = s_ADC_max[1] = -65536;
    s_ADC_sum[0] = s_ADC_sum[1] = 0;
    s_ADC_var[0] = s_ADC_var[1] = 0.0;
    
    if (s_ADC_ChannelToSave >= 2)
    {
        nchannels = 2;
    }
    else
    {
        nchannels = 1;
    }
    
    nsamples = s_ADC_NumSamples / nchannels;
    
    for (ch = 0; ch < nchannels; ch++)
    {
        /* Find min/max/avg */
        for (i = 0; i < nsamples; i++)
        {
            int samp = ADC_GetBufferValue(nchannels * i + ch);
            
            if (samp < s_ADC_min[ch])
                s_ADC_min[ch] = samp;
            
            if (samp > s_ADC_max[ch])
                s_ADC_max[ch] = samp;
    
            s_ADC_sum[ch] += samp;
        }
    
        s_ADC_avg[ch] = s_ADC_sum[ch] / ((float)nsamples);
    
        /* Calculate variance and standard deviation
         * Also update histogram data */
        for (i = 0; i < nsamples; i++)
        {
            int samp = ADC_GetBufferValue(nchannels * i + ch);
            s_ADC_var[ch] += pow(samp - s_ADC_avg[ch], 2);
        }
        
        s_ADC_stddev[ch] = sqrt(s_ADC_var[ch] / (nsamples - 1));
    }
    
    /* Reset the state to idle. */
    s_ADC_State          = ADC_STATE_IDLE;
}

/* ----------------------------------------------------------------------------
 * Function      : int ADC_CheckNewSample()
 * ----------------------------------------------------------------------------
 * Description   : Checks if a new ADC sample is available. If so, clears the
 *                 flag.
 * Inputs        : None
 * Outputs       : Returns 1 if a new output sample is available; otherwise 0.
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int ADC_CheckNewSample()
{
    if (s_ADC_NewSampleReady == 1)
    {
        s_ADC_NewSampleReady = 0;
        return 1;
    }
    else
    {
        return 0;
    }
}

/* ----------------------------------------------------------------------------
 * Function      : unsigned short ADC_GetLastSample()
 * ----------------------------------------------------------------------------
 * Description   : Returns the latest sample recorded from the ADC
 * Inputs        : None
 * Outputs       : Latest sample
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
unsigned short ADC_GetLastSample()
{
    return s_ADC_Buffer[s_ADC_Buf_CurIndex];    
}

/* ----------------------------------------------------------------------------
 * Function      : void ADC_MarkBuffer(int offset)
 * ----------------------------------------------------------------------------
 * Description   : Set a mark in the current buffer
 * Inputs        : offset - Offset in samples from current position
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void ADC_MarkBuffer(int offset)
{
    /* Disable interrupts */
    Sys_NVIC_MasterDisable();

    /* Adjust for the offset and wrap the buffer if required */
    s_ADC_Buf_MarkIndex = s_ADC_Buf_CurIndex + offset;
    if (s_ADC_Buf_MarkIndex < 0)
    {
        s_ADC_Buf_MarkIndex += ADC_BUF_SIZE;
    }
    else if (s_ADC_Buf_MarkIndex >= ADC_BUF_SIZE)
    {
        s_ADC_Buf_MarkIndex -= ADC_BUF_SIZE;
    }

    /* Re-enable interrupts */
    Sys_NVIC_MasterEnable();
}

/* ----------------------------------------------------------------------------
 * Function      : unsigned short ADC_GetBufferValue(int mark_offset)
 * ----------------------------------------------------------------------------
 * Description   : Returns buffer value offset from the mark
 * Inputs        : mark_offset - Offset in samples from the current mark
 * Outputs       : Value at specified offset
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
unsigned short ADC_GetBufferValue(int mark_offset)
{
    int idx = s_ADC_Buf_MarkIndex + mark_offset;
    
    if (idx < 0)
    {
        idx += ADC_BUF_SIZE;
    }
    else if (idx >= ADC_BUF_SIZE)
    {
        idx -= ADC_BUF_SIZE;
    }
    
    return s_ADC_Buffer[idx];
}

/* ----------------------------------------------------------------------------
 * Function      : int ADC_FillBuffer(int offset, unsigned char* buffer, 
 *                  int nbytes)
 * ----------------------------------------------------------------------------
 * Description   : Fills the provided buffer with ADC samples.
 * Inputs        : offset - Offset into the internal buffer at which to start 
 *                          copying.
 *                 buffer - Pointer to the output buffer.
 *                 nbytes - Number of bytes to copy.
 * Outputs       : Number of samples placed into the buffer.
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int ADC_FillBuffer(int offset, unsigned char* buffer, int nbytes)
{
    int i;
    int nsamples = nbytes / 2;
    
    if (offset + nsamples >= s_ADC_Buf_CurIndex)
    {
        nsamples = s_ADC_Buf_CurIndex - offset;
    }
    
    for (i = 0; i < nsamples; i++)
    {
        buffer[i * 2] = s_ADC_Buffer[offset + i] >> 8;
        buffer[i * 2 + 1] = s_ADC_Buffer[offset + i] & 0xff;
    }
    
    return nsamples;
}

/* ----------------------------------------------------------------------------
 * Function      : int ADC_GetState()
 * ----------------------------------------------------------------------------
 * Description   : Returns ADC state
 * Inputs        : None
 * Outputs       : Returns the current state of the ADC
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int ADC_GetState()
{
    return s_ADC_State;
}

/* ----------------------------------------------------------------------------
 * Function      : void ADC_IRQHandler()
 * ----------------------------------------------------------------------------
 * Description   : Interrupt handler for the ADC
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void ADC_IRQHandler()
{
    /* Store the latest sample as per the state machine */
    switch (s_ADC_State)
    {
        case ADC_STATE_IDLE:
        case ADC_STATE_DONE:
            break;

        case ADC_STATE_FILL:

            /* Store sample in buffer */
            
            if (s_ADC_ChannelToSave == 1)
            {
                s_ADC_Buffer[s_ADC_Buf_CurIndex++] = (AFE->ADC01_DATA & 
                                                      AFE_ADC01_DATA_ADC1_Mask) 
                                                      >> AFE_ADC01_DATA_ADC1_Pos;
            }
            else
            {
                s_ADC_Buffer[s_ADC_Buf_CurIndex++] = (AFE->ADC01_DATA & 
                                                      AFE_ADC01_DATA_ADC0_Mask) 
                                                      >> AFE_ADC01_DATA_ADC0_Pos;
            }
            
            if (s_ADC_ChannelToSave == 2)
                s_ADC_Buffer[s_ADC_Buf_CurIndex++] = (AFE->ADC01_DATA & 
                                                      AFE_ADC01_DATA_ADC1_Mask) 
                                                      >> AFE_ADC01_DATA_ADC1_Pos;
                
            if (s_ADC_Buf_CurIndex >= s_ADC_NumSamples)
            {
                s_ADC_State = ADC_STATE_DONE;
            }
            
            /* Indicate a new sample is available */
            s_ADC_NewSampleReady = 1;

            break;

        default:
            break;
    }
}   

/* ----------------------------------------------------------------------------
 * Function      : void ADC_NumberOfChannels()
 * ----------------------------------------------------------------------------
 * Description   : Returns the number of channels captured.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int ADC_NumberOfChannels()
{
    return s_ADC_ChannelToSave;
}

/* ----------------------------------------------------------------------------
 * Function      : void ADC_NumberOfSamples()
 * ----------------------------------------------------------------------------
 * Description   : Returns the number of samples captured.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int ADC_NumberOfSamples()
{
    return s_ADC_Buf_CurIndex;
}

/* ----------------------------------------------------------------------------
 * Function      : void ADC_Min(int channel)
 * ----------------------------------------------------------------------------
 * Description   : Returns the min ADC value for the specified channel.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int ADC_Min(int channel)
{
    return s_ADC_min[channel];
}

/* ----------------------------------------------------------------------------
 * Function      : void ADC_Max(int channel)
 * ----------------------------------------------------------------------------
 * Description   : Returns the max ADC value for the specified channel.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int ADC_Max(int channel)
{
    return s_ADC_max[channel];
}

/* ----------------------------------------------------------------------------
 * Function      : void ADC_Average(int channel)
 * ----------------------------------------------------------------------------
 * Description   : Returns the average ADC value for the specified channel.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
float ADC_Average(int channel)
{
    return s_ADC_avg[channel];
}

/* ----------------------------------------------------------------------------
 * Function      : void ADC_StandardDeviation(int channel)
 * ----------------------------------------------------------------------------
 * Description   : Returns the standard deviation of the sample values for the
 *                 specified channel.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
float ADC_StandardDeviation(int channel)
{
    return s_ADC_stddev[channel];
}

