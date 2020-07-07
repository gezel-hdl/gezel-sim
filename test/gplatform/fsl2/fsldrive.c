#include <stdio.h>

int main() {

  // channel from SW to HW -> 'fslslave'
  // data = data to write
  // status = 1 if data token is waiting in interface fifo
  //        = 0 if data token has left interface fifo
  volatile unsigned int *wchannel_data   = (volatile unsigned int *) 0x80000000;
  volatile unsigned int *wchannel_status = (volatile unsigned int *) 0x80000004;


  // channel from HW to SW -> 'fslmaster'
  // status = 1 if data token is available in interface fifo
  //        = 0 if no data token is available in interface fifo
  volatile unsigned int *rchannel_data   = (volatile unsigned int *) 0x80000008;
  volatile unsigned int *rchannel_status = (volatile unsigned int *) 0x8000000C;
  int i;
  
  for (i=0; i<5; i++) {

    while (*wchannel_status == 1) 
      printf("Waiting for write channel to be empty before write.. \n");

    printf("Write data \n");
    *wchannel_data = i;

    while (*wchannel_status == 1) 
      printf("Waiting for write channel to be empty after write.. \n");
    
    while (*rchannel_status != 1) 
      printf("Waiting for read channel to have a token.. \n");

    printf("Received data %d\n", *rchannel_data);

  }
  
  return 0;
}
