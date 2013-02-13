;/*****************************************************************************
; * @file:    startup_Q32M210.s
; * @purpose: CMSIS Cortex-M3 Core Device Startup File for the 
; * ON Semiconductor Q32M210
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
; *****************************************************************************/

; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x00000000

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit


                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    RESET, CODE, READONLY
                EXPORT  __vector_table
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler
                DCD     BusFault_Handler          ; Bus Fault Handler
                DCD     UsageFault_Handler        ; Usage Fault Handler
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     WAKEUP_IRQHandler          ; Wakeup
                DCD     WAKEUP_IF5_PIN0_IRQHandler ; IF5, Pin 0 Specific Wakeup
                DCD     RTC_ALARM_IRQHandler       ; Real-Time Clock (RTC) Alarm
                DCD     RTC_CLOCK_IRQHandler       ; Real-Time Clock (RTC) Clock
                DCD     THRESHOLD_COMPARE_IRQHandler; Threshold Compare
                DCD     NO_EXTCLK_DETECT_IRQHandler; No EXTCLK Detected
                DCD     GPIO_GP0_IRQHandler        ; GPIO 0
                DCD     GPIO_GP1_IRQHandler        ; GPIO 1
                DCD     WATCHDOG_IRQHandler        ; Watchdog
                DCD     TIMER0_IRQHandler          ; Timer 0
                DCD     TIMER1_IRQHandler          ; Timer 1
                DCD     TIMER2_IRQHandler          ; Timer 2
                DCD     TIMER3_IRQHandler          ; Timer 3
                DCD     UART0_RX_IRQHandler        ; UART 0 Receive
                DCD     UART0_TX_IRQHandler        ; UART 0 Transmit
                DCD     UART0_ERROR_IRQHandler     ; UART 0 Error
                DCD     UART1_RX_IRQHandler        ; UART 1 Receive
                DCD     UART1_TX_IRQHandler        ; UART 1 Transmit
                DCD     UART1_ERROR_IRQHandler     ; UART 1 Error
                DCD     I2C_IRQHandler             ; I2C
                DCD     SPI0_IRQHandler            ; SPI 0
                DCD     SPI0_ERROR_IRQHandler      ; SPI 0 Error
                DCD     SPI1_IRQHandler            ; SPI 1
                DCD     SPI1_ERROR_IRQHandler      ; SPI 1 Error
                DCD     PCM_RX_IRQHandler          ; PCM Receive
                DCD     PCM_TX_IRQHandler          ; PCM Transmit
                DCD     PCM_ERROR_IRQHandler       ; PCM Error
                DCD     ADC_IRQHandler             ; ADC
                DCD     DMA0_IRQHandler            ; DMA Channel 0
                DCD     DMA1_IRQHandler            ; DMA Channel 1
                DCD     DMA2_IRQHandler            ; DMA Channel 2
                DCD     DMA3_IRQHandler            ; DMA Channel 3
                DCD     USB_WAKEUP_IRQHandler      ; USB Wakeup
                DCD     USB_SDAV_IRQHandler        ; USB Setup Data Available
                DCD     USB_SOF_IRQHandler         ; USB Start Of Frame
                DCD     USB_SUTOK_IRQHandler       ; USB Setup Data Loading
                DCD     USB_SUSPEND_IRQHandler     ; USB Suspend
                DCD     USB_RESET_IRQHandler       ; USB Reset
                DCD     USB_EP0IN_IRQHandler       ; USB EP0IN
                DCD     USB_EP0OUT_IRQHandler      ; USB EP0OUT
                DCD     USB_EP2IN_IRQHandler       ; USB EP2IN
                DCD     0
                DCD     USB_EP3IN_IRQHandler       ; USB EP3IN
                DCD     0
                DCD     0
                DCD     USB_EP4OUT_IRQHandler      ; USB EP4OUT
                DCD     0
                DCD     USB_EP5OUT_IRQHandler      ; USB EP5OUT
                DCD     ECC_CORRECTED_IRQHandler   ; ECC Error Corrected
                DCD     GPIO_GP2_IRQHandler        ; GPIO 2
                DCD     GPIO_GP3_IRQHandler        ; GPIO 3
                DCD     GPIO_GP4_IRQHandler        ; GPIO 4
                DCD     GPIO_GP5_IRQHandler        ; GPIO 5
                DCD     GPIO_GP6_IRQHandler        ; GPIO 6
                DCD     GPIO_GP7_IRQHandler        ; GPIO 7

__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors
__vector_table  EQU		__Vectors

; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP

                AREA    |.text|, CODE, READONLY

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP

HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler\
                PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP


