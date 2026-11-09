#include <stdint.h>
#include "bram_vs_pl.h"

void accumulate
(
  int  in[SIZE],
  int *out
)
{
  int i, running_sum;

  *out = 0;
  running_sum = 0;

  for (i = 0; i < SIZE; i++)
  {
    running_sum += in[i];
  }

  *out = running_sum;
}

