#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>

uint8_t morse_to_key(uint8_t morse, uint8_t len, bool layout_jis);
