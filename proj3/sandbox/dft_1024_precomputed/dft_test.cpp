/*
This is DFT computation using matrix vector multiplication form.
INPUT:
  In_R, In_I[]: Real and Imag parts of Complex signal in time domain.
OUTPUT:
  Out_R, Out_I[]: Real and Imag parts of Complex signal in frequency domain.

*/

#include<stdio.h>
#include <stdlib.h>
#include<iostream>
#include <math.h>
#include "dft.h"

struct Rmse
{
  int num_sq;
  float sum_sq;
  float error;

  Rmse(){ num_sq = 0; sum_sq = 0; error = 0; }

  float add_value(float d_n)
  {
    num_sq++;
    sum_sq += (d_n*d_n);
    error = sqrtf(sum_sq / num_sq);
    return error;
  }

};


Rmse rmse_R,  rmse_I;

DTYPE In_R[SIZE], In_I[SIZE],Out_R[SIZE],Out_I[SIZE];   //Modify the testbench while checking for demo. You will have to access the data variable of the structure//

int main()
{
  int                   i, index;
  float                 gold_R, gold_I;
  transPkt              real_pkt, imag_pkt;
  hls::stream<transPkt> real_ip_stream, imag_ip_stream;
  hls::stream<transPkt> real_op_stream, imag_op_stream;

  FILE * fp = fopen("out.gold.dat","r");

  for (i = 0; i < SIZE; i += 1)
  {
    const bool last = (i == SIZE - 1);

    /* Form the real data packet */
    real_pkt.data = i;
    real_pkt.keep = 0xF;
    real_pkt.strb = 0xF;
    real_pkt.last = last;

    imag_pkt.data = 0.0;
    imag_pkt.keep = 0xF;
    imag_pkt.strb = 0xF;
    imag_pkt.last = last;

    real_ip_stream.write(real_pkt);
    imag_ip_stream.write(imag_pkt);
  }

  // DFT
  dft(real_ip_stream, imag_ip_stream, real_op_stream, imag_op_stream);

  // comparing with golden output
  for(i=0; i<SIZE; i++)
  {
    real_pkt = real_op_stream.read();
    imag_pkt = imag_op_stream.read();

    fscanf(fp, "%d %f %f", &index, &gold_R, &gold_I);
    rmse_R.add_value((float)real_pkt.data - gold_R);
    rmse_I.add_value((float)imag_pkt.data - gold_I);
  }
  fclose(fp);

  // printing error results
  printf("----------------------------------------------\n");
  printf("   RMSE(R)           RMSE(I)\n");
  printf("%0.15f %0.15f\n", rmse_R.error, rmse_I.error);
  printf("----------------------------------------------\n");

  if (rmse_R.error > 0.1 || rmse_I.error > 0.1 ) {
    fprintf(stdout, "*******************************************\n");
    fprintf(stdout, "FAIL: Output DOES NOT match the golden output\n");
    fprintf(stdout, "*******************************************\n");
      return 1;
  }else {
    fprintf(stdout, "*******************************************\n");
    fprintf(stdout, "PASS: The output matches the golden output!\n");
    fprintf(stdout, "*******************************************\n");
      return 0;
  }

}
