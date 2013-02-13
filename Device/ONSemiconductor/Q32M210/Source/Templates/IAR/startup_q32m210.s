;/*****************************************************************************
; * @file:    startup_Q32M210.s
; * @purpose: CMSIS Cortex-M3 Core Device Startup File  
; *           for the ON Semiconductor Q32M210
; * @version  $Revision: 1.3 $
; * @date     $Date: 2012/04/26 18:50:01 $
; *
; * @note
; * Copyright (c) 2012 Semiconductor Components Industries, LLC (d/b/a
; * ON Semiconductor), All Rights Reserved
; *
; * @par
; * This code is the property of ON Semiconductor and may not be redistributed
; * in any form without prior written permission from ON Semiconductor.
; * The terms of use and warranty for this code are covered by contractual
; * agreements between ON Semiconductor and the licensee.
; *
; ****************************************************************************/
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)
	
        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table
        PUBLIC  __vector_table_0x1c
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA

__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler

        DCD     NMI_Handler
        DCD     HardFault_Handler
        DCD     MemManage_Handler
        DCD     BusFault_Handler
        DCD     UsageFault_Handler
__vector_table_0x1c
        DCD     0
        DCD     0
        DCD     0
        DCD     0
        DCD     SVC_Handler
        DCD     DebugMon_Handler
        DCD     0
        DCD     PendSV_Handler
        DCD     SysTick_Handler

        ; External Interrupts
        DCD     WAKEUP_IRQHandler            ; 16+ 0 : Wakeup
        DCD     WAKEUP_IF5_PIN0_IRQHandler   ; 16+ 1 : IF5, Pin 0 Specific Wakeup
        DCD     RTC_ALARM_IRQHandler         ; 16+ 2 : Real-Time Clock (RTC) Alarm
        DCD     RTC_CLOCK_IRQHandler         ; 16+ 3 : Real-Time Clock (RTC) Clock
        DCD     THRESHOLD_COMPARE_IRQHandler ; 16+ 4 : Threshold Compare
        DCD     NO_EXTCLK_DETECT_IRQHandler  ; 16+ 5 : No EXTCLK Detected
        DCD     GPIO_GP0_IRQHandler          ; 16+ 6 : GPIO 0
        DCD     GPIO_GP1_IRQHandler          ; 16+ 7 : GPIO 1
        DCD     WATCHDOG_IRQHandler          ; 16+ 8 : Watchdog
        DCD     TIMER0_IRQHandler            ; 16+ 9 : Timer 0
        DCD     TIMER1_IRQHandler            ; 16+ 10: Timer 1
        DCD     TIMER2_IRQHandler            ; 16+ 11: Timer 2
        DCD     TIMER3_IRQHandler            ; 16+ 12: Timer 3
        DCD     UART0_RX_IRQHandler          ; 16+ 13: UART 0 Receive
        DCD     UART0_TX_IRQHandler          ; 16+ 14: UART 0 Transmit
        DCD     UART0_ERROR_IRQHandler       ; 16+ 15: UART 0 Error
        DCD     UART1_RX_IRQHandler          ; 16+ 16: UART 1 Receive
        DCD     UART1_TX_IRQHandler          ; 16+ 17: UART 1 Transmit
        DCD     UART1_ERROR_IRQHandler       ; 16+ 18: UART 1 Error
        DCD     I2C_IRQHandler               ; 16+ 19: I2C
        DCD     SPI0_IRQHandler              ; 16+ 20: SPI 0
        DCD     SPI0_ERROR_IRQHandler        ; 16+ 21: SPI 0 Error
        DCD     SPI1_IRQHandler              ; 16+ 22: SPI 1
        DCD     SPI1_ERROR_IRQHandler        ; 16+ 23: SPI 1 Error
        DCD     PCM_RX_IRQHandler            ; 16+ 24: PCM Receive
        DCD     PCM_TX_IRQHandler            ; 16+ 25: PCM Transmit
        DCD     PCM_ERROR_IRQHandler         ; 16+ 26: PCM Error
        DCD     ADC_IRQHandler               ; 16+ 27: ADC
        DCD     DMA0_IRQHandler              ; 16+ 28: DMA Channel 0
        DCD     DMA1_IRQHandler              ; 16+ 29: DMA Channel 1
        DCD     DMA2_IRQHandler              ; 16+ 30: DMA Channel 2
        DCD     DMA3_IRQHandler              ; 16+ 31: DMA Channel 3
        DCD     USB_WAKEUP_IRQHandler        ; 16+ 32: USB Wakeup
        DCD     USB_SDAV_IRQHandler          ; 16+ 33: USB Setup Data Available
        DCD     USB_SOF_IRQHandler           ; 16+ 34: USB Start Of Frame
        DCD     USB_SUTOK_IRQHandler         ; 16+ 35: USB Setup Data Loading
        DCD     USB_SUSPEND_IRQHandler       ; 16+ 36: USB Suspend
        DCD     USB_RESET_IRQHandler         ; 16+ 37: USB Reset
        DCD     USB_EP0IN_IRQHandler         ; 16+ 38: USB EP0IN
        DCD     USB_EP0OUT_IRQHandler        ; 16+ 39: USB EP0OUT
        DCD     USB_EP2IN_IRQHandler         ; 16+ 40: USB EP2IN
        DCD     0
        DCD     USB_EP3IN_IRQHandler         ; 16+ 42: USB EP3IN
        DCD     0
        DCD     0
        DCD     USB_EP4OUT_IRQHandler        ; 16+ 45: USB EP4OUT
        DCD     0
        DCD     USB_EP5OUT_IRQHandler        ; 16+ 47: USB EP5OUT
        DCD     ECC_CORRECTED_IRQHandler     ; 16+ 48: ECC Error Corrected
        DCD     GPIO_GP2_IRQHandler          ; 16+ 49: GPIO 2
        DCD     GPIO_GP3_IRQHandler          ; 16+ 50: GPIO 3
        DCD     GPIO_GP4_IRQHandler          ; 16+ 51: GPIO 4
        DCD     GPIO_GP5_IRQHandler          ; 16+ 52: GPIO 5
        DCD     GPIO_GP6_IRQHandler          ; 16+ 53: GPIO 6
        DCD     GPIO_GP7_IRQHandler          ; 16+ 54: GPIO 7

