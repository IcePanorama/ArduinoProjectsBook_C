#include <avr/io.h>

#include "adc.h"

static uint8_t is_valid_adc_channel (uint8_t c);
static uint8_t is_valid_adc_prescaler (uint8_t p);
static uint8_t adc_busy (void);

uint8_t
adc_init (uint8_t voltage_selection, uint8_t right_adjusted,
          uint8_t adc_channel_selection, uint8_t adc_prescaler_selection)
{
  // Disable power reduction ADC bit
  PRR &= ~(1 << PRADC);

  // makes sure adc isn't in use
  // before changing settings
  while (adc_busy ())
    ;

  /*
   *  Sets the reference selection bits in
   *  the ADC Multiplex Selection Register.
   *  See 23.9.1 ADMUX – ADC Multiplexer
   *  Selection Register, Bit 7:6 (pg. 217)
   *  for more details.
   */
  switch (voltage_selection)
    {
    case ADC_AREF:
      ADMUX &= ~((1 << REFS1) | (1 << REFS0));
      break;
    case ADC_AVCC:
      ADMUX &= ~(1 << REFS1);
      ADMUX |= (1 << REFS0);
      break;
    case ADC_INTERNAL_REF:
      ADMUX |= (1 << REFS1) | (1 << REFS0);
      break;
    default:
      return ADC_INIT_INVALID_VOLTAGE_SELECTION;
    }

  /*
   *  if right_adjusted, use 10-bit
   *  conversion results, else
   *  use 8-bit conversion results.
   */
  if (right_adjusted)
    {
      ADMUX &= ~(1 << ADLAR);
    }
  else
    {
      ADMUX |= (1 << ADLAR);
    }

  /*
   *  Select input channel
   */
  if (!is_valid_adc_channel (adc_channel_selection))
    {
      return ADC_INIT_INVALID_CHANNEL_SELECTION;
    }
  ADMUX |= adc_channel_selection;

  /*
   *  Select the clock prescaler value.
   */
  // TODO: how does one go about choosing a prescaler value?
  if (!is_valid_adc_prescaler (adc_prescaler_selection))
    {
      return ADC_INIT_INVALID_PRESCALER_SELECTION;
    }
  ADCSRA |= adc_prescaler_selection;

  if (!(ADCSRA & (1 << ADEN)))
    {
      // enable adc, but don't start conversion.
      ADCSRA |= (1 << ADEN);
    }

  return ADC_INIT_SUCCESS;
}

static uint8_t
is_valid_adc_channel (uint8_t c)
{
  switch (c)
    {
    case ADC_CHANNEL_0:
    case ADC_CHANNEL_1:
    case ADC_CHANNEL_2:
    case ADC_CHANNEL_3:
    case ADC_CHANNEL_4:
    case ADC_CHANNEL_5:
    case ADC_CHANNEL_6:
    case ADC_CHANNEL_7:
    case ADC_CHANNEL_8:
    case ADC_CHANNEL_1v1:
    case ADC_CHANNEL_GND:
      return 1;
    default:
      return 0;
    }
}

static uint8_t
is_valid_adc_prescaler (uint8_t p)
{
  switch (p)
    {
    case ADC_PRESCALE_BY_2:
    case ADC_PRESCALE_BY_4:
    case ADC_PRESCALE_BY_8:
    case ADC_PRESCALE_BY_16:
    case ADC_PRESCALE_BY_32:
    case ADC_PRESCALE_BY_64:
    case ADC_PRESCALE_BY_128:
      return 1;
    default:
      return 0;
    }
}

uint16_t
adc_start (uint8_t right_adjusted)
{
  ADCSRA |= (1 << ADSC);
  while (adc_busy ())
    ;

  /*
   *  "ADCL must be read first" 23.9.3 ADCL
   *  and ADCH - The ADC Data Register (pg. 219)
   */
  if (right_adjusted)
    {
      uint16_t res = ADCL;
      res |= ((ADCH & 0x3) << 8);
      return res;
    }
  return ADCH;
}

void
adc_disable (void)
{
  while (adc_busy ())
    ;

  // disable adc
  ADCSRA &= ~(1 << ADEN);

  // Enable power reduction ADC bit
  PRR |= (1 << PRADC);
}

static uint8_t
adc_busy (void)
{
  return ADCSRA & (1 << ADSC);
}
