// $Id: cycle.c,v 1.1.1.1 2005/07/14 17:32:45 schaum Exp $

#include <stdio.h>

void print_cycle() {
  volatile unsigned char *ins  = (volatile unsigned char *) 0x80000000;
  *ins = 1;
  *ins = 0;
}

int main() {

  int i, j, k;

  print_cycle();

  j = 0;
  for (i=0; i<10000; i++)
    j = j + 2;

  print_cycle();

  return 0;

}
