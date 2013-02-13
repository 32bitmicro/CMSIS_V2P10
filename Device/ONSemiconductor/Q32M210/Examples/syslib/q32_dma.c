/* ----------------------------------------------------------------------------
 * Copyright (c) 2009 - 2012 Semiconductor Components Industries, LLC (d/b/a ON
 * Semiconductor), All Rights Reserved
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor.
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * q32_dma.c
 * - Direct Memory Access (DMA) peripheral support function source file
 * ----------------------------------------------------------------------------
 * $Revision: 1.3 $
 * $Date: 2012/04/25 21:56:14 $
 * ------------------------------------------------------------------------- */

#include <q32.h>

/* ----------------------------------------------------------------------------
 * Function      : void Sys_DMA_Channel0_Config(uint32_t config,
 *                                              uint32_t transferLength,
 *                                              uint32_t counterInt,
 *                                              uint32_t srcAddr,
 *                                              uint32_t destAddr)
 * ----------------------------------------------------------------------------
 * Description   : Configure DMA channel 0 for a data transfer
 * Inputs        : - config - Configure the DMA transfer behavior; use
 *                            DMA_CH0_LITTLE_ENDIAN/DMA_CH0_BIG_ENDIAN,
 *                            DMA_CH0_DISABLE_INT_DISABLE/
 *                            DMA_CH0_DISABLE_INT_ENABLE,
 *                            DMA_CH0_ERROR_INT_DISABLE/
 *                            DMA_CH0_ERROR_INT_ENABLE,
 *                            DMA_CH0_COMPLETE_INT_DISABLE/
 *                            DMA_CH0_COMPLETE_INT_ENABLE,
 *                            DMA_CH0_COUNTER_INT_DISABLE/
 *                            DMA_CH0_COUNTER_INT_ENABLE,
 *                            DMA_CH0_START_INT_DISABLE/
 *                            DMA_CH0_START_INT_ENABLE,
 *                            DMA_CH0_DEST_WORD_SIZE_*, DMA_CH0_SRC_WORD_SIZE_*,
 *                            DMA_CH0_DEST_*, DMA_CH0_SRC_*, DMA_CH0_PRIORITY_*,
 *                            DMA_CH0_TRANSFER_*_TO_*,
 *                            DMA_CH0_DEST_ADDR_STATIC/DMA_CH0_DEST_ADDR_INC,
 *                            DMA_CH0_SRC_ADDR_STATIC/DMA_CH0_SRC_ADDR_INC,
 *                            DMA_CH0_ADDR_CIRC/DMA_CH0_ADDR_LIN,
 *                            DMA_CH0_DISABLE/DMA_CH0_ENABLE
 *                 - transferLength - Configure the DMA transfer length
 *                 - counterInt     - Configure when the counter interrupt will
 *                                    occur during the transfer
 *                 - srcAddr        - Base source address for the DMA transfer
 *                 - destAddr       - Base destination address for the DMA
 *                                    transfer
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Sys_DMA_Channel0_Config(uint32_t config,
                             uint32_t transferLength,
                             uint32_t counterInt,
                             uint32_t srcAddr,
                             uint32_t destAddr)
{
    /* Disable the DMA channel to ensure that it is safe to update the DMA
     * registers */
    DMA->CH0_CTRL0 = DMA_CH0_DISABLE;

    /* Setup the base addresses for the source and destination */
    DMA->CH0_SRC_BASE_ADDR = srcAddr;
    DMA->CH0_DEST_BASE_ADDR = destAddr;

    /* Setup the transfer length and transfer counter interrupt setting */
    DMA_CH0_CTRL1->TRANSFER_LENGTH_SHORT = (uint16_t) transferLength;
    DMA_CH0_CTRL1->COUNTER_INT_VALUE_SHORT = (uint16_t) counterInt;

    /* Configure the DMA channel */
    DMA->CH0_CTRL0 = (config & ((1U << DMA_CH0_CTRL0_BYTE_ORDER_Pos)
                               | (1U << DMA_CH0_CTRL0_DISABLE_INT_ENABLE_Pos)
                               | (1U << DMA_CH0_CTRL0_ERROR_INT_ENABLE_Pos)
                               | (1U << DMA_CH0_CTRL0_COMPLETE_INT_ENABLE_Pos)
                               | (1U << DMA_CH0_CTRL0_COUNTER_INT_ENABLE_Pos)
                               | (1U << DMA_CH0_CTRL0_START_INT_ENABLE_Pos)
                               | DMA_CH0_CTRL0_DEST_WORD_SIZE_Mask
                               | DMA_CH0_CTRL0_SRC_WORD_SIZE_Mask
                               | DMA_CH0_CTRL0_DEST_SELECT_Mask
                               | DMA_CH0_CTRL0_SRC_SELECT_Mask
                               | DMA_CH0_CTRL0_CHANNEL_PRIORITY_Mask
                               | DMA_CH0_CTRL0_TRANSFER_TYPE_Mask
                               | (1U << DMA_CH0_CTRL0_DEST_ADDR_INC_Pos)
                               | (1U << DMA_CH0_CTRL0_SRC_ADDR_INC_Pos)
                               | (1U << DMA_CH0_CTRL0_ADDR_MODE_Pos)
                               | (1U << DMA_CH0_CTRL0_ENABLE_Pos)));
}


