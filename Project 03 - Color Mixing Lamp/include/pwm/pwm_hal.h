#ifndef _PULSE_WIDTH_MODULATOR_HARDWARE_ABSTRACTION_LAYER_H_
#define _PULSE_WIDTH_MODULATOR_HARDWARE_ABSTRACTION_LAYER_H_

#include "pwm/clock_select.h"
#include "pwm/compare_output_mode.h"
#include "pwm/waveform_generation_mode.h"

#include <stdbool.h>
#include <stdint.h>

/**
 *  ~D11 = PB3 = OC2A (Timer/Counter2 output compare match A output)
 *  ~D10 = PB2 = OC1B (Timer/Counter1 output compare match B output)
 *  ~D09 = PB1 = OC1A (Timer/Counter1 output compare match A output)
 */

typedef enum TimerCounterSelect_e
{
  TCNTRS_0, // see: Ch. 14, pp.  74-88, ATmega328P data sheet.
  TCNTRS_1, // see: Ch. 15, pp. 89-113, ATmega328P data sheet.
  TCNTRS_2  // see: Ch. 17, pp. 116-34, ATmega328P data sheet.
} TimerCounterSelect_t;

int8_t pwm_init (TimerCounterSelect_t timer,
                 WaveformGenerationMode_t waveform_gen_mode,
                 CompareOutputMode_t cmp_output_mode, bool force_output_cmp_a,
                 bool force_output_cmp_b, ClockSelect_t prescale);

#endif /* _PULSE_WIDTH_MODULATOR_HARDWARE_ABSTRACTION_LAYER_H_ */
