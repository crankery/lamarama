#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/i2c.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/timer.h"
#include "hardware/watchdog.h"

#include "display.h"
#include "input.h"
#include "lama.h"
#include "soundCore.h"
#include "midi.h"
#include "messenger.h"
#include "menu.h"
#include "routing.h"
#include "ui_controls.h"
#include "cv_input.h"
#include "audio_debug.h"
#include "settings.h"
#include "calibration.h"

static void adc_updated(InputAdc adc, unsigned channel, uint16_t value)
{
    if (adc == INPUT_ADC_PICO && channel == 0)
    {
        cv_input_pitch(value);
        calibration_sample(CAL_PICO, value);
    }

    if (adc == INPUT_ADC_MCP3208 && channel == 4)
    {
        calibration_sample(CAL_MCP3208, value);
    }

    if (adc == INPUT_ADC_MCP3208)
    {
        routing_update(channel, value, time_us_64());
    }
}

int main()
{
    // Diagnostics use TX-only UART0 to the debug probe.
    stdio_init_all();
    audio_debug_init();

    start_messenger();
    menu_init();
    settings_load();
    ui_controls_init();
    midi_init();
    cv_input_init();
    input_set_update_handler(adc_updated);
    input_init();

    // start up the sound core
    sound_core_start();
    display_init();

    // UI loop
    while (true)
    {
        midi_poll();
        ui_controls_poll();
        poll_controls();
        cv_input_poll();
        menu_poll();
        display_poll();
        settings_poll(display_ready_for_flash());
        audio_debug_poll();
        tight_loop_contents();
    }
}

// // SPI initialisation. This example will use SPI at 1MHz.
// spi_init(SPI_PORT, 1000 * 1000);
// gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
// gpio_set_function(PIN_CS, GPIO_FUNC_SIO);
// gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
// gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

// // Chip select is active-low, so we'll initialise it to a driven-high state
// gpio_set_dir(PIN_CS, GPIO_OUT);
// gpio_put(PIN_CS, 1);
// // For more examples of SPI use see https://github.com/raspberrypi/pico-examples/tree/master/spi

// // I2C Initialisation. Using it at 400Khz.
// i2c_init(I2C_PORT, 400 * 1000);

// gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
// gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
// gpio_pull_up(I2C_SDA);
// gpio_pull_up(I2C_SCL);
// For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

// // Get a free channel, panic() if there are none
// int chan = dma_claim_unused_channel(true);

// // 8 bit transfers. Both read and write address increment after each
// // transfer (each pointing to a location in src or dst respectively).
// // No DREQ is selected, so the DMA transfers as fast as it can.

// dma_channel_config c = dma_channel_get_default_config(chan);
// channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
// channel_config_set_read_increment(&c, true);
// channel_config_set_write_increment(&c, true);

// dma_channel_configure(
//     chan,          // Channel to be configured
//     &c,            // The configuration we just created
//     dst,           // The initial write address
//     src,           // The initial read address
//     count_of(src), // Number of transfers; in this case each is 1 byte.
//     true           // Start immediately.
// );

// // We could choose to go and do something else whilst the DMA is doing its
// // thing. In this case the processor has nothing else to do, so we just
// // wait for the DMA to finish.
// dma_channel_wait_for_finish_blocking(chan);

// // The DMA has now copied our text from the transmit buffer (src) to the
// // receive buffer (dst), so we can print it out from there.
// puts(dst);

//     // PIO Blinking example
//     PIO pio = pio0;
//     uint offset = pio_add_program(pio, &blink_program);
//     printf("Loaded program at %d\n", offset);

// #ifdef PICO_DEFAULT_LED_PIN
//     blink_pin_forever(pio, 0, offset, PICO_DEFAULT_LED_PIN, 3);
// #else
//     blink_pin_forever(pio, 0, offset, 6, 3);
// #endif
// For more pio examples see https://github.com/raspberrypi/pico-examples/tree/master/pio

// Timer example code - This example fires off the callback after 2000ms
// add_alarm_in_ms(2000, alarm_callback, NULL, false);
// For more examples of timer use see https://github.com/raspberrypi/pico-examples/tree/master/timer

// Watchdog example code
// if (watchdog_caused_reboot())
// {
//     printf("Rebooted by Watchdog!\n");
//     // Whatever action you may take if a watchdog caused a reboot
// }

// Enable the watchdog, requiring the watchdog to be updated every 100ms or the chip will reboot
// second arg is pause on debug which means the watchdog will pause when stepping through code
// watchdog_enable(100, 1);

// You need to call this function at least more often than the 100ms in the enable call to prevent a reboot
// watchdog_update();