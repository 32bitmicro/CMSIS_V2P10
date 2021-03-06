/* ----------------------------------------------------------------------------
 * Copyright (c) 2012 Semiconductor Components Industries, LLC
 * (d/b/a ON Semiconductor). All Rights Reserved.
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor. The
 * terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * globals.h
 * - Global header file
 * - Contains all system and application settings for the sample code
 * ----------------------------------------------------------------------------
 * $Revision: 1.20 $
 * $Date: 2012/04/27 20:43:19 $
 * ------------------------------------------------------------------------- */

#ifndef GLOBALS_H
#define GLOBALS_H

/* ----------------------------------------------------------------------------
 * AFE Power Supply Configuration
 * ------------------------------------------------------------------------- */

#define AFE_PSU_CFG     (LCDBACKLIGHT_DISABLE | \
                         VCP_DISABLE          | \
                         LCDDRIVER_DISABLE    | \
                         VDBL_DISABLE)

#define AFE_OPMODE_CFG  (RC_OSC_ENABLE | \
                         VADC_DISABLE)

/* ----------------------------------------------------------------------------
 * AFE Clocking Configuration
 * ------------------------------------------------------------------------- */

/* Oscillator control configuration - select external clock source and set
 * divider to 1, so that the RTC clock runs at 2 Hz */
#define AFE_OSC_CFG             (RCLK_SELECT_RC_OSC | \
                                 RTC_CLK_SELECT_2HZ)

/* Configure RC oscillator to have a frequency of 3 MHz */
#define AFE_RC_CCR_FREQ_MHZ      3

#define AFE_RC_CCR_DEFAULT_CFG   (RC_OSC_FREQ_RANGE_2                      | \
                                 (0x2D << AFE_RC_CCR_CTRL_COARSE_CTRL_Pos) | \
                                 (0x1F << AFE_RC_CCR_CTRL_FINE_CTRL_Pos))

/* System clock frequency in Hz */
#define SYS_FREQ                 (AFE_RC_CCR_FREQ_MHZ * 1000000)
#define CLOCK_CAL_MASK           0x3FFFFF

/* ----------------------------------------------------------------------------
 * AFE Amplifier Configuration
 * ------------------------------------------------------------------------- */

/* Amplifier input select configuration */
#define AFE_INPUT_SEL_CFG       (ALT0_SW_SEL_NONE | \
                                 ALT1_SW_SEL_NONE | \
                                 A0_CFG_SEL_DISABLE)

/* Amplifier control - disable all amplifiers */
#define AFE_AMP_CTRL_CFG        (A0_DISABLE | \
                                 A1_DISABLE | \
                                 A2_DISABLE)

/* Amplifier output select configuration - select defaults and disable */
#define AFE_AMP_OUT_CFG         (A0_OUTA_DISABLE | \
                                 A0_OUTB_DISABLE | \
                                 A1_OUTA_DISABLE | \
                                 A1_OUTB_DISABLE | \
                                 A2_OUTA_DISABLE | \
                                 A2_OUTB_DISABLE)

/* ----------------------------------------------------------------------------
 * AFE Switch Control Configuration
 * ------------------------------------------------------------------------- */

/* Single-pole single-throw (SPST) configuration - leave all switches open */
#define AFE_SPST_CFG    (SPST3_SEL_OPEN | \
                         SPST2_SEL_OPEN | \
                         SPST1_SEL_OPEN | \
                         SPST0_SEL_OPEN)

/* Multi-Switch (MSW) configuration - select default switch positions */
#define AFE_SPDT_CFG    (MSW3_SEL_A | \
                         MSW2_SEL_A | \
                         MSW1_SEL_A | \
                         MSW0_SEL_A)

/* ----------------------------------------------------------------------------
 * AFE ADC and Pre-amplifier Configuration
 * ------------------------------------------------------------------------- */

/* Disable PGA0 and select default inputs */
#define AFE_PGA0_CTRL_CFG       (PGA0_DISABLE      | \
                                 PGA0_DIF_SEL_VSS  | \
                                 PGA0_SEL_A0REF)

