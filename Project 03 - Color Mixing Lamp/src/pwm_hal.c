#include "pwm_hal.h"
#include "uart_hal.h"

#include <avr/io.h>
#include <stdint.h>

#define TCNTR0_CONTROL_REGISTER_A (TCCR0A)
#define TCNTR0_CONTROL_REGISTER_B (TCCR0B)

#define TCNTR1_CONTROL_REGISTER_A (TCCR1A)
#define TCNTR1_CONTROL_REGISTER_B (TCCR1B)

#define TCNTR2_CONTROL_REGISTER_A (TCCR2A)
#define TCNTR2_CONTROL_REGISTER_B (TCCR2B)

#define TCNTR0_OUTPUT_COMPARE_REGISTER_A (OCR0A)
#define TCNTR0_OUTPUT_COMPARE_REGISTER_B (OCR0B)

/* These are both 16-bit registers. */
#define TCNTR1_OUTPUT_COMPARE_REGISTER_A (OCR1A)
#define TCNTR1_OUTPUT_COMPARE_REGISTER_B (OCR1B)

#define TCNTR2_OUTPUT_COMPARE_REGISTER_A (OCR2A)
#define TCNTR2_OUTPUT_COMPARE_REGISTER_B (OCR2B)

/* Control Register A Bits */
#define COMPARE_MATCH_OUTPUT_A_MODE_BIT_0 (COM0A0)
#define COMPARE_MATCH_OUTPUT_A_MODE_BIT_1 (COM0A1)
#define COMPARE_MATCH_OUTPUT_B_MODE_BIT_0 (COM0B0)
#define COMPARE_MATCH_OUTPUT_B_MODE_BIT_1 (COM0B1)

#define WAVEFORM_GENERATION_MODE_BIT_0 (WGM00)
#define WAVEFORM_GENERATION_MODE_BIT_1 (WGM01)
/***************************/

/* Control Register B Bits */
#define FORCE_OUTPUT_COMPARE_A_BIT (FOC0A)
#define FORCE_OUTPUT_COMPARE_B_BIT (FOC0B)

#define WAVEFORM_GENERATION_MODE_BIT_2 (WGM02)

#define CLOCK_SELECT_BIT_0 (CS00)
#define CLOCK_SELECT_BIT_1 (CS01)
#define CLOCK_SELECT_BIT_2 (CS02)
/***************************/

#define POWER_REDUCTION_REGISTER (PRR)

/* Power Reduction Register Bits */
#define TCNTR0_POWER_REDUCTION_BIT (PRTIM0)
#define TCNTR1_POWER_REDUCTION_BIT (PRTIM1)
#define TCNTR2_POWER_REDUCTION_BIT (PRTIM2)
/***************************/

// TODO: look up the actual size of these registers
typedef struct PMWTimerCntr_s
{
  uint16_t control_register_a;
  uint16_t control_register_b;
} PMWTimerCntr_t;

// This likely needs to be split up into separate files.
static int8_t validate_init_input (TimerCounterSelect_t t,
                                   WaveformGenerationMode_t w,
                                   CompareOutputMode_t c,
                                   bool force_output_cmp_a,
                                   bool force_output_cmp_b, ClockSelect_t s);
static bool is_valid_timer (TimerCounterSelect_t t);
static bool is_valid_waveform_gen_mode (WaveformGenerationMode_t w);
static bool is_valid_cmp_output_mode (CompareOutputMode_t c);
static bool is_valid_clock_select (ClockSelect_t s);
static void get_control_regs_from_selection (TimerCounterSelect_t t,
                                             uint16_t *ctrl_reg_a,
                                             uint16_t *ctrl_reg_b);
static void set_waveform_gen_mode (PMWTimerCntr_t *pmw,
                                   WaveformGenerationMode_t w);
static void set_cmp_output_mode (PMWTimerCntr_t *pmw,
                                 WaveformGenerationMode_t w,
                                 CompareOutputMode_t c);
