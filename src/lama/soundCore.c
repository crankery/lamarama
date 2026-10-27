#include "soundCore.h"

void soundCore();

void sound_core_start()
{
    multicore_launch_core1(soundCore);
}

float gain_smoothing_coeff(float sample_rate)
{
    if (sample_rate <= 0.0f)
        return 1.0f;

    return 1.0f - expf(-1.0f / (0.005f * sample_rate));
}

// prepare a dac sample buffer from the voice's buffer
// the dac is an MCP4822/MCP4821
// we are targetting channel A here (for the 2 channel one)
void prepare_dac_buffer(uint16_t *dac_buffer)
{
    for (int i = 0; i < SAMPLE_BUFFER_SIZE; i++)
    {
        // take each sample, clamp to -1.0..1.0
        float sample = buffer[i];
        sample = sample > 1.0f ? 1.0f : sample < -1.0f ? -1.0f
                                                       : sample;

        // push the -1 to 1 saple to dac range of 0..4096
        uint16_t dac_sample = (uint16_t)((sample + 1.0f) * 2047.5f);

        // 0x3000 = 0011 0000 0000 0000
        //          ││││ └────────────── 12-bit DAC value
        //          │││└── SHDN = 1 (active)
        //          ││└─── GA   = 1 (1× gain)
        //          │└──── BUF  = 0 (unbuffered reference)
        //          └───── A/B  = 0 (channel A)
        dac_buffer[i] = 0x3000 | dac_sample;
    }
}

void render_audio(float *buffer, uint32_t n)
{
    monk_voice_process(&voice, buffer, n);

    float target_gain = monk_voice_amplitude(&voice) * master_level;

    if (current_gain < 0.0f)
        current_gain = target_gain;

    for (uint32_t i = 0; i < n; i++)
    {
        current_gain += gain_coeff * (target_gain - current_gain);
        buffer[i] *= current_gain;
    }
}

void soundCoreInit()
{
    monk_voice_init(&voice, SAMPLE_RATE);
    gain_coeff = gain_smoothing_coeff(SAMPLE_RATE);
    sem_init(&free_buffers, 2, 2);
}

void dma_handler(void)
{
    // Acknowledge the DMA interrupt first

    // mark_buffer_free(completed_buffer);

    sem_release(&free_buffers);
}

void soundCore()
{
    soundCoreInit();

    // turn on a note for tests
    monk_voice_note_on(&voice, 440, 1.0f);

    while (true)
    {
        // Wait until a DAC buffer is available
        sem_acquire_blocking(&free_buffers);

        uint16_t *dac_buffer = get_free_dac_buffer();

        render_audio(buffer, SAMPLE_BUFFER_SIZE);
        prepare_dac_buffer(dac_buffer);

        submit_dac_buffer(dac_buffer);
    }
}