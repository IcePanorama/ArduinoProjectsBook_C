#include <avr/io.h>
#include <stdint.h>

int adc_init (uint8_t voltage_selection, uint8_t right_adjusted,
              uint8_t adc_channel_selection, uint8_t adc_prescaler_selection);
static uint8_t adc_init_voltage_selection (uint8_t selection);
static uint8_t is_valid_adc_channel (uint8_t c);
static void adc_init_channel_selection (uint8_t selection);
static uint8_t is_valid_adc_prescaler (uint8_t p);
static void adc_init_prescaler_selection (uint8_t selection);

enum adc_init_result
{
  ADC_INIT_SUCCESS,
  ADC_INIT_INVALID_VOLTAGE_SELECTION,
  ADC_INIT_INVALID_CHANNEL_SELECTION,
  ADC_INIT_INVALID_PRESCALER_SELECTION
};

enum high_voltage_selection
{
  AREF,
  AVCC,
  INTERNAL_REF
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

int
adc_init (uint8_t voltage_selection, uint8_t right_adjusted,
          uint8_t adc_channel_selection, uint8_t adc_prescaler_selection)
{
  if (adc_init_voltage_selection (voltage_selection)
      == ADC_INIT_INVALID_VOLTAGE_SELECTION)
    {
      return ADC_INIT_INVALID_VOLTAGE_SELECTION;
    }

  if (right_adjusted)
    {
      ADMUX &= ~(1 << ADLAR);
    }
  else
    {
      ADMUX |= (1 << ADLAR);
    }

  if (!is_valid_adc_channel (adc_channel_selection))
    {
      return ADC_INIT_INVALID_CHANNEL_SELECTION;
    }
  adc_init_channel_selection (adc_channel_selection);

  if (!is_valid_adc_prescaler (adc_prescaler_selection))
    {
      return ADC_INIT_INVALID_PRESCALER_SELECTION;
    }

  adc_init_prescaler_selection (adc_prescaler_selection);

  ADCSRA |= (1 << ADEN);
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

static uint8_t
adc_init_voltage_selection (uint8_t selection)
{
  switch (selection)
    {
    /*
     *  Sets the reference selection bits in
     *  the ADC Multiplex Selection Register.
     *  See 23.9.1 ADMUX – ADC Multiplexer
     *  Selection Register, Bit 7:6 (pg. 217)
     *  for more details.
     */
    case AREF:
      ADMUX &= ~((1 << REFS1) | (1 << REFS0));
      break;
    case AVCC:
      ADMUX &= ~(1 << REFS1);
      ADMUX |= (1 << REFS0);
      break;
    case INTERNAL_REF:
      ADMUX |= (1 << REFS1) | (1 << REFS0);
      break;
    default:
      return ADC_INIT_INVALID_VOLTAGE_SELECTION;
    }
  return 0;
}

static void
adc_init_channel_selection (uint8_t selection)
{
  const uint8_t ADC_CHANNEL_SELECTION_MASK = 0xF;

  // initial value should be
  // zero, just doing this to
  // be safe.
  ADMUX &= ~(ADC_CHANNEL_SELECTION_MASK);
  ADMUX |= selection;
}

static void
adc_init_prescaler_selection (uint8_t selection)
{
  const uint8_t ADC_PRESCALER_SELECTION_MASK = 0x7;

  // Doing this out of an abundance of
  // caution, probably unnecessary
  ADCSRA &= ~(ADC_PRESCALER_SELECTION_MASK);
  ADCSRA |= selection;
}
