#ifndef _COMPARE_OUTPUT_MODE_H_
#define _COMPARE_OUTPUT_MODE_H_

#include "pwm/pwm_timer_cntr.h"
#include "pwm/waveform_generation_mode.h"

#include <stdbool.h>

/** see: pp. 128-9, ATmega328P data sheet. */
typedef enum CompareOutputMode_e
{
  COM_NORMAL,
  COM_TOGGLE,
  COM_CLEAR,
  COM_SET
} CompareOutputMode_t;

bool cmp_is_valid_cmp_output_mode (CompareOutputMode_t c);
void cmp_set_cmp_output_mode (PWMTimerCntr_t *pmw, WaveformGenerationMode_t w,
                              CompareOutputMode_t c);

#endif /* _COMPARE_OUTPUT_MODE_H_ */
