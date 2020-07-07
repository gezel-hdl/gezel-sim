#include <8051.h>

// Driver code for AES block
// Copyright 2004 Patrick Schaumont (schaum@ee.ucla.edu)
// software implementation (aes_sw) is Copyright 2000 Baretto & Rijmen
// $Id: driver.c,v 1.1.1.1 2005/07/14 17:32:45 schaum Exp $

// P0 -> ctl
// P1 -> data I
// P2 -> data O

enum {ins_idle, ins_rst, ins_load, ins_key, ins_text, ins_crypt, ins_textout, ins_read};

void aescmd(char ctl, char d) {
  P1 = d;
  P0 = ctl;
  P0 = ins_idle;
}

char aesrd(char ctl) {
  char r;
  P0 = ctl;
  r  = P2;
  P0 = ins_idle;
  return r;
}

void terminate() {
  // special command to stop simulator
  P3 = 0x55;
}

void main() {

  char key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
		  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
  char txt[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
		  0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF};
  char out[16];

  char i;

  aescmd(ins_rst, 0x00);

  for (i=0; i<15; i++)
    aescmd(ins_load, key[i]);
  aescmd(ins_key, key[15]);

  for (i=0; i<15; i++) 
    aescmd(ins_load, txt[i]);
  aescmd(ins_text, txt[15]);

  aescmd(ins_crypt, 0x00);
  //  for (i=0; i<200; i++)
  //   ;

  P0     = ins_textout;
  out[0] = P2;
  P1     = out[0];
  P0     = ins_idle;

  for (i=1; i<16; i++) {
    P0     = ins_read;
    out[i] = P2;
    P1     = out[i];
    P0     = ins_idle;
  }

  // show data received
  P1 = 0x00;
  P1 = 0xAA;
  P1 = 0x55;
  P1 = 0xAA;
  P1 = 0x00;

  for (i=0; i<16; i++) 
    P1 = out[i];

  P1 = 0x00;
  P1 = 0xAA;
  P1 = 0x55;
  P1 = 0xAA;
  P1 = 0x00;

  terminate();
}
