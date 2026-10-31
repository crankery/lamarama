#ifndef LAMA_UI_CONTROLS_H
#define LAMA_UI_CONTROLS_H
#define ENCODER_A 16
#define ENCODER_B 17
#define ENCODER_SW 18
#define BACK_SW 19
/* Active-low contacts with pull-ups; four quadrature edges per detent. */
void ui_controls_init(void);
void ui_controls_poll(void);
#endif
