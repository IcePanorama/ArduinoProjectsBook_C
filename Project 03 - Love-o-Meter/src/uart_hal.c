#include "uart_hal.h"

#include <avr/interrupt.h>

#define USART_IO_DATA_REGISTER (UDR0)

#define CONTROL_STATUS_REGISTER_0A (UCSR0A)
#define CONTROL_STATUS_REGISTER_0B (UCSR0B)

/* USART Control and Status Register n A bits.*/
#define DOUBLE_TRANSMISSION_SPEED_BIT (U2X0)

#define BAUD_RATE_REGISTERS_HI (UBRR0H)
#define BAUD_RATE_REGISTERS_LO (UBRR0L)

/* USART Control and Status Register n B bits. */
#define RX_COMPLETE_INTERRUPT_ENABLE_BIT (RXCIE0)
#define TX_COMPLETE_INTERRUPT_ENABLE_BIT (TXCIE0)
#define RECEIVER_ENABLE_BIT (RXEN0)
#define TRANSMITTER_ENABLE_BIT (TXEN0)

#define RX_BUFFER_SIZE (128)

/* A circular buffer for storing received data. */
static volatile uint8_t rx_buffer[(RX_BUFFER_SIZE)] = { 0 };

static volatile uint16_t rx_count = 0;
static volatile bool uart_tx_busy = false;

/* USART RX Complete Interrupt */
ISR (USART_RX_vect)
{
  static volatile uint16_t rx_write_pos = 0;

  rx_buffer[rx_write_pos] = USART_IO_DATA_REGISTER;
  rx_count++;
  rx_write_pos++;
  if (rx_write_pos >= (RX_BUFFER_SIZE))
    {
      rx_write_pos = 0;
    }
}

/* USART TX Complete Interrupt */
ISR (USART_TX_vect) { uart_tx_busy = false; }

void
uart_init (uint32_t baud_rate, uint8_t high_speed)
{
  uint8_t speed = 16;

  if (high_speed)
    {
      speed = 8;
      CONTROL_STATUS_REGISTER_0A |= 1 << (DOUBLE_TRANSMISSION_SPEED_BIT);
    }

  /*
   *  Formula from table 19-1 of the ATmega328P datasheet on pg 146.
   *  Reusing baud_rate variable to save memory.
   */
  baud_rate = (F_CPU / (speed * baud_rate)) - 1;

  BAUD_RATE_REGISTERS_HI = (baud_rate & 0x0F00) >> 8;
  BAUD_RATE_REGISTERS_LO = (baud_rate & 0x00FF);

  CONTROL_STATUS_REGISTER_0B |= 1 << (RX_COMPLETE_INTERRUPT_ENABLE_BIT);
  CONTROL_STATUS_REGISTER_0B |= 1 << (TX_COMPLETE_INTERRUPT_ENABLE_BIT);
  CONTROL_STATUS_REGISTER_0B |= 1 << (RECEIVER_ENABLE_BIT);
  CONTROL_STATUS_REGISTER_0B |= 1 << (TRANSMITTER_ENABLE_BIT);
}

void
uart_send_byte (uint8_t b)
{
  while (uart_tx_busy)
    ;

  uart_tx_busy = true;

  USART_IO_DATA_REGISTER = b;
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
uart_send_string (const char *str)
{
  uint16_t i = 0;

  // small optimization for when str is emtpy
  // (avoids sending null character twice)
  if (str[i] != '\0')
    {
      do
        {
          uart_send_byte (str[i]);
          i++;
        }
      while (str[i] != '\0');
    }

  // transmit null character also
  uart_send_byte (str[i]);
}

uint16_t
uart_read_count (void)
{
  return rx_count;
}

uint8_t
uart_read (void)
{
  static uint16_t rx_read_pos = 0;
  uint8_t data = 0;

  data = rx_buffer[rx_read_pos];
  rx_read_pos++;
  rx_count--;
  if (rx_read_pos >= (RX_BUFFER_SIZE))
    {
      rx_read_pos = 0;
    }

  return data;
}
