#include <stdio.h>

int main() {
  volatile unsigned int  *datap;
  int data = 0;
  int i;

  datap = (unsigned int  *) 0x80000008;

  for (i=0; i<5; i++) {

    do 
      data = *datap;
    while (!(data & 0x80000000));

    do 
      data = *datap;
    while ( (data & 0x80000000));

  }

  printf("Receiver complete - last data = %d\n", data);

  return 0;
}
