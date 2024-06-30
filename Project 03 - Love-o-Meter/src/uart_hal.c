#include "uart_hal.h"
#include <avr/interrupt.h>

static volatile uint8_t uart_tx_busy = 0;

/*
 *  USART RX Complete Interrupt
 */
ISR (USART_RX_vect) {}

/*
 *  USART TX Complete Interrupt
 */
ISR (USART_TX_vect) { uart_tx_busy = 0; }

void
uart_init (uint32_t baud, uint8_t high_speed)
{
  uint8_t speed = 16;

  if (high_speed)
    {
      speed = 8;
      // set the flag for double USART transmission speed
      UCSR0A |= 1 << U2X0;
    }

  /*
   *  Formula from table 19-1 of the
   *  ATmega328P datasheet on pg 146.
   */
  baud = (F_CPU / (speed * baud)) - 1; // reusing baud variable to save memory

  UBRR0H = (baud & 0x0F00) >> 8;
  UBRR0L = (baud & 0x00FF);

  /*
   *  Enables the following flags on the USART Control and Status Register 0 B:
   *    * RX Complete Interrupt Enable (RXCIE0)
   *    * TX Complete Interrupt Enable (TXCIE0)
   *    * Trasmitter Enable (TXEN0)
   *    * Receiver Enable (RXEN0)
   */
  UCSR0B |= (1 << RXCIE0) | (1 << TXCIE0) | (1 << RXEN0) | (1 << TXEN0);
}

void
uart_send_byte (uint8_t b)
{
  while (uart_tx_busy)
    ;
  uart_tx_busy = 1;

  // UDR0 = tx byte
  UDR0 = b;
}

void
uart_send_array (uint8_t *arr, uint16_t length)
{
  for (uint16_t i = 0; i < length; i++)
    {
      uart_send_byte (arr[i]);
    }
}

void
uart_send_string (uint8_t *str)
{
  // when str is NULL, won't this send '\0' twice?
  // Look into just using a for loop instead
  uint16_t i = 0;
  do
    {
      uart_send_byte (str[i]);
      i++;
    }
  while (str[i] != '\0');

  // transmit null character also
  uart_send_byte (str[i]);
}