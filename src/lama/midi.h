#ifndef LAMA_MIDI_H
#define LAMA_MIDI_H

#include <stdint.h>

#define MIDI_RX_PIN 21
/* One-based MIDI channel. */
#define MIDI_CHANNEL 1

void midi_init(void);
void midi_poll(void);
/* Core 0 only; also usable with a byte stream in host tests. */
void midi_receive_byte(uint8_t byte);
void midi_reset_parser(void);

#endif
