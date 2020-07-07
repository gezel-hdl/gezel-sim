#include "nallasim.h"

int main() {
  unsigned data[64] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

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
  C.divideclock();
  C.createtop_fpga0();

  return 0;
}
