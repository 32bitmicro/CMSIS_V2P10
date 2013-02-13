/* ----------------------------------------------------------------------------
 * Copyright (c) 2009 - 2012 Semiconductor Components Industries, LLC
 * (d/b/a ON Semiconductor). All Rights Reserved.
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor. The
 * terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * q32_eeprom_transfer.c
 *  - Data transfer source file for EEPROM library
 * ----------------------------------------------------------------------------
 * $Revision: 1.7 $
 * $Date: 2012/04/26 20:54:38 $
 * ------------------------------------------------------------------------- */

#include <q32.h>
#include <q32_eeprom.h>
#include "q32_eeprom_internal.h"

/* ----------------------------------------------------------------------------
 * Function      : void EEPROM_Read_Init(uint32_t spi_interface,
 *                                       uint16_t address)
 * ----------------------------------------------------------------------------
 * Description   : Ready the EEPROM for reading at a specified address. Chip
 *                 select is left low.
 * Inputs        : - spi_interface  - Index of SPI interface; use 0, 1
 *                 - address        - EEPROM address to read from
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void EEPROM_Read_Init(uint32_t spi_interface, uint16_t address)
{
    /* Check if SPI interface exists. If interface is invalid, return. */
    if(spi_interface > (EEPROM_SPI_NUM_INTERFACE - 1))
    {
        return;
    }

    /* Send read opcode */
    SPI_WRITE(spi_interface, EEPROM_OPCODE_READ);
    SPI_TRANSFERCONFIG(spi_interface, EEPROM_SPI_WRITE_BYTE);
    while(SPI_IS_BUSY(spi_interface));

    /* Send address to read */
    SPI_WRITE(spi_interface, address);
    SPI_TRANSFERCONFIG(spi_interface, EEPROM_SPI_WRITE_SHORT);
    while(SPI_IS_BUSY(spi_interface));

    /* NOTE: Chip select is left low */
}

/* ----------------------------------------------------------------------------
 * Function      : void EEPROM_Write_Init(uint32_t spi_interface,
 *                                        uint16_t address)
 * ----------------------------------------------------------------------------
 * Description   : Ready the EEPROM for writing at a specified address. Chip
 *                 select is left low.
 * Inputs        : - spi_interface  - Index of SPI interface; use 0, 1
 *                 - address        - EEPROM address to write to
 * Outputs       : None
 * Assumptions   : EEPROM is write enabled
 * ------------------------------------------------------------------------- */
void EEPROM_Write_Init(uint32_t spi_interface, uint16_t address)
{
    /* Check if SPI interface exists. If interface is invalid, return. */
    if(spi_interface > (EEPROM_SPI_NUM_INTERFACE - 1))
    {
        return;
    }

    /* Send write opcode */
    SPI_WRITE(spi_interface, EEPROM_OPCODE_WRITE);
    SPI_TRANSFERCONFIG(spi_interface, EEPROM_SPI_WRITE_BYTE);
    while(SPI_IS_BUSY(spi_interface));

    /* Send address to write to */
    SPI_WRITE(spi_interface, address);
    SPI_TRANSFERCONFIG(spi_interface, EEPROM_SPI_WRITE_SHORT);
    while(SPI_IS_BUSY(spi_interface));

    /* NOTE: Chip select is left low. */
}

/* ----------------------------------------------------------------------------
 * Function      : uint8_t EEPROM_Read_Byte(uint32_t spi_interface)
 * ----------------------------------------------------------------------------
 * Description   : Read a byte from the EEPROM
 * Inputs        : spi_interface   - Index of SPI interface; use 0, 1
 * Outputs       : data            - Byte read (return 0xFF if the specified
 *                                   SPI interface is invalid)
 * Assumptions   : - EEPROM is initialized to an address
 *                 - EEPROM is selected by chip select
 * ------------------------------------------------------------------------- */
uint8_t EEPROM_Read_Byte(uint32_t spi_interface)
{
    /* Check if SPI interface exists. If interface is invalid, return 0xFF. */
    if(spi_interface > (EEPROM_SPI_NUM_INTERFACE - 1))
    {
        return 0xFF;
    }

    SPI_TRANSFERCONFIG(spi_interface, EEPROM_SPI_READ_BYTE);
    while(SPI_IS_BUSY(spi_interface));

    return (uint8_t)SPI_READ(spi_interface);

    /* NOTE: Chip select is left low. */
}

