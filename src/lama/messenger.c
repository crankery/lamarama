#include "messenger.h"

#include "pico/util/queue.h"

static queue_t monk_events;

void start_messenger()
{
    queue_init(&monk_events, sizeof(MonkEvent), 32);
}

bool add_message(MonkEventType e, float v)
{
    assert(get_core_num() == 0);

    MonkEvent event = {
        .type = e,
        .value = v};

    return queue_try_add(&monk_events, &event);
}

void process_messages()
{
    assert(get_core_num() == 1);

    MonkEvent event;

    while (queue_try_remove(&monk_events, &event))
    {
        switch (event.type)
        {
        case MONK_SET_PITCH:
            break;
            // etc
        }
    }
}