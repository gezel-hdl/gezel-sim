// Copyright (C) 2005 Patrick Schaumont (schaum@ee.ucla.edu)                   
// University of California at Los Angeles
// $Id: writer.c,v 1.1.1.1 2005/07/14 17:32:44 schaum Exp $

int main() {

  volatile int *wr = (int *) 0x80000000;
  volatile int *rd = (int *) 0x80000004;
  int i;

  for (i = 10; i < 50; i++) {

    *wr = i;
    while (*rd == 0) ;

    *wr = 0;
    while (*rd != 0) ;
  }

  return 0;
}
