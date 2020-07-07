// Win AVR code for Master SPI 
#include <avr/io.h>
#include <avr/interrupt.h> 
#include <stdio.h>

// ISR (TIMER1_CAPT_vect)
// {
//    PORTC=0x00; //low is on
// } 

ISR (INT0_vect)
{
   PORTB=0xff; //low is on
}

ISR (INT1_vect)
{
   PORTB=0x77; //low is on
}


int main(void) {
  
  SREG = 0x80;
  EICRA = 0x03;  // Int 0
  EICRA |= 0x0C; // Int 1

  EIMSK = 0x01;  // Int 0
  EIMSK |= 0x02; // Int 1
  
  DDRB = 0xFF;

  //PORTB ^= PORTB;
  while(1)
  {
	//EIMSK ^= 1;
	PORTB = 0;
  }
}
