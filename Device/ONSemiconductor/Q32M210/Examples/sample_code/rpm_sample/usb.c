/* ----------------------------------------------------------------------------
 * Copyright (c) 2012 Semiconductor Components Industries, LLC
 * (d/b/a ON Semiconductor). All Rights Reserved.
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor. The
 * terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * usb.c
 * - USB source file for the RPM sample application.
 * ----------------------------------------------------------------------------
 * $Revision: 1.6 $
 * $Date: 2012/04/26 20:45:55 $
 * ------------------------------------------------------------------------- */

#include <q32m210.h>
#include <string.h>
#include "usb.h"
#include "globals.h"

/* ----------------------------------------------------------------------------
 * Device Descriptor
 * ------------------------------------------------------------------------- */
static uint8_t descDevice[DESC_DEVICE_LENGTH] =
{
     DESC_DEVICE_LENGTH,                 /* Descriptor length (bLength) */
     USB_DT_DEVICE,                      /* Descriptor type */
     USB_WORD(0x0110),                   /* Specification version */
     0x00,                               /* Device Class */
     0x00,                               /* Device SubClass */
     0x00,                               /* Device Protocol */
     EP_BUFFER_SIZE,                     /* Maximum packet size */
     USB_WORD(VENDOR_ID),                /* Vendor ID */
     USB_WORD(PRODUCT_ID),               /* Product ID */
     USB_WORD(DEVICE_RELEASE),           /* Device Release number */
     MANUFACTURER_INDEX,                 /* Manufacturer string index */
     PRODUCT_INDEX,                      /* Product string index */
     0x00,                               /* Serial Number string index */
     0x01                                /* Number of possible configurations */
};

/* ----------------------------------------------------------------------------
 * Descriptor Configuration
 * ------------------------------------------------------------------------- */
static uint8_t descConfiguration[DESC_CONFIG_LENGTH] =
{
    /* Configuration descriptor */
    DESC_CONFIG_BASE_LENGTH,/* Size of descriptor, in bytes */
    USB_DT_CONFIGURATION,   /* Configuration descriptor type */
    USB_WORD(DESC_CONFIG_LENGTH), /* Total length of data for configuration */
    0x01,                   /* Number of interfaces */
    0x01,                   /* Configuration value */
    0x00,                   /* Index of string descriptor */
    0xA0,                   /* Configuration characteristics */
    0x00,                   /* Max power requirement */

    /* Standard Interface Descriptor */
    DESC_IFACE_LENGTH,      /* Size of descriptor */
    USB_DT_INTERFACE,       /* Interface descriptor type */
    0x00,                   /* Interface number */
    0x00,                   /* Alternate setting */
    0x02,                   /* Number of endpoints */
    USB_CLS_HID,            /* Interface class */
    0x00,                   /* Interface sub-class (optional) */
    0x00,                   /* Interface protocol (optional) */
    0x00,                   /* Index of string descriptor */

    /* HID Descriptor */
    DESC_HID_LENGTH,        /* Size of this descriptor */
    USB_DT_HID,             /* HID descriptor type */
    USB_WORD(0x1010),       /* HID Version */
    0x00,                   /* Country code */
    0x01,                   /* Number of HID descriptors */
    USB_DT_REPORT,          /* Descriptor type */
    USB_WORD(DESC_HID_REPORT_DESCRIPTOR_LENGTH),
                            /* Total report descriptor length
     * Standard Endpoint Descriptor */
    DESC_ENDPOINT_LENGTH,   /* Size of this descriptor */
    USB_DT_ENDPOINT,        /* Endpoint descriptor type */
    0x04,                   /* Endpoint address (EP4, OUT direction) */
    0x03,                   /* Endpoint attributes (0x3 = interrupt, 
                             *                      0x2 = bulk) */
    EP_BUFFER_SIZE, 0x00,   /* Maximum packet size (see pg 271 of 
                             * USB spec 2.0) */
    HID_POLL_INTERVAL,      /* Interval for polling endpoint for data 
                             * transfers */

    /* Standard Endpoint Descriptor */
    DESC_ENDPOINT_LENGTH,   /* Size of this descriptor */
    USB_DT_ENDPOINT,        /* Endpoint descriptor type */
    0x82,                   /* Endpoint address (EP2, IN direction) */
    0x03,                   /* Endpoint attributes (0x3 = interrupt, 
                             *                      0x2 = bulk) */
    EP_BUFFER_SIZE, 0x00,   /* Maximum packet size (see pg 271 of 
                             * USB spec 2.0) */
    HID_POLL_INTERVAL,      /* Interval for polling endpoint for data 
                             * transfers */
};

