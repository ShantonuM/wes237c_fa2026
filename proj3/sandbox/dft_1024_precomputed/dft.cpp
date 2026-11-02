#include <math.h>
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
  int   i, j;

  /* Calculate each frequency domain sample iteratively */
  for (i = 0; i < SIZE; i += 1)
  {
    /* (2 * pi * i) / SIZE */
    w = (2.0 * 3.141592653589 / SIZE) * (DTYPE)i;

    /* ZI the outputs */
    real_op[i] = 0;
    imag_op[i] = 0;

    /* Calculate the j-th frequency sample sequentially */
    for (j = 0; j < SIZE; j += 1)
    {
      /* Utilize HLS tool to calculate sine and cosine values */
      c = cos(j * w);
      s = -sin(j * w);

      /*
       * Multiply the current phasor with the appropriate input sample and keep
       * running sum.
       */
      real_op[i] += (real_sample[j] * c - imag_sample[j] * s);
      imag_op[i] += (real_sample[j] * s + imag_sample[j] * c);
    }
  }
}

