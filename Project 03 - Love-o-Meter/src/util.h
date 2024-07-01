#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdint.h>

/*
 *  ftoa
 *
 *  Converts a float to a string with
 *  afterpoint-many digits after the
 *  decimal point, making sure not to
 *  exceed max_len digits in total.
 *
 *  params:
 *    f : float - the number to be converted
 *    res: uint8_t* - the resulting string
 *    afterpoint: uint8_t - desired num of digits following the decimal
 */
void ftoa (float f, uint8_t *res, uint8_t afterpoint, uint8_t max_len);

#endif
