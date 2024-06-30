#include "uart_hal.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

// A0 | D14 - PC0 - ADC[0] (default)
static const uint32_t BAUD_RATE = 9600;

void setup (void);

int
main (void)
{
  // TODO: have the arduino calculate this itself
  //  do it either at startup or on a button press
  //  const float BASELINE_TMP = 20.0;
  // setup ();

  uint8_t data = 'A';
  uart_init (BAUD_RATE, 0);

  sei ();
  while (1)
    {
      uart_send_byte (data);
      _delay_ms (500);

      data++;
      if (data > 'Z')
        {
          data = 'A';
        }
    }

  return 0;
}

/*
 *  Setup
 *
 *  Configures Port C as follows:
 *    * DDC0 (PORTC0) as input for TMP36
 *  Configures Port D as follows:
 *    * DDD2 (PORTD2) as output for green LED
 *    * DDD3 (PORTD3) as output for first red LED
 *    * DDD4 (PORTD4) as output for second red LED
 *
 */
void
setup (void)
{
  uart_init (BAUD_RATE, 0);

  DDRD |= (1 << DDD2) | (1 << DDD3) | (1 << DDD4);

  // Initial value should be zero
  // doing this just to be safe
  DDRC &= ~(1 << DDC0);
}
