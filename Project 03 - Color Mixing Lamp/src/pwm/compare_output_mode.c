#include "pwm/compare_output_mode.h"

#include <avr/io.h>

#define COMPARE_MATCH_OUTPUT_A_MODE_BIT_0 (COM0A0)
#define COMPARE_MATCH_OUTPUT_A_MODE_BIT_1 (COM0A1)
#define COMPARE_MATCH_OUTPUT_B_MODE_BIT_0 (COM0B0)
#define COMPARE_MATCH_OUTPUT_B_MODE_BIT_1 (COM0B1)

static void set_com_non_pwm_mode (PWMTimerCntr_t *pwm, CompareOutputMode_t c);
static void set_com_fast_pwm_mode (PWMTimerCntr_t *pwm, CompareOutputMode_t c);
static void set_com_phase_correct_pmw_mode (PWMTimerCntr_t *pwm,
                                            CompareOutputMode_t c);

bool
cmp_is_valid_cmp_output_mode (CompareOutputMode_t c)
{
  switch (c)
    {
    case COM_NORMAL:
    case COM_TOGGLE:
    case COM_CLEAR:
    case COM_SET:
      return true;
    default:
      return false;
    }
}

void
cmp_set_cmp_output_mode (PWMTimerCntr_t *pwm, WaveformGenerationMode_t w,
                         CompareOutputMode_t c)
{
  switch (w)
    {
    case WGM_MODE_0:
    case WGM_MODE_2:
      set_com_non_pwm_mode (pwm, c);
      break;
    case WGM_MODE_1:
    case WGM_MODE_5:
      set_com_phase_correct_pmw_mode (pwm, c);
      break;
    case WGM_MODE_3:
    case WGM_MODE_7:
      set_com_fast_pwm_mode (pwm, c);
      break;
    }
}

void
set_com_non_pwm_mode (PWMTimerCntr_t *pwm, CompareOutputMode_t c)
{
  switch (c)
    {
    case COM_NORMAL:
      pwm->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_0));
      pwm->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_1));
      pwm->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_0));
      pwm->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_1));
      break;
    case COM_TOGGLE:
      pwm->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_0));
      pwm->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_1));
      pwm->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_0));
      pwm->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_1));
      break;
    case COM_CLEAR:
      pwm->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_0));
      pwm->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_1));
      pwm->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_0));
      pwm->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_1));
      break;
    case COM_SET:
      pwm->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_0));
      pwm->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_1));
      pwm->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_0));
      pwm->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_1));
      break;
    }
}

void
set_com_fast_pwm_mode (PWMTimerCntr_t *pwm, CompareOutputMode_t c)
{
  switch (c)
    {
    case COM_NORMAL:
      pwm->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_0));
      pwm->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_1));
      pwm->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_0));
      pwm->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_1));
      break;
    case COM_TOGGLE:
      pwm->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_0));
      pwm->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_1));
      break;
    case COM_CLEAR:
      pwm->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_0));
      pwm->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_1));
      pwm->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_0));
      pwm->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_1));
      break;
    case COM_SET:
      pwm->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_0));
      pwm->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_1));
      pwm->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_0));
      pwm->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_1));
      break;
    }
}

void
set_com_phase_correct_pmw_mode (PWMTimerCntr_t *pwm, CompareOutputMode_t c)
{
  switch (c)
    {
    case COM_NORMAL:
      pwm->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_0));
      pwm->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_1));
      pwm->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_0));
      pwm->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_1));
      break;
    case COM_TOGGLE:
      pwm->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_0));
      pwm->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_1));
      break;
    case COM_CLEAR:
      pwm->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_0));
      pwm->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_1));
      pwm->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_0));
      pwm->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_1));
      break;
    case COM_SET:
      pwm->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_0));
      pwm->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_1));
      pwm->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_0));
      pwm->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_1));
      break;
    }
}