static void set_com_non_pmw_mode (PMWTimerCntr_t *pmw, CompareOutputMode_t c);
static void set_com_fast_pmw_mode (PMWTimerCntr_t *pmw, CompareOutputMode_t c);
static void set_com_phase_correct_pmw_mode (PMWTimerCntr_t *pmw,
                                            CompareOutputMode_t c);
static void set_force_output_compare_bits (PMWTimerCntr_t *pmw,
                                           WaveformGenerationMode_t w,
                                           bool force_output_cmp_a,
                                           bool force_output_cmp_b);
static void set_clk_select_mode_bits (PMWTimerCntr_t *pmw, ClockSelect_t c);
static void enable_tcntr (TimerCounterSelect_t t);

int8_t
pwm_init (TimerCounterSelect_t timer,
          WaveformGenerationMode_t waveform_gen_mode,
          CompareOutputMode_t cmp_output_mode, bool force_output_cmp_a,
          bool force_output_cmp_b, ClockSelect_t prescale)
{
  if (validate_init_input (timer, waveform_gen_mode, cmp_output_mode,
                           force_output_cmp_a, force_output_cmp_b, prescale)
      != 0)
    {
      return -1;
    }

  PMWTimerCntr_t pmw;
  get_control_regs_from_selection (timer, &pmw.control_register_a,
                                   &pmw.control_register_b);
  /*
   *  Doing this first as "this bit must be set to zero when TCCR0B
   *  [TCNTR0_CONTROL_REGISTER_B] is written when operating in PWM mode."
   *  (pg. 86, ATmega328P data sheet).
   */
  set_force_output_compare_bits (&pmw, waveform_gen_mode, force_output_cmp_a,
                                 force_output_cmp_b);
  set_waveform_gen_mode (&pmw, waveform_gen_mode);
  set_cmp_output_mode (&pmw, waveform_gen_mode, cmp_output_mode);
  set_clk_select_mode_bits (&pmw, prescale);

  enable_tcntr(timer);
  return 0;
}

int8_t
validate_init_input (TimerCounterSelect_t t, WaveformGenerationMode_t w,
                     CompareOutputMode_t c, bool force_output_cmp_a,
                     bool force_output_cmp_b, ClockSelect_t s)
{
  if (!is_valid_timer (t))
    {
      uart_send_string ("Error: Invalid timer selection provided!\r\n");
      return -1;
    }
  else if (!is_valid_waveform_gen_mode (w))
    {
      uart_send_string (
          "Error: Invalid waveform generation mode provided!\r\n");
      return -1;
    }
  else if (!is_valid_cmp_output_mode (c))
    {
      uart_send_string ("Error: Invalid compare output mode provided!\r\n");
      return -1;
    }
  /*
   *  FOCnA/B should only be set in a non-PWM mode.
   *  see: pg. 86, ATmega328P data sheet.
   */
  else if (w != WGM_MODE_0 && w != WGM_MODE_2
           && (force_output_cmp_a || force_output_cmp_b))
    {
      uart_send_string ("Error: Conflict between given waveform generation "
                        "mode and force output compare flag.\r\n");
      return -1;
    }
  else if (!is_valid_clock_select (s))
    {
      uart_send_string ("Error: Invalid clock prescaler provided!\r\n");
      return -1;
    }

  return 0;
}

bool
is_valid_timer (TimerCounterSelect_t t)
{
  switch (t)
    {
    case TCNTRS_0:
    case TCNTRS_1:
    case TCNTRS_2:
      return true;
    default:
      return false;
    }
}

bool
is_valid_waveform_gen_mode (WaveformGenerationMode_t w)
{
  switch (w)
    {
    case WGM_MODE_0:
    case WGM_MODE_1:
    case WGM_MODE_2:
    case WGM_MODE_3:
    case WGM_MODE_5:
    case WGM_MODE_7:
      return true;
    default:
      return false;
    }
}

bool
is_valid_cmp_output_mode (CompareOutputMode_t c)
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