/* ----------------------------------------------------------------------------
 * Function      : void Sys_DMA_Channel1_Config(uint32_t config,
 *                                              uint32_t transferLength,
 *                                              uint32_t counterInt,
 *                                              uint32_t srcAddr,
 *                                              uint32_t destAddr)
 * ----------------------------------------------------------------------------
 * Description   : Configure DMA channel 1 for a data transfer
 * Inputs        : - config - Configure the DMA transfer behavior; use
 *                            DMA_CH1_LITTLE_ENDIAN/DMA_CH1_BIG_ENDIAN,
 *                            DMA_CH1_DISABLE_INT_DISABLE/
 *                            DMA_CH1_DISABLE_INT_ENABLE,
 *                            DMA_CH1_ERROR_INT_DISABLE/
 *                            DMA_CH1_ERROR_INT_ENABLE,
 *                            DMA_CH1_COMPLETE_INT_DISABLE/
 *                            DMA_CH1_COMPLETE_INT_ENABLE,
 *                            DMA_CH1_COUNTER_INT_DISABLE/
 *                            DMA_CH1_COUNTER_INT_ENABLE,
 *                            DMA_CH1_START_INT_DISABLE/
 *                            DMA_CH1_START_INT_ENABLE,
 *                            DMA_CH1_DEST_WORD_SIZE_*, DMA_CH1_SRC_WORD_SIZE_*,
 *                            DMA_CH1_DEST_*, DMA_CH1_SRC_*, DMA_CH1_PRIORITY_*,
 *                            DMA_CH1_TRANSFER_*_TO_*,
 *                            DMA_CH1_DEST_ADDR_STATIC/DMA_CH1_DEST_ADDR_INC,
 *                            DMA_CH1_SRC_ADDR_STATIC/DMA_CH1_SRC_ADDR_INC,
 *                            DMA_CH1_ADDR_CIRC/DMA_CH1_ADDR_LIN,
 *                            DMA_CH1_DISABLE/DMA_CH1_ENABLE
 *                 - transferLength - Configure the DMA transfer length
 *                 - counterInt     - Configure when the counter interrupt will
 *                                    occur during the transfer
 *                 - srcAddr        - Base source address for the DMA transfer
 *                 - destAddr       - Base destination address for the DMA
 *                                    transfer
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Sys_DMA_Channel1_Config(uint32_t config,
                             uint32_t transferLength,
                             uint32_t counterInt,
                             uint32_t srcAddr,
                             uint32_t destAddr)
{
    /* Disable the DMA channel to ensure that it is safe to update the DMA
     * registers */
    DMA->CH1_CTRL0 = DMA_CH1_DISABLE;

    /* Setup the base addresses for the source and destination */
    DMA->CH1_SRC_BASE_ADDR = srcAddr;
    DMA->CH1_DEST_BASE_ADDR = destAddr;

    /* Setup the transfer length and transfer counter interrupt setting */
    DMA_CH1_CTRL1->TRANSFER_LENGTH_SHORT = (uint16_t) transferLength;
    DMA_CH1_CTRL1->COUNTER_INT_VALUE_SHORT = (uint16_t) counterInt;

    /* Configure the DMA channel */
    DMA->CH1_CTRL0 = (config & ((1U << DMA_CH1_CTRL0_BYTE_ORDER_Pos)
                               | (1U << DMA_CH1_CTRL0_DISABLE_INT_ENABLE_Pos)
                               | (1U << DMA_CH1_CTRL0_ERROR_INT_ENABLE_Pos)
                               | (1U << DMA_CH1_CTRL0_COMPLETE_INT_ENABLE_Pos)
                               | (1U << DMA_CH1_CTRL0_COUNTER_INT_ENABLE_Pos)
                               | (1U << DMA_CH1_CTRL0_START_INT_ENABLE_Pos)
                               | DMA_CH1_CTRL0_DEST_WORD_SIZE_Mask
                               | DMA_CH1_CTRL0_SRC_WORD_SIZE_Mask
                               | DMA_CH1_CTRL0_DEST_SELECT_Mask
                               | DMA_CH1_CTRL0_SRC_SELECT_Mask
                               | DMA_CH1_CTRL0_CHANNEL_PRIORITY_Mask
                               | DMA_CH1_CTRL0_TRANSFER_TYPE_Mask
                               | (1U << DMA_CH1_CTRL0_DEST_ADDR_INC_Pos)
                               | (1U << DMA_CH1_CTRL0_SRC_ADDR_INC_Pos)
                               | (1U << DMA_CH1_CTRL0_ADDR_MODE_Pos)
                               | (1U << DMA_CH1_CTRL0_ENABLE_Pos)));
}


