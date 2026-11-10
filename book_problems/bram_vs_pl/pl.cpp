#include <stdint.h>
#include "bram_vs_pl.h"

void accumulate
(
  int  in[SIZE],
  int *out
)
{
  int buf[SIZE];
  int i, running_sum;

  /*
   * Local copy of the input, completely partitioned into registers (FFs)
   * in the programmable logic instead of BRAM.
   */
  #pragma HLS array_partition variable=buf complete

  for (i = 0; i < SIZE; i++)
  {
    buf[i] = in[i];
  }

  running_sum = 0;

  for (i = 0; i < SIZE; i++)
  {
    #pragma HLS UNROLL
    running_sum += buf[i];
  }

  *out = running_sum;
}

