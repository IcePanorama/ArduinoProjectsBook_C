#include "pwm/pwm_hal.h"
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

/* Control Register B Bits */
#define FORCE_OUTPUT_COMPARE_A_BIT (FOC0A)
#define FORCE_OUTPUT_COMPARE_B_BIT (FOC0B)
/***************************/

#define POWER_REDUCTION_REGISTER (PRR)

/* Power Reduction Register Bits */
#define TCNTR0_POWER_REDUCTION_BIT (PRTIM0)
#define TCNTR1_POWER_REDUCTION_BIT (PRTIM1)
#define TCNTR2_POWER_REDUCTION_BIT (PRTIM2)
/***************************/

static int8_t validate_init_input (TimerCounterSelect_t t,
                                   WaveformGenerationMode_t w,
                                   CompareOutputMode_t c,
                                   bool force_output_cmp_a,
                                   bool force_output_cmp_b, ClockSelect_t s);
static bool is_valid_timer (TimerCounterSelect_t t);
static void get_control_regs_from_selection (TimerCounterSelect_t t,
                                             uint16_t *ctrl_reg_a,
                                             uint16_t *ctrl_reg_b);
static void set_force_output_compare_bits (PWMTimerCntr_t *pwm,
                                           WaveformGenerationMode_t w,
                                           bool force_output_cmp_a,
                                           bool force_output_cmp_b);
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

  PWMTimerCntr_t pwm;
  get_control_regs_from_selection (timer, &pwm.control_register_a,
                                   &pwm.control_register_b);
  /*
   *  Doing this first as "this bit must be set to zero when
   *  [TCNTR0_CONTROL_REGISTER_B] is written when operating in PWM mode."
   *  (pg. 86, ATmega328P data sheet).
   */
  set_force_output_compare_bits (&pwm, waveform_gen_mode, force_output_cmp_a,
                                 force_output_cmp_b);
  wgm_set_waveform_gen_mode (&pwm, waveform_gen_mode);
  cmp_set_cmp_output_mode (&pwm, waveform_gen_mode, cmp_output_mode);
  clk_set_clk_select_mode_bits (&pwm, timer, prescale);

  enable_tcntr (timer);
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
  else if (!wgm_is_valid_waveform_gen_mode (w))
    {
      uart_send_string (
          "Error: Invalid waveform generation mode provided!\r\n");
      return -1;
    }
  else if (!cmp_is_valid_cmp_output_mode (c))
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
  else if (!clk_is_valid_clock_select (t, s))
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
set_force_output_compare_bits (PWMTimerCntr_t *pwm, WaveformGenerationMode_t w,
                               bool force_output_cmp_a,
                               bool force_output_cmp_b)
{
  switch (w)
    {
    case WGM_MODE_0:
    case WGM_MODE_2:
      if (force_output_cmp_a)
        pwm->control_register_b |= (1 << (FORCE_OUTPUT_COMPARE_A_BIT));
      else if (!force_output_cmp_a)
        pwm->control_register_b &= ~(1 << (FORCE_OUTPUT_COMPARE_A_BIT));

      if (force_output_cmp_b)
        pwm->control_register_b |= (1 << (FORCE_OUTPUT_COMPARE_B_BIT));
      else if (!force_output_cmp_b)
        pwm->control_register_b &= ~(1 << (FORCE_OUTPUT_COMPARE_B_BIT));
      break;
    case WGM_MODE_1:
    case WGM_MODE_5:
    case WGM_MODE_3:
    case WGM_MODE_7:
      pwm->control_register_b &= ~(1 << (FORCE_OUTPUT_COMPARE_A_BIT));
      pwm->control_register_b &= ~(1 << (FORCE_OUTPUT_COMPARE_B_BIT));
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
