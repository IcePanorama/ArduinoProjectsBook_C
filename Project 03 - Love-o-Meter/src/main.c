#include "uart_hal.h"

#include <avr/interrupt.h>

#define BAUD_RATE (9600)

static void init_serial_connection (void);

int
main (void)
{
  init_serial_connection ();

  return 0;
}

void
init_serial_connection (void)
{
  const char *START_MSG = "Connection start:\n";

  uart_init ((BAUD_RATE), false);
  sei ();
  uart_send_string (START_MSG);
}
