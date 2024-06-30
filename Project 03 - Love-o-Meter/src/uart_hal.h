/*
 *	Universal Asynchronous Receiver-Transmitter Hardware Abstraction Layer
 *
 *	Handles initializing an asynchronous serial connection as well as
 *	the sending and recieving of data through that connection.
 */
#ifndef _UART_HAL_
#define _UART_HAL_

#include <stdint.h>

/*
 *  uart_init
 *
 *  Sets USART0 up for a two-way (receive/transmit) asynchronous
 *  serial connection with the given baud rate, the option for
 *  "double speed asynchronous communication mode", and 8-bit
 *  serial frames.
 *
 *  params:
 *    baud : uint32_t - desired baud rate.
 *    high_speed : uint8_t - bool for high speed config option.
 */
void uart_init (uint32_t baud, uint8_t high_speed);

/*
 *  uart_send_byte
 *
 *  Transmits a single byte over the serial connection.
 *
 *  params:
 *    b : uint8_t - the desired byte to transmit
 */
void uart_send_byte (uint8_t b);

/*
 *  uart_send_array
 *
 *  Transmits an array of data over the serial
 *  connection using the uart_send_byte function.
 *
 *  params:
 *    arr : uint8_t* - pointer to array of data
 *    length : uint16_t - length of array
 */
void uart_send_array (uint8_t *arr, uint16_t length);

/*
 *  uart_send_string
 *
 *  Transmits a given string over the serial
 *  connection using the uart_send_byte function.
 *
 *  params:
 *    str : const uint8_t* - the desired string to transmit
 */
void uart_send_string (const uint8_t *str);

/*
 *  uart_read_count
 *
 *  returns:
 *    uint16_t - the number of received bytes which have yet to be read.
 */
uint16_t uart_read_count (void);

/*
 *  uart_read_count
 *
 *  Reads the recieved data into the rx_buffer.
 */
uint8_t uart_read (void);

#endif
