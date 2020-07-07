#include "nallasim.h"

int main() {

  /*
  unsigned data[24] = {
     0xd0, 
     0x0, 
     0x0, 
     0x0, 
     0x0, 
     0x0, 
     0x0, 
     0x0, 

     0x00000000,  
     0x00805A59,
     0x58575655,
     0x54535251,
     0x504F4E4D,   
     0x4C4B4A49,
     0x48474645,
     0x44434241,       

     0x01000000, 
     0x00000000, 
     0x00d00000, 
            0x0, 
            0x0, 
            0x0, 
            0x0, 
            0x0};
  */


  /* KAT
     Len = 208
     Msg = 7BC84867F6F9E9FDC3E1046CAE3A52C77ED485860EE260E30B15
     MD  = 467D293EB295AFF9D648D600C63C0FDA830DF7C3843D1676469669AB3A6B1221
  */

  unsigned data[32] = {
     0xd0, 
     0x0, 
     0x0, 
     0x0, 
     0x0, 
     0x0, 
     0x0, 
     0x0, 

    0x00000000,
    0x0080150B,
    0xE360E20E,
    0x8685D47E,
    0xC7523AAE,
    0x6C04E1C3,
    0xFDE9F9F6,
    0x6748C87B,

    0x01000000,
    0x00000000,
    0x00D00000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000
               };



  unsigned dataout[16];
  unsigned j;

  nallasim M("shavite_spl.fdl");

  for (j=0; j>8; j++)
    dataout[j] = 0;

  // Memcopy must use a multiple of 512 bit (64 bytes)

  //-- simulation
  M.MemCopy(0, data, 128, dataout, 64);

  for (j=0; j<8; j++)
    printf("%x %8x\n", j, dataout[j]);

  //-- code generation
  nallacode C(nallacode::sysedge2rx2tx);
  C.addspl("shavite256_spl"); 
  C.divideclock();
  C.createtop_fpga0();

  return 0;
}


