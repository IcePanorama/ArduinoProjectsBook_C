#include "love_o_meter.h"
#include "uart_hal.h"

#include <avr/interrupt.h>

#define BAUD_RATE (9600)

static void init_serial_connection (void);

int
main (void)
{
  init_serial_connection ();
  if (init_love_o_meter () == 0)
    {
      uart_send_string ("Love-o-meter initialized.\r");
    }

  return 0;
}

void
init_serial_connection (void)
{
  const char *START_MSG = "Connection start.\r";

  uart_init ((BAUD_RATE), false);
  sei ();
  uart_send_string (START_MSG);
}
