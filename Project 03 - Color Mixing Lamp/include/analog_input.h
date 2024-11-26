#ifndef _ANALOG_INPUT_H_
#define _ANALOG_INPUT_H_

#include "adc.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct AnalogInput_s
{
  ADCRefVoltage_t ref_voltage;
  bool right_adjusted;
  ADCChannel_t channel;
  ADCPrescalerDivisor_t prescaler;
} AnalogInput_t;

uint8_t ai_create_analog_input (AnalogInput_t *ai, ADCChannel_t channel);

#endif /* _ANALOG_INPUT_H_ */
