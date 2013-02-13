/* Copyright (c) 2012 Semiconductor Components Industries, LLC
 * (d/b/a ON Semiconductor). All Rights Reserved.
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor. The
 * terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * adc.h
 * - ADC functions for the RPM sample application.
 * ----------------------------------------------------------------------------
 * $Revision: 1.3 $
 * $Date: 2012/04/18 15:14:19 $
 * ------------------------------------------------------------------------- */
#ifndef AFE_H
#define AFE_H

/* Function prototypes */
extern void ADC_Init(void);
extern void ADC_EvalMeasure(void);
  
extern void ADC_Enable(int channel, int nsamples);
extern void ADC_Start(void);
extern void ADC_IntHandler(void);

extern int   ADC_CheckNewSample(void);
extern unsigned short ADC_GetLastSample(void);
extern void  ADC_MarkBuffer(int offset);
extern unsigned short ADC_GetBufferValue(int mark_offset);

extern int ADC_FillBuffer(int offset, unsigned char* buffer, int nbyte);
extern int ADC_GetState(void);

extern int ADC_NumberOfChannels(void);
extern int ADC_NumberOfSamples(void);
extern int ADC_Min(int channel);
extern int ADC_Max(int channel);
extern float ADC_Average(int channel);
extern float ADC_StandardDeviation(int channel);

/* ----------------------------------------------------------------------------
 * ADC Configuration
 * ------------------------------------------------------------------------- */

#define ADC_DUMMY_CYCLES            0x1DB
#define ADC_DECIMATION              0x0

/* ----------------------------------------------------------------------------
 * Synchronized DAC Waveform Generation Defines
 * ------------------------------------------------------------------------- */
#define DAC_SYNC_WAVEFORM_DISABLE   0
#define DAC_SYNC_WAVEFORM_ENABLE    1

/* ----------------------------------------------------------------------------
 * AFE
 * ----------------------------------------------------------------------------
 * NOTE: Actual sampling rate must be set in afe.c    
 * ADC buffer is set based on the sampling rate and length of the reaction */
#define ADC_BUF_SIZE            (10000)

#define ADC_FULLSCALE           65535
#define ADC_MAX_VOLTAGE         1.8

/* ADC states */
enum ADC_eval_states 
{
    ADC_STATE_IDLE = 0,
    ADC_STATE_DONE,
    ADC_STATE_FILL
};

enum ADC_Eval_Params
{
    PARAM_ADC_SAMPLES = 0
};

#define MAX_PARAM   1    
    
#endif
