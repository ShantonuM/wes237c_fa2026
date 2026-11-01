#include "dft.h"
#include "coefficients32_2D.h"

void dft
(
  DTYPE real_sample[SIZE],
  DTYPE imag_sample[SIZE]
)
{
  DTYPE c, s;
  int   i, j;

  /* Temporary arrays to hold the intermediate frequency domain results */
  DTYPE temp_real[SIZE];
  DTYPE temp_imag[SIZE];

  /* Calculate each frequency domain sample iteratively */
  for (i = 0; i < SIZE; i += 1)
  {
    temp_real[i] = 0;
    temp_imag[i] = 0;

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
      temp_real[i] += (real_sample[j] * c - imag_sample[j] * s);
      temp_imag[i] += (real_sample[j] * s + imag_sample[j] * c);
    }
  }

  /* Perform an in-place DFT, i.e. copy result into the input arrays */
  for (i = 0; i < SIZE; i += 1)
  {
    real_sample[i] = temp_real[i];
    imag_sample[i] = temp_imag[i];
  }
}

