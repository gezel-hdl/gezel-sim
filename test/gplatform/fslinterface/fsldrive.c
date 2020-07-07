#include <stdio.h>

int main() {
  volatile unsigned int *wchannel = (volatile unsigned int *) 0x80000000;
  volatile unsigned int *rchannel_data = (volatile unsigned int *) 0x80000004;
  volatile unsigned int *rchannel_status = (volatile unsigned int *) 0x80000008;
  int i;
  
  for (i=0; i<5; i++) {
    *wchannel = i;
    
    while (*rchannel_status != 1) ;
    printf("Received data %d\n", *rchannel_data);
  }
  
  return 0;
}
