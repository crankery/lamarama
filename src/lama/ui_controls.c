#include "ui_controls.h"
#include "menu.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "hardware/sync.h"
#include "pico/stdlib.h"

static volatile int pending_steps;
static unsigned previous_ab;
static int partial_step;

static struct Button
{
    uint pin;
    bool raw, stable;
    uint64_t changed;
} buttons[2];

static unsigned read_encoder(void)
{
    uint32_t pins = gpio_get_all();
    return ((pins >> ENCODER_A) & 1) * 2 + ((pins >> ENCODER_B) & 1);
}

static void encoder_irq(void)
{
    uint32_t edges = GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL;
    gpio_acknowledge_irq(ENCODER_A, gpio_get_irq_event_mask(ENCODER_A) & edges);
    gpio_acknowledge_irq(ENCODER_B, gpio_get_irq_event_mask(ENCODER_B) & edges);
    unsigned current = read_encoder();
    static const int8_t transitions[16] = {
        0,
        -1,
        1,
        0,
        1,
        0,
        0,
        -1,
        -1,
        0,
        0,
        1,
        0,
        1,
        -1,
        0,
    };
    if ((current ^ previous_ab) == 3)
    {
        partial_step = 0;
    }
    else
    {
        partial_step += transitions[previous_ab * 4 + current];
    }

    previous_ab = current;
    if (partial_step >= 4 || partial_step <= -4)
    {
        int step = partial_step > 0 ? 1 : -1;
        if (pending_steps > -128 && pending_steps < 128)
        {
            pending_steps += step;
        }

        partial_step = 0;
    }
}

void ui_controls_init(void)
{
    const uint pins[] = {
        ENCODER_A,
        ENCODER_B,
        ENCODER_SW,
        BACK_SW,
    };
    for (unsigned i = 0; i < 4; ++i)
    {
        gpio_init(pins[i]);
        gpio_set_dir(pins[i], GPIO_IN);
        gpio_pull_up(pins[i]);
    }

    previous_ab = read_encoder();
    buttons[0] = (struct Button){
        .pin = ENCODER_SW, .raw = gpio_get(ENCODER_SW), .stable = gpio_get(ENCODER_SW)};
    buttons[1] =
        (struct Button){.pin = BACK_SW, .raw = gpio_get(BACK_SW), .stable = gpio_get(BACK_SW)};
    gpio_add_raw_irq_handler_masked((1u << ENCODER_A) | (1u << ENCODER_B), encoder_irq);
    gpio_set_irq_enabled(ENCODER_A, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(ENCODER_B, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    irq_set_enabled(IO_IRQ_BANK0, true);
}

void ui_controls_poll(void)
{
    uint32_t saved = save_and_disable_interrupts();
    int steps = pending_steps;
    pending_steps = 0;
    restore_interrupts(saved);
    menu_turn(steps);
    uint64_t now = time_us_64();
    for (unsigned i = 0; i < 2; ++i)
    {
        bool raw = gpio_get(buttons[i].pin);
        if (raw != buttons[i].raw)
        {
            buttons[i].raw = raw;
            buttons[i].changed = now;
        }

        if (raw != buttons[i].stable && now - buttons[i].changed >= 20000)
        {
            buttons[i].stable = raw;
            if (!raw)
            {
                if (i == 0)
                {
                    menu_press();
                }
                else
                {
                    menu_back();
                }
            }
        }
    }
}
