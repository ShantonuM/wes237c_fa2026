/*
	Filename: phasedetector.cpp
		Phase detector

	INPUT:
		I: signal for I sample
		Q: signal for Q sample
		length: array size

	OUTPUT:
		R: Radius
		Theta: Angle

*/

#include "phasedetector.h"

void phasedetector (
  data_t I,
  data_t Q,

  data_t *R,
  data_t *Theta
  ){

	// Write your code here
	// cordiccart2pol(data_t x, data_t y, data_t * r,  data_t * theta)
	// fir (data_t I, data_t Q, data_t *X,data_t *Y)

	#pragma HLS PIPELINE II=1

	#pragma HLS INTERFACE mode=axis port=I
	#pragma HLS INTERFACE mode=axis port=Q
	#pragma HLS INTERFACE mode=axis port=R
	#pragma HLS INTERFACE mode=axis port=Theta
	#pragma HLS INTERFACE mode=s_axilite port=return

	data_t x, y;

	fir (I, Q, &x, &y);
	cordiccart2pol(x, y, R, Theta);
}




