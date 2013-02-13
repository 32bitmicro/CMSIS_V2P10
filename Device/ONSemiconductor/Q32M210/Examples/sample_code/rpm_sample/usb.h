/* ----------------------------------------------------------------------------
 * Copyright (c) 2012 Semiconductor Components Industries, LLC
 * (d/b/a ON Semiconductor). All Rights Reserved.
 *
 * This code is the property of ON Semiconductor and may not be redistributed
 * in any form without prior written permission from ON Semiconductor. The
 * terms of use and warranty for this code are covered by contractual
 * agreements between ON Semiconductor and the licensee.
 * ----------------------------------------------------------------------------
 * usb.h
 * - USB header file for the RPM sample application.
 * ----------------------------------------------------------------------------
 * $Revision: 1.5 $
 * $Date: 2012/04/26 18:20:39 $
 * ------------------------------------------------------------------------- */

#ifndef USB_H
#define USB_H

/* ----------------------------------------------------------------------------
 * USB-Specific Defines
 * ------------------------------------------------------------------------- */

/* Setup commands */
#define USB_SC_GET_STATUS           0x00  /* Setup command: Get Status */
#define USB_SC_CLEAR_FEATURE        0x01  /* Setup command: Clear Feature */
#define USB_SC_RESERVED             0x02  /* Setup command: Reserved */
#define USB_SC_SET_FEATURE          0x03  /* Setup command: Set Feature */
#define USB_SC_SET_ADDRESS          0x05  /* Setup command: Set Address */
#define USB_SC_GET_DESCRIPTOR       0x06  /* Setup command: Get Descriptor */
#define USB_SC_SET_DESCRIPTOR       0x07  /* Setup command: Set Descriptor */
#define USB_SC_GET_CONFIGURATION    0x08  /* Setup command: Get Configuration */
#define USB_SC_SET_CONFIGURATION    0x09  /* Setup command: Set Configuration */
#define USB_SC_GET_INTERFACE        0x0A  /* Setup command: Get Interface */
#define USB_SC_SET_INTERFACE        0x0B  /* Setup command: Set Interface */
#define USB_SC_SYNC_FRAME           0x0C  /* Setup command: Sync Frame */
#define USB_SC_ANCHOR_LOAD          0xA0  /* Setup command: Anchor load */

/* Descriptor types */
#define USB_DT_DEVICE               0x01  /* Get descriptor: Device */
#define USB_DT_CONFIGURATION        0x02  /* Get descriptor: Configuration */
#define USB_DT_STRING               0x03  /* Get descriptor: String */
#define USB_DT_INTERFACE            0x04  /* Get descriptor: Interface */
#define USB_DT_ENDPOINT             0x05  /* Get descriptor: Endpoint */
#define USB_DT_DEVICE_QUALIFIER     0x06  /* Get descriptor: Device Qualifier */
#define USB_DT_OTHER_SPEED_CONFIGURATION    0x07    /* Get descriptor:
                                                 * Other Speed Configuration */
#define USB_DT_INTERFACE_POWER      0x08  /* Get descriptor: Interface Power */
#define USB_DT_HID                  0x21  /* Get descriptor: HID */
#define USB_DT_REPORT               0x22  /* Get descriptor: Report */

/* Class codes */
#define USB_CLS_HID                 0x03  /* HID (Human Interface Device) */
#define USB_CLS_VENDOR              0x00  /* Vendor specific */

/* Status types */
#define USB_ST_DEVICE               0x80  /* Get Status: Device */
#define USB_ST_INTERFACE            0x81  /* Get Status: Interface */
#define USB_ST_ENDPOINT             0x82  /* Get Status: End Point */

/* ----------------------------------------------------------------------------
 * USB-Descriptor Defines
 * ------------------------------------------------------------------------- */
#define DESC_DEVICE_LENGTH          0x12
#define DESC_DEVICE_LEN_DEFAULT     0x08
#define DESC_CONFIG_BASE_LENGTH     0x09
#define DESC_IFACE_LENGTH           0x09
#define DESC_ENDPOINT_LENGTH        0x07
#define DESC_HID_LENGTH             0x09
#define DESC_HID_REPORT_DESCRIPTOR_LENGTH   0x2A
#define DESC_CONFIG_LENGTH          (DESC_CONFIG_BASE_LENGTH + \
                                     DESC_IFACE_LENGTH + \
                                     2*DESC_ENDPOINT_LENGTH + \
                                     DESC_HID_LENGTH)
#define DESC_CONFIG_LEN_DEFAULT     0x09

#define VENDOR_ID                   0x0EE2  /* CFG: Vendor ID */
#define PRODUCT_ID                  0x2005  /* CFG: Product ID */
#define DEVICE_RELEASE              0x0100

#define LANGUAGE_INDEX              0x00
#define MANUFACTURER_INDEX          0x01
#define PRODUCT_INDEX               0x02

#define LANGUAGE_STRING_LENGTH      0x04
#define MANUFACTURER_STRING_LENGTH  0x22    /* CFG: Length of manufacturing
                                             * string descriptor */
#define PRODUCT_STRING_LENGTH       0x18    /* CFG: Length of product string
                                             * descriptor */

#define HID_REPORT_SIZE             32      /* CFG: Report size in bytes 
                                             * (1-63) */
#define HID_POLL_INTERVAL           0x10    /* CFG: Polling interval (1-255)
                                             * in frames (1ms) */

#define HIBYTE(x) (((x) >> 8) & 0xFF)
#define LOBYTE(x) (((x)     ) & 0xFF)

#define USB_WORD(x) LOBYTE(x), HIBYTE(x)

/* ----------------------------------------------------------------------------
 * USB-Status Defines
 * ------------------------------------------------------------------------- */
#define STATUS_LENGTH               0x02

/* ----------------------------------------------------------------------------
 * USB API
 * ----------------------------------------------------------------------------
 * Check whether the device is enuemrated by the host */
extern int USB_HID_IsEnumerated(void);

/* Callback - Called from interrupt when HID output report received
 * data is buffer of size HID_REPORT_SIZE. Must be implemented by 
 * application. */
extern void USB_HID_ReportReceived( uint8_t *data );

/* Send a HID report. Can be called from interrupt or main.
 * data is buffer of size HID_REPORT_SIZE */
extern void USB_HID_ReportSend( uint8_t *data );

/* Callback - Called when the PC has obtained the last report that was sent. */
extern void USB_Report_Sent(void);

#endif /*USB_H */
