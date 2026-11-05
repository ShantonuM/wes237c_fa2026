/*
	Filename: fir.h
		Header file
		FIR lab wirtten for 237C class at UCSD.

*/
#ifndef PHASE_DETECTOR_H_
#define PHASE_DETECTOR_H_


#include <ap_fixed.h>
#include "ap_axi_sdata.h"
#include "hls_stream.h"

typedef ap_fixed<32, 8> fix_t;

/* From Lab: Axistream Multiple DMAs (axis) */
typedef hls::axis<float, 0, 0, 0> pkt_t;

const int N = 32;

#define NO_ITER 16

typedef int   coef_t;
typedef float data_t;
typedef float acc_t;

void phasedetector (
  data_t I,
  data_t Q,

  data_t *R,
  data_t *Theta
  );

void cordiccart2pol(data_t x, data_t y, data_t * r, data_t * theta);

void fir (
  data_t I,
  data_t Q,

  data_t *X,
  data_t *Y
  );
  
/* From Lab: Axistream Multiple DMAs (axis) */
void phasedetector_stream (
  hls::stream<pkt_t> &I_in,
  hls::stream<pkt_t> &Q_in,
  hls::stream<pkt_t> &R_out,
  hls::stream<pkt_t> &T_out
  );
  
#endif
