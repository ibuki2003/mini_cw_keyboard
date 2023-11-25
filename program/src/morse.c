#include "morse.h"
#include <avr/pgmspace.h>

const uint8_t MORSE_TABLE[];
const uint8_t MORSE_SYMBOLS_TABLE[][3];

uint8_t morse_to_key(uint8_t morse, uint8_t len, bool layout_jis) {
  if (len == 0) return 0;
  if (len <= 6) {
    uint8_t v = pgm_read_byte(&(MORSE_TABLE[morse]));
    if (v) return v;

    uint8_t (*p)[3] = &MORSE_SYMBOLS_TABLE[0];
    uint8_t k;
    while ((k = pgm_read_byte(&(*p)[0]))) {
      if (k == morse) {
        return pgm_read_byte(&((*p)[layout_jis ? 2 : 1]));
      }
      p++;
    }
  }

  if (len == 7 && morse == 136) return 0x21 | 0x80; // $
  if (len == 8 && morse == 255) return 0x2a; // backspace
  return 0;
}

PROGMEM const uint8_t MORSE_TABLE[128] = {
  [  1] = 0x08, // E
  [  2] = 0x17, // T
  [  3] = 0x0c, // I
  [  4] = 0x04, // A
  [  5] = 0x11, // N
  [  6] = 0x10, // M
  [  7] = 0x16, // S
  [  8] = 0x18, // U
  [  9] = 0x15, // R
  [ 10] = 0x1a, // W
  [ 11] = 0x07, // D
  [ 12] = 0x0e, // K
  [ 13] = 0x0a, // G
  [ 14] = 0x12, // O
  [ 15] = 0x0b, // H
  [ 16] = 0x19, // V
  [ 17] = 0x09, // F
  [ 19] = 0x0f, // L
  [ 21] = 0x13, // P
  [ 22] = 0x0d, // J
  [ 23] = 0x05, // B
  [ 24] = 0x1b, // X
  [ 25] = 0x06, // C
  [ 26] = 0x1c, // Y
  [ 27] = 0x1d, // Z
  [ 28] = 0x14, // Q
  [ 31] = 0x22, // 5
  [ 32] = 0x21, // 4
  [ 34] = 0x20, // 3
  [ 38] = 0x1f, // 2
  [ 46] = 0x1e, // 1
  [ 47] = 0x23, // 6
  [ 55] = 0x24, // 7
  [ 59] = 0x25, // 8
  [ 61] = 0x26, // 9
  [ 62] = 0x27, // 0

  // common symbols
  [ 49] = 0x38, // /
  [ 75] = 0x38 | 0x80, // ?
  [ 84] = 0x37, // .
  [ 96] = 0x2d, // -
  [105] = 0x33, // ;
  [106] = 0x1e | 0x80, // !
  [114] = 0x36, // ,

  // special keys
  [ 18] = 0x2c, // SPC: ..--
  [ 20] = 0x28, // Return: .-.-
  [ 33] = 0x2a, // BS: ...-.
};

// key-layout specific symbols
PROGMEM const uint8_t MORSE_SYMBOLS_TABLE[][3] = {
  // idx, US, JIS
  { 39, 0x24 | 0x80, 0x23 | 0x80 }, // &
  { 41, 0x2e | 0x80, 0x33 | 0x80 }, // +
  { 48, 0x2e       , 0x2d | 0x80 }, // =
  { 53, 0x26 | 0x80, 0x25 | 0x80 }, // (
  {108, 0x27 | 0x80, 0x26 | 0x80 }, // )
  { 76, 0x2d | 0x80, 0x87        }, // _ NOTE: JIS key 0x87 needs special support.
  { 93, 0x34       , 0x24 | 0x80 }, // '
  { 81, 0x34 | 0x80, 0x1f | 0x80 }, // "
  { 89, 0x1f | 0x80, 0x2f        }, // @
  {119, 0x33 | 0x80, 0x34        }, // :
  {0, 0, 0}, // end
};