bool
is_valid_clock_select (ClockSelect_t s)
{
  switch (s)
    {
    case CS_NONE:
    case CS_NO_PRESCALING:
    case CS_PRESCALE_BY_8:
    case CS_PRESCALE_BY_64:
    case CS_PRESCALE_BY_256:
    case CS_PRESCALE_BY_1024:
    case CS_EXT_FALLING_EDGE:
    case CS_EXT_RISING_EDGE:
      return true;
    default:
      return false;
    }
}

void
get_control_regs_from_selection (TimerCounterSelect_t t, uint16_t *ctrl_reg_a,
                                 uint16_t *ctrl_reg_b)
{
  switch (t)
    {
    case TCNTRS_0:
      *ctrl_reg_a = TCNTR0_CONTROL_REGISTER_A;
      *ctrl_reg_b = TCNTR0_CONTROL_REGISTER_B;
      break;
    case TCNTRS_1:
      *ctrl_reg_a = TCNTR1_CONTROL_REGISTER_A;
      *ctrl_reg_b = TCNTR1_CONTROL_REGISTER_B;
      break;
    case TCNTRS_2:
      *ctrl_reg_a = TCNTR2_CONTROL_REGISTER_A;
      *ctrl_reg_b = TCNTR2_CONTROL_REGISTER_B;
      break;
    }
}

void
set_waveform_gen_mode (PMWTimerCntr_t *pmw, WaveformGenerationMode_t w)
{
  switch (w)
    {
    case WGM_MODE_0:
      pmw->control_register_a &= ~(1 << (WAVEFORM_GENERATION_MODE_BIT_0));
      pmw->control_register_a &= ~(1 << (WAVEFORM_GENERATION_MODE_BIT_1));
      pmw->control_register_b &= ~(1 << (WAVEFORM_GENERATION_MODE_BIT_2));
      break;
    case WGM_MODE_1:
      pmw->control_register_a |= (1 << (WAVEFORM_GENERATION_MODE_BIT_0));
      pmw->control_register_a &= ~(1 << (WAVEFORM_GENERATION_MODE_BIT_1));
      pmw->control_register_b &= ~(1 << (WAVEFORM_GENERATION_MODE_BIT_2));
      break;
    case WGM_MODE_2:
      pmw->control_register_a &= ~(1 << (WAVEFORM_GENERATION_MODE_BIT_0));
      pmw->control_register_a |= (1 << (WAVEFORM_GENERATION_MODE_BIT_1));
      pmw->control_register_b &= ~(1 << (WAVEFORM_GENERATION_MODE_BIT_2));
      break;
    case WGM_MODE_3:
      pmw->control_register_a |= (1 << (WAVEFORM_GENERATION_MODE_BIT_0));
      pmw->control_register_a |= (1 << (WAVEFORM_GENERATION_MODE_BIT_1));
      pmw->control_register_b &= ~(1 << (WAVEFORM_GENERATION_MODE_BIT_2));
      break;
    case WGM_MODE_5:
      pmw->control_register_a |= (1 << (WAVEFORM_GENERATION_MODE_BIT_0));
      pmw->control_register_a &= ~(1 << (WAVEFORM_GENERATION_MODE_BIT_1));
      pmw->control_register_b |= (1 << (WAVEFORM_GENERATION_MODE_BIT_2));
      break;
    case WGM_MODE_7:
      pmw->control_register_a |= (1 << (WAVEFORM_GENERATION_MODE_BIT_0));
      pmw->control_register_a |= (1 << (WAVEFORM_GENERATION_MODE_BIT_1));
      pmw->control_register_b |= (1 << (WAVEFORM_GENERATION_MODE_BIT_2));
      break;
    }
}

