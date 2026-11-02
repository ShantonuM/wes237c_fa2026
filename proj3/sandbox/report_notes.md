## Q3

RAM usage comes down because dft() no longer needs temp_* variables (it is not doing an in-place DFT) - but, getting the output array addrs as input.  
Basically, no longer need to maintain a scratch area within the implementation - so no BRAM; output directly to the addr coming in as an "input to the HM".  
Also, the last loop for doing the in-place DFT was made unnecessary.

## Q4

Initially paritioned without mentioning the dimension. The pragma defaults to dim=1. However, for the coefficients, we need to partition on dim=2.  
This is because, when the inner loop is pipelined with II=1 (essentially, unrolled), we need to read the precomputed coeffs - whole rows at once.  

dim=1 splits by row - so all the inner loop reads get bottlenecked again (since they go to the same BRAM).  
dim=2 splits by col - so each column entry gets its own BRAM, and can all be read in parallel within the inner loop (unrolled).  

Also, need to partition the input arrays because of the outer loop being unrolled/pipeline II=1. Otherwise, HLS will have to preload the 32 values at 2 per cycle.  
Observed a latency drop from 221 -> 206 when input arrs were partitioned.
