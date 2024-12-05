#include "pwm/compare_output_mode.h"

#include <avr/io.h>

#define COMPARE_MATCH_OUTPUT_A_MODE_BIT_0 (COM0A0)
#define COMPARE_MATCH_OUTPUT_A_MODE_BIT_1 (COM0A1)
#define COMPARE_MATCH_OUTPUT_B_MODE_BIT_0 (COM0B0)
#define COMPARE_MATCH_OUTPUT_B_MODE_BIT_1 (COM0B1)

static void set_com_non_pmw_mode (PMWTimerCntr_t *pmw, CompareOutputMode_t c);
static void set_com_fast_pmw_mode (PMWTimerCntr_t *pmw, CompareOutputMode_t c);
static void set_com_phase_correct_pmw_mode (PMWTimerCntr_t *pmw,
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
cmp_set_cmp_output_mode (PMWTimerCntr_t *pmw, WaveformGenerationMode_t w,
                         CompareOutputMode_t c)
{
  switch (w)
    {
    case WGM_MODE_0:
    case WGM_MODE_2:
      set_com_non_pmw_mode (pmw, c);
      break;
    case WGM_MODE_1:
    case WGM_MODE_5:
      set_com_phase_correct_pmw_mode (pmw, c);
      break;
    case WGM_MODE_3:
    case WGM_MODE_7:
      set_com_fast_pmw_mode (pmw, c);
      break;
    }
}

void
set_com_non_pmw_mode (PMWTimerCntr_t *pmw, CompareOutputMode_t c)
{
  switch (c)
    {
    case COM_NORMAL:
      pmw->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_0));
      pmw->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_1));
      pmw->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_0));
      pmw->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_1));
      break;
    case COM_TOGGLE:
      pmw->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_0));
      pmw->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_1));
      pmw->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_0));
      pmw->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_1));
      break;
    case COM_CLEAR:
      pmw->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_0));
      pmw->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_1));
      pmw->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_0));
      pmw->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_1));
      break;
    case COM_SET:
      pmw->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_0));
      pmw->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_1));
      pmw->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_0));
      pmw->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_1));
      break;
    }
}

void
set_com_fast_pmw_mode (PMWTimerCntr_t *pmw, CompareOutputMode_t c)
{
  switch (c)
    {
    case COM_NORMAL:
      pmw->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_0));
      pmw->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_1));
      pmw->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_0));
      pmw->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_1));
      break;
    case COM_TOGGLE:
      pmw->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_0));
      pmw->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_1));
      break;
    case COM_CLEAR:
      pmw->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_0));
      pmw->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_1));
      pmw->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_0));
      pmw->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_1));
      break;
    case COM_SET:
      pmw->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_0));
      pmw->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_1));
      pmw->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_0));
      pmw->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_1));
      break;
    }
}

void
set_com_phase_correct_pmw_mode (PMWTimerCntr_t *pmw, CompareOutputMode_t c)
{
  switch (c)
    {
    case COM_NORMAL:
      pmw->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_0));
      pmw->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_1));
      pmw->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_0));
      pmw->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_1));
      break;
    case COM_TOGGLE:
      pmw->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_0));
      pmw->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_1));
      break;
    case COM_CLEAR:
      pmw->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_0));
      pmw->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_1));
      pmw->control_register_a &= ~(1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_0));
      pmw->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_1));
      break;
    case COM_SET:
      pmw->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_0));
      pmw->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_A_MODE_BIT_1));
      pmw->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_0));
      pmw->control_register_a |= (1 << (COMPARE_MATCH_OUTPUT_B_MODE_BIT_1));
      break;
    }
}
