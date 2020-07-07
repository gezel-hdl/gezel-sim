// Copyright (C) 2005 Patrick Schaumont (schaum@ee.ucla.edu)                   
// University of California at Los Angeles
// $Id: hello.c,v 1.1.1.1 2005/07/14 17:32:45 schaum Exp $

#include <stdio.h>

int main(int argc, char *argv[]) {
  int i;

  printf("Hello: ");
  for (i=1; i<argc; i++) 
    printf("[%s] ", argv[i]);
  printf("\n");

  return 0;
}
