#include "adc.h"
#include "analog_input.h"

#include <avr/io.h>
#include <stdint.h>

#define POWER_REDUCTION_REGISTER (PRR)
#define ADC_POWER_REDUCTION_BIT (PRADC)

#define ADC_CTRL_STATUS_REGISTER_A (ADCSRA)
#define ADC_ENABLE_BIT (ADEN)
#define START_CONVERSION_BIT (ADSC)

#define MULTIPLEXER_SELECTION_REGISTER (ADMUX)
#define REF_SELECTION_BIT_0 (REFS0)
#define REF_SELECTION_BIT_1 (REFS1)
#define LEFT_ADJUST_RESULT_BIT (ADLAR)

#define ADC_CONVERSION_RESULT (ADC)
#define ADC_DATA_REGISTER_HI (ADCH)

static bool adc_is_busy (void);
static bool adc_is_enabled (void);
static uint8_t init_reference_voltage (ADCRefVoltage_t rv);
static bool is_valid_adc_channel (ADCChannel_t c);
static bool is_valid_prescaler_value (ADCPrescalerDivisor_t p);

ADCInitResult_t
adc_init (ADCRefVoltage_t ref_voltage, bool right_adjusted,
          ADCChannel_t channel, ADCPrescalerDivisor_t prescaler)
{
  POWER_REDUCTION_REGISTER &= ~(1 << (ADC_POWER_REDUCTION_BIT));

  while (adc_is_busy ())
    ;

  if (init_reference_voltage (ref_voltage) != 0)
    return ADC_INIT_INVALID_REF_VOLTAGE_SELECTION;

  if (right_adjusted)
    MULTIPLEXER_SELECTION_REGISTER &= ~(1 << (LEFT_ADJUST_RESULT_BIT));
  else
    MULTIPLEXER_SELECTION_REGISTER |= (1 << (LEFT_ADJUST_RESULT_BIT));

  if (!is_valid_adc_channel (channel))
    return ADC_INIT_INVALID_CHANNEL_SELECTION;
  MULTIPLEXER_SELECTION_REGISTER |= channel;

  if (!is_valid_prescaler_value (prescaler))
    return ADC_INIT_INVALID_PRESCALER_SELECTION;
  ADC_CTRL_STATUS_REGISTER_A |= prescaler;

  if (!adc_is_enabled ())
    {
      ADC_CTRL_STATUS_REGISTER_A |= (1 << ADC_ENABLE_BIT);
    }

  return ADC_INIT_SUCCESS;
}

ADCInitResult_t
adc_init_with_analog_input (AnalogInput_t *ai)
{
  return adc_init (ai->ref_voltage, ai->right_adjusted, ai->channel,
                   ai->prescaler);
}

bool
adc_is_busy (void)
{
  return (ADC_CTRL_STATUS_REGISTER_A) & (1 << (START_CONVERSION_BIT));
}

uint8_t
init_reference_voltage (ADCRefVoltage_t rv)
{
  switch (rv)
    {
    case ADCRV_AREF:
      MULTIPLEXER_SELECTION_REGISTER &= ~(1 << (REF_SELECTION_BIT_0));
      MULTIPLEXER_SELECTION_REGISTER &= ~(1 << (REF_SELECTION_BIT_1));
      break;
    case ADCRV_AVCC:
      MULTIPLEXER_SELECTION_REGISTER |= (1 << (REF_SELECTION_BIT_0));
      MULTIPLEXER_SELECTION_REGISTER &= ~(1 << (REF_SELECTION_BIT_1));
      break;
    case ADCRV_1V1:
      MULTIPLEXER_SELECTION_REGISTER |= (1 << (REF_SELECTION_BIT_0));
      MULTIPLEXER_SELECTION_REGISTER |= (1 << (REF_SELECTION_BIT_1));
      break;
    default:
      return -1;
    }

  return 0;
}

bool
is_valid_adc_channel (ADCChannel_t c)
{
  switch (c)
    {
    case ADCC_ADC0:
    case ADCC_ADC1:
    case ADCC_ADC2:
    case ADCC_ADC3:
    case ADCC_ADC4:
    case ADCC_ADC5:
    case ADCC_ADC6:
    case ADCC_ADC7:
    case ADCC_ADC8:
    case ADCC_VBG:
    case ADCC_GND:
      return true;
    default:
      return false;
    }
}

bool
is_valid_prescaler_value (ADCPrescalerDivisor_t p)
{
  switch (p)
    {
    case ADCP_BY_2:
    case ADCP_BY_4:
    case ADCP_BY_8:
    case ADCP_BY_16:
    case ADCP_BY_32:
    case ADCP_BY_64:
    case ADCP_BY_128:
      return true;
    default:
      return false;
    }
}

bool
adc_is_enabled (void)
{
  return (ADC_CTRL_STATUS_REGISTER_A) & (1 << (ADC_ENABLE_BIT));
}

uint16_t
adc_start (bool right_adjusted)
{
  ADC_CTRL_STATUS_REGISTER_A |= (1 << (START_CONVERSION_BIT));

  while (adc_is_busy ())
    ;

  return right_adjusted ? ADC_CONVERSION_RESULT : ADC_DATA_REGISTER_HI;
}