__Vectors_End

__Vectors       EQU   __vector_table
__Vectors_Size 	EQU 	__Vectors_End - __Vectors


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .startup:CODE:REORDER(2)
Reset_Handler
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER(1)
NMI_Handler
        B NMI_Handler

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER(1)
HardFault_Handler
        B HardFault_Handler

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:REORDER(1)
MemManage_Handler
        B MemManage_Handler

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:REORDER(1)
BusFault_Handler
        B BusFault_Handler

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:REORDER(1)
UsageFault_Handler
        B UsageFault_Handler

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER(1)
SVC_Handler
        B SVC_Handler

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:REORDER(1)
DebugMon_Handler
        B DebugMon_Handler

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER(1)
PendSV_Handler
        B PendSV_Handler

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER(1)
SysTick_Handler
        B SysTick_Handler

	PUBWEAK WAKEUP_IRQHandler
	SECTION .text:CODE:REORDER(1)
WAKEUP_IRQHandler
	B WAKEUP_IRQHandler

	PUBWEAK WAKEUP_IF5_PIN0_IRQHandler
	SECTION .text:CODE:REORDER(1)
WAKEUP_IF5_PIN0_IRQHandler
	B WAKEUP_IF5_PIN0_IRQHandler

	PUBWEAK RTC_ALARM_IRQHandler
	SECTION .text:CODE:REORDER(1)
RTC_ALARM_IRQHandler
	B RTC_ALARM_IRQHandler

	PUBWEAK RTC_CLOCK_IRQHandler
	SECTION .text:CODE:REORDER(1)
RTC_CLOCK_IRQHandler
	B RTC_CLOCK_IRQHandler

	PUBWEAK THRESHOLD_COMPARE_IRQHandler
	SECTION .text:CODE:REORDER(1)
THRESHOLD_COMPARE_IRQHandler
	B THRESHOLD_COMPARE_IRQHandler

	PUBWEAK NO_EXTCLK_DETECT_IRQHandler
	SECTION .text:CODE:REORDER(1)