/* ----------------------------------------------------------------------------
 * Function      : void EEPROM_Write_Byte(uint32_t spi_interface,
 *                                        uint8_t data)
 * ----------------------------------------------------------------------------
 * Description   : Write a byte to the EEPROM
 * Inputs        : - spi_interface  - Index of SPI interface; use 0, 1
 *                 - data           - Byte to write
 * Outputs       : None
 * Assumptions   : - EEPROM is initialized to an address
 *                 - Current EEPROM address is outside write protected blocks
 *                 - EEPROM is selected by chip select
 * ------------------------------------------------------------------------- */
void EEPROM_Write_Byte(uint32_t spi_interface, uint8_t data)
{
    /* Check if SPI interface exists. If interface is invalid, return. */
    if (spi_interface > (EEPROM_SPI_NUM_INTERFACE - 1))
    {
        return;
    }

    SPI_WRITE(spi_interface, data);
    SPI_TRANSFERCONFIG(spi_interface, EEPROM_SPI_WRITE_BYTE);
    while (SPI_IS_BUSY(spi_interface));

    /* NOTE: Chip select is left low. */
}

/* ----------------------------------------------------------------------------
 * Function      : uint32_t EEPROM_Read_Word(uint32_t spi_interface)
 * ----------------------------------------------------------------------------
 * Description   : Read a 32-bit word from the EEPROM
 * Inputs        : spi_interface   - Index of SPI interface; use 0, 1
 * Outputs       : data            - Word read (return 0xFFFFFFFF if the
 *                                   specified SPI interface is invalid)
 * Assumptions   : - EEPROM is initialized to an address
 *                 - EEPROM is selected by chip select
 * ------------------------------------------------------------------------- */
uint32_t EEPROM_Read_Word(uint32_t spi_interface)
{
    uint32_t i;
    uint32_t data = 0;

    /* Check if SPI interface exists. If interface is invalid, return
     * 0xFFFFFFFF. */
    if(spi_interface > (EEPROM_SPI_NUM_INTERFACE - 1))
    {
        return 0xFFFFFFFF;
    }

    for (i = 0; i < 4; i++)
    {
        ((uint8_t *)&data)[i] = EEPROM_Read_Byte(spi_interface);
    }

    return data;

    /* NOTE: Chip select is left low. */
}

/* ----------------------------------------------------------------------------
 * Function      : void EEPROM_Write_Word(uint32_t spi_interface,
 *                                        uint32_t data)
 * ----------------------------------------------------------------------------
 * Description   : Write a 32-bit word to the EEPROM
 * Inputs        : - spi_interface  - Index of SPI interface; use 0, 1
 *                 - data           - Word to write
 * Outputs       : None
 * Assumptions   : - EEPROM is initialized to an address
 *                 - Current EEPROM address is outside write protected blocks
 *                 - EEPROM is selected by chip select
 * ------------------------------------------------------------------------- */
void EEPROM_Write_Word(uint32_t spi_interface, uint32_t data)
{
    uint32_t i;

    /* Check if SPI interface exists. If interface is invalid, return. */
    if(spi_interface > (EEPROM_SPI_NUM_INTERFACE - 1))
    {
        return;
    }

    for (i = 0; i < 4; i++)
    {
        EEPROM_Write_Byte(spi_interface, ((uint8_t *)&data)[i]);
    }
    /* NOTE: Chip select is left low. */
}

/* ----------------------------------------------------------------------------
 * Function      : void EEPROM_Read_Done(uint32_t spi_interface)
 * ----------------------------------------------------------------------------
 * Description   : Close read process by pulling chip select high
 * Inputs        : spi_interface    - Index of SPI interface; use 0, 1
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void EEPROM_Read_Done(uint32_t spi_interface)
{
    /* Check if SPI interface exists. If interface is invalid, return. */
    if(spi_interface > (EEPROM_SPI_NUM_INTERFACE - 1))
    {
        return;
    }

    SPI_TRANSFERCONFIG(spi_interface, EEPROM_SPI_SET_IDLE);
}

