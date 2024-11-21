#include "love_o_meter.h"

#include <avr/io.h>

#define PORT_C_DATA_DIRECTION_REGISTER (DDRC)
#define PORT_C_DATA_DIRECTION_BIT (DDC0)

#define PORT_D_DATA_DIRECTION_REGISTER (DDRD)

uint8_t
init_love_o_meter (void)
{
  /* Configure Port C as an input. */
  PORT_C_DATA_DIRECTION_REGISTER |= ~(1 << (PORT_C_DATA_DIRECTION_BIT));
}
