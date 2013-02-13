/* ----------------------------------------------------------------------------
 * Copyright (c) 2009 - 2012 Semiconductor Components Industries, LLC (d/b/a ON
 * Semiconductor), All Rights Reserved
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor.
 * The terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * q32_usb.c
 * - USB interface controller hardware support code source file
 * ----------------------------------------------------------------------------
 * $Revision: 1.8 $
 * $Date: 2012/04/26 20:50:15 $
 * ------------------------------------------------------------------------- */

#include <q32.h>

/* ----------------------------------------------------------------------------
 * Function      : void Sys_USB_Initialize()
 * ----------------------------------------------------------------------------
 * Description   : Configure the USB interface controller into typical operation
 *                 enabling all interrupts and endpoints under DMA control. USB
 *                 endpoint pairing is enabled for endpoints (2, 3) and (4, 5).
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Sys_USB_Initialize()
{
    /* Enable the USB clock and puts the interface in to DMA controlled mode */
    Sys_USB_Config(USB_ENABLE | USB_CONTROLLER_DMA);

    /* Enable USB endpoint pairing for endpoint 2/3 & 4/5 */
    USB_SYS_CTRL3->EP_PAIRING_BYTE = USB_PAIR_IN_EP23_ENABLE_BYTE |
                          USB_PAIR_OUT_EP45_ENABLE_BYTE;

    /* Configures the USB memory addresses based on 64 bytes for EP0,2,3,4,5 */
    USB->BULK_OUT45_ADDR = (USB_BULK_OUT4_ADDR_VALUE |
                            USB_BULK_OUT5_ADDR_VALUE);
    USB->BULK_IN23_ADDR = (USB_BULK_IN2_ADDR_VALUE
                          | USB_BULK_IN3_ADDR_VALUE
                          | USB_BULK_OFFSET_ADDR_VALUE);

    /* Enable all USB interrupts */
    USB_INT_CTRL->IEN_BYTE = USB_RST_IEN_ENABLE_BYTE |
                   USB_SUS_IEN_ENABLE_BYTE |
                   USB_SETUPTKN_IEN_ENABLE_BYTE |
                   USB_SOF_IEN_ENABLE_BYTE |
                   USB_DAT_VALID_IEN_ENABLE_BYTE;

    /* Enable Bulk in interrupts 0 and 2 */
    USB_INT_CTRL->BULK_IN_IEN_BYTE = USB_BULK_IN_0_IEN_ENABLE_BYTE |
                           USB_BULK_IN_2_IEN_ENABLE_BYTE;
    /* Enable Bulk out interrupts 0 and 4 */
    USB_INT_CTRL->BULK_OUT_IEN_BYTE = USB_BULK_OUT_0_IEN_ENABLE_BYTE|
                            USB_BULK_OUT_4_IEN_ENABLE_BYTE;
}

