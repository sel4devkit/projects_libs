/*
 * SPDX-License-Identifier: BSD-2-Clause
 */

#define USB_HOST0_XHCI_PADDR     0x38100000
#define USB_HOST0_XHCI_SIZE      0x10000
#define USB_HOST1_XHCI_PADDR     0x38200000
#define USB_HOST1_XHCI_SIZE      0x10000

#define USB_HOST0_IRQ            40
#define USB_HOST1_IRQ            41

enum usb_host_id {
    USB_HOST0,
    USB_HOST1,
    USB_NHOSTS,
    USB_HOST_DEFAULT = USB_HOST0
};

enum usb_otg_id {
    USB_NOTGS,
    USB_OTG_DEFAULT = -1
};
