#include "analog_input.h"

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
