#include <8051.h>


void main() {
  int i;

  volatile __xdata __at (0x4000) unsigned char shared[0x40];

  for (i=0; i<64; i++) {
    shared[i] = 64 - i;
  }

  P0 = 0x0;
  while (1) {
    P0 = 0x1;
    P2 = 0x55;
    while (P1 != 0x1) P2 = 0x66;
    P2 = 0x77;
    // hw is accessing section 0 here.
    // we can access section 1

    for (i = 0x20; i < 0x3F; i++)
      shared[i] = 0xff - i;
    
    P0 = 0x0 | 0x2;
    P2 = 0x88;
    while ((P1 & 0x1)) P2 = 0x99;
    P2 = 0xAA;
    // hw is accessing section 1 here
    // we can access section 0

    for (i = 0x00; i < 0x1F; i++)
      shared[i] = 0x80 - i;

  }

}
