// Copyright (C) 2005 Patrick Schaumont (schaum@ee.ucla.edu)                   
// University of California at Los Angeles
// $Id: driver.c,v 1.1.1.1 2005/07/14 17:32:45 schaum Exp $


#include <8051.h>

enum {ins_idle, ins_hello};

void sayhello(char d) {
  P1 = d;
  P0 = ins_hello;
  P0 = ins_idle;
}

void terminate() {
  // special command to stop simulator
  P3 = 0x55;
}

void main() {
  sayhello(3);
  sayhello(2);
  sayhello(1);
  terminate();
}
