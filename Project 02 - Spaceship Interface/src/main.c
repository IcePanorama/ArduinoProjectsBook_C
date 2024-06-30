#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

void main_loop (void);
void setup (void);
void flash_red_LEDs (void);

int
main (void)
{
  setup ();
  main_loop ();

  return 0;
}

/*
 *  Setup
 *
 *  Configures Port D as follows:
 *    * DDR2 (PORTD2) as input for switch
 *    * DDD3 (PORTD3) as output for green LED
 *    * DDR4 (PORTD4) as output for first red LED
 *    * DDR5 (PORTD5) as output for second red LED
 *
 */
void
setup (void)
{
  // initial value is 0 so this
  // is probably unnecessary,
  // but just to be safe
  DDRD &= ~(1 << DDD2);

  DDRD |= (1 << DDD3);
  DDRD |= (1 << DDD4);
  DDRD |= (1 << DDD5);
}

void
main_loop (void)
{
  while (1)
    {
      uint8_t switch_state = PIND & (1 << PIND2);
      if (switch_state == (1 << PIND2))
        {
          flash_red_LEDs ();
        }
      else
        {
          PORTD |= (1 << PORTD3);
          PORTD &= ~(1 << PORTD4);
          PORTD &= ~(1 << PORTD5);
        }
    }
}

void
flash_red_LEDs (void)
{
  const uint8_t DELAY_TIME = 250;

  PORTD &= ~(1 << PORTD3);
  PORTD &= ~(1 << PORTD4);
  PORTD |= (1 << PORTD5);

  _delay_ms (DELAY_TIME);

  PORTD |= (1 << PORTD4);
  PORTD &= ~(1 << PORTD5);

  _delay_ms (DELAY_TIME);
}