/* ----------------------------------------------------------------------------
 * Function      : void Sys_DMA_Channel2_Config(uint32_t config,
 *                                              uint32_t transferLength,
 *                                              uint32_t counterInt,
 *                                              uint32_t srcAddr,
 *                                              uint32_t destAddr)
 * ----------------------------------------------------------------------------
 * Description   : Configure DMA channel 2 for a data transfer
 * Inputs        : - config - Configure the DMA transfer behavior; use
 *                            DMA_CH2_LITTLE_ENDIAN/DMA_CH2_BIG_ENDIAN,
 *                            DMA_CH2_DISABLE_INT_DISABLE/
 *                            DMA_CH2_DISABLE_INT_ENABLE,
 *                            DMA_CH2_ERROR_INT_DISABLE/
 *                            DMA_CH2_ERROR_INT_ENABLE,
 *                            DMA_CH2_COMPLETE_INT_DISABLE/
 *                            DMA_CH2_COMPLETE_INT_ENABLE,
 *                            DMA_CH2_COUNTER_INT_DISABLE/
 *                            DMA_CH2_COUNTER_INT_ENABLE,
 *                            DMA_CH2_START_INT_DISABLE/
 *                            DMA_CH2_START_INT_ENABLE,
 *                            DMA_CH2_DEST_WORD_SIZE_*, DMA_CH2_SRC_WORD_SIZE_*,
 *                            DMA_CH2_DEST_*, DMA_CH2_SRC_*, DMA_CH2_PRIORITY_*,
 *                            DMA_CH2_TRANSFER_*_TO_*,
 *                            DMA_CH2_DEST_ADDR_STATIC/DMA_CH2_DEST_ADDR_INC,
 *                            DMA_CH2_SRC_ADDR_STATIC/DMA_CH2_SRC_ADDR_INC,
 *                            DMA_CH2_ADDR_CIRC/DMA_CH2_ADDR_LIN,
 *                            DMA_CH2_DISABLE/DMA_CH2_ENABLE
 *                 - transferLength - Configure the DMA transfer length
 *                 - counterInt     - Configure when the counter interrupt will
 *                                    occur during the transfer
 *                 - srcAddr        - Base source address for the DMA transfer
 *                 - destAddr       - Base destination address for the DMA
 *                                    transfer
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Sys_DMA_Channel2_Config(uint32_t config,
                             uint32_t transferLength,
                             uint32_t counterInt,
                             uint32_t srcAddr,
                             uint32_t destAddr)
{
    /* Disable the DMA channel to ensure that it is safe to update the DMA
     * registers */
    DMA->CH2_CTRL0 = DMA_CH2_DISABLE;

    /* Setup the base addresses for the source and destination */
    DMA->CH2_SRC_BASE_ADDR = srcAddr;
    DMA->CH2_DEST_BASE_ADDR = destAddr;

    /* Setup the transfer length and transfer counter interrupt setting */
    DMA_CH2_CTRL1->TRANSFER_LENGTH_SHORT = (uint16_t) transferLength;
    DMA_CH2_CTRL1->COUNTER_INT_VALUE_SHORT = (uint16_t) counterInt;

    /* Configure the DMA channel */
    DMA->CH2_CTRL0 = (config & ((1U << DMA_CH2_CTRL0_BYTE_ORDER_Pos)
                               | (1U << DMA_CH2_CTRL0_DISABLE_INT_ENABLE_Pos)
                               | (1U << DMA_CH2_CTRL0_ERROR_INT_ENABLE_Pos)
                               | (1U << DMA_CH2_CTRL0_COMPLETE_INT_ENABLE_Pos)
                               | (1U << DMA_CH2_CTRL0_COUNTER_INT_ENABLE_Pos)
                               | (1U << DMA_CH2_CTRL0_START_INT_ENABLE_Pos)
                               | DMA_CH2_CTRL0_DEST_WORD_SIZE_Mask
                               | DMA_CH2_CTRL0_SRC_WORD_SIZE_Mask
                               | DMA_CH2_CTRL0_DEST_SELECT_Mask
                               | DMA_CH2_CTRL0_SRC_SELECT_Mask
                               | DMA_CH2_CTRL0_CHANNEL_PRIORITY_Mask
                               | DMA_CH2_CTRL0_TRANSFER_TYPE_Mask
                               | (1U << DMA_CH2_CTRL0_DEST_ADDR_INC_Pos)
                               | (1U << DMA_CH2_CTRL0_SRC_ADDR_INC_Pos)
                               | (1U << DMA_CH2_CTRL0_ADDR_MODE_Pos)
                               | (1U << DMA_CH2_CTRL0_ENABLE_Pos)));
}


