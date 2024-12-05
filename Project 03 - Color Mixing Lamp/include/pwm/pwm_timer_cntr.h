#ifndef _PWM_TIMER_CNTR_H_
#define _PWM_TIMER_CNTR_H_

#include <stdint.h>

// TODO: look up the actual size of these registers
typedef struct PWMTimerCntr_s
{
  uint16_t control_register_a;
  uint16_t control_register_b;
} PWMTimerCntr_t;

#endif /* _PWM_TIMER_CNTR_H_ */