/* Disable PGA1 and select default inputs */
#define AFE_PGA1_CTRL_CFG       (PGA1_DISABLE      | \
                                 PGA1_DIF_SEL_VSS  | \
                                 PGA1_SEL_A1REF)

/* Configure PGA gain to bypass (0 dB) and 160 Hz cut-off for LPF */
#define AFE_PGA_GAIN_CTRL_CFG   (PGA_CUT_OFF_LOW   | \
                                 PGA0_GAIN_0DB     | \
                                 PGA1_GAIN_0DB )

/* Disable ADC0 and ADC1, use two's complement format */
#define AFE_ADC_CTRL_CFG        (ADC0_DISABLE          | \
                                 ADC0_FORMAT_TWOS_COMP | \
                                 ADC1_DISABLE          | \
                                 ADC1_FORMAT_TWOS_COMP)

/* Use ADC decimation filtering adjustment factor of 0x10 */
#define AFE_DATA_RATE_CFG       (0x10)

/* ----------------------------------------------------------------------------
 * AFE DAC Configuration
 * ------------------------------------------------------------------------- */

/* Disable all DACs */
#define AFE_DAC_CFG             (DAC0_DISABLE | \
                                 DAC1_DISABLE | \
                                 DAC2_DISABLE)

/* ----------------------------------------------------------------------------
 * AFE Temperature Sensor Configuration
 * ------------------------------------------------------------------------- */

/* Disable the internal temperature sensor */
#define AFE_TEMP_SENSE_CFG      (TEMP_SENSE_DISABLE)

/* ----------------------------------------------------------------------------
 * CRC Generator Configuration
 * ------------------------------------------------------------------------- */

/* Configure the CRC generator to use little-endian byte ordering when adding
 * multiple bytes of data to the CRC calculation */
#define CRC_CFG                 (CRC_LITTLE_ENDIAN)

/* ----------------------------------------------------------------------------
 * Direct Memory Access Configuration
 * ------------------------------------------------------------------------- */

/* DMA Channel 0 configuration - Disable the DMA and related components */
#define DMA_CH0_CFG                 (DMA_CH0_LITTLE_ENDIAN        | \
                                     DMA_CH0_DISABLE_INT_DISABLE  | \
                                     DMA_CH0_ERROR_INT_DISABLE    | \
                                     DMA_CH0_COMPLETE_INT_DISABLE | \
                                     DMA_CH0_COUNTER_INT_DISABLE  | \
                                     DMA_CH0_START_INT_DISABLE    | \
                                     DMA_CH0_DEST_WORD_SIZE_8     | \
                                     DMA_CH0_SRC_WORD_SIZE_8      | \
                                     DMA_CH0_PRIORITY_3           | \
                                     DMA_CH0_TRANSFER_M_TO_M      | \
                                     DMA_CH0_DEST_ADDR_STATIC     | \
                                     DMA_CH0_SRC_ADDR_STATIC      | \
                                     DMA_CH0_ADDR_LIN             | \
                                     DMA_CH0_DISABLE)
#define DMA_CH0_TRANSFER_LENGTH     (0)
#define DMA_CH0_COUNTER_CFG         (0)
#define DMA_CH0_SRC_ADDRESS         (0)
#define DMA_CH0_DEST_ADDRESS        (0)

/* DMA Channel 1 configuration - Disable the DMA and related components */
#define DMA_CH1_CFG                 (DMA_CH1_LITTLE_ENDIAN        | \
                                     DMA_CH1_DISABLE_INT_DISABLE  | \
                                     DMA_CH1_ERROR_INT_DISABLE    | \
                                     DMA_CH1_COMPLETE_INT_DISABLE | \
                                     DMA_CH1_COUNTER_INT_DISABLE  | \
                                     DMA_CH1_START_INT_DISABLE    | \
                                     DMA_CH1_DEST_WORD_SIZE_8     | \
                                     DMA_CH1_SRC_WORD_SIZE_8      | \
                                     DMA_CH1_PRIORITY_3           | \
                                     DMA_CH1_TRANSFER_M_TO_M      | \
                                     DMA_CH1_DEST_ADDR_STATIC     | \
                                     DMA_CH1_SRC_ADDR_STATIC      | \
                                     DMA_CH1_ADDR_LIN             | \
                                     DMA_CH1_DISABLE)
