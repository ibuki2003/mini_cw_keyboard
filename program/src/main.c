#include "avr/interrupt.h"
#include "util/delay.h"
#include <avr/io.h>
#include <avr/wdt.h>
#include <usbdrv.h>
#include <stdbool.h>

#include "morse.h"

#define LED_PIN 2
#define BTN_PIN 3
#define JMP_PIN 5 // jumper to select keyboard layout; GND for US, open for JIS
#define TOUCH_IN_PIN 7
#define TOUCH_OUT_PIN 6

#define BIT(x) (1 << (x))

#define TCNT1H_ ((uint8_t)(TCNT1 >> 8))

#define TOUCH_THRES 9

#define DASH_THRES 0x30 // 0x30 * 256 / (20M / 256) ~ 150 ms
#define BLANK_THRES 0x50

uint8_t reportBuffer[2];

int main() {
  // setup
  DDRA = BIT(LED_PIN) | BIT(TOUCH_OUT_PIN);
  PORTA = BIT(JMP_PIN); // pull-up for input pin
  // MCUCR &= ~BIT(PUD); // allowed by default

  TCCR1A = 0;
  TCCR1B = (0b100 << CS10); // 256 prescaler

  usbInit();
  usbDeviceConnect();
  sei();

  // key to send at next interrupt
  uint8_t k = 0;
  bool shift = 0;

  uint8_t h = 0; // hysteresis
  bool v = 0; // current btn state
  bool v_last = 0; // last btn state

  uint8_t t = 0; // timer state

  /**
    the value represents the morse code input

    empty: 0
    dot: v = v * 2 + 1
    dash: v = v * 2 + 2

    this enables us to use a single byte to represent the morse code input
    */
  uint8_t morse_value = 0;
  uint8_t morse_len = 0;

  uint8_t pulser_cnt = 0;

  while (1) {
    wdt_reset();
    usbPoll();

    if (k && usbInterruptIsReady()) {
      if (k == 0xff) k = 0;

      reportBuffer[0] = (k & BIT(7)) >> 6; // Left Shift key
      reportBuffer[1] = k & 0x7f; // msb for shift key
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

    if (t == 0 && TCNT1H_ > (v_last ? DASH_THRES : BLANK_THRES)) {
      t = 1;
      if (v_last == 0) {
        if (morse_value == 30) {// shift key: ----
          shift = !shift;
        } else {
          // commit char
          k = morse_to_key(
            morse_value,
            morse_len,
            (PINA & BIT(JMP_PIN)) // read jumper
          );
          // TODO: think of behaviour for non-alphabet keys
          if (shift && k && k <= 0x1D) {
            k |= BIT(7);
          }
        }
        morse_value = 0;
        morse_len = 0;
      }
    }

    if (v != v_last) {
      v_last = v;
      if (v == 0) {
        // falling edge
        if (t == 1) {
          // dash
          morse_value = morse_value * 2 + 2;
        } else {
          // dot
          morse_value = morse_value * 2 + 1;
        }
        ++morse_len;
      }
      TCNT1 = 0;
      t = 0;
    }
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

