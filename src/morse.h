#include <stdint.h>
#include <avr/io.h>

uint8_t morse_to_key(uint8_t morse, uint8_t len);
extern const uint8_t MORSE_TABLE[128];
