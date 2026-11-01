#include "dft.h"
#include "coefficients32_2D.h"

void dft
(
  DTYPE in_real[SIZE],
  DTYPE in_imag[SIZE],
  DTYPE out_real[SIZE],
  DTYPE out_imag[SIZE]
)
{
  DTYPE c, s;
  int   i, j;

  /* Calculate each frequency domain sample iteratively */
  for (i = 0; i < SIZE; i += 1)
  {
    /* ZI the outputs */
    out_real[i] = 0;
    out_imag[i] = 0;

    /* Calculate the j-th frequency sample sequentially */
    for (j = 0; j < SIZE; j += 1)
    {
      /* Utilize the precomputed sine and cosine values */
      c = cos_coeff_table[i][j];
      s = sin_coeff_table[i][j];

      /*
       * Multiply the current phasor with the appropriate input sample and keep
       * running sum.
       */
      out_real[i] += (in_real[j] * c - in_imag[j] * s);
      out_imag[i] += (in_real[j] * s + in_imag[j] * c);
    }
  }
}

