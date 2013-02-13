/* ----------------------------------------------------------------------------
 * Copyright (c) 2009 - 2012 Semiconductor Components Industries, LLC
 * (d/b/a ON Semiconductor). All Rights Reserved.
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor. The
 * terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * q32_eeprom_init.c
 *  - Initialization and finalization source file for EEPROM library
 * ----------------------------------------------------------------------------
 * $Revision: 1.3 $
 * $Date: 2012/04/25 21:56:14 $
 * ------------------------------------------------------------------------- */
#include <q32.h>
#include <q32_eeprom.h>
#include "q32_eeprom_internal.h"

/* ----------------------------------------------------------------------------
 * Global Variables
 * ------------------------------------------------------------------------- */
struct EEPROM_config EEPROM_spi_config[EEPROM_SPI_NUM_INTERFACE];

uint32_t EEPROM_system_frequency;

/* ----------------------------------------------------------------------------
 * Function      : void EEPROM_Init(uint32_t spi_interface,
 *                                  uint32_t spi_prescalar,
 *                                  uint32_t freq_in_hertz)
 * ----------------------------------------------------------------------------
 * Description   : Configure SPI for EEPROM use. Initialize internal library
 *                 variables.
 * Inputs        : - spi_interface  - Index of SPI interface; use 0, 1
 *                 - spi_prescalar  - Prescalar for SPI; use SPI*_PRESCALE_*
 *                 - freq_in_hertz  - System frequency in hertz
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void EEPROM_Init(uint32_t spi_interface,
                 uint32_t spi_prescalar,
                 uint32_t freq_in_hertz)
{
    /* Check if SPI interface exists. If interface is invalid, return. */
    if(spi_interface > (EEPROM_SPI_NUM_INTERFACE - 1))
    {
        return;
    }

    SPI_CONFIG(spi_interface, EEPROM_SPI_CFG | spi_prescalar);

    SPI_TRANSFERCONFIG(spi_interface, EEPROM_SPI_SET_IDLE);

    EEPROM_system_frequency = freq_in_hertz;

    EEPROM_spi_config[spi_interface].commit_delay = DEFAULT_COMMIT_DELAY;
    EEPROM_spi_config[spi_interface].commit_timeout = DEFAULT_COMMIT_TIMEOUT;
    EEPROM_spi_config[spi_interface].write_enable_opcode = EEPROM_OPCODE_NOP;
}

/* ----------------------------------------------------------------------------
 * Function      : void EEPROM_Close(uint32_t spi_interface)
 * ----------------------------------------------------------------------------
 * Description   : Disable SPI
 * Inputs        : spi_interface    - Index of SPI interface; use 0, 1
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void EEPROM_Close(uint32_t spi_interface)
{
    /* Check if SPI interface exists. If interface is invalid, return. */
    if(spi_interface > (EEPROM_SPI_NUM_INTERFACE - 1))
    {
        return;
    }

    SPI_SET_DISABLE(spi_interface);

    SPI_TRANSFERCONFIG(spi_interface, EEPROM_SPI_SET_IDLE);
}
