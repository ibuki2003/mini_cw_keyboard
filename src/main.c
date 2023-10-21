#include "avr/interrupt.h"
#include "util/delay.h"
#include <avr/io.h>
#include <avr/wdt.h>
#include <usbdrv.h>

#define KEY_A 0x04

uint8_t reportBuffer[1];

int main() {
  // setup
  DDRC = 0b10;
  PORTC = 0;

  usbInit();
  usbDeviceConnect();
  sei();

  // main
  uint8_t v = 0;

  while (1) {
    wdt_reset();
    usbPoll();

    if (PINC & 0b1) {
      PORTC |= 0b10;
    } else {
      PORTC &= ~0b10;
    }

    if ((PINC & 0b1) != v) {
      v = PINC & 0b1;

      if (usbInterruptIsReady()) {
        reportBuffer[0] = v ? KEY_A : 0;
        usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
      }
    }
  }
}

static uchar idleRate = 0;

usbMsgLen_t usbFunctionSetup(uchar data[8]) {
  usbRequest_t *rq = (void *)data;

  if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {
    switch(rq->bRequest) {
      case USBRQ_HID_GET_REPORT:
        reportBuffer[0] = (PINC & 0b1) ? KEY_A : 0;
        usbMsgPtr = reportBuffer;
        return sizeof(reportBuffer);
      case USBRQ_HID_GET_IDLE:
        usbMsgPtr = &idleRate;
        return 1;
      case USBRQ_HID_SET_IDLE:
        idleRate = rq->wValue.bytes[1];
    }
  }
  return 0;
}

