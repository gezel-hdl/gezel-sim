#include <avr/io.h>
//#include <util/delay.h>
#include <stdio.h>

int main(void)
{
   int j = 0;
  uint8_t data;
  
  /* set PORTA for output*/
  DDRA = 0x00;
  /* set PORTB for input*/
  DDRB = 0xFF;  // DDRB &= 0x00; //FB
  //PORTB |= 0x04; 
  /* set PORTA for output*/
  DDRC = 0xFF;


  PORTB = 0x09;
  PORTC = 0xff;
  //   data ++;
  PORTC = 0x01;
  PORTC = 0x02;
  PORTC = 0x00;

//   while(PINB != 0x0a);
  //for(j = 0; j < 1000; j++);
  data = PINA;
  //for(j = 0; j < 1000; j++);


  PORTC = 0x01;
  PORTC = 0x02;
  PORTC = 0xff;
  PORTB = data;  
  PORTC = 0x00;

  while(data < 20) {
  //data ++;
  PORTC = 0xff;

  data = PINA;
  
  PORTB = data;  
  PORTC = 0x00;
  }
//   while(PINB != 0x0b);
  
/*  data ++;
//  while(data != 0x20) {
    data = PINB; //PORTB
    PORTC = 0xff;
    j++;
    j--;
    j++;
    PORTA = data;
    j++;
    j++;
    j++;

//    PINB = 0x00;
    PORTC = 0x00;
    data = PINB; //PORTB
    PORTC = 0xff;
    j++;
    j--;
    j++;
    PORTA = data;
    j++;
    j++;
    j++;
*/
//    PINB = 0x00;
    PORTC = 0x00;    
//   }
  
//   i = 1;
//   i++;
//   j = i;
//   
//   PORTA = PINB;
//   
//   i++;
//   j = i;
//   j++;
//   PORTA = PINB;
  
//  for(j = 1; j < 10; j++)
//  {
//    PORTA = PINB;
    //i++;
    //while(PINB == 0xff);
    //printf("Port B value = %x", PINB);
    //PORTB = 0xff;
    //a = i - b;
//  }
}