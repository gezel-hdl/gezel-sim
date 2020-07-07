#include <stdio.h>

extern long long getcyclecount();

void showshared() {
  volatile unsigned int *shared = (volatile unsigned int *) 0x80001000;
  int i;

  for (i=0; i<1024; i++) {
    printf("%10x", shared[i]);
    if (!((i+1) % 8)) 
      printf("\n");
  }
}


int main() {
  volatile unsigned int *req    = (volatile unsigned int *) 0x80000000;
  volatile unsigned int *ack    = (volatile unsigned int *) 0x80000004;
  volatile unsigned int *shared = (volatile unsigned int *) 0x80001000;
  unsigned long long c0, c1;

  int k, v, acc;

  c0 = getcyclecount();
  for (k=0; k<10; k++) {
    *req = 0x1;
    while (*ack != 0x1) ;
    
    *req = 0x0;
    while (*ack != 0x0) ;
  }
  c1 = getcyclecount();

  printf("Hardware writes 10 * 2048 words in %lld cycles\n", c1- c0);

  c0 = getcyclecount();
  for (k=0; k<10; k++) {
    for (v=0; v<1024; v++)
      shared[v] = v;
    for (v=0; v<1024; v++)
      shared[v] = v;
  }
  c1 = getcyclecount();

  printf("Software writes 10 * 2048 words in %lld cycles\n", c1- c0);

  c0 = getcyclecount();
  acc = 0;
  for (k=0; k<10; k++) {
    *req = 0x1;
    while (*ack != 0x1) ;
    for (v=0; v<1024; v++)
      acc += shared[v];
    
    *req = 0x0;
    while (*ack != 0x0) ;
    for (v=0; v<1024; v++)
      acc += shared[v];
  }
  c1 = getcyclecount();

  printf("Software accumulates 10 * 2048 words from HW in %lld cycles\n", c1- c0);


}
