/*
 *  Universal Asynchronous Receiver-Transmitter Hardware Abstraction Layer
 *  Handles initializing an asynchronous serial connection as well as the
 *  sending and recieving of data through that connection.
 */
#ifndef _UART_HAL_H_
#define _UART_HAL_H_

#include <stdbool.h>
#include <stdint.h>

/*
 *  Sets USART0 up for a two-way (receive/transmit) asynchronous serial
 *  connection with the given baud rate, the option for "double speed
 *  asynchronous communication mode", and 8-bit serial frames.
 *  @param  baud_rate   the desired baud rate.
 *  @param  high_speed  bool for high speed config option
 */
void uart_init (uint32_t baud_rate, uint8_t high_speed);

/*
 *  Transmits a single byte over the serial connection.
 *  @param  b  the desired byte to transmit
 */
void uart_send_byte (uint8_t b);

/*
 *  Transmits an array of data over the serial connection using the
 *  `uart_send_byte` function.
 *  @param  arr     pointer to an array of bytes
 *  @param  length  length of array
 */
void uart_send_array (uint8_t *arr, uint16_t length);

/*
 *  Transmits a given string over the serial connection using the
 *  `uart_send_byte` function.
 *  @param  str  a string to transmit
 */
void uart_send_string (const char *str);

/* @return the number of received bytes which have yet to be read. */
uint16_t uart_read_count (void);

/* Reads the recieved data into the rx_buffer. */
uint8_t uart_read (void);

#endif /* _UART_HAL_H_ */
