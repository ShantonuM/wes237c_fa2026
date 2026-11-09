#include <stdint.h>
#include "dsp_vs_pl.h"

void mac
(
  int  in1[SIZE],
  int  in2[SIZE],
  int *out
)
{
  int i, running_sum, product;

  #pragma HLS bind_op variable=product     op=mul impl=dsp
  #pragma HLS bind_op variable=running_sum op=add impl=dsp

  *out = 0;
  running_sum = 0;

  for (i = 0; i < SIZE; i++)
  {
    product = in1[i] * in2[i];
    running_sum += product;
  }

  *out = running_sum;
}

