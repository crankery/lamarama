#ifndef _DAC_DOT_H
#define _DAC_DOT_H

#include <stdint.h>
#include "soundCore.h"
#include "lama.h"

static uint16_t dac_buffer_a[SAMPLE_BUFFER_SIZE];
static uint16_t dac_buffer_b[SAMPLE_BUFFER_SIZE];

uint16_t *get_free_dac_buffer();
void submit_dac_buffer(uint16_t *buffer);

#endif // _DAC_DOT_H