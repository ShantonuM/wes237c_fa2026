## Q3

RAM usage comes down because dft() no longer needs temp_* variables (it is not doing an in-place DFT) - but, getting the output array addrs as input.  
Basically, no longer need to maintain a scratch area within the implementation - so no BRAM; output directly to the addr coming in as an "input to the HM".  
Also, the last loop for doing the in-place DFT was made unnecessary.
