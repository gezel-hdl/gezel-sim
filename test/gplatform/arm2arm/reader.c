// Copyright (C) 2005 Patrick Schaumont (schaum@ee.ucla.edu)                   
// University of California at Los Angeles
// $Id: reader.c,v 1.1.1.1 2005/07/14 17:32:44 schaum Exp $

#include <stdio.h>

int main() {

  volatile int *wr = (int *) 0x80000000;
  volatile int *rd = (int *) 0x80000004;
  int a, i;

  for (i = 10; i < 50; i++) {
    
    while (*rd == 0) ;

    a = *rd;
    printf("read value %d\n", a);

    *wr = 1;
    while (*rd != 0) ;

    *wr = 0;
  }

  return 0;
}
