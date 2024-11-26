#include "lamp.h"
#include "analog_input.h"
#include "uart_hal.h"

#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>

#define STR_BUFFER_SIZE (256)

#define PORT_B_DATA_DIRECTION_REGISTER (DDRB)

#define RED_LED_PIN_DATA_DIR_BIT (DDB3)
#define GREEN_LED_PIN_DATA_DIR_BIT (DDB1)
#define BLUE_LED_PIN_DATA_DIR_BIT (DDB2)

#define PORT_C_DATA_DIRECTION_REGISTER (DDRC)

#define RED_PHOTORESISTOR_DATA_DIR_BIT (DDC0)
#define GREEN_PHOTORESISTOR_DATA_DIR_BIT (DDC1)
#define BLUE_PHOTORESISTOR_DATA_DIR_BIT (DDC1)

#define READ_ANALOG_INPUT_DELAY_MS (5000)

AnalogInput_t red_photoresistor = { 0 };
AnalogInput_t green_photoresistor = { 0 };
AnalogInput_t blue_photoresistor = { 0 };

static const ADCChannel_t RED_PHOTORESISTOR_CHANNEL = ADCC_ADC0;
static const ADCChannel_t GREEN_PHOTORESISTOR_CHANNEL = ADCC_ADC1;
static const ADCChannel_t BLUE_PHOTORESISTOR_CHANNEL = ADCC_ADC2;

int8_t
l_init_lamp (void)
{
  // Configure the red, green, and blue photoresistors as input.
  PORT_C_DATA_DIRECTION_REGISTER &= ~(1 << (RED_PHOTORESISTOR_DATA_DIR_BIT));
  PORT_C_DATA_DIRECTION_REGISTER &= ~(1 << (GREEN_PHOTORESISTOR_DATA_DIR_BIT));
  PORT_C_DATA_DIRECTION_REGISTER &= ~(1 << (BLUE_PHOTORESISTOR_DATA_DIR_BIT));

  /* clang-format off */
  if (
    (ai_create_analog_input (&red_photoresistor, RED_PHOTORESISTOR_CHANNEL) != ADC_INIT_SUCCESS)
    || (ai_create_analog_input (&green_photoresistor, GREEN_PHOTORESISTOR_CHANNEL) != ADC_INIT_SUCCESS)
    || (ai_create_analog_input (&blue_photoresistor, BLUE_PHOTORESISTOR_CHANNEL) != ADC_INIT_SUCCESS)
  )
  {
    uart_send_string("Error initializing analog inputs!\r\n");
    return -1;
  }
  /* clang-format on */

  // Configure the red, green, and blue pins as output.
  PORT_B_DATA_DIRECTION_REGISTER |= (1 << (RED_LED_PIN_DATA_DIR_BIT));
  PORT_B_DATA_DIRECTION_REGISTER |= (1 << (GREEN_LED_PIN_DATA_DIR_BIT));
  PORT_B_DATA_DIRECTION_REGISTER |= (1 << (BLUE_LED_PIN_DATA_DIR_BIT));

  return 0;
}

int8_t
l_lamp_loop (void)
{
  char buffer[(STR_BUFFER_SIZE)] = { 0 };

  while (true)
    {
      uint16_t red_sensor_val;
      if (ai_analog_read (&red_photoresistor, &red_sensor_val) != 0)
        goto error_cleanup;

      _delay_ms ((READ_ANALOG_INPUT_DELAY_MS));

      uint16_t green_sensor_val;
      if (ai_analog_read (&green_photoresistor, &green_sensor_val) != 0)
        goto error_cleanup;

      _delay_ms ((READ_ANALOG_INPUT_DELAY_MS));

      uint16_t blue_sensor_val;
      if (ai_analog_read (&blue_photoresistor, &blue_sensor_val) != 0)
        goto error_cleanup;

      sprintf (buffer, "Raw sensor values - red: %d green: %d blue: %d\r\n",
               red_sensor_val, green_sensor_val, blue_sensor_val);
      uart_send_string (buffer);

      const uint8_t red_value = (red_sensor_val >> 2);
      const uint8_t green_value = (green_sensor_val >> 2);
      const uint8_t blue_value = (blue_sensor_val >> 2);

      sprintf (buffer, "Mapped sensor values - red: %d green: %d blue: %d\r\n",
               red_value, green_value, blue_value);
      uart_send_string (buffer);
    }

  return 0;
error_cleanup:
  uart_send_string ("Error reading from analog input!\r\n");
  return -1;
}
