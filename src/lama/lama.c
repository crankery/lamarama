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

int main()
{
    // we wanna use stdio (for debug)
    stdio_init_all();

    // start up the sound core
    sound_core_start();

    // UI loop
    while (true)
    {
        // poll input
        //   submit to messenger

        // update display? (maybe it can do something reactive too)
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