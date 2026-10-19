#ifndef CORDICCART2POL_H
#define CORDICCART2POL_H

#define NO_ITER 16 // we will use 7 for now. 
//#define NO_ITER 7

typedef int   coef_t;
typedef float data_t;
typedef float acc_t;

static const float ROTATING_ANGLES[NO_ITER] = {
    45.000, 26.565, 14.036, 7.125, 3.576, 1.790, 0.895, 0.448,
    0.224,  0.112,  0.056,  0.028, 0.014, 0.007, 0.003, 0.002
};

static const float SCALING_FACTORS[NO_ITER] = {
    1.41421, 1.11803, 1.03078, 1.00778, 1.00195, 1.00049, 1.00012, 1.00003,
    1.00001, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000
};

void cordiccart2pol(data_t x, data_t y, data_t * r,  data_t * theta);

#endif