#define DMA_CH1_TRANSFER_LENGTH     (0)
#define DMA_CH1_COUNTER_CFG         (0)
#define DMA_CH1_SRC_ADDRESS         (0)
#define DMA_CH1_DEST_ADDRESS        (0)

/* DMA Channel 2 configuration - Disable the DMA and related components */
#define DMA_CH2_CFG                 (DMA_CH2_LITTLE_ENDIAN        | \
                                     DMA_CH2_DISABLE_INT_DISABLE  | \
                                     DMA_CH2_ERROR_INT_DISABLE    | \
                                     DMA_CH2_COMPLETE_INT_DISABLE | \
                                     DMA_CH2_COUNTER_INT_DISABLE  | \
                                     DMA_CH2_START_INT_DISABLE    | \
                                     DMA_CH2_DEST_WORD_SIZE_8     | \
                                     DMA_CH2_SRC_WORD_SIZE_8      | \
                                     DMA_CH2_PRIORITY_3           | \
                                     DMA_CH2_TRANSFER_M_TO_M      | \
                                     DMA_CH2_DEST_ADDR_STATIC     | \
                                     DMA_CH2_SRC_ADDR_STATIC      | \
                                     DMA_CH2_ADDR_LIN             | \
                                     DMA_CH2_DISABLE)
#define DMA_CH2_TRANSFER_LENGTH     (0)
#define DMA_CH2_COUNTER_CFG         (0)
#define DMA_CH2_SRC_ADDRESS         (0)
#define DMA_CH2_DEST_ADDRESS        (0)

/* DMA Channel 3 configuration - Disable the DMA and related components */
#define DMA_CH3_CFG                 (DMA_CH3_LITTLE_ENDIAN        | \
                                     DMA_CH3_DISABLE_INT_DISABLE  | \
                                     DMA_CH3_ERROR_INT_DISABLE    | \
                                     DMA_CH3_COMPLETE_INT_DISABLE | \
                                     DMA_CH3_COUNTER_INT_DISABLE  | \
                                     DMA_CH3_START_INT_DISABLE    | \
                                     DMA_CH3_DEST_WORD_SIZE_8     | \
                                     DMA_CH3_SRC_WORD_SIZE_8      | \
                                     DMA_CH3_PRIORITY_3           | \
                                     DMA_CH3_TRANSFER_M_TO_M      | \
                                     DMA_CH3_DEST_ADDR_STATIC     | \
                                     DMA_CH3_SRC_ADDR_STATIC      | \
                                     DMA_CH3_ADDR_LIN             | \
                                     DMA_CH3_DISABLE)
#define DMA_CH3_TRANSFER_LENGTH     (0)
#define DMA_CH3_COUNTER_CFG         (0)
#define DMA_CH3_SRC_ADDRESS         (0)
#define DMA_CH3_DEST_ADDRESS        (0)

/* ----------------------------------------------------------------------------
 * Clock Divisor Configuration
 * ------------------------------------------------------------------------- */

/* User Clock configuration - disable USR_CLK0 */
#define USR_CLK_CFG                 (USR_CLK0_DIV_32  | \
                                     USR_CLK0_DISABLE | \
                                     USR_CLK0_DIV2_ENABLE)

/* MCLK clock divisor to 2 and enable MCLK */
#define MCLK_CLK_DIV_CFG            (MCLK_CLK_DIV_2_BYTE | MCLK_CLK_ENABLE_BYTE)

/* ----------------------------------------------------------------------------
 * NVIC Configuration
 * ------------------------------------------------------------------------- */

/* Default base priority configuration - configure so that no interrupts
 * are masked by priority level */
#define NVIC_BASE_PRIORITY_CFG      (0)