static uint8_t descHIDReport[DESC_HID_REPORT_DESCRIPTOR_LENGTH] = {
    /* HID Report Descriptor
     * Input report - 64 bytes vendor-defined usage */
    0x06, 0xFF, 0xFF, /* USAGE_PAGE       0xFFFF */
    0x09, 0xFF,       /* USAGE            0xFF */
    0xA1, 0x01,       /* MAIN_COLLECTION  APPLICATION */

    0x06, 0xFF, 0xFF, /*   USAGE_PAGE     0xFFFF */
    0x19, 0x80,       /*   USAGE_MIN      0x80 */
    0x29, 0x7F,       /*   USAGE_MAX      0x7F */
    0x15, 0x00,       /*   LOGICAL_MIN    0x00 */
    0x25, 0x7F,       /*   LOGICAL_MAX    0x7F */
    0x75, 0x08,       /*   REPORT_SIZE    8 */
    0x95, HID_REPORT_SIZE,/*REPORT_COUNT  HID_REPORT_SIZE */
    0x81, 0x02,       /*   INPUT          DATA | VARIABLE | ABSOLUTE */

    0x06, 0xFF, 0xFF, /*   USAGE_PAGE     0xFFFF */
    0x19, 0x80,       /*   USAGE_MIN      0x80 */
    0x29, 0x7F,       /*   USAGE_MAX      0x7F */
    0x15, 0x00,       /*   LOGICAL_MIN    0x00 */
    0x25, 0x7F,       /*   LOGICAL_MAX    0x7F */
    0x75, 0x08,       /*   REPORT_SIZE    8 */
    0x95, HID_REPORT_SIZE,/*REPORT_COUNT  HID_REPORT_SIZE */
    0x91, 0x02,       /*   OUTPUT         DATA | VARIABLE | ABSOLUTE */

    0xC0,             /* END_COLLECTION */
};

/* ----------------------------------------------------------------------------
 * String Descriptors
 * ------------------------------------------------------------------------- */
static uint8_t descLanguageString[LANGUAGE_STRING_LENGTH] =
{
    /* Default Language String */
    LANGUAGE_STRING_LENGTH,             /* Length */
    USB_DT_STRING,                      /* Descriptor type */
    USB_WORD(0x0409),                   /* Language (English_United_States) */
};

static uint8_t descManufacturerString[MANUFACTURER_STRING_LENGTH] =
{
    /* CFG: Manufacturer String Descriptor */
    MANUFACTURER_STRING_LENGTH,         /* Length */
    USB_DT_STRING,                      /* Descriptor type */
    'O', 0x0,
    'N', 0x0,
    ' ', 0x0,
    'S', 0x0,
    'e', 0x0,
    'm', 0x0,
    'i', 0x0,
    'c', 0x0,
    'o', 0x0,
    'n', 0x0,
    'd', 0x0,
    'u', 0x0,
    'c', 0x0,
    't', 0x0,
    'o', 0x0,
    'r', 0x0,
};

