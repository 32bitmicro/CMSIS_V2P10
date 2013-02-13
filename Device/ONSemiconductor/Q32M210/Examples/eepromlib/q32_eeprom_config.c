/* ----------------------------------------------------------------------------
 * Copyright (c) 2009 - 2012 Semiconductor Components Industries, LLC
 * (d/b/a ON Semiconductor). All Rights Reserved.
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor. The
 * terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * q32_eeprom_config.c
 *  - Configuration support source file for EEPROM library
 * ----------------------------------------------------------------------------
 * $Revision: 1.3 $
 * $Date: 2012/04/25 21:56:14 $
 * ------------------------------------------------------------------------- */

#include <q32.h>
#include <q32_eeprom.h>
#include "q32_eeprom_internal.h"

/* ----------------------------------------------------------------------------
 * Function      : uint8_t
 *                 EEPROM_Read_StatusReg(uint32_t spi_interface)
 * ----------------------------------------------------------------------------
 * Description   : Return the status bits read from the EEPROM
 * Inputs        : spi_interface   - Index of SPI interface; use 0, 1
 * Outputs       : status          - Status bits read from the EEPROM (return
 *                                   0xFF if the specified SPI interface is
 *                                   invalid)
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
uint8_t EEPROM_Read_StatusReg(uint32_t spi_interface)
{
    uint8_t status;

    /* Check if SPI interface exists. If interface is invalid, return 0xFF. */
    if (spi_interface > (EEPROM_SPI_NUM_INTERFACE - 1))
    {
        return 0xFF;
    }

    /* Enable read-status-registor operation */
    SPI_WRITE(spi_interface, EEPROM_OPCODE_READ_STATUS);
    SPI_TRANSFERCONFIG(spi_interface, EEPROM_SPI_WRITE_BYTE);
    while (SPI_IS_BUSY(spi_interface));

    /* Retrieve the status byte */
    SPI_TRANSFERCONFIG(spi_interface, EEPROM_SPI_READ_BYTE);
    while (SPI_IS_BUSY(spi_interface));
    status = (uint8_t)SPI_READ(spi_interface);

    SPI_TRANSFERCONFIG(spi_interface, EEPROM_SPI_SET_IDLE);

    return status;
}

/* ----------------------------------------------------------------------------
 * Function      : void EEPROM_Write_StatusReg(uint32_t spi_interface,
 *                                             uint8_t status)
 * ----------------------------------------------------------------------------
 * Description   : Write the status bits to the EEPROM
 * Inputs        : - spi_interface  - Index of SPI interface; use 0, 1
 *                 - status         - Status bits to be written to EEPROM
 * Outputs       : None
 * Assumptions   : Write-enable opcode has been set
 * ------------------------------------------------------------------------- */
void EEPROM_Write_StatusReg(uint32_t spi_interface, uint8_t status)
{
    /* Check if SPI interface exists. If interface is invalid, return. */
    if(spi_interface > (EEPROM_SPI_NUM_INTERFACE - 1))
    {
        return;
    }

    /* Write enable the EEPROM */
    EEPROM_Write_Enable(spi_interface);

    /* Enable write-status-registor operation */
    SPI_WRITE(spi_interface, EEPROM_OPCODE_WRITE_STATUS);
    SPI_TRANSFERCONFIG(spi_interface, EEPROM_SPI_WRITE_BYTE);
    while (SPI_IS_BUSY(spi_interface));

    /* Write the status byte */
    SPI_WRITE(spi_interface, status);
    SPI_TRANSFERCONFIG(spi_interface, EEPROM_SPI_WRITE_BYTE);
    while (SPI_IS_BUSY(spi_interface));

    EEPROM_Write_Done(spi_interface);
}

/* ----------------------------------------------------------------------------
 * Function      : void EEPROM_Set_WriteCommitConfig(uint32_t spi_interface,
 *                                                   uint32_t milliseconds,
 *                                                   uint32_t timeout)
 * ----------------------------------------------------------------------------
 * Description   : Set the write commit configuration
 * Inputs        : - spi_interface  - Index of SPI interface; use 0, 1
 *                 - milliseconds   - Write commit delay
 *                 - timeout        - Number of iterations to check if EEPROM
 *                                    commit completed
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void EEPROM_Set_WriteCommitConfig(uint32_t spi_interface,
                                  uint32_t milliseconds,
                                  uint32_t timeout)
{
    /* Check if SPI interface exists. If interface is invalid, return. */
    if(spi_interface > (EEPROM_SPI_NUM_INTERFACE - 1))
    {
        return;
    }

    EEPROM_spi_config[spi_interface].commit_delay = milliseconds;
    EEPROM_spi_config[spi_interface].commit_timeout = timeout;
}

