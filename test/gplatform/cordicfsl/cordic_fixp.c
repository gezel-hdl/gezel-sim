#include <stdio.h>

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

void cordic_driver(int target, int *rX, int *rY) {

  volatile unsigned int *wchannel_data   = (volatile unsigned int *) 0x80000000;
  volatile unsigned int *wchannel_status = (volatile unsigned int *) 0x80000004;

  volatile unsigned int *rchannel_data   = (volatile unsigned int *) 0x80000008;
  volatile unsigned int *rchannel_status = (volatile unsigned int *) 0x8000000C;
  int i;

  while (*wchannel_status == 1) ;
  *wchannel_data = target;
    
  while (*rchannel_status != 1) ; 
  *rX = *rchannel_data;

  while (*rchannel_status != 1) ; 
  *rY = *rchannel_data;
  
}

int main(void) {
  fixed X, Y, target;
  fixed accsw, accfsl;

  accsw = 0;
  for (target = 0; target < PI/2; target += (1 << (UNIT - 12))) {
    cordic(target, &X, &Y);
    accsw += (X + Y);
  }

  accfsl = 0;
  for (target = 0; target < PI/2; target += (1 << (UNIT - 12))) {
    cordic_driver(target, &X, &Y);
    accfsl += (X + Y);
  }

  printf("Checksum SW %x FSL %x\n", accsw, accfsl);

  return(0); 
}
