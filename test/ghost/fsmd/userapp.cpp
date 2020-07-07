#include "nallasim.h"

int main() {

  unsigned data[32];
  unsigned dataout[32];
  unsigned j;

  nallasim M("fsmd_spl.fdl");

  for (j=0; j<32; j++)
    data[j] = j;

  // Memcopy must use a multiple of 512 bit (64 bytes)

  //-- simulation
  M.MemCopy(0, data, 128, dataout, 128);

  for (j=0; j<32; j++)
    printf("%x %8x %8x\n", j, data[j], dataout[j]);

  //-- code generation
  nallacode C(nallacode::sysedge2rx2tx);
  C.addspl("fsmd_spl"); 
  C.divideclock();
  C.createtop_fpga0();

  return 0;
}


