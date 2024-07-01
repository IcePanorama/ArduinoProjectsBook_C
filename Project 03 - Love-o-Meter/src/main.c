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
#define SENSOR_VALUE_STR_MAX_LEN 8

void setup_love_o_meter (void);
void setup_serial_connection (void);
void main_loop (void);

int
main (void)
{
  setup_love_o_meter ();
  setup_serial_connection ();
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
  const uint8_t label[] = "Sensor value: ";
  const uint8_t err_txt[] = "err\n\r";

  while (1)
    {
      uart_send_string (label);

      if (adc_init (ADC_AVCC, 1, ADC_CHANNEL_0, ADC_PRESCALE_BY_128)
          != ADC_INIT_SUCCESS)
        {
          uart_send_string (err_txt);
          continue;
        }

      uint16_t sensor_value = adc_start (1);
      uint8_t output[SENSOR_VALUE_STR_MAX_LEN];
      itoa (sensor_value, (char *)output, 10);
      uint8_t len = strlen ((char *)output);
      output[len] = '\n';
      output[len + 1] = '\r';
      output[len + 2] = '\0';
      uart_send_string (output);

      _delay_ms (1000);
    }
}
