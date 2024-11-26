#include "pwm_hal.h"
#include "uart_hal.h"

#define TCNTR0_CONTROL_REGISTER_A (TCCR0A)
#define TCNTR0_CONTROL_REGISTER_B (TCCR0B)

#define TCNTR1_CONTROL_REGISTER_A (TCCR1A)
#define TCNTR1_CONTROL_REGISTER_B (TCCR1B)

#define TCNTR2_CONTROL_REGISTER_A (TCCR2A)
#define TCNTR2_CONTROL_REGISTER_B (TCCR2B)

static int8_t validate_init_input (TimerCounterSelect_t t,
                                   WaveformGenerationMode_t w,
                                   CompareOutputMode_t c,
                                   bool force_output_cmp_a,
                                   bool force_output_cmp_b, ClockSelect_t s);
static bool is_valid_timer (TimerCounterSelect_t t);
static bool is_valid_waveform_gen_mode (WaveformGenerationMode_t w);
static bool is_valid_cmp_output_mode (CompareOutputMode_t c);
static bool is_valid_clock_select (ClockSelect_t s);

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
   *  `COM_TOGGLE` is reserved in PWM modes.
   *  see: pg. 85, ATmega328P data sheet.
   */
  else if (w != WGM_MODE_0 && w != WGM_MODE_2 && c == COM_TOGGLE)
    {
      uart_send_string ("Error: Conflict between given waveform generation "
                        "mode and compare output mode.\r\n");
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
    case CS_PRESCALE_BY_32:
    case CS_PRESCALE_BY_64:
    case CS_PRESCALE_BY_128:
    case CS_PRESCALE_BY_256:
    case CS_PRESCALE_BY_1024:
    case CS_EXT_FALLING_EDGE:
    case CS_EXT_RISING_EDGE:
      return true;
    default:
      return false;
    }
}
