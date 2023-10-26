#include "avr/interrupt.h"
#include "util/delay.h"
#include <avr/io.h>
#include <avr/wdt.h>
#include <usbdrv.h>
#include <stdbool.h>

#define LED_PIN 2
#define BTN_PIN 3
#define TOUCH_IN_PIN 6
#define TOUCH_OUT_PIN 7

#define BIT(x) (1 << (x))

#define TOUCH_THRES 15

uint8_t reportBuffer[1];

int main() {
  // setup
  DDRA |= BIT(LED_PIN) | BIT(TOUCH_OUT_PIN);
  PORTA = 0;

  usbInit();
  usbDeviceConnect();
  sei();

  // key to send at next interrupt
  uint8_t k = 0;

  uint8_t h = 0; // hysteresis
  bool v = 0; // current btn state
  bool v_last = 0; // last btn state

  uint8_t pulser_cnt = 0;

  while (1) {
    wdt_reset();
    usbPoll();

    if (k && usbInterruptIsReady()) {
      if (k == 0xff) k = 0;

      reportBuffer[0] = k;
      usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
      k = k ? 0xff : 0; // release
    }


    // touch sensor
    // check every 16th time
    ++pulser_cnt; pulser_cnt &= 0x0f;
    if (pulser_cnt == 0) {
      if (!(PORTA & BIT(TOUCH_OUT_PIN))) {
        uint8_t rise_time_cnt = 1;
        PORTA |= BIT(TOUCH_OUT_PIN);
        while (rise_time_cnt <= TOUCH_THRES && !(PINA & BIT(TOUCH_IN_PIN))) {
          ++rise_time_cnt;
        }
        rise_time_cnt = rise_time_cnt > TOUCH_THRES;

        // hyseteresis
        if (v != rise_time_cnt) {
          ++h;
          if (h > 20) {
            h = 0;
            v = rise_time_cnt;
          }
        } else {
          h = 0;
        }

      } else {
        PORTA &= ~BIT(TOUCH_OUT_PIN);
      }
    }

    if (v) PORTA |= BIT(LED_PIN);
    else PORTA &= ~BIT(LED_PIN);

    if (v && !v_last) {
      k = 0x04; // A
    }
    v_last = v;
  }
}

static uchar idleRate = 0;

usbMsgLen_t usbFunctionSetup(uchar data[8]) {
  usbRequest_t *rq = (void *)data;

  if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {
    switch(rq->bRequest) {
      case USBRQ_HID_GET_REPORT:
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

