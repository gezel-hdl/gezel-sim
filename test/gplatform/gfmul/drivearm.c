// Copyright (C) 2005 Patrick Schaumont (schaum@ee.ucla.edu)                   
// University of California at Los Angeles
// $Id: drivearm.c,v 1.1.1.1 2005/07/14 17:32:45 schaum Exp $


volatile int * P0 = (int *) 0x80000000;
volatile int * P1 = (int *) 0x80000004;
volatile int * P2 = (int *) 0x80000008;
volatile int * P3 = (int *) 0x8000000C;

void synchronize() {
  (*P3) = 0;
  while ((*P2)) ;
}

int sync_master(int v) {
  int r;
  (*P1) = v;
  (*P3) = 1;
  while ((*P2) == 0) ;
  r = (*P0);
  return r;
}

int sync_slave(int v) {
  int r;
  while ((*P2) == 0) ;
  r = (*P0);  
  (*P1) = v;
  (*P3) = 1;
  return r;
}

//--------------------------------------------------

#define SEGS     11

int a[] ={0x04,0xd0,0x03,0xee,0xf0,0xda,0x0e,0x02,0x00,0x02,0x03} ;
int b[] ={0x03,0xf0,0x03,0x0f,0x79,0x5a,0x0e,0x0f,0x20,0x02,0x02} ;
int c[] ={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} ;

void receive_seg(int n[]) {
  int i;
  for ( i=0 ; i<SEGS ; i++ ) {
    n[i] = sync_master(1);
    synchronize();
  }
}

void send_seg(int n[]) {
  int i;
  int r;
  for ( i=0 ; i<SEGS ; i++ ) {
    printf("Send %d\n", n[i]);
    r = sync_master(n[i]);
    synchronize();
  }
}


int main() {

  int r;

  synchronize();

  printf("Send a\n");
  send_seg(a);
  printf("Send b\n");
  send_seg(b);
  printf("Receive c\n");
  receive_seg(c);

  printf("ARM Received:\n");
  for (r=0; r<SEGS; r++)
    printf("%4x ", c[r]);
  printf("\n");

  return 0;
}