/* Priority bit grouping configuration - configure so that all bits are assigned
 * to choosing preemptable priority levels */
#define NVIC_PRIORITY_GROUP_CFG     NVIC_PRIGROUP_4_0

/* ----------------------------------------------------------------------------
 * SYSTICK Timer Configuration
 * ------------------------------------------------------------------------- */

/* SYSTICK timer clock source configuration - select core (i.e. Cortex-M3)
 * clock */
#define SYSTICK_CLKSOURCE_CFG       NVIC_CLKSOURCE_CORE_CLK

/* Set SYSTICK timer counter for 1 second, asssuming a system clock rate of
 * approximately 3.0 MHz */
#define SYSTICK_RELOAD_COUNT        (3000000)

/* ----------------------------------------------------------------------------
 * General Purpose Timer Configuration
 * ------------------------------------------------------------------------- */

/* TIMER0 config - configure for fixed-count run mode and prescaler of 1 */
#define TIMER0_CTRL_CFG     (TIMER0_COUNT_1         | \
                             TIMER0_FIXED_COUNT_RUN | \
                             TIMER0_PRESCALE_1)

/* TIMER1 config - configure for fixed-count run mode and prescaler of 1 */
#define TIMER1_CTRL_CFG     (TIMER1_COUNT_1         | \
                             TIMER1_FIXED_COUNT_RUN | \
                             TIMER1_PRESCALE_1)

/* TIMER2 config - configure for fixed-count run mode and prescaler of 1 */
#define TIMER2_CTRL_CFG     (TIMER2_COUNT_1         | \
                             TIMER2_FIXED_COUNT_RUN | \
                             TIMER2_PRESCALE_1)

/* TIMER3 config - configure for fixed-count run mode and prescaler of 1 */
#define TIMER3_CTRL_CFG     (TIMER3_COUNT_1         | \
                             TIMER3_FIXED_COUNT_RUN | \
                             TIMER3_PRESCALE_1)

/* TIMERS_STATUS config - configure TIMER0 to start and TIMER1 to stop */
#define TIMERS_STATUS_CFG   (TIMER0_STOP  | \
                             TIMER1_STOP  | \
                             TIMER2_STOP  | \
                             TIMER3_STOP)

/* ----------------------------------------------------------------------------
 * Watchdog Timer Configuration
 * ------------------------------------------------------------------------- */

/* Watchdog timer - configure for the maximum timeout setting */
#define WATCHDOG_TIMEOUT_CFG    WATCHDOG_TIMEOUT_2E22

/* ----------------------------------------------------------------------------
 * GPIO Configuration
 * ------------------------------------------------------------------------- */

/* GPIO interface 0 configuration */
#define GPIO_IF0_FUNCTION_CFG       (GPIO_IF0_PIN3_PULLUP_DISABLE   | \
                                     GPIO_IF0_PIN2_PULLUP_DISABLE   | \
                                     GPIO_IF0_PIN1_PULLUP_DISABLE   | \
                                     GPIO_IF0_PIN0_PULLUP_DISABLE   | \
                                     GPIO_IF0_PIN3_OUTPUT_DISABLE   | \
                                     GPIO_IF0_PIN2_OUTPUT_DISABLE   | \
                                     GPIO_IF0_PIN1_OUTPUT_DISABLE   | \
                                     GPIO_IF0_PIN0_OUTPUT_DISABLE   | \
                                     GPIO_IF0_SELECT_GPIO)

/* GPIO interface 1 configuration */
#define GPIO_IF1_FUNCTION_CFG       (GPIO_IF1_PIN3_PULLUP_DISABLE   | \
                                     GPIO_IF1_PIN2_PULLUP_DISABLE   | \
                                     GPIO_IF1_PIN1_PULLUP_DISABLE   | \
                                     GPIO_IF1_PIN0_PULLUP_DISABLE   | \
                                     GPIO_IF1_PIN3_OUTPUT_DISABLE   | \
                                     GPIO_IF1_PIN2_OUTPUT_DISABLE   | \
                                     GPIO_IF1_PIN1_OUTPUT_DISABLE   | \
                                     GPIO_IF1_PIN0_OUTPUT_DISABLE   | \
                                     GPIO_IF1_SELECT_GPIO)

