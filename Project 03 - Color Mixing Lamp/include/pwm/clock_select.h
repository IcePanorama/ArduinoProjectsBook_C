#ifndef _CLOCK_SELECT_H_
#define _CLOCK_SELECT_H_

#include "pwm/pwm_timer_cntr.h"

#include <stdbool.h>

/** see: Table 14-9, pg. 87, ATmega328P data sheet. */
typedef enum ClockSelect_e
{
  CS_NONE, // Timer/Counter stopped.
  CS_NO_PRESCALING,
  CS_PRESCALE_BY_8,
  CS_PRESCALE_BY_64,
  CS_PRESCALE_BY_256,
  CS_PRESCALE_BY_1024,
  CS_EXT_FALLING_EDGE, // External clk source on T0 pin. Clock on falling edge.
  CS_EXT_RISING_EDGE,  // External clk source on T0 pin. Clock on rising edge.
} ClockSelect_t;

bool clk_is_valid_clock_select (ClockSelect_t c);
void clk_set_clk_select_mode_bits (PMWTimerCntr_t *pmw, ClockSelect_t c);

#endif /* _CLOCK_SELECT_H_ */
