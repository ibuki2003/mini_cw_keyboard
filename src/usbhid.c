#include <usbdrv.h>
#include <avr/io.h>

PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {
  0x05, 0x01, // USAGE_PAGE (Generic Desktop)
  0x09, 0x06, // USAGE (Keyboard)
  0xa1, 0x01, // COLLECTION (Application)
  0x95, 0x01, //   REPORT_COUNT (1)
  0x75, 0x08, //   REPORT_SIZE (8)
  0x15, 0x00, //   LOGICAL_MINIMUM (0)
  0x25, 0x7f, //   LOGICAL_MAXIMUM (127)
  0x05, 0x07, //   USAGE_PAGE (Keyboard)
  0x19, 0x00, //   USAGE_MINIMUM (0)
  0x29, 0x7f, //   USAGE_MAXIMUM (127)
  0x81, 0x00, //   INPUT (Data,Ary,Abs)
  0xc0        // END_COLLECTION
};
