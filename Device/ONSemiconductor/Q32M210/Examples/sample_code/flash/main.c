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
 * $Revision: 1.16 $
 * $Date: 2012/04/26 18:50:01 $
 * ------------------------------------------------------------------------- */

#include <q32.h>
#include "globals.h"

/* ----------------------------------------------------------------------------
 * Allocate an area of flash for the LED data
 * ------------------------------------------------------------------------- */

#if defined(__ICCARM__)
__no_init const uint32_t flashLEDData[DATA_LENGTH] @ ".flashdata";
#elif defined(__GNUC__) || defined(__CC_ARM)
__attribute__((section(".flashdata")))
const uint32_t flashLEDData[DATA_LENGTH];
#endif

/* ----------------------------------------------------------------------------
 * Global Variables
 * ------------------------------------------------------------------------- */

/* Data to write to flash */
uint32_t data_array[DATA_LENGTH] = {0};

/* Data to output to the LEDs */
uint32_t LED_DATA = 0x0;

uint32_t crc_data;

/* Valid data flag */
uint32_t data_valid;

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
}


/* ----------------------------------------------------------------------------
 * Function      : void delay(void)
 * ----------------------------------------------------------------------------
 * Description   : Waits until the SYSTICK timer count flag indicates that
 *                 the timer has expired.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : SYSTICK timer is enabled
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
 * Function      : void crc_new(void)
 * ----------------------------------------------------------------------------
 * Description   : Initialize and configure the CRC.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void crc_new()
{
    Sys_CRC_Initialize();
    Sys_CRC_Config(CRC_CFG);
}

/* ----------------------------------------------------------------------------
 * Function      : void flash_write(uint32_t* data,
 *                                  uint32_t data_length,
 *                                  uint32_t sys_clk_freq,
 *                                  uint32_t flash_addr_base)
 * ----------------------------------------------------------------------------
 * Description   : Unlock flash for writing, erase the page where data is to be
 *                 written, write data buffer to flash and then lock the flash.
 * Inputs        : - data: Pointer to data to be written in flash memory
 *                 - data_length: Length of the buffer to be written in flash
 *                 - freq_in_hz: System clock frequency in hertz
 *                 - flash_addr_base: Start address of flash memory to write to
 * Outputs       : None
 * Assumptions   : flash_addr_base is an address in the flash memory
 * ------------------------------------------------------------------------- */
void flash_write(uint32_t* data,
                 uint32_t data_length,
                 uint32_t freq_in_hz,
                 uint32_t flash_addr_base)
{
    /* Unlock Flash by Writing Key to FLASH_WRITE_LOCK register */
    FLASH->WRITE_LOCK = FLASH_WRITE_KEY;

    /* Erase the flash page that will be used to store the data buffer */
    Sys_Flash_ErasePage(freq_in_hz, flash_addr_base);

    /* Write data array to flash */
    Sys_Flash_WriteBuffer(freq_in_hz, flash_addr_base, data_length, data);

    /* Lock Flash Writing by clearing the FLASH_WRITE_LOCK register */
    FLASH->WRITE_LOCK = 0x0;
}

/* ----------------------------------------------------------------------------
 * Function      : void flash_verify(uint32_t address,
 *                                   uint32_t data_length,
 *                                   uint32_t crc)
 * ----------------------------------------------------------------------------
 * Description   : Read data from the flash memory and compare it with the
 *                 input CRC.
 * Inputs        : - data:        The data to be verified.
 *                 - data_length: Length of the data in flash memory to read.
 *                 - crc:         Input CRC to compare with.
 * Outputs       : 1 - CRCs match; data is consistent
 *                 0 - CRCs do not match; data is corrupted
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
uint32_t flash_verify(uint32_t* data, uint32_t data_length, uint32_t crc)
{
    uint32_t current_data, i;

    /* Initialize and configure the CRC */
    crc_new();

    for (i = 0; i < data_length; i++)
    {
        /* Read data from the flash data area */
        current_data = data[i];

        /* Generate CRC for the data in flash */
        Sys_CRC_AddInt(current_data);
    }

    if (Sys_CRC_GetCurrentCRC() == crc)
    {
        return 1; /* Data is consistent */
    }
    else
    {
        return 0; /* Data is corrupted */
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void main(void)
 * ----------------------------------------------------------------------------
 * Description   : Generates a pattern of blinking lights on the LEDs.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int main()
{
    int i;
    
    /* Call the system initialization function */
    initialize();

    /* Enable interrupts now that initialization is complete */
    Sys_NVIC_MasterEnable();

    /* Initialize and configure the CRC */
    crc_new();

    /* Create a data buffer to be written to the flash data area */
    for (i = 0; i < DATA_LENGTH; i++)
    {
        /* Explicitly make each element of the buffer an unsigned integer
         * in order to avoid ECC status errors. */

        /* For example: Writing 0x1 to a recently erased page of flash
         * will result in an ECC error when reading 32-bits at that address
         * because the bits 2 to 32 have not been explicitly written.
         * Writing 0x0001 to flash and then reading will not result in an ECC
         * error. */
        data_array[i] = (uint32_t)(0x1 << i);

        /* Generate CRC for each value in the data_array */
        Sys_CRC_AddInt(data_array[i]);
    }

    /* Store current CRC value */
    crc_data = Sys_CRC_GetCurrentCRC();

    /* Write to flash */
    flash_write(data_array,
                DATA_LENGTH,
                SYS_FREQ,
                (uint32_t)flashLEDData);

    /* Verify flash contents with CRC */
    data_valid = flash_verify((uint32_t*)flashLEDData,
                              DATA_LENGTH,
                              crc_data);

    /* Enable the SYSTICK timer */
    Sys_SYSTICK_Enable();

    while(1)
    {
        if (data_valid)
        {
            /* Data write has been successful, blink contents of the buffer
             * written to flash */
            for (i = 0; i < DATA_LENGTH; i++)
            {
                /* Temporarily store GPIO IF4 contents - zero the 8 bits
                 * which correspond to the LEDs */
                int temp = GPIO->IF4_LCD_OUT & ~0xFF0;

                /* OR GPIO IF4 contents with the data written to flash */
                LED_DATA = (flashLEDData[i] << 4 | temp);

                /* Output flash data on LEDs */
                Sys_GPIO_Set_IF4(LED_DATA);

                /* Show this value on the LEDs for 1 second */
                delay();
            }
        }
        else
        {
            /* Data written to flash has been corrupted.
             * Write a pattern to the LEDs which indicates an error. */
            Sys_GPIO_Set_IF4(0xAA0);
            delay();
            Sys_GPIO_Set_IF4(0x550);
            delay();
        }
    }
}
