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

void writereg(int id, int n) {
  if (id)
    *reg1 = n;
  else
    *reg2 = n;
}
#endif


int main() {
  writereg(0, 34);
  writereg(1, 56);
  printf("Result: gcd of 34 and 56 is 2? %d\n", readreg(0));

  writereg(0, 56);
  writereg(1, 34);
  printf("Result: gcd of 56 and 34 is 2? %d\n", readreg(0));

  writereg(0, 34);
  writereg(1, 34);
  printf("Result: gcd of 34 and 34 is 34? %d\n", readreg(0));

  writereg(0, 34);
  writereg(1, 33);
  printf("Result: gcd of 34 and 33 is 1? %d\n", readreg(0));

  return 0;
}

