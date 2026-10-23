#include "phasedetector.h"

fix_t Kvalues[NO_ITER] = {1,	0.500000000000000,	0.250000000000000,	0.125000000000000,	0.0625000000000000,	0.0312500000000000,	0.0156250000000000,	0.00781250000000000,	0.00390625000000000,	0.00195312500000000,	0.000976562500000000,	0.000488281250000000,	0.000244140625000000,	0.000122070312500000,	6.10351562500000e-05,	3.05175781250000e-05};

fix_t angles[NO_ITER] = {0.785398163397448,	0.463647609000806,	0.244978663126864,	0.124354994546761,	0.0624188099959574,	0.0312398334302683,	0.0156237286204768,	0.00781234106010111,	0.00390623013196697,	0.00195312251647882,	0.000976562189559320,	0.000488281211194898,	0.000244140620149362,	0.000122070311893670,	6.10351561742088e-05,	3.05175781155261e-05};

// cordic gain inverse K = 1/sqrt(1+2^-i)
fix_t cordic_gain[NO_ITER] = {0.707106781186547, 0.632455532033676, 0.613571991077896, 0.608833912517752, 0.607648256256168, 0.607351770141296, 0.607277644093526, 0.607259112298893, 0.607254479332562, 0.607253321089875, 0.607253031529134, 0.607252959138945, 0.607252941041397, 0.607252936517010, 0.607252935385914, 0.607252935103139};

const fix_t INITAL_QUAD_ROTATION = 1.570796326794897;


void cordiccart2pol(data_t x, data_t y, data_t * r,  data_t * theta)
{
	#pragma HLS PIPELINE II=1

	#pragma HLS INTERFACE mode=s_axilite port=return
	#pragma HLS INTERFACE mode=s_axilite port=x
	#pragma HLS INTERFACE mode=s_axilite port=y
	#pragma HLS INTERFACE mode=s_axilite port=r
	#pragma HLS INTERFACE mode=s_axilite port=theta
	
	fix_t xf, yf, thetaf, x_temp;
	/**
	 * // pseudo code for rotation after baseline implementation(bit shifting)
	 *
	 * for i in lengthOfRotations:
	 * x1 = x0 - (y0 >> 1)
	 * y1 = y0 + (x0 >> 1)
	 *
	 * x0 = x1; y0 = y1;
	 */
	// temporary x to be used in quadrant rotation check
	// need a sanity check when y = 0;
	if (y > 0.0f) // initial angle is +90; (x, y) -> (y, -x)
	{ 
		xf = y;
		yf = -x;
		thetaf= INITAL_QUAD_ROTATION;
	}
	else // intial angle is -90deg
	{
		xf = -y;
		yf = x;
		thetaf = -INITAL_QUAD_ROTATION;
	}

	// loop for our rotations
	for (int i = 0; i < NO_ITER; i++)
	{
		x_temp = xf;
		if (yf > 0)
		{
			xf = xf + (yf >> i);
			yf = yf - (x_temp >> i);
			thetaf += angles[i];
		}
		else
		{
			xf = xf - (yf >> i);
			yf = yf + (x_temp >> i);
			thetaf -= angles[i];
		}
	}

	*r = xf * cordic_gain[NO_ITER-1];

	*theta = thetaf;
	if (thetaf < -3.14159265359) {
		*theta = thetaf + (fix_t)6.28318530718;
	}
	
}
