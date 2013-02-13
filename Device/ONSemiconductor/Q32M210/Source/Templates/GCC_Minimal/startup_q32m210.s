/* ----------------------------------------------------------------------------
 * Copyright (c) 2012 Semiconductor Components Industries, LLC (d/b/a        
 * ON Semiconductor), All Rights Reserved
 * 
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor.
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * startup_q32m210.s
 * - GCC compatible CMSIS Cortex-M3 Core Device start-up file for the 
 *   Q32M210.
 * ----------------------------------------------------------------------------
 * $Revision: 1.3 $
 * $Date: 2012/04/26 18:50:01 $
 * ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 * Vector Table 
 * - Mapped to Address 0 at Reset
 * ------------------------------------------------------------------------- */

    .section ".vectors"
    .globl  __vectors_Q32M210
    .type   __vectors_Q32M210, %object

__vectors_Q32M210:
    .long   _estack                 	/* Top of Stack                  */
    .long   Reset_Handler               /* Reset Handler                 */
    .long   NMI_Handler                 /* NMI Handler                   */
    .long   HardFault_Handler           /* Hard Fault Handler            */
    .long   MemManage_Handler           /* MPU Fault Handler             */
    .long   BusFault_Handler            /* Bus Fault Handler             */
    .long   UsageFault_Handler          /* Usage Fault Handler           */
    .long   0                           /* Reserved                      */
    .long   0                           /* Reserved                      */
    .long   0                           /* Reserved                      */
    .long   0                           /* Reserved                      */
    .long   SVC_Handler                 /* SVCall Handler                */
    .long   DebugMon_Handler            /* Debug Monitor Handler         */
    .long   0                           /* Reserved                      */
    .long   PendSV_Handler              /* PendSV Handler                */
    .long   SysTick_Handler             /* SysTick Handler               */

    /* External Interrupts */
    .long   WAKEUP_IRQHandler           /* 16+ 0: Wakeup*/
    .long   WAKEUP_IF5_PIN0_IRQHandler  /* 16+ 1: IF5, Pin 0 Specific Wakeup*/
    .long   RTC_ALARM_IRQHandler        /* 16+ 2: Real-Time Clock (RTC) Alarm*/
    .long   RTC_CLOCK_IRQHandler        /* 16+ 3: Real-Time Clock (RTC) Clock*/
    .long   THRESHOLD_COMPARE_IRQHandler/* 16+ 4: Threshold Compare*/
    .long   NO_EXTCLK_DETECT_IRQHandler /* 16+ 5: No EXTCLK Detected*/
    .long   GPIO_GP0_IRQHandler         /* 16+ 6: GPIO 0*/
    .long   GPIO_GP1_IRQHandler         /* 16+ 7: GPIO 1*/
    .long   WATCHDOG_IRQHandler         /* 16+ 8: Watchdog*/
    .long   TIMER0_IRQHandler           /* 16+ 9: Timer 0*/
    .long   TIMER1_IRQHandler           /* 16+10: Timer 1*/
    .long   TIMER2_IRQHandler           /* 16+11: Timer 2*/
    .long   TIMER3_IRQHandler           /* 16+12: Timer 3*/
    .long   UART0_RX_IRQHandler         /* 16+13: UART 0 Receive*/
    .long   UART0_TX_IRQHandler         /* 16+14: UART 0 Transmit*/
    .long   UART0_ERROR_IRQHandler      /* 16+15: UART 0 Error*/
    .long   UART1_RX_IRQHandler         /* 16+16: UART 1 Receive*/
    .long   UART1_TX_IRQHandler         /* 16+17: UART 1 Transmit*/
    .long   UART1_ERROR_IRQHandler      /* 16+18: UART 1 Error*/
    .long   I2C_IRQHandler              /* 16+19: I2C*/
    .long   SPI0_IRQHandler             /* 16+20: SPI 0*/
    .long   SPI0_ERROR_IRQHandler       /* 16+21: SPI 0 Error*/
    .long   SPI1_IRQHandler             /* 16+22: SPI 1*/
    .long   SPI1_ERROR_IRQHandler       /* 16+23: SPI 1 Error*/
    .long   PCM_RX_IRQHandler           /* 16+24: PCM Receive*/
    .long   PCM_TX_IRQHandler           /* 16+25: PCM Transmit*/
    .long   PCM_ERROR_IRQHandler        /* 16+26: PCM Error*/
    .long   ADC_IRQHandler              /* 16+27: ADC*/
    .long   DMA0_IRQHandler             /* 16+28: DMA Channel 0*/
    .long   DMA1_IRQHandler             /* 16+29: DMA Channel 1*/
    .long   DMA2_IRQHandler             /* 16+30: DMA Channel 2*/
    .long   DMA3_IRQHandler             /* 16+31: DMA Channel 3*/
    .long   USB_WAKEUP_IRQHandler       /* 16+32: USB Wakeup*/
    .long   USB_SDAV_IRQHandler         /* 16+33: USB Setup Data Available*/
    .long   USB_SOF_IRQHandler          /* 16+34: USB Start Of Frame*/
    .long   USB_SUTOK_IRQHandler        /* 16+35: USB Setup Data Loading*/
    .long   USB_SUSPEND_IRQHandler      /* 16+36: USB Suspend*/
    .long   USB_RESET_IRQHandler        /* 16+37: USB Reset*/
    .long   USB_EP0IN_IRQHandler        /* 16+38: USB EP0IN*/
    .long   USB_EP0OUT_IRQHandler       /* 16+39: USB EP0OUT*/
    .long   USB_EP2IN_IRQHandler        /* 16+40: USB EP2IN*/
    .long   0                           /* Reserved */
    .long   USB_EP3IN_IRQHandler        /* 16+42: USB EP3IN*/
    .long   0                           /* Reserved */
    .long   0                           /* Reserved */
    .long   USB_EP4OUT_IRQHandler       /* 16+45: USB EP4OUT*/
    .long   0                           /* Reserved */
    .long   USB_EP5OUT_IRQHandler       /* 16+47: USB EP5OUT*/
    .long   ECC_CORRECTED_IRQHandler    /* 16+48: ECC Error Corrected*/
    .long   GPIO_GP2_IRQHandler         /* 16+49: GPIO 2*/
    .long   GPIO_GP3_IRQHandler         /* 16+50: GPIO 3*/
    .long   GPIO_GP4_IRQHandler         /* 16+51: GPIO 4*/
    .long   GPIO_GP5_IRQHandler         /* 16+52: GPIO 5*/
    .long   GPIO_GP6_IRQHandler         /* 16+53: GPIO 6*/
    .long   GPIO_GP7_IRQHandler         /* 16+54: GPIO 7*/

    .size   __vectors_Q32M210, . - __vectors_Q32M210

    .thumb

