#ifndef LAMA_UI_CONTROLS_H
#define LAMA_UI_CONTROLS_H

#define ENCODER_A 16
#define ENCODER_B 17
#define ENCODER_SW 18
#define BACK_SW 19

/* PIO2 counts GP16/17 edges. Active-low OK/Back use pull-ups and debounce.
 * Four quadrature edges per detent; see encoder_steps.h. */
void ui_controls_init(void);
void ui_controls_poll(void);

#endif
