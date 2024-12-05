#include "pwm/clock_select.h"

#include <avr/io.h>

#define CLOCK_SELECT_BIT_0 (CS00)
#define CLOCK_SELECT_BIT_1 (CS01)
#define CLOCK_SELECT_BIT_2 (CS02)

bool
clk_is_valid_clock_select (ClockSelect_t c)
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
    default:
      return false;
    }
}

void
clk_set_clk_select_mode_bits (PMWTimerCntr_t *pmw, ClockSelect_t c)
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