static uint8_t descProductString[PRODUCT_STRING_LENGTH] =
{
    /* CFG: Product String Descriptor */
    PRODUCT_STRING_LENGTH,              /* Length */
    USB_DT_STRING,                      /* Descriptor type */
    'Q', 0x0,
    '3', 0x0,
    '2', 0x0,
    'M', 0x0,
    '2', 0x0,
    '1', 0x0,
    '0', 0x0,
    ' ', 0x0,
    'R', 0x0,
    'P', 0x0,
    'M', 0x0
};

/* ----------------------------------------------------------------------------
 * Device status
 * ------------------------------------------------------------------------- */
static uint8_t statusDevice[] =
{
    0x03, 0x00,
};

/* ----------------------------------------------------------------------------
 * Interface status
 * ------------------------------------------------------------------------- */
static uint8_t statusInterface[] =
{
    0x00, 0x00,
};

/* ----------------------------------------------------------------------------
 * Global USB state
 * ------------------------------------------------------------------------- */
static uint8_t s_configuration = 0;

typedef enum
{
    NO_REPORT_IN_FLIGHT,
    REPORT_IN_FLIGHT,
    REPORT_BUFFERED
} enumSentReportStatus;

static enumSentReportStatus s_sentReportStatus = NO_REPORT_IN_FLIGHT;
static uint8_t s_outgoingReportBuffer[HID_REPORT_SIZE];

/* ----------------------------------------------------------------------------
 * Function prototypes
 * ------------------------------------------------------------------------- */
void send_next_extended_buffer_packet(void);

/* ----------------------------------------------------------------------------
 * Function      : void USB_WAKEUP_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : Clears the USB status on wakeup to ready it for the next
 *                 interrupt
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void USB_WAKEUP_IRQHandler(void)
{
    /* Reset flag to start enumeration parsing */
    Sys_USB_Clear_Interrupt(USB_RST_IRQ_CLEAR_BYTE | USB_SUS_IRQ_CLEAR_BYTE);
}