/* ----------------------------------------------------------------------------
 * Function      : uint8_t
 *                 EEPROM_Get_WriteEnableOpcode(uint32_t spi_interface)
 * ----------------------------------------------------------------------------
 * Description   : Get the write-enable opcode currently used by the EEPROM
 *                 library.
 * Inputs        : spi_interface          - Index of SPI interface; use 0, 1
 * Outputs       : write_enable_opcode    - Write-enable opcode (return 0xFF
 *                                          if the specified SPI interface is
 *                                          invalid)
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
uint8_t EEPROM_Get_WriteEnableOpcode(uint32_t spi_interface)
{
    /* Check if SPI interface exists. If interface is invalid, return 0xFF. */
    if(spi_interface > (EEPROM_SPI_NUM_INTERFACE - 1))
    {
        return 0xFF;
    }

    return EEPROM_spi_config[spi_interface].write_enable_opcode;
}

/* ----------------------------------------------------------------------------
 * Function      : void EEPROM_Set_WriteEnableOpcode(uint32_t spi_interface,
 *                                                   uint8_t opcode)
 * ----------------------------------------------------------------------------
 * Description   : Set the write-enable opcode to be used by the EEPROM library
 * Inputs        : - spi_interface  - Index of SPI interface; use 0, 1
 *                 - opcode         - EEPROM operation code
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void EEPROM_Set_WriteEnableOpcode(uint32_t spi_interface,
                                  uint8_t opcode)
{
    /* Check if SPI interface exists. If interface is invalid, return. */
    if(spi_interface > (EEPROM_SPI_NUM_INTERFACE - 1))
    {
        return;
    }

    EEPROM_spi_config[spi_interface].write_enable_opcode = opcode;
}

/* ----------------------------------------------------------------------------
 * Function      : void EEPROM_Write_Enable(uint32_t spi_interface)
 * ----------------------------------------------------------------------------
 * Description   : Write enable the EEPROM
 * Inputs        : spi_interface    - Index of SPI interface; use 0, 1
 * Outputs       : None
 * Assumptions   : Write-enable opcode has been set.
 * ------------------------------------------------------------------------- */
void EEPROM_Write_Enable(uint32_t spi_interface)
{
    /* Check if SPI interface exists. If interface is invalid, return. */
    if (spi_interface > (EEPROM_SPI_NUM_INTERFACE - 1))
    {
        return;
    }

    /* Send write-enable opcode */
    SPI_WRITE(spi_interface,
              EEPROM_spi_config[spi_interface].write_enable_opcode);
    SPI_TRANSFERCONFIG(spi_interface, EEPROM_SPI_WRITE_BYTE);
    while (SPI_IS_BUSY(spi_interface));

    SPI_TRANSFERCONFIG(spi_interface, EEPROM_SPI_SET_IDLE);
}

/* ----------------------------------------------------------------------------
 * Function      : void EEPROM_Write_Disable(uint32_t spi_interface)
 * ----------------------------------------------------------------------------
 * Description   : Write disable the EEPROM
 * Inputs        : spi_interface    - Index of SPI interface; use 0, 1
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void EEPROM_Write_Disable(uint32_t spi_interface)
{
    /* Check if SPI interface exists. If interface is invalid, return. */
    if (spi_interface > (EEPROM_SPI_NUM_INTERFACE - 1))
    {
        return;
    }

    /* Send write-disable opcode */
    SPI_WRITE(spi_interface, EEPROM_OPCODE_WRITE_DISABLE);
    SPI_TRANSFERCONFIG(spi_interface, EEPROM_SPI_WRITE_BYTE);
    while (SPI_IS_BUSY(spi_interface));

    SPI_TRANSFERCONFIG(spi_interface, EEPROM_SPI_SET_IDLE);
}
