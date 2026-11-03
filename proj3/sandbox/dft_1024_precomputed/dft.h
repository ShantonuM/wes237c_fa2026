#include <ap_int.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

typedef float                     DTYPE;
typedef hls::axis<DTYPE, 0, 0, 0> transPkt;

#define SIZE 1024     /* SIZE OF DFT */

void dft
(
  hls::stream<transPkt> &real_ip_stream,
  hls::stream<transPkt> &imag_ip_stream,
  hls::stream<transPkt> &real_op_stream,
  hls::stream<transPkt> &imag_op_stream
);

