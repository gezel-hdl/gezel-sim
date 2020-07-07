#include <stdio.h>
#include "arm_ext.h"

int main() { 

  int p;
  int a,b,c,d; 
  
  a = 10; 
  b = 20; 
  
  OP2x2_1(c, d, a, b);
  printf("%d %d %d %d\n", a, b, c, d); 

  a = 50;
  b = 20;

  OP2x2_1(c, d, a, b);
  printf("%d %d %d %d\n", a, b, c, d); 

  
  return 0; 
 } 
