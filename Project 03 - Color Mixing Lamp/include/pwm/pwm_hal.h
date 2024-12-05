#ifndef _PULSE_WIDTH_MODULATOR_HARDWARE_ABSTRACTION_LAYER_H_
#define _PULSE_WIDTH_MODULATOR_HARDWARE_ABSTRACTION_LAYER_H_

#include "pwm/clock_select.h"
#include "pwm/compare_output_mode.h"
#include "pwm/timer_cntr_selection.h"
#include "pwm/waveform_generation_mode.h"

#include <stdbool.h>
#include <stdint.h>

int8_t pwm_init (TimerCounterSelect_t timer,
                 WaveformGenerationMode_t waveform_gen_mode,
                 CompareOutputMode_t cmp_output_mode, bool force_output_cmp_a,
                 bool force_output_cmp_b, ClockSelect_t prescale);

#endif /* _PULSE_WIDTH_MODULATOR_HARDWARE_ABSTRACTION_LAYER_H_ */