/* GPIO interface 2, 3 configuration */
#define GPIO_IF23_FUNCTION_CFG      (GPIO_IF3_PIN1_PULLUP_DISABLE   | \
                                     GPIO_IF3_PIN0_PULLUP_DISABLE   | \
                                     GPIO_IF2_PIN1_PULLUP_DISABLE   | \
                                     GPIO_IF2_PIN0_PULLUP_DISABLE   | \
                                     GPIO_IF3_PIN1_OUTPUT_DISABLE   | \
                                     GPIO_IF3_PIN0_OUTPUT_DISABLE   | \
                                     GPIO_IF2_PIN1_OUTPUT_DISABLE   | \
                                     GPIO_IF2_PIN0_OUTPUT_DISABLE   | \
                                     GPIO_IF3_SELECT_GPIO           | \
                                     GPIO_IF2_SELECT_GPIO)

/* Function config - not currently used (included for future compatibility) */
#define GPIO_IF4_FUNCTION_CFG       GPIO_IF4_ALL_GPIO

/* Output enable config - enable all GPIOs as outputs */
#define GPIO_IF4_OUTPUT_ENABLE_CFG  GPIO_IF4_OUTPUT_ENABLE_ALL

/* Pull-down configuration - disable all pulldowns */
#define GPIO_IF4_PULLDOWN_CFG       GPIO_IF4_PULLDOWN_DISABLE_ALL

/* GPIO interface 5 configuration */
#define GPIO_IF5_FUNCTION_CFG       (GPIO_IF5_PIN3_PULLDOWN_DISABLE | \
                                     GPIO_IF5_PIN2_PULLDOWN_DISABLE | \
                                     GPIO_IF5_PIN1_PULLUP_DISABLE   | \
                                     GPIO_IF5_PIN0_PULLUP_DISABLE   | \
                                     GPIO_IF5_PIN3_OUTPUT_DISABLE   | \
                                     GPIO_IF5_PIN2_OUTPUT_DISABLE   | \
                                     GPIO_IF5_PIN1_OUTPUT_DISABLE   | \
                                     GPIO_IF5_PIN0_OUTPUT_DISABLE   | \
                                     GPIO_IF5_PIN3_WAKEUP_DISABLE   | \
                                     GPIO_IF5_PIN2_WAKEUP_DISABLE   | \
                                     GPIO_IF5_PIN1_WAKEUP_DISABLE   | \
                                     GPIO_IF5_PIN0_WAKEUP_DISABLE)

/* GPIO Input control configuration - disable GPIO inputs */
#define GPIO_INPUT_CTRL_CFG         (GPIO_IF0_IN_DISABLE | \
                                     GPIO_IF1_IN_DISABLE | \
                                     GPIO_IF2_IN_DISABLE | \
                                     GPIO_IF3_IN_DISABLE | \
                                     GPIO_IF4_IN_DISABLE | \
                                     GPIO_IF5_IN_DISABLE)

/* Interrupt control configuration - disable all GPIO interrupt triggering */
#define GPIO_INT_CTRL_GP0_CFG       (GPIO_GP0_INT_DISABLE_SHORT)
#define GPIO_INT_CTRL_GP1_CFG       (GPIO_GP1_INT_DISABLE_SHORT)
#define GPIO_INT_CTRL_GP2_CFG       (GPIO_GP2_INT_DISABLE_SHORT)
#define GPIO_INT_CTRL_GP3_CFG       (GPIO_GP3_INT_DISABLE_SHORT)
#define GPIO_INT_CTRL_GP4_CFG       (GPIO_GP4_INT_DISABLE_SHORT)
#define GPIO_INT_CTRL_GP5_CFG       (GPIO_GP5_INT_DISABLE_SHORT)
#define GPIO_INT_CTRL_GP6_CFG       (GPIO_GP6_INT_DISABLE_SHORT)
#define GPIO_INT_CTRL_GP7_CFG       (GPIO_GP7_INT_DISABLE_SHORT)