/* ----------------------------------------------------------------------------
 * Function      : void USB_SDAV_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : Enumerates device and responds to control transfers
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void USB_SDAV_IRQHandler(void)
{
    static uint8_t alternate_setting = 0;
    uint8_t usb_setup_buffer[8];
    uint8_t request_type;
    uint8_t request;
    uint8_t value_L;
    uint8_t value_H;
    uint8_t length_L;

    /* Clear SETUP data valid interrupt request */
    Sys_USB_Clear_Interrupt(USB_DAT_VALID_IRQ_CLEAR_BYTE);

    /* Store the 8 bytes of Setup data */
    Sys_USB_Get_SetupBuffer(usb_setup_buffer);

    /* Obtain current setup packet data */
    request_type = usb_setup_buffer[0];
    request = usb_setup_buffer[1];
    value_L = usb_setup_buffer[2];
    value_H = usb_setup_buffer[3];
    length_L = usb_setup_buffer[6];

    /* Respond to descriptor, configuration, settings, etc. requests */
    switch (request)
    {
      case USB_SC_GET_STATUS:
        switch (request_type)
        {
          case USB_ST_DEVICE:
            Sys_USB_ClearHandshakeNAK();
            Sys_USB_SendEndpoint(0, STATUS_LENGTH, statusDevice);
            Sys_USB_Set_EndpointByteCount(0, USB_IN, STATUS_LENGTH);
            Sys_USB_ClearHandshakeNAK();
            break;
          case USB_ST_INTERFACE:
            Sys_USB_ClearHandshakeNAK();
            Sys_USB_SendEndpoint(0, STATUS_LENGTH, statusInterface);
            Sys_USB_Set_EndpointByteCount(0, USB_IN, STATUS_LENGTH);
            Sys_USB_ClearHandshakeNAK();
            break;
          case USB_ST_ENDPOINT:
          default:
            Sys_USB_StallEndpoint(0, USB_IN);
            break;
        }
        break;
      case USB_SC_GET_DESCRIPTOR:
        switch (value_H)
        {
          case USB_DT_DEVICE:
            if (length_L == DESC_DEVICE_LENGTH)
            {
                Sys_USB_SendEndpoint(0, DESC_DEVICE_LENGTH, descDevice);
                Sys_USB_Set_EndpointByteCount(0, USB_IN, DESC_DEVICE_LENGTH);
                Sys_USB_ClearHandshakeNAK();
            }
            else
            {
                Sys_USB_SendEndpoint(0, DESC_DEVICE_LEN_DEFAULT, descDevice);
                Sys_USB_Set_EndpointByteCount(0, USB_IN,
                                              DESC_DEVICE_LEN_DEFAULT);
                Sys_USB_ClearHandshakeNAK();
            }
            break;
          case USB_DT_CONFIGURATION:
            if (length_L == DESC_CONFIG_LEN_DEFAULT)
            {
                Sys_USB_SendEndpoint(0,
                                     DESC_CONFIG_LEN_DEFAULT,
                                     descConfiguration);
                Sys_USB_Set_EndpointByteCount(0, USB_IN,
                                              DESC_CONFIG_LEN_DEFAULT);
                Sys_USB_ClearHandshakeNAK();
            }
            else
            {
                Sys_USB_SendEndpoint(0,
                                     DESC_CONFIG_LENGTH,
                                     descConfiguration);
                Sys_USB_Set_EndpointByteCount(0, USB_IN, DESC_CONFIG_LENGTH);
                Sys_USB_ClearHandshakeNAK();
            }
            break;
          case USB_DT_STRING:
            if (value_L == LANGUAGE_INDEX)
            {
                Sys_USB_SendEndpoint(0,
                                     LANGUAGE_STRING_LENGTH,
                                     descLanguageString);
                Sys_USB_Set_EndpointByteCount(0, USB_IN,
                                              LANGUAGE_STRING_LENGTH);
                Sys_USB_ClearHandshakeNAK();
            }
            else if (value_L == MANUFACTURER_INDEX)
            {
                Sys_USB_SendEndpoint(0,
                                     MANUFACTURER_STRING_LENGTH,
                                     descManufacturerString);
                Sys_USB_Set_EndpointByteCount(0, USB_IN,
                                              MANUFACTURER_STRING_LENGTH);
                Sys_USB_ClearHandshakeNAK();
            }
            else if (value_L == PRODUCT_INDEX)
            {
                Sys_USB_SendEndpoint(0,
                                     PRODUCT_STRING_LENGTH,
                                     descProductString);
                Sys_USB_Set_EndpointByteCount(0, USB_IN, PRODUCT_STRING_LENGTH);
                Sys_USB_ClearHandshakeNAK();
            }
            break;
          case USB_DT_REPORT:
            if (value_L == 0) {
                Sys_USB_SendEndpoint(0,
                                     DESC_HID_REPORT_DESCRIPTOR_LENGTH,
                                     descHIDReport);
                Sys_USB_Set_EndpointByteCount(0, USB_IN,
                                              DESC_HID_REPORT_DESCRIPTOR_LENGTH);
                Sys_USB_ClearHandshakeNAK();
            }
            break;
          case USB_DT_INTERFACE:
          case USB_DT_ENDPOINT:
          case USB_DT_DEVICE_QUALIFIER:
          case USB_DT_OTHER_SPEED_CONFIGURATION:
          case USB_DT_INTERFACE_POWER:
          case USB_DT_HID:
          default:
            Sys_USB_StallEndpoint(0, USB_IN);
        }
        break;
      case USB_SC_SET_ADDRESS:
        Sys_USB_ClearHandshakeNAK();
        break;
      case USB_SC_GET_CONFIGURATION:
        Sys_USB_SendEndpoint(0, 1, &s_configuration);
        Sys_USB_Set_EndpointByteCount(0, USB_IN, 1);
        Sys_USB_ClearHandshakeNAK();
        break;
      case USB_SC_SET_CONFIGURATION:
        s_configuration = value_L;
        Sys_USB_ClearHandshakeNAK();
        break;
      case USB_SC_GET_INTERFACE:
        Sys_USB_SendEndpoint(0, 1, &alternate_setting);
        Sys_USB_Set_EndpointByteCount(0, USB_IN, 1);
        Sys_USB_ClearHandshakeNAK();
        break;
      case USB_SC_SET_INTERFACE:
        alternate_setting = value_L;
        Sys_USB_ClearHandshakeNAK();
        break;
      case USB_SC_CLEAR_FEATURE:
        Sys_USB_ClearHandshakeNAK();
        break;
      case USB_SC_RESERVED:
        Sys_USB_ClearHandshakeNAK();
        break;
      case USB_SC_SET_FEATURE:
      case USB_SC_SET_DESCRIPTOR:
      case USB_SC_SYNC_FRAME:
      case USB_SC_ANCHOR_LOAD:
      default:
        Sys_USB_StallEndpoint(0, USB_IN);
        break;
    }
}

