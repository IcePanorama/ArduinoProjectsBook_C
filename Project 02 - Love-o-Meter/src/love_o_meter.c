#include "love_o_meter.h"
#include "adc.h"
#include "uart_hal.h"

#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

#define PORT_C_DATA_DIRECTION_REGISTER (DDRC)
#define PORT_C0_DATA_DIRECTION_BIT (DDC0)

#define PORT_D_DATA_REGISTER (PORTD)
#define PORT_D2 (PORTD2)
#define PORT_D3 (PORTD3)
#define PORT_D4 (PORTD4)
#define PORT_D_DATA_DIRECTION_REGISTER (DDRD)
#define PORT_D2_DATA_DIRECTION_BIT (DDD2)
#define PORT_D3_DATA_DIRECTION_BIT (DDD3)
#define PORT_D4_DATA_DIRECTION_BIT (DDD4)

static float calculate_baseline_temp (void);
static float sensor_value_to_voltage (uint16_t val);
static float voltage_to_temperature (float v);
static float celsius_to_fahrenheit (float c);
static void configure_output_leds_w_temperature (float temp);

static float baseline_temp = 20.0;

uint8_t
init_love_o_meter (void)
{
  /* Configure Port C0 as an input. */
  PORT_C_DATA_DIRECTION_REGISTER &= ~(1 << (PORT_C0_DATA_DIRECTION_BIT));

  /* Configure Port D2-4 as output. */
  PORT_D_DATA_DIRECTION_REGISTER |= (1 << PORT_D2_DATA_DIRECTION_BIT);
  PORT_D_DATA_DIRECTION_REGISTER |= (1 << PORT_D3_DATA_DIRECTION_BIT);
  PORT_D_DATA_DIRECTION_REGISTER |= (1 << PORT_D4_DATA_DIRECTION_BIT);

  if (adc_init (ADCRV_AVCC, true, ADCC_ADC0, ADCP_BY_128) != ADC_INIT_SUCCESS)
    {
      uart_send_string ("Fatal Error: Error initializing ADC.\r\n");
      return -1;
    }

  uart_send_string ("Calculating baseline temperature...\r\n");
  baseline_temp = calculate_baseline_temp ();
  uart_send_string ("Baseline temperature calculation complete.\r\n");

  return 0;
}

float
calculate_baseline_temp (void)
{
  float total = 0.0;

  for (uint8_t i = 0; i < 5; i++)
    {
      const uint16_t sensor_val = adc_start (true);
      const float voltage = sensor_value_to_voltage (sensor_val);
      const float temp = voltage_to_temperature (voltage);
      total += temp;
      _delay_ms (1000);
    }

  return total / 5;
}

void
love_o_meter_loop (void)
{
  char output_buffer[256] = { 0 };

  sprintf (output_buffer, "Baseline temperature (C): %d\r\n",
           (int)baseline_temp);
  uart_send_string (output_buffer);

  while (true)
    {
      const uint16_t sensor_val = adc_start (true);
      const float voltage = sensor_value_to_voltage (sensor_val);
      const float temperature = voltage_to_temperature (voltage);
      const float temp_f = celsius_to_fahrenheit (temperature);

      /*
       *  avr libc sprintf can't handle floats, apparently.
       *  @see:
       * https://onlinedocs.microchip.com/oxy/GUID-317042D4-BCCE-4065-BB05-AC4312DBC2C4-en-US-2/GUID-BC6AFB6B-C75E-4B3B-9185-1F369F36AE22.html#GUID-BC6AFB6B-C75E-4B3B-9185-1F369F36AE22
       */
      sprintf (output_buffer,
               "Sensor value: %d Voltage: 0.%02d Temperature (C): %d "
               "Temperature (F): %d\r\n",
               sensor_val, (int)(voltage * 100), (int)temperature,
               (int)temp_f);
      uart_send_string (output_buffer);

      configure_output_leds_w_temperature (temperature);
      _delay_ms (1000);
    }
}

float
sensor_value_to_voltage (uint16_t val)
{
  return (val / 1024.0) * 5.0;
}

float
voltage_to_temperature (float v)
{
  return (v - 0.5) * 100;
}

float
celsius_to_fahrenheit (float c)
{
  return (c * 1.8) + 32;
}

void
configure_output_leds_w_temperature (float temp)
{
  if (temp < baseline_temp + 2)
    {
      PORT_D_DATA_REGISTER &= ~(1 << (PORT_D2));
      PORT_D_DATA_REGISTER &= ~(1 << (PORT_D3));
      PORT_D_DATA_REGISTER &= ~(1 << (PORT_D4));
    }
  else if (temp < baseline_temp + 4)
    {
      PORT_D_DATA_REGISTER |= (1 << (PORT_D2));
      PORT_D_DATA_REGISTER &= ~(1 << (PORT_D3));
      PORT_D_DATA_REGISTER &= ~(1 << (PORT_D4));
    }
  else if (temp < baseline_temp + 6)
    {
      PORT_D_DATA_REGISTER |= (1 << (PORT_D2));
      PORT_D_DATA_REGISTER |= (1 << (PORT_D3));
      PORT_D_DATA_REGISTER &= ~(1 << (PORT_D4));
    }
  else
    {
      PORT_D_DATA_REGISTER |= (1 << (PORT_D2));
      PORT_D_DATA_REGISTER |= (1 << (PORT_D3));
      PORT_D_DATA_REGISTER |= (1 << (PORT_D4));
    }
}
