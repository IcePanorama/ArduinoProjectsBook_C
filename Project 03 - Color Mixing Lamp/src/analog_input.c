#include "analog_input.h"

#include <stdlib.h>

static const ADCRefVoltage_t REF_VOLTAGE = ADCRV_AVCC;
static const bool RIGHT_ADJUSTED = false;
static const ADCPrescalerDivisor_t PRESCALER = ADCP_BY_128;

uint8_t
ai_create_analog_input (AnalogInput_t *ai, ADCChannel_t channel)
{
  ai->ref_voltage = REF_VOLTAGE;
  ai->right_adjusted = RIGHT_ADJUSTED;
  ai->channel = channel;
  ai->prescaler = PRESCALER;

  return adc_init_with_analog_input (ai);
}

int8_t
ai_analog_read (AnalogInput_t *ai, uint16_t *output)
{
  static AnalogInput_t *last_input = NULL;

  if (ai == NULL || output == NULL)
    return -1;

  if (ai != last_input)
    {
      if (adc_init_with_analog_input (ai) != ADC_INIT_SUCCESS)
        return -1;
    }

  *output = adc_start (ai->right_adjusted);
  return 0;
}
