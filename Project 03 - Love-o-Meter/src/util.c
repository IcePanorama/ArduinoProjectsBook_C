#include <stdlib.h>
#include <string.h>

#include "util.h"

void
ftoa (float f, uint8_t *res, uint8_t afterpoint, uint8_t max_len)
{
  uint8_t int_part = (uint8_t)f;
  float fl_part = f - (float)int_part;
  itoa (int_part, (char *)res, 10);

  uint8_t len = strlen ((char *)res);
  if (afterpoint != 0 && (len + afterpoint < max_len))
    {
      res[len] = '.';
      for (uint8_t i = 0; i < afterpoint; i++)
        {
          fl_part *= 10;
        }
      itoa ((uint16_t)fl_part, (char *)(res + len + 1), 10);
    }
}
