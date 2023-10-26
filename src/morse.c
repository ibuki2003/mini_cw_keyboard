#include "morse.h"
#include <avr/pgmspace.h>

uint8_t morse_to_key(uint8_t morse, uint8_t len) {
  if (len == 0) return 0;
  if (len <= 6) {
    return pgm_read_byte(&(MORSE_TABLE[morse]));
  }

  if (len == 8 && morse == 255) return 0x2a; // backspace
  if (len == 5 && morse == 41) return 0x28; // enter (AR)
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
  [ 49] = 0x38, // /
  [ 55] = 0x24, // 7
  [ 59] = 0x25, // 8
  [ 61] = 0x26, // 9
  [ 62] = 0x27, // 0
  [ 75] = 0xb8, // ?
  [ 84] = 0x37, // .
  [ 96] = 0x2d, // -
  [105] = 0x33, // ;
  [106] = 0x9e, // !
  [114] = 0x36, // ,
};
