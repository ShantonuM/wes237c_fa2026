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
   * Local copy of the input. Top-level arguments become ports to memory
   * outside the IP, so only a local array can be placed in BRAM inside it.
   */

  #pragma HLS bind_storage variable=buf type=ram_1p impl=bram

  for (i = 0; i < SIZE; i++)
  {
    buf[i] = in[i];
  }

  running_sum = 0;

  for (i = 0; i < SIZE; i++)
  {
    running_sum += buf[i];
  }

  *out = running_sum;
}

