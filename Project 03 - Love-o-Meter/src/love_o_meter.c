#include "love_o_meter.h"
#include "adc.h"
#include "uart_hal.h"

#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>

#define PORT_C_DATA_DIRECTION_REGISTER (DDRC)
#define PORT_C0_DATA_DIRECTION_BIT (DDC0)

#define PORT_D_DATA_DIRECTION_REGISTER (DDRD)
#define PORT_D2_DATA_DIRECTION_BIT (DDD2)
#define PORT_D3_DATA_DIRECTION_BIT (DDD3)
#define PORT_D4_DATA_DIRECTION_BIT (DDD4)

uint8_t
init_love_o_meter (void)
{
  /* Configure Port C0 as an input. */
  PORT_C_DATA_DIRECTION_REGISTER &= ~(1 << (PORT_C0_DATA_DIRECTION_BIT));

  /* Configure Port D2-4 as output. */
  PORT_D_DATA_DIRECTION_REGISTER |= (1 << PORT_D2_DATA_DIRECTION_BIT);
  PORT_D_DATA_DIRECTION_REGISTER |= (1 << PORT_D3_DATA_DIRECTION_BIT);
  PORT_D_DATA_DIRECTION_REGISTER |= (1 << PORT_D4_DATA_DIRECTION_BIT);

  if (adc_init (ADCRV_AVCC, true, ADCC_ADC0, ADCP_BY_128) != ADC_INIT_SUCCESS)
    {
      uart_send_string ("Fatal Error: Error initializing ADC.\r");
      return -1;
    }

  return 0;
}

void
love_o_meter_loop (void)
{
  char output_buffer[256] = { 0 };

  while (true)
    {
      const uint16_t sensor_val = adc_start (true);
      sprintf (output_buffer, "Sensor value: %d\r\n", sensor_val);
      uart_send_string (output_buffer);
    }
}