void
set_cmp_output_mode (PMWTimerCntr_t *pmw, WaveformGenerationMode_t w,
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

void
set_force_output_compare_bits (PMWTimerCntr_t *pmw, WaveformGenerationMode_t w,
                               bool force_output_cmp_a,
                               bool force_output_cmp_b)
{
  switch (w)
    {
    case WGM_MODE_0:
    case WGM_MODE_2:
      if (force_output_cmp_a)
        pmw->control_register_b |= (1 << (FORCE_OUTPUT_COMPARE_A_BIT));
      else if (!force_output_cmp_a)
        pmw->control_register_b &= ~(1 << (FORCE_OUTPUT_COMPARE_A_BIT));

      if (force_output_cmp_b)
        pmw->control_register_b |= (1 << (FORCE_OUTPUT_COMPARE_B_BIT));
      else if (!force_output_cmp_b)
        pmw->control_register_b &= ~(1 << (FORCE_OUTPUT_COMPARE_B_BIT));
      break;
    case WGM_MODE_1:
    case WGM_MODE_5:
    case WGM_MODE_3:
    case WGM_MODE_7:
      pmw->control_register_b &= ~(1 << (FORCE_OUTPUT_COMPARE_A_BIT));
      pmw->control_register_b &= ~(1 << (FORCE_OUTPUT_COMPARE_B_BIT));
      break;
    }
}

void
set_clk_select_mode_bits (PMWTimerCntr_t *pmw, ClockSelect_t c)
{
  switch (c)
    {
    case CS_NONE:
      pmw->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_0));
      pmw->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_1));
      pmw->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_2));
      break;
    case CS_NO_PRESCALING:
      pmw->control_register_b |= (1 << (CLOCK_SELECT_BIT_0));
      pmw->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_1));
      pmw->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_2));
      break;
    case CS_PRESCALE_BY_8:
      pmw->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_0));
      pmw->control_register_b |= (1 << (CLOCK_SELECT_BIT_1));
      pmw->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_2));
      break;
    case CS_PRESCALE_BY_64:
      pmw->control_register_b |= (1 << (CLOCK_SELECT_BIT_0));
      pmw->control_register_b |= (1 << (CLOCK_SELECT_BIT_1));
      pmw->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_2));
      break;
    case CS_PRESCALE_BY_256:
      pmw->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_0));
      pmw->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_1));
      pmw->control_register_b |= (1 << (CLOCK_SELECT_BIT_2));
      break;
    case CS_PRESCALE_BY_1024:
      pmw->control_register_b |= (1 << (CLOCK_SELECT_BIT_0));
      pmw->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_1));
      pmw->control_register_b |= (1 << (CLOCK_SELECT_BIT_2));
      break;
    case CS_EXT_FALLING_EDGE:
      pmw->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_0));
      pmw->control_register_b |= (1 << (CLOCK_SELECT_BIT_1));
      pmw->control_register_b |= (1 << (CLOCK_SELECT_BIT_2));
      break;
    case CS_EXT_RISING_EDGE:
      pmw->control_register_b |= (1 << (CLOCK_SELECT_BIT_0));
      pmw->control_register_b |= (1 << (CLOCK_SELECT_BIT_1));
      pmw->control_register_b |= (1 << (CLOCK_SELECT_BIT_2));
      break;
    }
}

void
enable_tcntr (TimerCounterSelect_t t)
{
  /*
   *  "Writing a logic one to this bit shuts down the [Timer/CounterN] module.
   *  When the [Timer/CounterN] is enabled, operation will continue like before
   *  the shutdown." (pg. 39, ATmega328P data sheet).
   */
  switch (t)
    {
    case TCNTRS_0:
      POWER_REDUCTION_REGISTER &= ~(1 << (TCNTR0_POWER_REDUCTION_BIT));
      break;
    case TCNTRS_1:
      POWER_REDUCTION_REGISTER &= ~(1 << (TCNTR1_POWER_REDUCTION_BIT));
      break;
    case TCNTRS_2:
      POWER_REDUCTION_REGISTER &= ~(1 << (TCNTR2_POWER_REDUCTION_BIT));
      break;
    }
}