/* ----------------------------------------------------------------------------
 * Function      : void EEPROM_Write_Done(uint32_t spi_interface)
 * ----------------------------------------------------------------------------
 * Description   : Pull chip select high and delay until the EEPROM is finished
 *                 writing buffer to memory
 * Inputs        : spi_interface    - Index of SPI interface; use 0, 1
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void EEPROM_Write_Done(uint32_t spi_interface)
{
    uint32_t timeout;

    /* Check if SPI interface exists. If interface is invalid, return. */
    if(spi_interface > (EEPROM_SPI_NUM_INTERFACE - 1))
    {
        return;
    }

    SPI_TRANSFERCONFIG(spi_interface, EEPROM_SPI_SET_IDLE);

    for(timeout = EEPROM_spi_config[spi_interface].commit_timeout;
        timeout > 0; timeout--)
    {
        /* Dividing by 1000 to convert to a delay in milliseconds */
        Sys_Delay((EEPROM_system_frequency
                   * EEPROM_spi_config[spi_interface].commit_delay)
                   / 1000);

        if((EEPROM_Read_StatusReg(spi_interface) & EEPROM_STATUS_STATUS_Mask)
           == EEPROM_STATUS_READY)
        {
            return;
        }
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void EEPROM_Read(uint32_t spi_interface,
 *                                  uint16_t address,
 *                                  uint8_t* buffer,
 *                                  uint32_t byte_count)
 * ----------------------------------------------------------------------------
 * Description   : Read a number of bytes starting at an address
 * Inputs        : - spi_interface  - Index of SPI interface; use 0, 1
 *                 - address        - EEPROM address
 *                 - buffer         - Memory to transfer read data
 *                 - byte_count     - Number of bytes to read
 * Outputs       : None
 * Assumptions   : byte_count is less than the buffer size
 * ------------------------------------------------------------------------- */
void EEPROM_Read(uint32_t spi_interface,
                 uint16_t address,
                 uint8_t* buffer,
                 uint32_t byte_count)
{
    /* Check if SPI interface exists. If interface is invalid, return. */
    if(spi_interface > (EEPROM_SPI_NUM_INTERFACE - 1))
    {
        return;
    }

    EEPROM_Read_Init(spi_interface, address);

    for( ; byte_count > 0; byte_count--)
    {
        *buffer = EEPROM_Read_Byte(spi_interface);
        buffer++;
    }

    EEPROM_Read_Done(spi_interface);
}

/* ----------------------------------------------------------------------------
 * Function      : void EEPROM_Write(uint32_t spi_interface,
 *                                   uint16_t address,
 *                                   uint8_t* buffer,
 *                                   uint32_t byte_count)
 * ----------------------------------------------------------------------------
 * Description   : Write a number of bytes starting at an address
 * Inputs        : - spi_interface  - Index of SPI interface; use 0, 1
 *                 - address        - EEPROM address
 *                 - buffer         - Data to write to EEPROM
 *                 - byte_count     - Number of bytes to write
 * Outputs       : None
 * Assumptions   : - byte_count is less than the buffer size
 *                 - Write-enable opcode is set
 *                 - Block of data to be written is outside write protected
 *                   blocks
 * ------------------------------------------------------------------------- */
void EEPROM_Write(uint32_t spi_interface,
                  uint16_t address,
                  uint8_t* buffer,
                  uint32_t byte_count)
{
    uint16_t page_address;

    /* Check if SPI interface exists. If interface is invalid, return. */
    if(spi_interface > (EEPROM_SPI_NUM_INTERFACE - 1))
    {
        return;
    }

    page_address = (address & EEPROM_PAGE_ADDRESS_Mask);

    while(byte_count > 0)
    {
        /* Set page_address to the end of current page */
        page_address += EEPROM_PAGE_LENGTH;

        EEPROM_Write_Enable(spi_interface);
        EEPROM_Write_Init(spi_interface, address);

        /* Write bytes until either byte_count is zero or writing past a
         * page. When writing past a page, commit to memory before 
         * proceeding. */
        while((address != page_address) && (byte_count > 0))
        {
            EEPROM_Write_Byte(spi_interface, *buffer);
            buffer++;
            address++;

            byte_count--;
        }

        EEPROM_Write_Done(spi_interface);
    }
}