/* ----------------------------------------------------------------------------
 * Reset Handler
 * ------------------------------------------------------------------------- */

    .section .reset,"ax",%progbits
    .thumb_func
    .globl  Reset_Handler
    .type   Reset_Handler, %function
Reset_Handler:
    .fnstart
    LDR     R0, =SystemInit
    BLX     R0
    LDR     R0,=_start
    BX      R0
    .pool
    .cantunwind
    .fnend
    .size   Reset_Handler, . - Reset_Handler

    .section ".text"

/* ----------------------------------------------------------------------------
 * Place-holder Exception Handlers
 * - Weak definitions
 * - If not modified or replaced, these handlers implement infinite loops
 * ------------------------------------------------------------------------- */

    .weak   NMI_Handler
    .type   NMI_Handler, %function
NMI_Handler:
    B       .
    .size   NMI_Handler, . - NMI_Handler

    .weak   HardFault_Handler
    .type   HardFault_Handler, %function
HardFault_Handler:
    B       .
    .size   HardFault_Handler, . - HardFault_Handler

    .weak   MemManage_Handler
    .type   MemManage_Handler, %function
MemManage_Handler:
    B       .
    .size   MemManage_Handler, . - MemManage_Handler

    .weak   BusFault_Handler
    .type   BusFault_Handler, %function
