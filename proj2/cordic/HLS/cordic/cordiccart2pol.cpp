#include "cordiccart2pol.h"

data_t Kvalues[NO_ITER] = {1, 0.500000000000000, 0.250000000000000, 0.125000000000000, 0.0625000000000000, 0.0312500000000000, 0.0156250000000000, 0.00781250000000000, 0.00390625000000000, 0.00195312500000000, 0.000976562500000000, 0.000488281250000000, 0.000244140625000000, 0.000122070312500000, 6.10351562500000e-05, 3.05175781250000e-05, 1.52587890625000e-05, 7.62939453125000e-06, 3.81469726562500e-06, 1.90734863281250e-06};

data_t angles[NO_ITER] = {0.785398163397448, 0.463647609000806, 0.244978663126864, 0.124354994546761, 0.0624188099959574, 0.0312398334302683, 0.0156237286204768, 0.00781234106010111, 0.00390623013196697, 0.00195312251647882, 0.000976562189559320, 0.000488281211194898, 0.000244140620149362, 0.000122070311893670, 6.10351561742088e-05, 3.05175781155261e-05, 1.52587890613158e-05, 7.62939453110197e-06, 3.81469726560650e-06, 1.90734863281019e-06};

// cordic gain inverse 1/K = 1/sqrt(1+2^-i)
data_t cordic_gain[NO_ITER] = {0.707106781186547, 0.632455532033676, 0.613571991077896, 0.608833912517752, 0.607648256256168, 0.607351770141296, 0.607277644093526, 0.607259112298893, 0.607254479332562, 0.607253321089875, 0.607253031529134, 0.607252959138945, 0.607252941041397, 0.607252936517010, 0.607252935385914, 0.607252935103139, 0.607252935032446, 0.607252935014772, 0.607252935010354, 0.607252935009249};

const data_t INITAL_QUAD_ROTATION = 1.570796326794897;

void cordiccart2pol(data_t x, data_t y, data_t *r, data_t *theta)
{
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
	data_t x_temp = x;
	// need a sanity check when y = 0;
	if (y > 0.0f)
	{ // rotate by +90deg; (x, y) -> (y, -x)
		x = y;
		y = -x_temp;
		*theta = INITAL_QUAD_ROTATION;
	}
	else // rotate by -90deg
	{
		x = -y;
		y = x_temp;
		*theta = -INITAL_QUAD_ROTATION;

	}

	// loop for our rotations
	int ITERATIONS = NO_ITER;
	for (int i = 0; i < ITERATIONS; i++)
	{
		x_temp = x;
		if (y > 0.0f)
		{
			x += y * Kvalues[i];
			y -= x_temp * Kvalues[i];
			*theta += angles[i];
		}
		else
		{
			x -= y * Kvalues[i];
			y += x_temp * Kvalues[i];
			*theta -= angles[i];
		}
	}

	*r = x * cordic_gain[ITERATIONS-1];
}
