#include <stdio.h>

#define ARMCOSIM

#ifdef ARMCOSIM
volatile unsigned *regid = (volatile unsigned *) 0x80000000;
volatile unsigned *opid  = (volatile unsigned *) 0x80000004;
volatile unsigned *data  = (volatile unsigned *) 0x8000000C;

int readreg(int id) {
  *regid = id;
  *opid  = 2;  // read
  while (*opid) ;
  return *data;
}

void writereg(int id, int n) {
  *regid = id;
  *data  = n;
  *opid  = 1;
  while (*opid) ;
}
#endif

#ifdef IPIFREG
volatile unsigned *reg1 = (volatile unsigned *) 0x41600000;
volatile unsigned *reg2 = (volatile unsigned *) 0x41600004;

int readreg(int id) {
  return id ? *reg1 : *reg2;
}

int readreg(int id) {
  return id ? *reg1 : *reg2;
}
#endif


int main() {

  writereg(0, 0x00010000);
  writereg(1, 0x02345677);

  printf("Expect: 0x00010000 - 0x02345677 = %x\n", 0x00010000 - 0x02345677);
  printf("MSB: %x\n", readreg(1));
  printf("LSB: %x\n", readreg(0));

  return 0;
}

