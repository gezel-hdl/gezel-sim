#include "nallasim.h"

int main() {
  unsigned data[64] = {
     1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16,
     17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32,
     33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
     49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64};

  unsigned dataout[64];
  unsigned j;

  nallasim M("flowtru.fdl");

  //-- simulation
  M.MemCopy(0, data, 64*4, dataout, 64*4);

  for (j=0; j<64; j++)
    printf("%x %8x %8x\n", j, data[j], dataout[j]);

  //-- code generation
  nallacode C(nallacode::sysedge2rx2tx);
  C.addspl("flowtru"); 
  C.createtop_fpga0();

  return 0;
}
