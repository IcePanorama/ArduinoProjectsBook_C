#ifndef _TIMER_COUNTER_SELECTION_H_
#define _TIMER_COUNTER_SELECTION_H_

/**
 *  ~D11 = PB3 = OC2A (Timer/Counter2 output compare match A output)
 *  ~D10 = PB2 = OC1B (Timer/Counter1 output compare match B output)
 *  ~D09 = PB1 = OC1A (Timer/Counter1 output compare match A output)
 */
typedef enum TimerCounterSelect_e
{
  TCNTRS_0, // see: Ch. 14, pp.  74-88, ATmega328P data sheet.
  TCNTRS_1, // see: Ch. 15, pp. 89-113, ATmega328P data sheet.
  TCNTRS_2  // see: Ch. 17, pp. 116-34, ATmega328P data sheet.
} TimerCounterSelect_t;

#endif /* _TIMER_COUNTER_SELECTION_H_ */
