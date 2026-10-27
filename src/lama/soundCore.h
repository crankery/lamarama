#ifndef SOUND_H
#define SOUND_H

#include <stdio.h>
#include <math.h>
#include "pico/multicore.h"
#include "pico/sem.h"
#include "pico/stdlib.h"
#include "lama.h"
#include "dac.h"
#include "voice.h"

static MonkVoice voice;
static float buffer[SAMPLE_BUFFER_SIZE];

static semaphore_t free_buffers;

static float current_gain = -1.0f;
static float gain_coeff;
static float master_level = 1.0f;

void sound_core_start();
float gain_smoothing_coeff(float sample_rate);
void prepare_dac_buffer(uint16_t *dac_buffer);
void render_audio(float *buffer, uint32_t n);
void soundCoreInit();
void dma_handler(void);

#endif // SOUND_H