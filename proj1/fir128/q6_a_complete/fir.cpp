/*
  Filename: fir.cpp
    FIR lab wirtten for WES/CSE237C class at UCSD.
    Match filter
  INPUT:
    x: signal (chirp)

  OUTPUT:
    y: filtered output

*/

#include "fir.h"

void fir (
  data_t *y,
  data_t x
  )
{

  coef_t c[N] = {10, 11, 11, 8, 3, -3, -8, -11, -11, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -10, -11, -11, -8, -3, 3, 8, 11, 11, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 8, 3, -3, -8, -11, -11, -10, -10, -10, -10, -10, -10, -10, -10, -11, -11, -8, -3, 3, 8, 11, 11, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 8, 3, -3, -8, -11, -11, -10, -10, -10, -10, -10, -10, -10, -10, -11, -11, -8, -3, 3, 8, 11, 11, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10};
  
  static
    data_t shift_reg[N];
    acc_t acc;
    int i;

  /* NOTE: Only 1 array patitioning scheme can be used at a time */

  /* cyclic memory partitioning */
  // #pragma HLS array_partition variable=shift_reg cyclic factor=4
  // #pragma HLS array_partition variable=c         cyclic factor=4

  /* complete memory partitioning */
  #pragma HLS array_partition variable=shift_reg complete
  #pragma HLS array_partition variable=c         complete

  /* block memory partitioning */
  // #pragma HLS array_partition variable=shift_reg block factor=2
  // #pragma HLS array_partition variable=c         block factor=2

  /* Leveraged from pp4fpgas book section 2.8/figure 2.5 */
  Tapped_Delayed_Line:
  for (i = N - 1; i > 0; i--){
    #pragma HLS unroll factor=2
    shift_reg[i] = shift_reg[i - 1];
  }
  shift_reg[0] = x;

  acc = 0;

  Multiply_Accumulate:
  for (i = N - 1; i >= 0; i--){
    #pragma HLS unroll factor=2
    acc += shift_reg[i] * c[i];
  }

  *y = acc;
}