BusFault_Handler:
    B       .
    .size   BusFault_Handler, . - BusFault_Handler

    .weak   UsageFault_Handler
    .type   UsageFault_Handler, %function
UsageFault_Handler:
    B       .
    .size   UsageFault_Handler, . - UsageFault_Handler

    .weak   SVC_Handler
    .type   SVC_Handler, %function
SVC_Handler:
    B       .
    .size   SVC_Handler, . - SVC_Handler

    .weak   DebugMon_Handler
    .type   DebugMon_Handler, %function
DebugMon_Handler:
    B       .
    .size   DebugMon_Handler, . - DebugMon_Handler

    .weak   PendSV_Handler
    .type   PendSV_Handler, %function
PendSV_Handler:
    B       .
    .size   PendSV_Handler, . - PendSV_Handler

    .weak   SysTick_Handler
    .type   SysTick_Handler, %function
SysTick_Handler:
    B       .
    .size   SysTick_Handler, . - SysTick_Handler

/* ----------------------------------------------------------------------------
 * Default Interrupt Request Handlers for External Interrupts
 * ------------------------------------------------------------------------- */

    .globl  Default_Handler
    .type   Default_Handler, %function
Default_Handler:
    B       .
    .size   Default_Handler, . - Default_Handler

    .macro  IRQ handler
    .weak   \handler
    .set    \handler, Default_Handler
    .endm

    IRQ     WAKEUP_IRQHandler
    IRQ     WAKEUP_IF5_PIN0_IRQHandler
    IRQ     RTC_ALARM_IRQHandler
    IRQ     RTC_CLOCK_IRQHandler
    IRQ     THRESHOLD_COMPARE_IRQHandler
    IRQ     NO_EXTCLK_DETECT_IRQHandler
    IRQ     GPIO_GP0_IRQHandler
    IRQ     GPIO_GP1_IRQHandler
    IRQ     WATCHDOG_IRQHandler
    IRQ     TIMER0_IRQHandler
    IRQ     TIMER1_IRQHandler
    IRQ     TIMER2_IRQHandler
    IRQ     TIMER3_IRQHandler
    IRQ     UART0_RX_IRQHandler
    IRQ     UART0_TX_IRQHandler
    IRQ     UART0_ERROR_IRQHandler
    IRQ     UART1_RX_IRQHandler
    IRQ     UART1_TX_IRQHandler
    IRQ     UART1_ERROR_IRQHandler
    IRQ     I2C_IRQHandler
    IRQ     SPI0_IRQHandler
    IRQ     SPI0_ERROR_IRQHandler
    IRQ     SPI1_IRQHandler
    IRQ     SPI1_ERROR_IRQHandler
    IRQ     PCM_RX_IRQHandler
    IRQ     PCM_TX_IRQHandler
    IRQ     PCM_ERROR_IRQHandler
    IRQ     ADC_IRQHandler
    IRQ     DMA0_IRQHandler
    IRQ     DMA1_IRQHandler
    IRQ     DMA2_IRQHandler
    IRQ     DMA3_IRQHandler
    IRQ     USB_WAKEUP_IRQHandler
    IRQ     USB_SDAV_IRQHandler
    IRQ     USB_SOF_IRQHandler
    IRQ     USB_SUTOK_IRQHandler
    IRQ     USB_SUSPEND_IRQHandler
    IRQ     USB_RESET_IRQHandler
    IRQ     USB_EP0IN_IRQHandler
    IRQ     USB_EP0OUT_IRQHandler
    IRQ     USB_EP2IN_IRQHandler
    IRQ     USB_EP3IN_IRQHandler
    IRQ     USB_EP4OUT_IRQHandler
    IRQ     USB_EP5OUT_IRQHandler
    IRQ     ECC_CORRECTED_IRQHandler
    IRQ     GPIO_GP2_IRQHandler
    IRQ     GPIO_GP3_IRQHandler
    IRQ     GPIO_GP4_IRQHandler
    IRQ     GPIO_GP5_IRQHandler
    IRQ     GPIO_GP6_IRQHandler
    IRQ     GPIO_GP7_IRQHandler

    .end
