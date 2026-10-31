#ifndef SOUND_H
#define SOUND_H

#include <stdio.h>
#include <math.h>
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "lama.h"
#include "dac.h"
#include "voice.h"

typedef struct
{
    float vowel;
    bool note_active;
} SoundState;

typedef struct
{
    DacDebugStats dac;
    uint32_t blocks, last_fill_us, max_fill_us, over_budget;
    uint64_t total_fill_us;
} AudioDebugStats;

/* Coherent latest completed-block snapshot, safe on core 0 after start. */
AudioDebugStats sound_core_get_debug_stats(void);

/* Latest state, copied under a short lock; call after sound_core_start(). */
SoundState sound_core_get_state(void);
void sound_core_start();
float gain_smoothing_coeff(float sample_rate);
void prepare_dac_buffer(uint16_t *dac_buffer);
void render_audio(float *buffer, uint32_t n);
void soundCoreInit();

#endif // SOUND_H