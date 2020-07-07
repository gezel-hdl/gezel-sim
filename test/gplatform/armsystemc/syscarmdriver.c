#include <stdio.h>


int main() {

  volatile int *d1 = (volatile unsigned *) 0x80000000;
  volatile int *d2 = (volatile unsigned *) 0x80000004;

  unsigned n;

  while (1) {
    n = *d2;
    printf("ARM - data read is %x\n", n);
    *d1 = n + 2;
  }

  return 0;
}