/* ----------------------------------------------------------------------------
 * Function      : void Sys_USB_Set_EndpointByteCount(uint32_t EP,
 *                                                   Direction direction,
 *                                                   uint8_t count)
 * ----------------------------------------------------------------------------
 * Description   : Set the USB endpoint byte count
 * Inputs        : - EP         - USB in or out endpoint selection; use 0, 2, 3,
 *                                4 or 5
 *                 - direction  - USB endpoint direction; use USB_IN, USB_OUT
 *                 - count      - Endpoint byte count; range 0 - 64 bytes
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Sys_USB_Set_EndpointByteCount(uint32_t EP, Direction direction,
                                   uint8_t count)
{
    if (direction == USB_IN)
    {
        switch (EP)
        {
            case 0:
                USB_EP0_IN_CTRL->BYTE_COUNT_BYTE = count;
                break;
            case 2:
                USB_EP23_IN_CTRL->EP2_IN_BYTE_COUNT_BYTE = count;
                break;
            case 3:
                USB_EP23_IN_CTRL->EP3_IN_BYTE_COUNT_BYTE = count;
                break;
            default:
                break;
        }
    }
    else if (direction == USB_OUT)
    {
        switch (EP)
        {
            case 0:
                USB_EP0_OUT_CTRL->BYTE_COUNT_BYTE = count;
                break;
            case 4:
                USB_EP45_OUT_CTRL->EP4_OUT_BYTE_COUNT_BYTE = count;
                break;
            case 5:
                USB_EP45_OUT_CTRL->EP5_OUT_BYTE_COUNT_BYTE = count;
                break;
            default:
                break;
        }
    }
}

/* ----------------------------------------------------------------------------
 * Function      : uint8_t Sys_USB_Get_EndpointByteCount(uint32_t EP)
 * ----------------------------------------------------------------------------
 * Description   : Get the USB out endpoint byte count
 * Inputs        : EP       - USB out endpoint selection; use 0, 4 or 5
 * Outputs       : count    - Byte count value
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
uint8_t Sys_USB_Get_EndpointByteCount(uint32_t EP)
{
    switch (EP)
    {
        case 0:
            return USB_EP0_OUT_CTRL->BYTE_COUNT_BYTE;
        case 4:
            return USB_EP45_OUT_CTRL->EP4_OUT_BYTE_COUNT_BYTE;
        case 5:
            return USB_EP45_OUT_CTRL->EP5_OUT_BYTE_COUNT_BYTE;
        default:
            return 0;
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void Sys_USB_SendEndpoint(uint32_t EP, uint32_t size,
 *                                           uint8_t* data)
 * ----------------------------------------------------------------------------
 * Description   : Fills a USB in endpoint with size bytes
 * Inputs        : - EP     - USB in endpoint selection; use 0, 2 or 3
 *                 - size   - Number of bytes to fill the endpoint buffer with;
 *                            range 0 - 64 bytes
 *                 - data   - Pointer to the buffer where data will be copied
 *                            from
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Sys_USB_SendEndpoint(uint32_t EP, uint32_t size,
                          uint8_t* data)
{
    uint32_t address;
    int i;

    switch (EP)
    {
        case 0:
            address = USB_BULK_IN0_BUF_BASE;
            break;
        case 2:
            address = USB_BULK_IN2_BUF_BASE;
            break;
        case 3:
            address = USB_BULK_IN3_BUF_BASE;
            break;
        default:
            address = 0x0;
            break;
    }

    /* Copy data to the endpoint data buffer */
    for (i = 0; i < size; i++)
    {
        REG8_POINTER(address + i) = data[i];
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void Sys_USB_ReceiveEndpoint(uint32_t EP,
 *                                              uint32_t* size,
 *                                              uint8_t* data)
 * ----------------------------------------------------------------------------
 * Description   : Return the data received on the specified endpoint and size
 * Inputs        : EP       - USB out endpoint selection; use 0, 4 or 5
 * Outputs       : - size   - Number of bytes to be filled into data;
 *                            range 0 - 64 bytes
 *                 - data   - Pointer to the buffer where data will be stored
 * Assumptions   : data buffer is 64 bytes in size or larger
 * ------------------------------------------------------------------------- */
void Sys_USB_ReceiveEndpoint(uint32_t EP, uint32_t* size,
                             uint8_t* data)
{
    uint32_t address;
    int i;

    switch (EP)
    {
        case 0:
            address = USB_BULK_OUT0_BUF_BASE;
            *(size) = USB_EP0_OUT_CTRL->BYTE_COUNT_BYTE;
            break;
        case 4:
            address = USB_BULK_OUT4_BUF_BASE;
            *(size) = USB_EP45_OUT_CTRL->EP4_OUT_BYTE_COUNT_BYTE;
            break;
        case 5:
            address = USB_BULK_OUT5_BUF_BASE;
            *(size) = USB_EP45_OUT_CTRL->EP5_OUT_BYTE_COUNT_BYTE;
            break;
        default:
            address = 0x0;
            *(size) = 0x0;
            break;
    }

    /* Copy data to the ouput buffer */
    for (i = 0; i < (*size); i++)
    {
        data[i] = REG8_POINTER(address + i);
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void Sys_USB_Get_SetupBuffer(uint8_t* setupBuffer)
 * ----------------------------------------------------------------------------
 * Description   : Obtains the last received setup buffer information
 * Inputs        : None
 * Outputs       : setupBuffer  - 8 bytes of setup data
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Sys_USB_Get_SetupBuffer(uint8_t* setupBuffer)
{
    int i;
    
    for (i = 0; i < USB_SETUP_BUFFER_SIZE; i++)
    {
        setupBuffer[i] = READONLY_REG8_POINTER(USB_SETUP_DATA_BUF_BASE_0_BASE
                                               + i);
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void Sys_USB_StallEndpoint(uint32_t EP,
 *                                            Direction direction)
 * ----------------------------------------------------------------------------
 * Description   : Stalls the specified endpoint.
 * Inputs        : - EP         - USB in or out endpoint selection; use 0, 2, 3,
 *                                4 or 5
 *                 - direction  - USB endpoint direction; use USB_IN, USB_OUT
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void Sys_USB_StallEndpoint(uint32_t EP, Direction direction)
{
    if (direction == USB_IN)
    {
      switch (EP)
      {
          case 0:
              USB_EP0_IN_CTRL->EP0_CTRL_STALL_ALIAS = USB_EPSTALL_Mask;
              break;
          case 2:
              USB_EP23_IN_CTRL->EP2_IN_STALL_ALIAS = USB_EPSTALL_Mask;
              break;
          case 3:
              USB_EP23_IN_CTRL->EP3_IN_STALL_ALIAS = USB_EPSTALL_Mask;
              break;
          default:
              break;
      }
    }
    else if (direction == USB_OUT)
    {
      switch (EP)
      {
          case 4:
              USB_EP45_OUT_CTRL->EP4_OUT_STALL_ALIAS = USB_EPSTALL_Mask;
              break;
          case 5:
              USB_EP45_OUT_CTRL->EP5_OUT_STALL_ALIAS = USB_EPSTALL_Mask;
              break;
          default:
              break;
      }
    }

};
