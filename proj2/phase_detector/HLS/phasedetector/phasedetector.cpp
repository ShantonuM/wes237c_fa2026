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

void phasedetector(
	data_t I,
	data_t Q,

	data_t *R,
	data_t *Theta)
{

	// Write your code here
	// cordiccart2pol(data_t x, data_t y, data_t * r,  data_t * theta)
	// fir (data_t I, data_t Q, data_t *X,data_t *Y)

#pragma HLS PIPELINE II = 1

	data_t x, y;

	fir(I, Q, &x, &y);
	cordiccart2pol(x, y, R, Theta);
}

void phasedetector_stream(
	hls::stream<pkt_t> &I_in,
	hls::stream<pkt_t> &Q_in,
	hls::stream<pkt_t> &R_out,
	hls::stream<pkt_t> &T_out)
{
#pragma HLS INTERFACE mode = axis port = I_in, Q_in, R_out, T_out
#pragma HLS INTERFACE mode = s_axilite port = return
	while (true)
	{
		pkt_t i_pkt = I_in.read();
		pkt_t q_pkt = Q_in.read();
		bool last = (i_pkt.last == 1 || q_pkt.last == 1);

		data_t r, t;
		phasedetector(i_pkt.data, q_pkt.data, &r, &t);

		pkt_t r_pkt, t_pkt;
		r_pkt.data = r;
		r_pkt.keep = 0xf;
		r_pkt.strb = 0xf;
		r_pkt.last = last;
		t_pkt.data = t;
		t_pkt.keep = 0xf;
		t_pkt.strb = 0xf;
		t_pkt.last = last;

		R_out.write(r_pkt);
		T_out.write(t_pkt);

		if (last)
		{
			break;
		}
	}
}
