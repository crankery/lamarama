#ifndef _MESSENGER_DOT_H
#define _MESSENGER_DOT_H

#include "stdbool.h"

typedef enum
{
    MONK_SET_PITCH,
    MONK_SET_VOWEL,
    MONK_SET_VIBRATO,
    MONK_SET_ASPIRATION,
    MONK_SET_LEVEL,
    MONK_GATE_ON,
    MONK_GATE_OFF
} MonkEventType;

typedef struct
{
    MonkEventType type;
    float value;
} MonkEvent;

void start_messenger();
bool add_message(MonkEventType e, float v);
void process_messages();

#endif // _MESSENGER_DOT_H