NO_EXTCLK_DETECT_IRQHandler
	B NO_EXTCLK_DETECT_IRQHandler

	PUBWEAK GPIO_GP0_IRQHandler
	SECTION .text:CODE:REORDER(1)
GPIO_GP0_IRQHandler
	B GPIO_GP0_IRQHandler

	PUBWEAK GPIO_GP1_IRQHandler
	SECTION .text:CODE:REORDER(1)
GPIO_GP1_IRQHandler
	B GPIO_GP1_IRQHandler

	PUBWEAK WATCHDOG_IRQHandler
	SECTION .text:CODE:REORDER(1)
WATCHDOG_IRQHandler
	B WATCHDOG_IRQHandler

	PUBWEAK TIMER0_IRQHandler
	SECTION .text:CODE:REORDER(1)
TIMER0_IRQHandler
	B TIMER0_IRQHandler

	PUBWEAK TIMER1_IRQHandler
	SECTION .text:CODE:REORDER(1)
TIMER1_IRQHandler
	B TIMER1_IRQHandler

	PUBWEAK TIMER2_IRQHandler
	SECTION .text:CODE:REORDER(1)
TIMER2_IRQHandler
	B TIMER2_IRQHandler

	PUBWEAK TIMER3_IRQHandler
	SECTION .text:CODE:REORDER(1)
TIMER3_IRQHandler
	B TIMER3_IRQHandler

	PUBWEAK UART0_RX_IRQHandler
	SECTION .text:CODE:REORDER(1)
UART0_RX_IRQHandler
	B UART0_RX_IRQHandler

	PUBWEAK UART0_TX_IRQHandler
	SECTION .text:CODE:REORDER(1)
UART0_TX_IRQHandler
	B UART0_TX_IRQHandler

	PUBWEAK UART0_ERROR_IRQHandler
	SECTION .text:CODE:REORDER(1)
UART0_ERROR_IRQHandler
	B UART0_ERROR_IRQHandler

	PUBWEAK UART1_RX_IRQHandler
	SECTION .text:CODE:REORDER(1)
UART1_RX_IRQHandler
	B UART1_RX_IRQHandler

	PUBWEAK UART1_TX_IRQHandler
	SECTION .text:CODE:REORDER(1)
UART1_TX_IRQHandler
	B UART1_TX_IRQHandler

	PUBWEAK UART1_ERROR_IRQHandler
	SECTION .text:CODE:REORDER(1)
UART1_ERROR_IRQHandler
	B UART1_ERROR_IRQHandler

	PUBWEAK I2C_IRQHandler
	SECTION .text:CODE:REORDER(1)
I2C_IRQHandler
	B I2C_IRQHandler

	PUBWEAK SPI0_IRQHandler
	SECTION .text:CODE:REORDER(1)
SPI0_IRQHandler
	B SPI0_IRQHandler

	PUBWEAK SPI0_ERROR_IRQHandler
	SECTION .text:CODE:REORDER(1)
SPI0_ERROR_IRQHandler
	B SPI0_ERROR_IRQHandler

	PUBWEAK SPI1_IRQHandler
	SECTION .text:CODE:REORDER(1)
SPI1_IRQHandler
	B SPI1_IRQHandler

	PUBWEAK SPI1_ERROR_IRQHandler
	SECTION .text:CODE:REORDER(1)
SPI1_ERROR_IRQHandler
	B SPI1_ERROR_IRQHandler

	PUBWEAK PCM_RX_IRQHandler
	SECTION .text:CODE:REORDER(1)
PCM_RX_IRQHandler
	B PCM_RX_IRQHandler

	PUBWEAK PCM_TX_IRQHandler
	SECTION .text:CODE:REORDER(1)
PCM_TX_IRQHandler
	B PCM_TX_IRQHandler

	PUBWEAK PCM_ERROR_IRQHandler
	SECTION .text:CODE:REORDER(1)
PCM_ERROR_IRQHandler
	B PCM_ERROR_IRQHandler

	PUBWEAK ADC_IRQHandler
	SECTION .text:CODE:REORDER(1)
ADC_IRQHandler
	B ADC_IRQHandler

	PUBWEAK DMA0_IRQHandler
	SECTION .text:CODE:REORDER(1)
DMA0_IRQHandler
	B DMA0_IRQHandler

	PUBWEAK DMA1_IRQHandler
	SECTION .text:CODE:REORDER(1)
