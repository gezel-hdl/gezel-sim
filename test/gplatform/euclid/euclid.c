#include <stdio.h>


int gcdsw(int m, int n) {
  int factor = 0;
  while ((m != 0) && (n != 0)) {
    if (m & 0x1) {
      if (n & 0x1) {
	m = (m >= n) ? m - n : m;  
	n = (n >  m) ? n - m : n; 
      } else {
	n = n >> 1;
      }
    } else {
      if (n & 0x1) {
	m = m >> 1;
      } else {
	m = m >> 1;
	n = n >> 1;
	factor = factor + 1;
      }
    }
  }
  return (((m > n) ? m : n) << factor);
}


int gcd(int m, int n) {

  volatile unsigned char *ins  = (volatile unsigned char *) 0x80000000;
  volatile unsigned int  *din  = (volatile unsigned int  *) 0x80000008;
  volatile unsigned int  *dout = (volatile unsigned int  *) 0x80000004;
  int r;

  enum {idle, load_m, load_n, dogcd};

  *ins = idle;

  *din = m;
  *ins = load_m;
  *ins = load_m;

  *ins = idle;

  *din = n;
  *ins = load_n;
  
  *ins = idle;

  *ins = dogcd;

  while (! (r = *dout) );

  *ins = idle;
  return r;
}

int main() {

  int i;

  for (i=894; i<904; i++)
    printf("The GCD of 2322 and %d is %d\n", i, gcd(2322, i));

  return 0;

}
