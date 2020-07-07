// send serial data to hardware module
#include <avr/io.h>

int main() {

  char i, j;

  DDRA  = 0x0;
  PORTA = 0xFF;

  PINA;

  // terminate simulation here with break instruction
  asm volatile ("break");

  return 0;
}