/* ----------------------------------------------------------------------------
 * Function      : void Sys_DMA_Channel3_Config(uint32_t config,
 *                                              uint32_t transferLength,
 *                                              uint32_t counterInt,
 *                                              uint32_t srcAddr,
 *                                              uint32_t destAddr)
 * ----------------------------------------------------------------------------
 * Description   : Configure DMA channel 3 for a data transfer
 * Inputs        : - config - Configure the DMA transfer behavior; use
 *                            DMA_CH3_LITTLE_ENDIAN/DMA_CH3_BIG_ENDIAN,
 *                            DMA_CH3_DISABLE_INT_DISABLE/
 *                            DMA_CH3_DISABLE_INT_ENABLE,
 *                            DMA_CH3_ERROR_INT_DISABLE/
 *                            DMA_CH3_ERROR_INT_ENABLE,
 *                            DMA_CH3_COMPLETE_INT_DISABLE/
 *                            DMA_CH3_COMPLETE_INT_ENABLE,
 *                            DMA_CH3_COUNTER_INT_DISABLE/
 *                            DMA_CH3_COUNTER_INT_ENABLE,
 *                            DMA_CH3_START_INT_DISABLE/
 *                            DMA_CH3_START_INT_ENABLE,
 *                            DMA_CH3_DEST_WORD_SIZE_*, DMA_CH3_SRC_WORD_SIZE_*,
 *                            DMA_CH3_DEST_*, DMA_CH3_SRC_*, DMA_CH3_PRIORITY_*,
 *                            DMA_CH3_TRANSFER_*_TO_*,
 *                            DMA_CH3_DEST_ADDR_STATIC/DMA_CH3_DEST_ADDR_INC,
 *                            DMA_CH3_SRC_ADDR_STATIC/DMA_CH3_SRC_ADDR_INC,
 *                            DMA_CH3_ADDR_CIRC/DMA_CH3_ADDR_LIN,
 *                            DMA_CH3_DISABLE/DMA_CH3_ENABLE
 *                 - transferLength - Configure the DMA transfer length
 *                 - counterInt     - Configure when the counter interrupt will
 *                                    occur during the transfer
 *                 - srcAddr        - Base source address for the DMA transfer
 *                 - destAddr       - Base destination address for the DMA
 *                                    transfer
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Sys_DMA_Channel3_Config(uint32_t config,
                             uint32_t transferLength,
                             uint32_t counterInt,
                             uint32_t srcAddr,
                             uint32_t destAddr)
{
    /* Disable the DMA channel to ensure that it is safe to update the DMA
     * registers */
    DMA->CH3_CTRL0 = DMA_CH3_DISABLE;

    /* Setup the base addresses for the source and destination */
    DMA->CH3_SRC_BASE_ADDR = srcAddr;
    DMA->CH3_DEST_BASE_ADDR = destAddr;

    /* Setup the transfer length and transfer counter interrupt setting */
    DMA_CH3_CTRL1->TRANSFER_LENGTH_SHORT = (uint16_t) transferLength;
    DMA_CH3_CTRL1->COUNTER_INT_VALUE_SHORT = (uint16_t) counterInt;

    /* Configure the DMA channel */
    DMA->CH3_CTRL0 = (config & ((1U << DMA_CH3_CTRL0_BYTE_ORDER_Pos)
                               | (1U << DMA_CH3_CTRL0_DISABLE_INT_ENABLE_Pos)
                               | (1U << DMA_CH3_CTRL0_ERROR_INT_ENABLE_Pos)
                               | (1U << DMA_CH3_CTRL0_COMPLETE_INT_ENABLE_Pos)
                               | (1U << DMA_CH3_CTRL0_COUNTER_INT_ENABLE_Pos)
                               | (1U << DMA_CH3_CTRL0_START_INT_ENABLE_Pos)
                               | DMA_CH3_CTRL0_DEST_WORD_SIZE_Mask
                               | DMA_CH3_CTRL0_SRC_WORD_SIZE_Mask
                               | DMA_CH3_CTRL0_DEST_SELECT_Mask
                               | DMA_CH3_CTRL0_SRC_SELECT_Mask
                               | DMA_CH3_CTRL0_CHANNEL_PRIORITY_Mask
                               | DMA_CH3_CTRL0_TRANSFER_TYPE_Mask
                               | (1U << DMA_CH3_CTRL0_DEST_ADDR_INC_Pos)
                               | (1U << DMA_CH3_CTRL0_SRC_ADDR_INC_Pos)
                               | (1U << DMA_CH3_CTRL0_ADDR_MODE_Pos)
                               | (1U << DMA_CH3_CTRL0_ENABLE_Pos)));
}
