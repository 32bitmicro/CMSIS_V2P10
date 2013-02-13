/* ----------------------------------------------------------------------------
 * Copyright (c) 2009 - 2012 Semiconductor Components Industries, LLC
 * (d/b/a ON Semiconductor). All Rights Reserved.
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor. The
 * terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * q32_eeprom_internal.h
 * - Internal header file for the EEPROM library
 * ----------------------------------------------------------------------------
 * $Revision: 1.4 $
 * $Date: 2012/04/25 21:56:14 $
 * ------------------------------------------------------------------------- */

#ifndef Q32_EEPROM_INTERNAL_H
#define Q32_EEPROM_INTERNAL_H

/* ----------------------------------------------------------------------------
 * SPI Control Masks
 * ------------------------------------------------------------------------- */

/* The bit-fields used by the EEPROM library are identical for both SPI
 * interfaces. */
#define SPI_CTRL0_Mask         ((0x1U << SPI_CTRL0_SQI_ENABLE_Pos) | \
                                (0x1U << SPI_CTRL0_OVERRUN_INT_ENABLE_Pos) | \
                                (0x1U << SPI_CTRL0_UNDERRUN_INT_ENABLE_Pos) | \
                                (0x1U << SPI_CTRL0_CONTROLLER_Pos) | \
                                (0x1U << SPI_CTRL0_SERI_PULLUP_ENABLE_Pos) | \
                                (0x1U << SPI_CTRL0_CLK_POLARITY_Pos) | \
                                (0x1U << SPI_CTRL0_MODE_SELECT_Pos) | \
                                (0x1U << SPI_CTRL0_ENABLE_Pos) | \
                                 SPI_CTRL0_PRESCALE_Mask)

#define SPI_CTRL1_Mask         ((0x1U << SPI_CTRL1_START_BUSY_Pos) | \
                                (0x1U << SPI_CTRL1_RW_CMD_Pos) | \
                                (0x1U << SPI_CTRL1_CS_Pos) | \
                                 SPI_CTRL1_WORD_SIZE_Mask)

/* ----------------------------------------------------------------------------
 * Internal EEPROM Library Macros
 * ------------------------------------------------------------------------- */

/* The bit-fields used by the EEPROM library are identical for both SPI
 * interfaces. Each set of SPI registers are seperated by 0x100 addresses.
 * To access a specific SPI interface, multiply 0x100 by the index of the
 * interface and add to the SPI0 register address. */
#define SPI_CONFIG(interface, config)  \
            (REG32_POINTER(SPI0_CTRL0_BASE + (interface * 0x100)) \
            = (config & SPI_CTRL0_Mask))

#define SPI_TRANSFERCONFIG(interface, config)  \
            (REG32_POINTER(SPI0_CTRL1_BASE + (interface * 0x100)) \
            = (config & SPI_CTRL1_Mask))

#define SPI_WRITE(interface, data)  \
            (REG32_POINTER(SPI0_DATA_BASE + (interface * 0x100)) = data)

#define SPI_READ(interface)  \
            (REG32_POINTER(SPI0_DATA_BASE + (interface * 0x100)))

#define SPI_IS_BUSY(interface)  \
            (REG32_POINTER(SYS_CALC_BITBAND((SPI0_CTRL1_BASE + \
            (interface * 0x100)), SPI_CTRL1_START_BUSY_Pos)) \
            == SPI_BUSY_BITBAND)

#define SPI_SET_DISABLE(interface)  \
            (REG32_POINTER(SYS_CALC_BITBAND(SPI0_CTRL0_BASE + \
            (interface * 0x100), SPI_CTRL0_ENABLE_Pos)) = SPI_DISABLE)

/* ----------------------------------------------------------------------------
 * Internal EEPROM Library Variables
 * ------------------------------------------------------------------------- */
struct EEPROM_config
{
    uint32_t commit_delay;
    uint32_t commit_timeout;
    uint8_t write_enable_opcode;
};

extern struct EEPROM_config EEPROM_spi_config[EEPROM_SPI_NUM_INTERFACE];

extern uint32_t EEPROM_system_frequency;

#endif /* Q32_EEPROM_INTERNAL_H */
