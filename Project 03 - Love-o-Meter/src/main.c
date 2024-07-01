/* clang-format off */
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include "adc.h"
#include "uart_hal.h"
/* clang-format on */

// max value of uint16_t is 65535 (5 digits)
// + '\n' (1 = 6) + '\r' (1 = 7) + '\0' (1 = 8)
#define STR_MAX_LEN 16

void setup_love_o_meter (void);
void setup_serial_connection (void);
void main_loop (void);
void ftoa (float f, uint8_t *res, uint8_t afterpoint);

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
 *    * DDD2 (PORTD2) as output for green LED
 *    * DDD3 (PORTD3) as output for first red LED
 *    * DDD4 (PORTD4) as output for second red LED
 */
void
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

void
setup_serial_connection (void)
{
  const uint32_t BAUD_RATE = 9600;
  uint8_t START_MSG[] = "Connection Start:\n\r";

  uart_init (BAUD_RATE, 0);

  sei ();
  uart_send_string (START_MSG);
}

void
main_loop (void)
{
  // TODO: have the arduino calculate this itself
  //  do it either at startup or on a button press
  // const float BASELINE_TMP = 20.0;
  const uint8_t sensor_value_label[] = "Sensor value: ";
  const uint8_t voltage_label[] = "Voltage: ";
  const uint8_t temperature_label[] = "Temperature (C): ";

  while (1)
    {
      uint8_t output[STR_MAX_LEN];

      uart_send_string (sensor_value_label);

      uint16_t sensor_value = adc_start (1);
      itoa (sensor_value, (char *)output, 10);
      uint8_t len = strlen ((char *)output);
      output[len] = '\t';
      output[len + 1] = '\0';
      uart_send_string (output);

      uart_send_string (voltage_label);

      float voltage = (sensor_value / 1024.0f) * 5.0f;
      ftoa (voltage, output, 2);
      len = strlen ((char *)output);
      output[len] = '\t';
      output[len + 1] = '\0';
      uart_send_string (output);

      uart_send_string (temperature_label);

      float temperature = (voltage - 0.5) * 100;
      ftoa (temperature, output, 2);
      len = strlen ((char *)output);
      output[len] = '\n';
      output[len + 1] = '\r';
      output[len + 2] = '\0';
      uart_send_string (output);

      _delay_ms (1000);
    }
}

void
ftoa (float f, uint8_t *res, uint8_t afterpoint)
{
  uint8_t int_part = (uint8_t)f;
  float fl_part = f - (float)int_part;
  itoa (int_part, (char *)res, 10);

  uint8_t len = strlen ((char *)res);
  if (afterpoint != 0 && (len + afterpoint < STR_MAX_LEN))
    {
      res[len] = '.';
      for (uint8_t i = 0; i < afterpoint; i++)
        {
          fl_part *= 10;
        }
      itoa ((uint16_t)fl_part, (char *)(res + len + 1), 10);
    }
}
