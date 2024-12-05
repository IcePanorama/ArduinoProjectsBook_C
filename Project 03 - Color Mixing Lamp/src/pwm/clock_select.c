#include "pwm/clock_select.h"

#include <avr/io.h>

#define CLOCK_SELECT_BIT_0 (CS00)
#define CLOCK_SELECT_BIT_1 (CS01)
#define CLOCK_SELECT_BIT_2 (CS02)

static bool validate_tcntr0_or_1_clk_select (ClockSelect_t c);
static bool validate_tcntr2_clk_select (ClockSelect_t c);
static void set_tcntr0_or_1_clk_select_mode_bits (PWMTimerCntr_t *pwm,
                                                  ClockSelect_t c);
static void set_tcntr2_clk_select_mode_bits (PWMTimerCntr_t *pwm,
                                             ClockSelect_t c);

bool
clk_is_valid_clock_select (TimerCounterSelect_t t, ClockSelect_t c)
{
  switch (t)
    {
    case TCNTRS_0:
      return validate_tcntr0_or_1_clk_select (c);
    case TCNTRS_1:
      return validate_tcntr0_or_1_clk_select (c);
    case TCNTRS_2:
      return validate_tcntr2_clk_select (c);
    default:
      return false;
    }
}

bool
validate_tcntr2_clk_select (ClockSelect_t c)
{
  switch (c)
    {
    case CS_NONE:
    case CS_NO_PRESCALING:
    case CS_PRESCALE_BY_8:
    case CS_PRESCALE_BY_32:
    case CS_PRESCALE_BY_64:
    case CS_PRESCALE_BY_128:
    case CS_PRESCALE_BY_256:
    case CS_PRESCALE_BY_1024:
      return true;
    case CS_EXT_FALLING_EDGE:
    case CS_EXT_RISING_EDGE:
    default:
      return false;
    }
}

bool
validate_tcntr0_or_1_clk_select (ClockSelect_t c)
{
  switch (c)
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
    case CS_PRESCALE_BY_32:
    case CS_PRESCALE_BY_128:
    default:
      return false;
    }
}

// FIXME: clk selection is different depending on which timer you're using.
void
clk_set_clk_select_mode_bits (PWMTimerCntr_t *pwm, TimerCounterSelect_t t,
                              ClockSelect_t c)
{
  switch (t)
    {
    case TCNTRS_0:
    case TCNTRS_1:
      set_tcntr0_or_1_clk_select_mode_bits (pwm, c);
      break;
    case TCNTRS_2:
      set_tcntr2_clk_select_mode_bits (pwm, c);
      break;
    }
}

void
set_tcntr0_or_1_clk_select_mode_bits (PWMTimerCntr_t *pwm, ClockSelect_t c)
{
  switch (c)
    {
    case CS_NONE:
      pwm->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_0));
      pwm->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_1));
      pwm->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_2));
      break;
    case CS_NO_PRESCALING:
      pwm->control_register_b |= (1 << (CLOCK_SELECT_BIT_0));
      pwm->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_1));
      pwm->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_2));
      break;
    case CS_PRESCALE_BY_8:
      pwm->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_0));
      pwm->control_register_b |= (1 << (CLOCK_SELECT_BIT_1));
      pwm->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_2));
      break;
    case CS_PRESCALE_BY_64:
      pwm->control_register_b |= (1 << (CLOCK_SELECT_BIT_0));
      pwm->control_register_b |= (1 << (CLOCK_SELECT_BIT_1));
      pwm->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_2));
      break;
    case CS_PRESCALE_BY_256:
      pwm->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_0));
      pwm->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_1));
      pwm->control_register_b |= (1 << (CLOCK_SELECT_BIT_2));
      break;
    case CS_PRESCALE_BY_1024:
      pwm->control_register_b |= (1 << (CLOCK_SELECT_BIT_0));
      pwm->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_1));
      pwm->control_register_b |= (1 << (CLOCK_SELECT_BIT_2));
      break;
    case CS_EXT_FALLING_EDGE:
      pwm->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_0));
      pwm->control_register_b |= (1 << (CLOCK_SELECT_BIT_1));
      pwm->control_register_b |= (1 << (CLOCK_SELECT_BIT_2));
      break;
    case CS_EXT_RISING_EDGE:
      pwm->control_register_b |= (1 << (CLOCK_SELECT_BIT_0));
      pwm->control_register_b |= (1 << (CLOCK_SELECT_BIT_1));
      pwm->control_register_b |= (1 << (CLOCK_SELECT_BIT_2));
      break;
    case CS_PRESCALE_BY_32:
    case CS_PRESCALE_BY_128:
      return;
    }
}

void
set_tcntr2_clk_select_mode_bits (PWMTimerCntr_t *pwm, ClockSelect_t c)
{
  switch (c)
    {
    case CS_NONE:
      pwm->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_0));
      pwm->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_1));
      pwm->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_2));
      break;
    case CS_NO_PRESCALING:
      pwm->control_register_b |= (1 << (CLOCK_SELECT_BIT_0));
      pwm->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_1));
      pwm->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_2));
      break;
    case CS_PRESCALE_BY_8:
      pwm->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_0));
      pwm->control_register_b |= (1 << (CLOCK_SELECT_BIT_1));
      pwm->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_2));
      break;
    case CS_PRESCALE_BY_32:
      pwm->control_register_b |= (1 << (CLOCK_SELECT_BIT_0));
      pwm->control_register_b |= (1 << (CLOCK_SELECT_BIT_1));
      pwm->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_2));
      break;
    case CS_PRESCALE_BY_64:
      pwm->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_0));
      pwm->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_1));
      pwm->control_register_b |= (1 << (CLOCK_SELECT_BIT_2));
      break;
    case CS_PRESCALE_BY_128:
      pwm->control_register_b |= (1 << (CLOCK_SELECT_BIT_0));
      pwm->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_1));
      pwm->control_register_b |= (1 << (CLOCK_SELECT_BIT_2));
      break;
    case CS_PRESCALE_BY_256:
      pwm->control_register_b &= ~(1 << (CLOCK_SELECT_BIT_0));
      pwm->control_register_b |= (1 << (CLOCK_SELECT_BIT_1));
      pwm->control_register_b |= (1 << (CLOCK_SELECT_BIT_2));
      break;
    case CS_PRESCALE_BY_1024:
      pwm->control_register_b |= (1 << (CLOCK_SELECT_BIT_0));
      pwm->control_register_b |= (1 << (CLOCK_SELECT_BIT_1));
      pwm->control_register_b |= (1 << (CLOCK_SELECT_BIT_2));
      break;
    case CS_EXT_FALLING_EDGE:
    case CS_EXT_RISING_EDGE:
      return;
    }
}
