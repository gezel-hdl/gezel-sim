// Win AVR code for Master SPI 
#include <avr/io.h>
#include <avr/interrupt.h> 
#include <stdio.h>

ISR (TIMER1_CAPT_vect)
{
	//int icpval;
	//icpval = ICR1L; //TCNT1L ; 
   PORTA=ICR1L; //low is on
	PORTB = 0xff;
	PORTA = 0;
} 

ISR(TIMER1_COMPA_vect){
  //OCR1A+=500;
  //PORTA = 0x80;
}

ISR(TIMER1_COMPB_vect){
  //OCR1A+=500;
  //PORTA = 0x82;
}

int main(void) {
  
//   char cdata;
   EICRA = 0x03;  // For the sake of starting clock, need to find some other way
//   EICRA |= 0x0C; // Int 1
	TCCR1B = 0x07; //no prescaling 
	TCCR1A = 0x00;
	//TCCR1B = (1<<ICES1); //trigger on rising edge 
	TIMSK = (1<<TICIE1); //enable timer1 input capture interrrupt 
  SREG = 0x80;
  TIMSK = 0x10;  // Timer 1 Input capture Enable

  DDRA = 0xFF;  
  DDRB = 0xFF;

  PORTB = 0xff;
  //PORTB = 0;
  while(1)
  {
	//EIMSK ^= 1;
	PORTB = 0;
  }
}