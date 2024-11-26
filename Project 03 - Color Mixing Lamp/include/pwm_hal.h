#ifndef _PULSE_WIDTH_MODULATOR_HARDWARE_ABSTRACTION_LAYER_H_
#define _PULSE_WIDTH_MODULATOR_HARDWARE_ABSTRACTION_LAYER_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum TimerCounterSelect_e
{
  TCNTRS_0, // see: Ch. 14, pp. 74-88, ATmega328P data sheet.
  TCNTRS_1, // see: Ch. 15, pp. 89-113, ATmega328P data sheet.
  TCNTRS_2  // see: Ch. 17, pp. 116-34, ATmega328P data sheet.
} TimerCounterSelect_t;

/** see: Table 17-8, pg. 130, ATmega328P data sheet. */
typedef enum WaveformGenerationMode_e
{
  WGM_MODE_0, // Normal
  WGM_MODE_1, // PWM, phase correct (TOP = 0xFF)
  WGM_MODE_2, // CTC (Clear Timer on Compare match)
  WGM_MODE_3, // Fast PWM (TOP = 0xFF)
  WGM_MODE_5, // PWM, phase correct (TOP = OCRA)
  WGM_MODE_7, // Fast PWM (TOP = OCRA)
} WaveformGenerationMode_t;

/** see: pp. 128-9, ATmega328P data sheet. */
typedef enum CompareOutputMode_e
{
  COM_NORMAL,
  COM_TOGGLE,
  COM_CLEAR,
  COM_SET
} CompareOutputMode_t;

/** see: Table 14-9, pg. 87, ATmega328P data sheet. */
typedef enum ClockSelect_e
{
  CS_NONE, // Timer/Counter stopped.
  CS_NO_PRESCALING,
  CS_PRESCALE_BY_8,
  CS_PRESCALE_BY_32,
  CS_PRESCALE_BY_64,
  CS_PRESCALE_BY_128,
  CS_PRESCALE_BY_256,
  CS_PRESCALE_BY_1024,
  CS_EXT_FALLING_EDGE, // External clk source on T0 pin. Clock on falling edge.
  CS_EXT_RISING_EDGE, // External clk source on T0 pin. Clock on rising edge.
} ClockSelect_t;

int8_t pwm_init (TimerCounterSelect_t timer,
                 WaveformGenerationMode_t waveform_gen_mode,
                 CompareOutputMode_t cmp_output_mode, bool force_output_cmp_a,
                 bool force_output_cmp_b, ClockSelect_t prescale);

#endif /* _PULSE_WIDTH_MODULATOR_HARDWARE_ABSTRACTION_LAYER_H_ */
