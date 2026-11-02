#include "dft.h"
#include "coefficients1024.h"

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

  /* Calculate each frequency domain sample iteratively */
  for (i = 0; i < SIZE; i += 1)
  {
    /* ZI the outputs */
    real_op[i] = 0;
    imag_op[i] = 0;

    /* Calculate the j-th frequency sample sequentially */
    for (j = 0; j < SIZE; j += 1)
    {
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

