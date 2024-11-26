#include "lamp.h"
#include "analog_input.h"

#include <avr/io.h>

#define PORT_C_DATA_DIRECTION_REGISTER (DDRC)

#define RED_PHOTORESISTOR_DATA_DIR_BIT (DDC0)
#define GREEN_PHOTORESISTOR_DATA_DIR_BIT (DDC1)
#define BLUE_PHOTORESISTOR_DATA_DIR_BIT (DDC1)

#define PORT_B_DATA_DIRECTION_REGISTER (DDRB)

#define RED_LED_PIN_DATA_DIR_BIT (DDB3)
#define GREEN_LED_PIN_DATA_DIR_BIT (DDB1)
#define BLUE_LED_PIN_DATA_DIR_BIT (DDB2)

AnalogInput_t red_photoresistor = { 0 };
AnalogInput_t green_photoresistor = { 0 };
AnalogInput_t blue_photoresistor = { 0 };

uint8_t
l_init_lamp (void)
{
  // Configure the red, green, and blue photoresistors as input.
  PORT_C_DATA_DIRECTION_REGISTER &= ~(1 << (RED_PHOTORESISTOR_DATA_DIR_BIT));
  PORT_C_DATA_DIRECTION_REGISTER &= ~(1 << (GREEN_PHOTORESISTOR_DATA_DIR_BIT));
  PORT_C_DATA_DIRECTION_REGISTER &= ~(1 << (BLUE_PHOTORESISTOR_DATA_DIR_BIT));

  ai_create_analog_input (&red_photoresistor, ADCC_ADC0);
  ai_create_analog_input (&green_photoresistor, ADCC_ADC1);
  ai_create_analog_input (&blue_photoresistor, ADCC_ADC2);

  // Configure the red, green, and blue pins as output.
  PORT_B_DATA_DIRECTION_REGISTER |= (1 << (RED_LED_PIN_DATA_DIR_BIT));
  PORT_B_DATA_DIRECTION_REGISTER |= (1 << (GREEN_LED_PIN_DATA_DIR_BIT));
  PORT_B_DATA_DIRECTION_REGISTER |= (1 << (BLUE_LED_PIN_DATA_DIR_BIT));

  return 0;
}

void
l_lamp_loop (void)
{
}
