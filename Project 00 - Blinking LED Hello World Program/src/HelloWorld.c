#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

const uint16_t DELAY_TIME = 1000;

int
main (void)
{
  DDRB = DDRB | (1 << DDB5);

  while (1)
    {
      PORTB |= (1 << PORTB5);
      _delay_ms (DELAY_TIME);
      PORTB &= ~(1 << PORTB5);
      _delay_ms (DELAY_TIME);
    }
}
