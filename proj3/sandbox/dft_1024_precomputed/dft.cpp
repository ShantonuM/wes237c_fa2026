#include "dft.h"
#include "coefficients1024.h"

#define UNROLL_FACTOR   8

/* Keeping the array partitioning factor the same as the loop unrolling one */
#define ARR_PART_FACTOR UNROLL_FACTOR

void dft
(
  hls::stream<transPkt> &real_ip_stream,
  hls::stream<transPkt> &imag_ip_stream,
  hls::stream<transPkt> &real_op_stream,
  hls::stream<transPkt> &imag_op_stream
)
{
  #pragma HLS INTERFACE axis      port=real_ip_stream
  #pragma HLS INTERFACE axis      port=imag_ip_stream
  #pragma HLS INTERFACE axis      port=real_op_stream
  #pragma HLS INTERFACE axis      port=imag_op_stream
  #pragma HLS INTERFACE s_axilite port=return

  DTYPE    c, s;
  int      i, j, k;
  DTYPE    real_op[SIZE];
  DTYPE    imag_op[SIZE];
  DTYPE    real_sample[SIZE];
  DTYPE    imag_sample[SIZE];
  transPkt real_pkt, imag_pkt;

  #pragma HLS array_partition variable=real_op cyclic factor=ARR_PART_FACTOR
  #pragma HLS array_partition variable=imag_op cyclic factor=ARR_PART_FACTOR

  /* Read the streams into local vars and ZI the outputs */
  for (i = 0; i < SIZE; i += 1)
  {
    real_pkt = real_ip_stream.read();
    imag_pkt = imag_ip_stream.read();

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

  /* Write the outputs to stream */
  for (i = 0; i < SIZE; i += 1)
  {
    const bool last = (i == SIZE - 1);

    /* Form the real data packet */
    real_pkt.data = real_op[i];
    real_pkt.keep = 0xF;
    real_pkt.strb = 0xF;
    real_pkt.last = last;

    imag_pkt.data = imag_op[i];
    imag_pkt.keep = 0xF;
    imag_pkt.strb = 0xF;
    imag_pkt.last = last;

    real_op_stream.write(real_pkt);
    imag_op_stream.write(imag_pkt);
  }
}

