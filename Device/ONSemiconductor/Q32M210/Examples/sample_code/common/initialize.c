/* ----------------------------------------------------------------------------
 * Copyright (c) 2012 Semiconductor Components Industries, LLC
 * (d/b/a ON Semiconductor). All Rights Reserved.
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor. The
 * terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * initialize.c
 *  - Initialization source file
 * ----------------------------------------------------------------------------
 * $Revision: 1.15 $
 * $Date: 2012/04/27 20:43:19 $
 * ------------------------------------------------------------------------- */

#include <q32.h>
#include <q32m210_info_map.h>
#include "globals.h"

extern void *__vector_table;

/* ----------------------------------------------------------------------------
 * Function      : void initialize()
 * ----------------------------------------------------------------------------
 * Description   : Performs all system initialization tasks.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void initialize()
{
    /* Disable all interrupts during initialization */
    Sys_NVIC_MasterDisable();

    /* Reset the I2C */
    Sys_I2C_Reset();

    /* Disable the PCM interface */
    Sys_PCM_Disable();

    /* Disable DMAs */
    Sys_DMA_Channel0_Disable();
    Sys_DMA_Channel1_Disable();
    Sys_DMA_Channel2_Disable();
    Sys_DMA_Channel3_Disable();

    /* Clear the DMA status */
    Sys_DMA_Clear_Channel0_Status();
    Sys_DMA_Clear_Channel1_Status();
    Sys_DMA_Clear_Channel2_Status();
    Sys_DMA_Clear_Channel3_Status();

    /* ------------------------------------
     * Power Supply Configuration
     * ------------------------------------ */

    Sys_Analog_Set_PSUControl(AFE_PSU_CFG);
    Sys_Analog_Set_OpModeControl(AFE_OPMODE_CFG);

    /* ------------------------------------
     * Clocking Configuration
     * ------------------------------------ */

    /* Setup clocking source and clock divisors */
    Sys_Clk_Config_Root(AFE_OSC_CFG);
    Sys_Clk_Config_MCLK(MCLK_CLK_DIV_CFG);

    /* Setup RC Oscillator Clock Frequency */
    if (REG32_POINTER(Q32M210_MFTR_TEST_VERSION2) == 0x1)
    {
        Sys_Analog_Set_CCRControlRC((REG32_POINTER(Q32M210_CAL_RC_CCR_CTRL_BASE
            + (AFE_RC_CCR_FREQ_MHZ - 1) * 4) ) & CLOCK_CAL_MASK);
    }
    else
    {
        /* If the calibration value was not found, set the default value. */
        if (Sys_Analog_Set_RCFreq(SYS_FREQ) == FLASH_ERR_GENERAL_FAILURE)
        {
            Sys_Analog_Set_CCRControlRC(AFE_RC_CCR_DEFAULT_CFG);
        }
    }

    /* Setup USR clock */
    Sys_Clk_Config_USR_CLK(USR_CLK_CFG);

    /* ------------------------------------
     * Reset PCM
     * ------------------------------------ */

    /* Enable the PCM in CM3 controller mode in order to reset the PCM
     * interface */
    Sys_PCM_Config(PCM_CFG_RESET);

    /* Disable the PCM interface */
    Sys_PCM_Disable();

    /* Delay */
    Sys_Delay(20000);


    /* --------------------------------------
     * Clock Divisor Configurations
     * -------------------------------------- */

    /* Set the UART clock divisor */
    Sys_Clk_Config_UART0(UART0_CLK_DIV_CFG);
    Sys_Clk_Config_UART1(UART1_CLK_DIV_CFG);


    /* ------------------------------------
     * Amplifier Configuration
     * ------------------------------------ */

    /* Select inputs, outputs and amplifier control settings */
    Sys_Analog_Set_AmplifierInputSelect(AFE_INPUT_SEL_CFG);
    Sys_Analog_Set_AmplifierControl(AFE_AMP_CTRL_CFG);
    Sys_Analog_Set_AmplifierOutputSelect(AFE_AMP_OUT_CFG);

    /* ------------------------------------
     * Switch Control Configuration
     * ------------------------------------ */

    /* Select SPST switch configuration */
    Sys_Analog_Set_SPSTControl(AFE_SPST_CFG);

    /* Select SPDT switch configuration */
    Sys_Analog_Set_MSWControl(AFE_SPDT_CFG);

    /* --------------------------------------
     * ADC and Pre-amplifier Configuration
     * -------------------------------------- */

    /* Configure pre-amplifier */
    Sys_Analog_Set_PGA0Control(AFE_PGA0_CTRL_CFG);
    Sys_Analog_Set_PGA1Control(AFE_PGA1_CTRL_CFG);
    Sys_Analog_Set_PGAGainControl(AFE_PGA_GAIN_CTRL_CFG);

    /* Configure ADCs */
    Sys_Analog_Set_ADCControl(AFE_ADC_CTRL_CFG);

    /* Configure sample rate */
    Sys_Analog_Set_ADCDataRateConfig(AFE_DATA_RATE_CFG);

    /* --------------------------------------
     * DAC Configuration
     * -------------------------------------- */

    /* Configure DAC settings */
    Sys_Analog_Set_DACControl(AFE_DAC_CFG);

    /* --------------------------------------
     * Temperature Sensor Configuration
     * -------------------------------------- */

    /* Configure the internal temperature sensor settings */
    Sys_Analog_Set_TempSenseControl(AFE_TEMP_SENSE_CFG);

    /* ------------------------------
     *  CRC Generator Configuration
     * ------------------------------ */
    Sys_CRC_Config(CRC_CFG);

    /* --------------------------------------
     *  Direct Memory Access Configuration
     * -------------------------------------- */
    Sys_DMA_Channel0_Config(DMA_CH0_CFG,
                            DMA_CH0_TRANSFER_LENGTH,
                            DMA_CH0_COUNTER_CFG,
                            DMA_CH0_SRC_ADDRESS,
                            DMA_CH0_DEST_ADDRESS);
    Sys_DMA_Clear_Channel0_Status();

    Sys_DMA_Channel1_Config(DMA_CH1_CFG,
                            DMA_CH1_TRANSFER_LENGTH,
                            DMA_CH1_COUNTER_CFG,
                            DMA_CH1_SRC_ADDRESS,
                            DMA_CH1_DEST_ADDRESS);
    Sys_DMA_Clear_Channel1_Status();

    Sys_DMA_Channel2_Config(DMA_CH2_CFG,
                            DMA_CH2_TRANSFER_LENGTH,
                            DMA_CH2_COUNTER_CFG,
                            DMA_CH2_SRC_ADDRESS,
                            DMA_CH2_DEST_ADDRESS);
    Sys_DMA_Clear_Channel2_Status();

    Sys_DMA_Channel3_Config(DMA_CH3_CFG,
                            DMA_CH3_TRANSFER_LENGTH,
                            DMA_CH3_COUNTER_CFG,
                            DMA_CH3_SRC_ADDRESS,
                            DMA_CH3_DEST_ADDRESS);
    Sys_DMA_Clear_Channel3_Status();

    /* ------------------------------
     * SYSTICK Timer Configuration
     * ------------------------------ */

    /* Disable the SYSTICK timer */
    Sys_SYSTICK_Disable();

    /* Set clock source */
    Sys_SYSTICK_Set_ClkSource(SYSTICK_CLKSOURCE_CFG);

    /* Set the reload count */
    Sys_SYSTICK_Set_ReloadCount(SYSTICK_RELOAD_COUNT);

    /* Disable generation of the SYSTICK interrupt */
    Sys_SYSTICK_DisableInt();

    /* -------------------------------------
     * General Purpose Timer Configuration
     * ------------------------------------- */

    /* Ensure timers are stopped */
    Sys_Timers_Stop(SELECT_ALL_TIMERS);

    /* Configure all timers */
    Sys_Timer0_Set_Control(TIMER0_CTRL_CFG);
    Sys_Timer1_Set_Control(TIMER1_CTRL_CFG);
    Sys_Timer2_Set_Control(TIMER2_CTRL_CFG);
    Sys_Timer3_Set_Control(TIMER3_CTRL_CFG);

    /* -------------------------------------
     * Watchdog Configuration
     * ------------------------------------- */

    /* Setup watchdog timeout period */
    Sys_Watchdog_Set_Timeout(WATCHDOG_TIMEOUT_CFG);

    /* -------------------------------------
     * GPIO Configuration
     * ------------------------------------- */

    /* Setup GPIO interface 0 configuration */
    Sys_GPIO_IF0_Config(GPIO_IF0_FUNCTION_CFG);

    /* Setup GPIO interface 1 configuration */
    Sys_GPIO_IF1_Config(GPIO_IF1_FUNCTION_CFG);

    /* Setup GPIO interface 2, 3 configuration */
    Sys_GPIO_IF23_Config(GPIO_IF23_FUNCTION_CFG);

    /* Setup GPIO interface 4 configuration */
    Sys_GPIO_IF4_Config(GPIO_IF4_FUNCTION_CFG,
                        GPIO_IF4_OUTPUT_ENABLE_CFG,
                        GPIO_IF4_PULLDOWN_CFG);

    /* Setup GPIO interface 5 configuration */
    Sys_GPIO_IF5_Config(GPIO_IF5_FUNCTION_CFG);

    /* Configure GPIO interrupts */
    Sys_GPIO_Int_GP0Config(GPIO_INT_CTRL_GP0_CFG);
    Sys_GPIO_Int_GP1Config(GPIO_INT_CTRL_GP1_CFG);
    Sys_GPIO_Int_GP2Config(GPIO_INT_CTRL_GP2_CFG);
    Sys_GPIO_Int_GP3Config(GPIO_INT_CTRL_GP3_CFG);
    Sys_GPIO_Int_GP4Config(GPIO_INT_CTRL_GP4_CFG);    
    Sys_GPIO_Int_GP5Config(GPIO_INT_CTRL_GP5_CFG);
    Sys_GPIO_Int_GP6Config(GPIO_INT_CTRL_GP6_CFG);
    Sys_GPIO_Int_GP7Config(GPIO_INT_CTRL_GP7_CFG);    
    
    /* Configure GPIO input control */
    Sys_GPIO_IF_Input_Config(GPIO_INPUT_CTRL_CFG);

    /* -------------------------------------
     * PCM Configuration
     * ------------------------------------- */

    /* Setup the PCM interface */
    Sys_PCM_Config(PCM_CFG);

    /* -------------------------------------
     * SPI Configuration
     * ------------------------------------- */

    /* Setup the SPI0 interface */
    Sys_SPI0_Config(SPI0_CFG0);
    Sys_SPI0_TransferConfig(SPI0_CFG1);

    /* Setup the SPI1 interface */
    Sys_SPI1_Config(SPI1_CFG0);
    Sys_SPI1_TransferConfig(SPI1_CFG1);

    /* -------------------------------------
     * UART Configuration
     * ------------------------------------- */

    /* Setup UART interface configurations */
    Sys_UART0_Config(UART0_CTRL_CFG);
    Sys_UART1_Config(UART1_CTRL_CFG);

    /* Configure UART interface speeds */
    Sys_UART0_Set_Speed(UART0_SPEED_DEFAULT, UART_SYSCLOCK_SPEED_DEFAULT);
    Sys_UART1_Set_Speed(UART1_SPEED_DEFAULT, UART_SYSCLOCK_SPEED_DEFAULT);

    /* -------------------------------------
     * USB Controller Configuration
     * ------------------------------------- */

    /* Setup USB interface configuration */
    Sys_USB_Config(USB_CFG);

    /* -------------------------------------
     * I2C Clock Configuration
     * ------------------------------------- */

    Sys_Clk_Config_I2C(I2C_CLK_CFG);

    /* -------------------------------------
     * I2C Controller Configuration
     * ------------------------------------- */

    /* Setup I2C inferface configuration */
    Sys_I2C_Config(I2C_CTRL_CFG);
    Sys_I2C_PhysicalConfig(I2C_PHYSICAL_CTRL_CFG);

    /* ------------------------------
     * NVIC Configuration
     * ------------------------------ */

    /* Setup vector table location */
    Sys_NVIC_Set_VectTable((unsigned int)&__vector_table);

    /* Setup base priority and priority bit groupings */
    Sys_NVIC_Set_BasePriority(NVIC_BASE_PRIORITY_CFG);
    Sys_NVIC_Set_PriorityGroup(NVIC_PRIORITY_GROUP_CFG);

    /* Disable all faults - this will escalate these faults
     * to a hard fault */
    Sys_NVIC_Fault_DisableInt(MemoryManagement_IRQn);
    Sys_NVIC_Fault_DisableInt(BusFault_IRQn);
    Sys_NVIC_Fault_DisableInt(UsageFault_IRQn);

    /* Disable all external interrupts */
    Sys_NVIC_Int_DisableAllInt();

    /* Clear pending interrupts */
    Clear_Pending_Interrupts();

    /* Enable indivudal external interrupts */
    Enable_Individual_Interrupts();
}
