/* clang-format off */
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include "adc.h"
#include "uart_hal.h"
#include "util.h"
/* clang-format on */

// max value of uint16_t is 65535 (5 digits)
// 5 + len("\n\r\0") [3] = 8
// rounded up for floats
#define STR_MAX_LEN 10

static void setup_love_o_meter (void);
static void setup_serial_connection (void);
static void main_loop (void);
static void output_sensor_value (uint16_t sensor_value,
                                 uint8_t *output_buffer);
static float TMP36_sensor_value_to_voltage (uint16_t sensor_value);
static void output_voltage (float voltage, uint8_t *output_buffer);
static float voltage_to_temperature (float voltage);
static void output_temperature (float temperature, uint8_t *output_buffer);

int
main (void)
{
  setup_serial_connection ();
  setup_love_o_meter ();
  main_loop ();

  return 0;
}

/*
 *  setup_love_o_meter
 *
 *  Configures Port C as follows:
 *    * DDC0 (PORTC0) as input for TMP36
 *  Configures Port D as follows:
 *    * DDD2 (PORTD2) as output_buffer for green LED
 *    * DDD3 (PORTD3) as output_buffer for first red LED
 *    * DDD4 (PORTD4) as output_buffer for second red LED
 */
static void
setup_love_o_meter (void)
{
  // Initial value should be zero
  // doing this just to be safe
  DDRC &= ~(1 << DDC0);

  DDRD |= (1 << DDD2) | (1 << DDD3) | (1 << DDD4);

  const uint8_t ERROR[] = "Error initializing ADC!";
  if (adc_init (ADC_AVCC, 1, ADC_CHANNEL_0, ADC_PRESCALE_BY_128)
      != ADC_INIT_SUCCESS)
    {
      uart_send_string (ERROR);
      exit (1);
    }
}

static void
setup_serial_connection (void)
{
  const uint32_t BAUD_RATE = 9600;
  uint8_t START_MSG[] = "Connection Start:\n\r";

  uart_init (BAUD_RATE, 0);

  sei ();
  uart_send_string (START_MSG);
}

static void
main_loop (void)
{
  // TODO: have the arduino calculate this itself
  //  do it either at startup or on a button press
  // const float BASELINE_TMP = 20.0;

  while (1)
    {
      uint8_t output_buffer[STR_MAX_LEN];

      uint16_t sensor_value = adc_start (1);
      output_sensor_value (sensor_value, output_buffer);

      float voltage = TMP36_sensor_value_to_voltage (sensor_value);
      output_voltage (voltage, output_buffer);

      float temperature = voltage_to_temperature (voltage);
      output_temperature (temperature, output_buffer);

      _delay_ms (1000);
    }
}

static void
output_sensor_value (uint16_t sensor_value, uint8_t *output_buffer)
{
  const uint8_t sensor_value_label[] = "Sensor value: ";
  uart_send_string (sensor_value_label);

  itoa (sensor_value, (char *)output_buffer, 10);
  uint8_t len = strlen ((char *)output_buffer);
  output_buffer[len] = '\t';
  output_buffer[len + 1] = '\0';

  uart_send_string (output_buffer);
}

static float
TMP36_sensor_value_to_voltage (uint16_t sensor_value)
{
  return (sensor_value / 1024.0f) * 5.0f;
}

static void
output_voltage (float voltage, uint8_t *output_buffer)
{
  const uint8_t voltage_label[] = "Voltage: ";
  uart_send_string (voltage_label);

  ftoa (voltage, output_buffer, 2, STR_MAX_LEN);
  int len = strlen ((char *)output_buffer);
  output_buffer[len] = '\t';
  output_buffer[len + 1] = '\0';

  uart_send_string (output_buffer);
}

static float
voltage_to_temperature (float voltage)
{
  return (voltage - 0.5) * 100;
}

static void
output_temperature (float temperature, uint8_t *output_buffer)
{
  const uint8_t temperature_label[] = "Temperature (C): ";
  uart_send_string (temperature_label);

  ftoa (temperature, output_buffer, 2, STR_MAX_LEN);
  int len = strlen ((char *)output_buffer);
  output_buffer[len] = '\n';
  output_buffer[len + 1] = '\r';
  output_buffer[len + 2] = '\0';

  uart_send_string (output_buffer);
}
