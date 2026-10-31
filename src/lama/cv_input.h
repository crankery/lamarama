#ifndef LAMA_CV_INPUT_H
#define LAMA_CV_INPUT_H
#include <stdint.h>
#define GATE_PIN 20
#define CV_ADC_REFERENCE_VOLTS 3.0f
#define CV_INPUT_GAIN 0.6f /* R18 20K / R10 30K divider, then unity buffer */
#define CV_ZERO_NOTE 36.0f /* C2; user calibration can change this later */
float cv_pitch_hz(uint16_t raw);
void cv_input_init(void);
void cv_input_pitch(uint16_t raw);
void cv_input_poll(void);
#endif
