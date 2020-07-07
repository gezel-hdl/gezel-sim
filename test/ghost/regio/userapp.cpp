#include "nallasim.h"

int main() {
  unsigned data;
  nallasim M("useradd.fdl");

  //-- simulation
  M.WriteUserRegister(0,0, 0x0, 28);  // socket 0, interface 0, address 0
  M.WriteUserRegister(0,0, 0x4, 32);  // socket 0, interface 0, address 4
  M.ReadUserRegister (0,0, 0x8, &data);

  printf("Result %d\n", data);

  //-- code generation
  nallacode C(nallacode::sysedge0rx0tx);
  C.adduser(0, "nalla_add");  // user channel 0, gezel cell name = nalla_add
  C.createtop_fpga0();

  return 0;
}
