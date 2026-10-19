#include "cordiccart2pol.h"

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
	{ // rotate by -90deg; (x, y) -> (y, -x)
		x = y;
		y = -x_temp;
		*theta-=INITAL_QUAD_ROTATION;
	}
	else // rotate by +90deg
	{
		x = -y;
		y = x_temp;
		*theta+=INITAL_QUAD_ROTATION;

	}

	// loop for our rotations
	for (int i = 0; i < NO_ITER; i++)
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

	*r = x * cordic_gain_inv;
}
