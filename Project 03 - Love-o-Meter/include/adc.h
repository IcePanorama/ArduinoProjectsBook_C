#ifndef _ANALOG_TO_DIGITAL_CONVERTER_HAL_H_
#define _ANALOG_TO_DIGITAL_CONVERTER_HAL_H_

#include <stdbool.h>
#include <stdint.h>

typedef enum ADCInitResult_e
{
  ADC_INIT_SUCCESS,
  ADC_INIT_INVALID_REF_VOLTAGE_SELECTION,
  ADC_INIT_INVALID_CHANNEL_SELECTION,
  ADC_INIT_INVALID_PRESCALER_SELECTION
} ADCInitResult_t;

/* Reference Voltages */
typedef enum ADCRefVoltage_e
{
  ADCRV_AREF, // Internal Vref turned off
  ADCRV_AVCC, // AVcc
  ADCRV_1V1   // Internal 1.1V voltage reference
} ADCRefVoltage_t;

typedef enum ADCChannel_e
{
  ADCC_ADC0, // Arduino A0
  ADCC_ADC1, // Arduino A1
  ADCC_ADC2, // Arduino A2
  ADCC_ADC3, // Arduino A3
  ADCC_ADC4, // Arduino A4
  ADCC_ADC5,
  ADCC_ADC6,
  ADCC_ADC7,
  ADCC_ADC8,      // For onboard temperature sensor
  ADCC_VBG = 0xE, // 1.1V (Vbg) - Internal Bandgap voltage reference
  ADCC_GND = 0xF, // 0V (GND)
} ADCChannel_t;

typedef enum ADCPrescalerDivisor_e
{
  ADCP_BY_2 = 0x0, // Both 0b000 and 0b001 prescale by 2
  ADCP_BY_4 = 0x2,
  ADCP_BY_8,
  ADCP_BY_16,
  ADCP_BY_32,
  ADCP_BY_64,
  ADCP_BY_128,
} ADCPrescalerDivisor_t;

ADCInitResult_t adc_init (ADCRefVoltage_t ref_voltage, bool right_adjusted,
                          ADCChannel_t channel,
                          ADCPrescalerDivisor_t prescaler);

#endif /* _ANALOG_TO_DIGITAL_CONVERTER_HAL_H_ */