DMA1_IRQHandler
	B DMA1_IRQHandler

	PUBWEAK DMA2_IRQHandler
	SECTION .text:CODE:REORDER(1)
DMA2_IRQHandler
	B DMA2_IRQHandler

	PUBWEAK DMA3_IRQHandler
	SECTION .text:CODE:REORDER(1)
DMA3_IRQHandler
	B DMA3_IRQHandler

	PUBWEAK USB_WAKEUP_IRQHandler
	SECTION .text:CODE:REORDER(1)
USB_WAKEUP_IRQHandler
	B USB_WAKEUP_IRQHandler

	PUBWEAK USB_SDAV_IRQHandler
	SECTION .text:CODE:REORDER(1)
USB_SDAV_IRQHandler
	B USB_SDAV_IRQHandler

	PUBWEAK USB_SOF_IRQHandler
	SECTION .text:CODE:REORDER(1)
USB_SOF_IRQHandler
	B USB_SOF_IRQHandler

	PUBWEAK USB_SUTOK_IRQHandler
	SECTION .text:CODE:REORDER(1)
USB_SUTOK_IRQHandler
	B USB_SUTOK_IRQHandler

	PUBWEAK USB_SUSPEND_IRQHandler
	SECTION .text:CODE:REORDER(1)
USB_SUSPEND_IRQHandler
	B USB_SUSPEND_IRQHandler

	PUBWEAK USB_RESET_IRQHandler
	SECTION .text:CODE:REORDER(1)
USB_RESET_IRQHandler
	B USB_RESET_IRQHandler

	PUBWEAK USB_EP0IN_IRQHandler
	SECTION .text:CODE:REORDER(1)
USB_EP0IN_IRQHandler
	B USB_EP0IN_IRQHandler

	PUBWEAK USB_EP0OUT_IRQHandler
	SECTION .text:CODE:REORDER(1)
USB_EP0OUT_IRQHandler
	B USB_EP0OUT_IRQHandler

	PUBWEAK USB_EP2IN_IRQHandler
	SECTION .text:CODE:REORDER(1)
USB_EP2IN_IRQHandler
	B USB_EP2IN_IRQHandler

	PUBWEAK USB_EP3IN_IRQHandler
	SECTION .text:CODE:REORDER(1)
USB_EP3IN_IRQHandler
	B USB_EP3IN_IRQHandler

	PUBWEAK USB_EP4OUT_IRQHandler
	SECTION .text:CODE:REORDER(1)
USB_EP4OUT_IRQHandler
	B USB_EP4OUT_IRQHandler

	PUBWEAK USB_EP5OUT_IRQHandler
	SECTION .text:CODE:REORDER(1)
USB_EP5OUT_IRQHandler
	B USB_EP5OUT_IRQHandler

	PUBWEAK ECC_CORRECTED_IRQHandler
	SECTION .text:CODE:REORDER(1)
ECC_CORRECTED_IRQHandler
	B ECC_CORRECTED_IRQHandler

	PUBWEAK GPIO_GP2_IRQHandler
	SECTION .text:CODE:REORDER(1)
GPIO_GP2_IRQHandler
	B GPIO_GP2_IRQHandler

	PUBWEAK GPIO_GP3_IRQHandler
	SECTION .text:CODE:REORDER(1)
GPIO_GP3_IRQHandler
	B GPIO_GP3_IRQHandler

	PUBWEAK GPIO_GP4_IRQHandler
	SECTION .text:CODE:REORDER(1)
GPIO_GP4_IRQHandler
	B GPIO_GP4_IRQHandler

	PUBWEAK GPIO_GP5_IRQHandler
	SECTION .text:CODE:REORDER(1)
GPIO_GP5_IRQHandler
	B GPIO_GP5_IRQHandler

	PUBWEAK GPIO_GP6_IRQHandler
	SECTION .text:CODE:REORDER(1)
GPIO_GP6_IRQHandler
	B GPIO_GP6_IRQHandler

	PUBWEAK GPIO_GP7_IRQHandler
	SECTION .text:CODE:REORDER(1)
GPIO_GP7_IRQHandler
	B GPIO_GP7_IRQHandler


        END
