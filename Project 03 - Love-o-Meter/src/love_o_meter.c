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

static float sensor_value_to_voltage (uint16_t val);
static float voltage_to_temperature (float v);
static void configure_output_leds_w_temperature (float temp);

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

  return 0;
}

void
love_o_meter_loop (void)
{
  char output_buffer[256] = { 0 };

  while (true)
    {
      uint16_t sensor_val = adc_start (true);
      float voltage = sensor_value_to_voltage (sensor_val);
      float temperature = voltage_to_temperature (voltage);

      /*
       *  avr libc sprintf can't handle floats, apparently.
       *  @see:
       * https://onlinedocs.microchip.com/oxy/GUID-317042D4-BCCE-4065-BB05-AC4312DBC2C4-en-US-2/GUID-BC6AFB6B-C75E-4B3B-9185-1F369F36AE22.html#GUID-BC6AFB6B-C75E-4B3B-9185-1F369F36AE22
       */
      sprintf (output_buffer,
               "Sensor value: %d Voltage: 0.%02d Temperature (C): %d.%d\r\n",
               sensor_val, (int)(voltage * 100), (int)temperature,
               ((int)(temperature * 10) % 100));
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

void
configure_output_leds_w_temperature (float temp)
{
  const float BASELINE_TEMP = 20.0;

  if (temp < BASELINE_TEMP + 2)
    {
      // all off
      PORT_D_DATA_REGISTER &= ~(1 << (PORT_D2));
      PORT_D_DATA_REGISTER &= ~(1 << (PORT_D3));
      PORT_D_DATA_REGISTER &= ~(1 << (PORT_D4));
    }
  else if (temp < BASELINE_TEMP + 4)
    {
      PORT_D_DATA_REGISTER |= (1 << (PORT_D2));
      PORT_D_DATA_REGISTER &= ~(1 << (PORT_D3));
      PORT_D_DATA_REGISTER &= ~(1 << (PORT_D4));
    }
  else if (temp < BASELINE_TEMP + 6)
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
