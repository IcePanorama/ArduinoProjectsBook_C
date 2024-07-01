#ifndef _ADC_H_
#define _ADC_H_

#include <stdint.h>

enum adc_init_result
{
  ADC_INIT_SUCCESS,
  ADC_INIT_INVALID_VOLTAGE_SELECTION,
  ADC_INIT_INVALID_CHANNEL_SELECTION,
  ADC_INIT_INVALID_PRESCALER_SELECTION
};

enum high_voltage_selection
{
  ADC_AREF,
  ADC_AVCC,
  ADC_INTERNAL_REF
};

/* clang-format off */
enum adc_channel_options
{
  ADC_CHANNEL_0   = 0x0,  // 0b0000
  ADC_CHANNEL_1   = 0x1,  // 0b0001
  ADC_CHANNEL_2   = 0x2,  // 0b0010
  ADC_CHANNEL_3   = 0x3,  // 0b0011
  ADC_CHANNEL_4   = 0x4,  // 0b0100
  ADC_CHANNEL_5   = 0x5,  // 0b0101
  ADC_CHANNEL_6   = 0x6,  // 0b0110
  ADC_CHANNEL_7   = 0x7,  // 0b0111
  ADC_CHANNEL_8   = 0x8,  // 0b1000
  ADC_CHANNEL_1v1 = 0xE,  // 0b1110
  ADC_CHANNEL_GND = 0xF   // 0b1111
};

enum adc_prescaler_options
{
  ADC_PRESCALE_BY_2   = 0x0,  // 0b000 or 0b001 (both work)
  ADC_PRESCALE_BY_4   = 0x2,  // 0b010
  ADC_PRESCALE_BY_8   = 0x3,  // 0b011
  ADC_PRESCALE_BY_16  = 0x4,  // 0b100
  ADC_PRESCALE_BY_32  = 0x5,  // 0b101
  ADC_PRESCALE_BY_64  = 0x6,  // 0b110
  ADC_PRESCALE_BY_128 = 0x7,  // 0b111
};
/* clang-format on */

/*
 *  adc_init
 *
 *  Choosing a prescaler:
 *    "By default, the successive approximation circuitry [of the ADC]
 *    requires an input clock frequency between 50kHz and 200kHz to get
 *    maximum resolution." (23.4 Prescaling and Conversion Timing, pg. 208)
 *    Here's a formula I came up with for picking a prescaler value:
 *      50p <= CPU_FREQ_IN_KHZ <= 200p,
 *      where `p` is your prescaler value.
 *    Start with p = 128 and keep reducing p until you find the smallest
 *    value for p that you can use given your CPU frequency in kHz.
 */
uint8_t adc_init (uint8_t voltage_selection, uint8_t right_adjusted,
                  uint8_t adc_channel_selection,
                  uint8_t adc_prescaler_selection);
uint16_t adc_start (uint8_t right_adjusted);
void adc_disable (void);

#endif
