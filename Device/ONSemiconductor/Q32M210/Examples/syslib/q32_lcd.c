/* ----------------------------------------------------------------------------
 * Copyright (c) 2008 - 2012 Semiconductor Components Industries, LLC (d/b/a ON
 * Semiconductor), All Rights Reserved
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor.
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * q32_lcd.c
 * - LCD driver interface hardware support code source file
 * ----------------------------------------------------------------------------
 * $Revision: 1.5 $
 * $Date: 2012/04/26 20:50:15 $
 * ------------------------------------------------------------------------- */

#include <q32.h>

/* ----------------------------------------------------------------------------
 * Function      : void Sys_LCDDriver_SegmentConfig(uint32_t config0,
 *                                                  uint32_t config1,
 *                                                  uint32_t config2,
 *                                                  uint32_t config3)
 * ----------------------------------------------------------------------------
 * Description   : Configure which LCD driver outputs enable display segments
 * Inputs        : - config0    - Output driver configuration for LCD
 *                                segments 0-31 (segment pins 0-7)
 *                 - config1    - Output driver configuration for LCD
 *                                segments 32-63 (segment pins 8-15)
 *                 - config2    - Output driver configuration for LCD
 *                                segments 64-95 (segment pins 16-23)
 *                 - config3    - Output driver configuration for LCD
 *                                segments 96-111 (segment pins 24-27)
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Sys_LCDDriver_SegmentConfig(uint32_t config0, uint32_t config1,
                                 uint32_t config2, uint32_t config3)
{
    /* Masking is only required for GPIO_IF4_LCD_DRV_SEL3, as the other three
     * configuration registers use all 32 bits in the configuration register
     * already. */
    GPIO->IF4_LCD_DRV_SEL0 = config0;
    GPIO->IF4_LCD_DRV_SEL1 = config1;
    GPIO->IF4_LCD_DRV_SEL2 = config2;
    GPIO->IF4_LCD_DRV_SEL3 = (config3 &
                             (GPIO_IF4_LCD_DRV_SEL3_DRIVER_SELECT_PIN24_Mask
                             | GPIO_IF4_LCD_DRV_SEL3_DRIVER_SELECT_PIN25_Mask
                             | GPIO_IF4_LCD_DRV_SEL3_DRIVER_SELECT_PIN26_Mask
                             | GPIO_IF4_LCD_DRV_SEL3_DRIVER_SELECT_PIN27_Mask));
}

/* ----------------------------------------------------------------------------
 * Function      : void Sys_LCDDriver_SingleSegmentConfig(
 *                                                  uint32_t segmentNumber,
 *                                                  uint32_t config)
 * ----------------------------------------------------------------------------
 * Description   : Configure the output driver configuration for one LCD display
 *                 segment input
 * Inputs        : - segmentNumber  - LCD segment to update the driver level for
 *                 - config         - Output driver configuration for the
 *                                    specified LCD display segment
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Sys_LCDDriver_SingleSegmentConfig(uint32_t segmentNumber,
                                       uint32_t config)
{
    uint32_t segmentAddr;

    /* Calculate the address of the specified segment in the bit-band addressing
     * region and update just that segment. Each of the 112 segments has two
     * configuration bits so an individual segment configuration uses an offset
     * of 2 from the previous segment. */
    if (segmentNumber < 112)
    {
        segmentAddr = (SYS_CALC_BITBAND(GPIO_IF4_LCD_DRV_SEL0_BASE, 0)
                      + (segmentNumber << 2));
        REG32_POINTER(segmentAddr) = config;
    }
}