/* ----------------------------------------------------------------------------
 * Function      : void USB_SOF_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : Clears the USB status on start-of-frame (SOF) to ready it for
 *                 the next interrupt
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void USB_SOF_IRQHandler(void)
{
    /* Clear the Start-Of-Frame interrupt request */
    Sys_USB_Clear_Interrupt(USB_SOF_IRQ_CLEAR_BYTE);
}

/* ----------------------------------------------------------------------------
 * Function      : void USB_RESET_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : Clears the USB status on reset. Ready and wait for endpoint 
 *                 4 to receive transfer information.
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void USB_RESET_IRQHandler(void)
{
    s_configuration = 0;

    /* Clear the USB reset interrupt request */
    Sys_USB_Clear_Interrupt(USB_RST_IRQ_CLEAR_BYTE);

    /* Arm EP4 to receive data.  Call this twice to arm both EP4/EP5
     * for double buffering. */
    Sys_USB_Set_EndpointByteCount(4, USB_OUT, EP_BUFFER_SIZE);
    Sys_USB_Set_EndpointByteCount(4, USB_OUT, EP_BUFFER_SIZE);
}

/* ----------------------------------------------------------------------------
 * Function      : void USB_SUTOK_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : Clears the USB status on receiving setup token (SUTOK) to
 *                 ready it for the next interrupt
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void USB_SUTOK_IRQHandler(void)
{
    /* Clear the SETUP token interrupt request */
    Sys_USB_Clear_Interrupt(USB_SETUPTKN_IRQ_CLEAR_BYTE);
}

/* ----------------------------------------------------------------------------
 * Function      : void USB_SUSPEND_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : Clears the USB status on suspend to ready it for the next
 *                 interrupt
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void USB_SUSPEND_IRQHandler(void)
{
    /* Clear the USB suspend interrupt request */
    Sys_USB_Clear_Interrupt(USB_SUS_IRQ_CLEAR_BYTE);
}

/* ----------------------------------------------------------------------------
 * Function      : void USB_EP0IN_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : Clears the USB EP0 In status bit
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void USB_EP0IN_IRQHandler(void)
{
    /* Clears the Bulk 0 In IRQ */
    Sys_USB_Clear_BulkInInterrupt(USB_BULK_IN_0_IRQ_CLEAR_BYTE);
}

/* ----------------------------------------------------------------------------
 * Function      : void USB_EP0OUT_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : Clears the USB EP0 Out status bit
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void USB_EP0OUT_IRQHandler(void)
{
    /* Clears the Bulk 0 Out IRQ */
    Sys_USB_Clear_BulkOutInterrupt(USB_BULK_OUT_0_IRQ_CLEAR_BYTE);
}