/* ----------------------------------------------------------------------------
 * PCM Configuration
 * ------------------------------------------------------------------------- */

/* PCM configuration - no DMA, disable pullups, master mode, and disabled with
 * a basic PCM signal and frame configuration */
#define PCM_CFG             (PCM_PULLUP_DISABLE      | \
                             PCM_BIT_ORDER_MSB_FIRST | \
                             PCM_TX_ALIGN_MSB        | \
                             PCM_WORD_SIZE_8         | \
                             PCM_FRAME_ALIGN_LAST    | \
                             PCM_FRAME_WIDTH_SHORT   | \
                             PCM_FRAME_LENGTH_2      | \
                             PCM_SUBFRAMES_DISABLE   | \
                             PCM_CONTROLLER_CM3      | \
                             PCM_DISABLE             | \
                             PCM_SELECT_MASTER)

/* PCM reset configuration - select CM3 as controller */
#define PCM_CFG_RESET       (PCM_PULLUP_DISABLE      | \
                             PCM_BIT_ORDER_LSB_FIRST | \
                             PCM_TX_ALIGN_LSB        | \
                             PCM_WORD_SIZE_32        | \
                             PCM_FRAME_ALIGN_LAST    | \
                             PCM_FRAME_WIDTH_LONG    | \
                             PCM_FRAME_LENGTH_2      | \
                             PCM_SUBFRAMES_DISABLE   | \
                             PCM_CONTROLLER_CM3      | \
                             PCM_ENABLE              | \
                             PCM_SELECT_MASTER)


/* ----------------------------------------------------------------------------
 * SPI Configuration
 * ------------------------------------------------------------------------- */

/* SPI0 configuration - no DMA, disable pullups, normal clock polarity,
 * manual mode, disabled and a prescaler of 4 */
#define SPI0_CFG0           (SPI_OVERRUN_INT_DISABLE  | \
                             SPI_UNDERRUN_INT_DISABLE | \
                             SPI_CONTROLLER_CM3       | \
                             SPI_SERI_PULLUP_DISABLE  | \
                             SPI_CLK_POLARITY_NORMAL  | \
                             SPI_MODE_SELECT_MANUAL   | \
                             SPI_DISABLE              | \
                             SPI_PRESCALE_4)

#define SPI0_CFG1           (SPI_IDLE | SPI_WRITE_DATA | \
                             SPI_CS_1 | SPI_WORD_SIZE_16)

/* SPI1 configuration - no DMA, disable pullups, normal clock polarity,
 * manual mode, disabled and a prescaler of 4 */
#define SPI1_CFG0           (SPI_OVERRUN_INT_DISABLE  | \
                             SPI_UNDERRUN_INT_DISABLE | \
                             SPI_CONTROLLER_CM3       | \
                             SPI_SERI_PULLUP_DISABLE  | \
                             SPI_CLK_POLARITY_NORMAL  | \
                             SPI_MODE_SELECT_MANUAL   | \
                             SPI_DISABLE              | \
                             SPI_PRESCALE_4)

#define SPI1_CFG1           (SPI_IDLE | SPI_WRITE_DATA | \
                             SPI_CS_1 | SPI_WORD_SIZE_16)

/* ----------------------------------------------------------------------------
 * Universal Asynchronous Receiver/Transmitter Configuration
 * ------------------------------------------------------------------------- */

/* UART0 configuration - no DMA, disable UART0, and disable the prescaler */
#define UART0_CTRL_CFG              (UART_CONTROLLER_CM3       | \
                                     UART_DISABLE              | \
                                     UART_PRESCALE_DISABLE)

/* UART0 default speed configuration */
#define UART0_SPEED_DEFAULT         (0)

/* Set UART 0 clock divisor - divisor set to 1 */
#define UART0_CLK_DIV_CFG           UART0_CLK_DIV_1_BYTE

