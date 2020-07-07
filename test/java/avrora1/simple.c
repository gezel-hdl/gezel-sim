// send serial data to hardware module
#include <avr/io.h>

// port A: bit 0 - data
//         bit 1 - clk (data is accepted on upgoing edge by HW)

int main() {

  char i, j;

  DDRA = 0x3;
  PORTA = PORTA | _BV(2); 

  for (j=0; j<4; j++) {
    for (i=0; i<8; i++) {
      
      //      PINA & _BV(2);

      // toggle data bit
      PORTA = (PORTA & _BV(0)) ? (PORTA & ~ _BV(0)) : (PORTA | _BV(0)); 
      
      PORTA = PORTA |   _BV(1);  // toggle the clock
      PORTA = PORTA & ~ _BV(1);
    }

    PORTA = PORTA |   _BV(1);  // toggle the clock for stopbit
    PORTA = PORTA & ~ _BV(1);
  }

  PINA;

  // terminate simulation here with break instruction
  asm volatile ("break");

  return 0;
}
