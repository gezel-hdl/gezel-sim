#include <stdio.h>

extern unsigned long long getcyclecount();

#define AG_CONST 163008218 /* 0.60725293510314 in <32,28> */
#define PI 843314856       /* 3.141593.. in <32,28>       */
#define UNIT 28            /* weight of LSB is 2^-UNIT    */
typedef int fixed;         /* <32,28> fixed-point representation */

static const int angles[] = {
  210828714,
  124459457,
   65760959,
   33381289,
   16755421,
    8385878,
    4193962,
    2097109,
    1048570,
     524287,
     262143,
     131071,
      65535,
      32767,
      16383,
       8191,
       4095,
       2047,
       1024,
        511 };

void cordic(int target, int *rX, int *rY) {
  fixed X, Y, T, current;
  unsigned step;

  X       = AG_CONST;  
  Y       = 0;	       
  current = 0;

  for(step=0; step < 20; step++) {
    
    if (target > current) {
      T          =  X - (Y >> step);
      Y          = (X >> step) + Y;
      X          = T;
      current   += angles[step];
    } else {
      T          = X + (Y >> step);
      Y          = -(X >> step) + Y;
      X          = T;
      current   -= angles[step]; 
    }
  }

  *rX = X;
  *rY = Y;
}


int main(void) {
  fixed X, Y, target;
  fixed accsw, accfsl;
  unsigned long long c0, c1;

  target = PI / 23;

  c0 = getcyclecount();
  cordic(target, &X, &Y);
  c1 = getcyclecount();

  printf("Target %d X %d Y %d\n", target, X, Y);
  printf("Cycles %lld\n", c1-c0);

  return(0); 
}
