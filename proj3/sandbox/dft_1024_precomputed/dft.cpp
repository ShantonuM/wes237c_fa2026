#include "dft.h"
#include "coefficients1024.h"

#define UNROLL_FACTOR   8

/* Keeping the array partitioning factor the same as the loop unrolling one */
#define ARR_PART_FACTOR UNROLL_FACTOR

void dft
(
  DTYPE real_sample[SIZE],
  DTYPE imag_sample[SIZE],
  DTYPE real_op[SIZE],
  DTYPE imag_op[SIZE]
)
{
  DTYPE w;
  DTYPE c, s;
  int   i, j, k;

  #pragma HLS array_partition variable=real_op cyclic factor=ARR_PART_FACTOR
  #pragma HLS array_partition variable=imag_op cyclic factor=ARR_PART_FACTOR

  /* ZI the outputs */
  for (i = 0; i < SIZE; i += 1)
  {
    real_op[i] = 0;
    imag_op[i] = 0;
  }

  /*
   * Calculate the contribution of the j-th input sample to every frequency
   * component/output.
   */
  for (j = 0; j < SIZE; j += 1)
  {
    for (i = 0; i < SIZE; i += 1)
    {
      #pragma HLS UNROLL factor=UNROLL_FACTOR

      /* Utilize the precomputed sine and cosine values */
      k = (i * j) & (SIZE - 1);
      c = cos_coefficients_table[k];
      s = sin_coefficients_table[k];

      /*
       * Multiply the current phasor with the appropriate input sample and keep
       * running sum.
       */
      real_op[i] += (real_sample[j] * c - imag_sample[j] * s);
      imag_op[i] += (real_sample[j] * s + imag_sample[j] * c);
    }
  }
}

