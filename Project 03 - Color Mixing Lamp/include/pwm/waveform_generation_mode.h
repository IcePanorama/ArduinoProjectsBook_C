#ifndef _WAVEFORM_GENERATION_MODE_H_
#define _WAVEFORM_GENERATION_MODE_H_

#include "pwm/pwm_timer_cntr.h"

#include <stdbool.h>

/** see: Table 17-8, pg. 130, ATmega328P data sheet. */
typedef enum WaveformGenerationMode_e
{
  WGM_MODE_0,       // Normal (0b000)
  WGM_MODE_1,       // PWM, phase correct (TOP = 0xFF, 0b001)
  WGM_MODE_2,       // CTC (Clear Timer on Compare match, 0b010)
  WGM_MODE_3,       // Fast PWM (TOP = 0xFF, 0b011)
  WGM_MODE_5 = 0x5, // PWM, phase correct (TOP = OCRA, 0b101)
  WGM_MODE_7 = 0x7, // Fast PWM (TOP = OCRA, 0b111)
} WaveformGenerationMode_t;

bool wgm_is_valid_waveform_gen_mode (WaveformGenerationMode_t w);
void wgm_set_waveform_gen_mode (PWMTimerCntr_t *pmw,
                                WaveformGenerationMode_t w);

#endif /* _WAVEFORM_GENERATION_MODE_H_ */