/* UART1 configuration - no DMA, disable UART1, and disable the prescaler */
#define UART1_CTRL_CFG              (UART_CONTROLLER_CM3       | \
                                     UART_DISABLE              | \
                                     UART_PRESCALE_DISABLE)

/* UART1 default speed configuration */
#define UART1_SPEED_DEFAULT         (0)

/* Set UART 1 clock divisor - divisor set to 1 */
#define UART1_CLK_DIV_CFG           UART1_CLK_DIV_1_BYTE


/* The default system clock speed is 3000000 hertz or 3.0 MHz */
#define UART_SYSCLOCK_SPEED_DEFAULT (3000000)

/* ----------------------------------------------------------------------------
 * USB Controller Configuration
 * ------------------------------------------------------------------------- */

#define USB_CFG                     (USB_RESET_ENABLE | USB_DISABLE)

/* ----------------------------------------------------------------------------
 * I2C Configuration
 * ------------------------------------------------------------------------- */

/* I2C configuration - slave mode, DMA controled, stop interrupt enabled,
 * auto-ACK enabled and a slave address of "0x11" */
#define I2C_CTRL_CFG                ((0x11 << I2C_CTRL0_SLAVE_ADDRESS_Pos) | \
                                     I2C_CONTROLLER_CM3                    | \
                                     I2C_STOP_INT_ENABLE                   | \
                                     I2C_AUTO_ACK_ENABLE                   | \
                                     I2C_SLAVE_ENABLE)

/* I2C physical configuration */
#define I2C_PHYSICAL_CTRL_CFG       (I2C_PULLUP_DISABLE | \
                                     I2C_FILTER_DISABLE)

/* ----------------------------------------------------------------------------
 * I2C Clock Configuration
 * ------------------------------------------------------------------------- */

/* Enable the I2C Clock */
#define I2C_CLK_CFG                 (I2C_CLK_ENABLE_BYTE)

/* ----------------------------------------------------------------------------
 * Prototypes for global functions
 * ------------------------------------------------------------------------- */
extern void initialize(void);
extern void Clear_Pending_Interrupts(void);
extern void Enable_Individual_Interrupts(void);

/* ----------------------------------------------------------------------------
 * Global Variables
 * ------------------------------------------------------------------------- */

/* State constants stored in I2C_CONTROL_STATE */
#define I2C_CONTROL_STATE_GET_CMD       0
#define I2C_CONTROL_STATE_GET_DATA      1
#define I2C_CONTROL_STATE_SEND_DATA     2

/* Maximum RX/TX buffer sizes */
#define MAX_RX_BUFFER_SIZE             8
#define MAX_TX_BUFFER_SIZE             8

/* Command numbers */
#define I2C_CMD2                       2
#define I2C_CMD3                       3
#define I2C_CMD4                       4
#define I2C_CMD5                       5
#define I2C_CMD6                       6

/* Internal state variables */
extern unsigned int I2C_Control_State;
extern unsigned int I2C_Control_Cmd_Byte;
extern unsigned int I2C_Control_Num_RX_Bytes;
extern unsigned int I2C_Control_Num_TX_Bytes;
extern unsigned char I2C_RX_buffer[MAX_RX_BUFFER_SIZE];
extern unsigned char I2C_TX_buffer[MAX_TX_BUFFER_SIZE];
extern unsigned int I2C_RX_buffer_index;
extern unsigned int I2C_TX_buffer_index;

/* ----------------------------------------------------------------------------
 * Global Functions
 * ------------------------------------------------------------------------- */

/* Support functions for I2C interrupt service routine */
void I2C_Control_Process_Cmd_Byte(void);
void I2C_Control_Call_Command_Handler(unsigned int cmd);
void Error_Handler(void);
void I2C_Control_Initialize(void);
void Cleanup(void);

/* Command Handlers */
void Clear_LEDs_Command_Handler(void);
void Turn_On_All_LEDs_Command_Handler(void);
void Set_LEDs_Command_Handler(void);
void Get_LEDs_Command_Handler(void);
void Get_and_Set_LEDs_Command_Handler(void);
void Default_Command_Handler(void);

#endif /* GLOBALS_H */
