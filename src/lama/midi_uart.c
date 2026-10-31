#include "midi.h"
#include "messenger.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"

void midi_init(void)
{
    midi_reset_parser();
    uart_init(uart1, 31250);
    uart_set_hw_flow(uart1, false, false);
    uart_set_format(uart1, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(uart1, true);
    gpio_set_function(MIDI_RX_PIN, GPIO_FUNC_UART);
}

void midi_poll(void)
{
    /* Poll on core 0. The UI loop must service this frequently enough to
     * drain the UART FIFO (32 bytes is about 10 ms at MIDI baud). */
    while (uart_is_readable(uart1))
    {
        uint32_t data = uart_get_hw(uart1)->dr;
        if (data & 0xf00)
        {
            uart_get_hw(uart1)->rsr = 0;
            midi_reset_parser();
            add_midi_message(MONK_ALL_NOTES_OFF, 0);
        }
        else
        {
            midi_receive_byte((uint8_t)data);
        }
    }
}
