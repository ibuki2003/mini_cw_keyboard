#include <usbdrv.h>
#include <avr/io.h>

PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {
  0x05, 0x01, // USAGE_PAGE (Generic Desktop)
  0x09, 0x06, // USAGE (Keyboard)
  0xa1, 0x01, // COLLECTION (Application)
  0x05, 0x07, //   USAGE_PAGE (Keyboard)
  0x19, 0xe0, //   USAGE_MINIMUM (Keyboard LeftControl)
  0x29, 0xe7, //   USAGE_MAXIMUM (Keyboard Right GUI)
  0x15, 0x00, //   LOGICAL_MINIMUM (0)
  0x25, 0x01, //   LOGICAL_MAXIMUM (1)
  0x75, 0x01, //   REPORT_SIZE (1)
  0x95, 0x08, //   REPORT_COUNT (8)
  0x81, 0x02, //   INPUT (Data,Var,Abs)
  0x95, 0x01, //   REPORT_COUNT (1)
  0x75, 0x08, //   REPORT_SIZE (8)
  0x15, 0x00, //   LOGICAL_MINIMUM (0)
  0x25, 0x91, //   LOGICAL_MAXIMUM (145)
  0x19, 0x00, //   USAGE_MINIMUM (0)
  0x29, 0x91, //   USAGE_MAXIMUM (145)
  0x81, 0x00, //   INPUT (Data,Ary,Abs)
  0xc0        // END_COLLECTION
};
