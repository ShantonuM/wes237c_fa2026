/*
	Filename: fir.cpp
		Complex FIR or Match filter
		firI1 and firI2 share coef_t c[N]
		firQ1 and firQ2 share coef_t c[N]
		
	INPUT:
		I: signal for I sample
		I: signal for Q sample

	OUTPUT:
		X: filtered output
		Y: filtered output

*/

#include "fir.h"

void firI1 (
  data_t *y,
  data_t x
  ) {

	coef_t c[N] = {1,    -1,    1,    -1,    -1,    -1,    1,    1,    -1,    -1,    -1,    1,    1,    -1,    1,    -1,    -1,    -1,    -1,    1,    1,    1,    1,    1,    -1,    -1,    1,    1,    1,    -1,    -1,    -1};

	// Write your code here
	// From proj1:
	static
    data_t shift_reg[N];
    acc_t acc;
    int i;

	/* NOTE: Only 1 array patitioning scheme can be used at a time */

	/* cyclic memory partitioning */
	#pragma HLS array_partition variable=shift_reg cyclic factor=16
	#pragma HLS array_partition variable=c         cyclic factor=16

	/* complete memory partitioning */
	// #pragma HLS array_partition variable=shift_reg complete
	// #pragma HLS array_partition variable=c         complete

	/* block memory partitioning */
	// #pragma HLS array_partition variable=shift_reg block factor=2
	// #pragma HLS array_partition variable=c         block factor=2

	#pragma HLS PIPELINE II=1

	/* Leveraged from pp4fpgas book section 2.8/figure 2.5 */
	Tapped_Delayed_Line:
	for (i = N - 1; i > 0; i--){
		shift_reg[i] = shift_reg[i - 1];
	}
	shift_reg[0] = x;

	acc = 0;

	Multiply_Accumulate:
	for (i = N - 1; i >= 0; i--){
		acc += shift_reg[i] * c[i];
	}

	*y = acc;
}


void firI2 (
  data_t *y,
  data_t x
  ) {

	coef_t c[N] = {1,    -1,    1,    -1,    -1,    -1,    1,    1,    -1,    -1,    -1,    1,    1,    -1,    1,    -1,    -1,    -1,    -1,    1,    1,    1,    1,    1,    -1,    -1,    1,    1,    1,    -1,    -1,    -1};

	// Write your code here
	static
    data_t shift_reg[N];
    acc_t acc;
    int i;

	/* NOTE: Only 1 array patitioning scheme can be used at a time */

	/* cyclic memory partitioning */
	#pragma HLS array_partition variable=shift_reg cyclic factor=16
	#pragma HLS array_partition variable=c         cyclic factor=16

	/* complete memory partitioning */
	// #pragma HLS array_partition variable=shift_reg complete
	// #pragma HLS array_partition variable=c         complete

	/* block memory partitioning */
	// #pragma HLS array_partition variable=shift_reg block factor=2
	// #pragma HLS array_partition variable=c         block factor=2

	#pragma HLS PIPELINE II=1

	/* Leveraged from pp4fpgas book section 2.8/figure 2.5 */
	Tapped_Delayed_Line:
	for (i = N - 1; i > 0; i--){
		shift_reg[i] = shift_reg[i - 1];
	}
	shift_reg[0] = x;

	acc = 0;

	Multiply_Accumulate:
	for (i = N - 1; i >= 0; i--){
		acc += shift_reg[i] * c[i];
	}

	*y = acc;
}




void firQ1 (
  data_t *y,
  data_t x
  ) {

	coef_t c[N] = {-1,    -1,    1,    -1,    1,    -1,    1,    -1,    -1,    -1,    -1,    1,    -1,    1,    -1,    1,    1,    -1,    1,    -1,    -1,    1,    -1,    1,    1,    1,    1,    -1,    1,    -1,    1,    1};


	// Write your code here
	static
    data_t shift_reg[N];
    acc_t acc;
    int i;

	/* NOTE: Only 1 array patitioning scheme can be used at a time */

	/* cyclic memory partitioning */
	#pragma HLS array_partition variable=shift_reg cyclic factor=16
	#pragma HLS array_partition variable=c         cyclic factor=16

	/* complete memory partitioning */
	// #pragma HLS array_partition variable=shift_reg complete
	// #pragma HLS array_partition variable=c         complete

	/* block memory partitioning */
	// #pragma HLS array_partition variable=shift_reg block factor=2
	// #pragma HLS array_partition variable=c         block factor=2

	#pragma HLS PIPELINE II=1

	/* Leveraged from pp4fpgas book section 2.8/figure 2.5 */
	Tapped_Delayed_Line:
	for (i = N - 1; i > 0; i--){
		shift_reg[i] = shift_reg[i - 1];
	}
	shift_reg[0] = x;

	acc = 0;

	Multiply_Accumulate:
	for (i = N - 1; i >= 0; i--){
		acc += shift_reg[i] * c[i];
	}

	*y = acc;
}

void firQ2 (
  data_t *y,
  data_t x
  ) {

	coef_t c[N] = {-1,    -1,    1,    -1,    1,    -1,    1,    -1,    -1,    -1,    -1,    1,    -1,    1,    -1,    1,    1,    -1,    1,    -1,    -1,    1,    -1,    1,    1,    1,    1,    -1,    1,    -1,    1,    1};


	// Write your code here
	static
    data_t shift_reg[N];
    acc_t acc;
    int i;

	/* NOTE: Only 1 array patitioning scheme can be used at a time */

	/* cyclic memory partitioning */
	#pragma HLS array_partition variable=shift_reg cyclic factor=16
	#pragma HLS array_partition variable=c         cyclic factor=16

	/* complete memory partitioning */
	// #pragma HLS array_partition variable=shift_reg complete
	// #pragma HLS array_partition variable=c         complete

	/* block memory partitioning */
	// #pragma HLS array_partition variable=shift_reg block factor=2
	// #pragma HLS array_partition variable=c         block factor=2

	#pragma HLS PIPELINE II=1

	/* Leveraged from pp4fpgas book section 2.8/figure 2.5 */
	Tapped_Delayed_Line:
	for (i = N - 1; i > 0; i--){
		shift_reg[i] = shift_reg[i - 1];
	}
	shift_reg[0] = x;

	acc = 0;

	Multiply_Accumulate:
	for (i = N - 1; i >= 0; i--){
		acc += shift_reg[i] * c[i];
	}

	*y = acc;
}


void fir (
  data_t I,
  data_t Q,

  data_t *X,
  data_t *Y
  ) {

	// Write your code here
	data_t q1;
	data_t q2;
	data_t i1;
	data_t i2;

	firI1(&i1, I);
	firI2(&i2, Q);
	firQ1(&q1, I);
	firQ2(&q2, Q);

	//Calculate X
	*X = i1 + q2;

	//Calculate Y
	*Y = i2 - q1;
}


