#ifndef CORDICCART2POL_H
#define CORDICCART2POL_H

#include <ap_fixed.h>

#define NO_ITER 20

typedef int   coef_t;
typedef float data_t;
typedef float acc_t;
typedef ap_fixed<16, 3> fix_t;
typedef ap_fixed<32, 3> fixed_2c_t; // this definition is for question 2c
void cordiccart2pol(data_t x, data_t y, data_t * r,  data_t * theta);

#endif