/* ----------------------------------------------------------------------------
 * Function      : void USB_EP2IN_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : Clears the USB EP2 In status bit
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void USB_EP2IN_IRQHandler(void)
{
    /* Clears the Bulk 2 In IRQ */
    Sys_USB_Clear_BulkInInterrupt(USB_BULK_IN_2_IRQ_CLEAR_BYTE);
    
    switch (s_sentReportStatus)
    {
    case REPORT_IN_FLIGHT:
        s_sentReportStatus = NO_REPORT_IN_FLIGHT;
        /* Tell the application that the report has been sent to the PC. */
        USB_Report_Sent();
        break;
    case REPORT_BUFFERED:
        s_sentReportStatus = REPORT_IN_FLIGHT;
        Sys_USB_SendEndpoint(2, HID_REPORT_SIZE, s_outgoingReportBuffer);
        Sys_USB_Set_EndpointByteCount(2, USB_IN, HID_REPORT_SIZE);
        break;
    }   
}

/* ----------------------------------------------------------------------------
 * Function      : void USB_EP4OUT_IRQHandler(void)
 * ----------------------------------------------------------------------------
 * Description   : Receives transfer information and initiates the read/write
 *                 transfer
 * Inputs        : None
 * Outputs       : None
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
void USB_EP4OUT_IRQHandler(void)
{
    uint8_t ep4_data[EP_BUFFER_SIZE];
    uint32_t size;

    /* Clears the Bulk 4 Out IRQ */
    Sys_USB_Clear_BulkOutInterrupt(USB_BULK_OUT_4_IRQ_CLEAR_BYTE);

    /* Retrieve EP4 data */
    Sys_USB_ReceiveEndpoint(4, &size, ep4_data);

    if (size > 0)
    {
        /* Pass data to application */
        USB_HID_ReportReceived(ep4_data);
    }
    
    /* Arm EP4 to receive data */
    Sys_USB_Set_EndpointByteCount(4, USB_OUT, EP_BUFFER_SIZE);

}

/* ----------------------------------------------------------------------------
 * Function      : void USB_HID_ReportSend(uint8_t *data)
 * ----------------------------------------------------------------------------
 * Description   : Set the data for the next HID report and tell USB controller
 *                 that there is a report available.
 * Inputs        : data     - Content of HID report.
 * Outputs       : None
 * Assumptions   : data is exactly HID_REPORT_SIZE bytes long
 * ------------------------------------------------------------------------- */
void USB_HID_ReportSend(uint8_t *data)
{
    /* Temporarily disable the USB_EP2IN interrupt to avoid a race condition
     * as we test and update s_sentReportStatus below. */
    Sys_NVIC_Int_DisableInt(USB_EP2IN_IRQn);
    
    switch (s_sentReportStatus)
    {
    case REPORT_IN_FLIGHT:
    case REPORT_BUFFERED:
        /* If there is a report in flight, overwrite the buffered
         * report regardless of whether there is already a report there. */
        s_sentReportStatus = REPORT_BUFFERED;
        memcpy(s_outgoingReportBuffer, data, HID_REPORT_SIZE);
        break;
    case NO_REPORT_IN_FLIGHT:
        s_sentReportStatus = REPORT_IN_FLIGHT;
        Sys_USB_SendEndpoint(2, HID_REPORT_SIZE, data);
        Sys_USB_Set_EndpointByteCount(2, USB_IN, HID_REPORT_SIZE);
        break;
    }

    /* Re-enable the USB_EP2IN interrupt. */
    Sys_NVIC_Int_EnableInt(USB_EP2IN_IRQn);
}

/* ----------------------------------------------------------------------------
 * Function      : void USB_HID_IsEnumerated()
 * ----------------------------------------------------------------------------
 * Description   : Determine whether the device has been enumerated by the host
 * Inputs        : None
 * Outputs       : 1 if the device has been enumerated, 0 otherwise
 * Assumptions   : None
 * ------------------------------------------------------------------------- */
int USB_HID_IsEnumerated()
{
    return s_configuration != 0;
}