Default_Handler PROC
                EXPORT  WAKEUP_IRQHandler         [WEAK]
                EXPORT  WAKEUP_IF5_PIN0_IRQHandler         [WEAK]
                EXPORT  RTC_ALARM_IRQHandler         [WEAK]
                EXPORT  RTC_CLOCK_IRQHandler         [WEAK]
                EXPORT  THRESHOLD_COMPARE_IRQHandler         [WEAK]
                EXPORT  NO_EXTCLK_DETECT_IRQHandler         [WEAK]
                EXPORT  GPIO_GP0_IRQHandler         [WEAK]
                EXPORT  GPIO_GP1_IRQHandler         [WEAK]
                EXPORT  WATCHDOG_IRQHandler         [WEAK]
                EXPORT  TIMER0_IRQHandler         [WEAK]
                EXPORT  TIMER1_IRQHandler         [WEAK]
                EXPORT  TIMER2_IRQHandler         [WEAK]
                EXPORT  TIMER3_IRQHandler         [WEAK]
                EXPORT  UART0_RX_IRQHandler         [WEAK]
                EXPORT  UART0_TX_IRQHandler         [WEAK]
                EXPORT  UART0_ERROR_IRQHandler         [WEAK]
                EXPORT  UART1_RX_IRQHandler         [WEAK]
                EXPORT  UART1_TX_IRQHandler         [WEAK]
                EXPORT  UART1_ERROR_IRQHandler         [WEAK]
                EXPORT  I2C_IRQHandler         [WEAK]
                EXPORT  SPI0_IRQHandler         [WEAK]
                EXPORT  SPI0_ERROR_IRQHandler         [WEAK]
                EXPORT  SPI1_IRQHandler         [WEAK]
                EXPORT  SPI1_ERROR_IRQHandler         [WEAK]
                EXPORT  PCM_RX_IRQHandler         [WEAK]
                EXPORT  PCM_TX_IRQHandler         [WEAK]
                EXPORT  PCM_ERROR_IRQHandler         [WEAK]
                EXPORT  ADC_IRQHandler         [WEAK]
                EXPORT  DMA0_IRQHandler         [WEAK]
                EXPORT  DMA1_IRQHandler         [WEAK]
                EXPORT  DMA2_IRQHandler         [WEAK]
                EXPORT  DMA3_IRQHandler         [WEAK]
                EXPORT  USB_WAKEUP_IRQHandler         [WEAK]
                EXPORT  USB_SDAV_IRQHandler         [WEAK]
                EXPORT  USB_SOF_IRQHandler         [WEAK]
                EXPORT  USB_SUTOK_IRQHandler         [WEAK]
                EXPORT  USB_SUSPEND_IRQHandler         [WEAK]
                EXPORT  USB_RESET_IRQHandler         [WEAK]
                EXPORT  USB_EP0IN_IRQHandler         [WEAK]
                EXPORT  USB_EP0OUT_IRQHandler         [WEAK]
                EXPORT  USB_EP2IN_IRQHandler         [WEAK]
                EXPORT  USB_EP3IN_IRQHandler         [WEAK]
                EXPORT  USB_EP4OUT_IRQHandler         [WEAK]
                EXPORT  USB_EP5OUT_IRQHandler         [WEAK]
                EXPORT  ECC_CORRECTED_IRQHandler         [WEAK]
                EXPORT  GPIO_GP2_IRQHandler         [WEAK]
                EXPORT  GPIO_GP3_IRQHandler         [WEAK]
                EXPORT  GPIO_GP4_IRQHandler         [WEAK]
                EXPORT  GPIO_GP5_IRQHandler         [WEAK]
                EXPORT  GPIO_GP6_IRQHandler         [WEAK]
                EXPORT  GPIO_GP7_IRQHandler         [WEAK]


WAKEUP_IRQHandler
WAKEUP_IF5_PIN0_IRQHandler
RTC_ALARM_IRQHandler
RTC_CLOCK_IRQHandler
THRESHOLD_COMPARE_IRQHandler
NO_EXTCLK_DETECT_IRQHandler
GPIO_GP0_IRQHandler
GPIO_GP1_IRQHandler
WATCHDOG_IRQHandler
TIMER0_IRQHandler
TIMER1_IRQHandler
TIMER2_IRQHandler
TIMER3_IRQHandler
UART0_RX_IRQHandler
UART0_TX_IRQHandler
UART0_ERROR_IRQHandler
UART1_RX_IRQHandler
UART1_TX_IRQHandler
UART1_ERROR_IRQHandler
I2C_IRQHandler
SPI0_IRQHandler
SPI0_ERROR_IRQHandler
SPI1_IRQHandler
SPI1_ERROR_IRQHandler
PCM_RX_IRQHandler
PCM_TX_IRQHandler
PCM_ERROR_IRQHandler
ADC_IRQHandler
DMA0_IRQHandler
DMA1_IRQHandler
DMA2_IRQHandler
DMA3_IRQHandler
USB_WAKEUP_IRQHandler
USB_SDAV_IRQHandler
USB_SOF_IRQHandler
USB_SUTOK_IRQHandler
USB_SUSPEND_IRQHandler
USB_RESET_IRQHandler
USB_EP0IN_IRQHandler
USB_EP0OUT_IRQHandler
USB_EP2IN_IRQHandler
USB_EP3IN_IRQHandler
USB_EP4OUT_IRQHandler
USB_EP5OUT_IRQHandler
ECC_CORRECTED_IRQHandler
GPIO_GP2_IRQHandler
GPIO_GP3_IRQHandler
GPIO_GP4_IRQHandler
GPIO_GP5_IRQHandler
GPIO_GP6_IRQHandler
GPIO_GP7_IRQHandler

                B       .
                ENDP


                ALIGN


; User Initial Stack & Heap

                IF      :DEF:__MICROLIB
                
                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit
                
                ELSE
                
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF


                END
