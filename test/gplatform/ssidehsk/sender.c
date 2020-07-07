
#include <stdio.h>

int main() {
  volatile unsigned int  *datap;
  int data = 0;
  int i;

  datap = (unsigned int  *) 0x80000008;

  for (i=0; i<5; i++) {

    *datap = data | 0x80000000;
    printf("Sender sends %d\n", data);
    data++;

    data &= 0x7FFFFFFF;

    *datap = data;
    printf("Sender sends %d\n", data);
    data++;

  }

  return 0;
}
