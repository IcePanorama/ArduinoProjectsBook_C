#include "lamp.h"
#include "uart_hal.h"

#include <avr/interrupt.h>

#define BAUD_RATE (9600)

static void init_serial_connection (void);

int
main (void)
{
  init_serial_connection ();
  l_init_lamp ();

  return l_lamp_loop ();
}

void
init_serial_connection (void)
{
  const char *START_MSG = "UART connection started.\r\n";

  uart_init ((BAUD_RATE), false);
  sei ();
  uart_send_string (START_MSG);
}
