// Galois Field Multiplier Design for 8051
// (c) David Hwang (dhwang@ee.ucla.edu

#include <8051.h>

typedef unsigned char uchar;
#define WORDMSB  0x80
#define SEGS     11

uchar a[] ={0x04,0xd0,0x03,0xee,0xf0,0xda,0x0e,0x00,0x00,0x00,0x00} ;
uchar b[] ={0x03,0xf0,0x03,0x0f,0x79,0x5a,0x0e,0x0f,0x20,0x00,0x02} ;
uchar c[] ={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} ;

void shift_left_copy(char l, uchar in[], uchar out[]) {
  // plain old shift left of entire data copies in into out
  char i;
  char word_msb = 0;
  char word_lsb = 0;  
  
  // shift left, making sure of boundaries between words
  for (i=l-1;i>=0;i--) {
    if ((in[(uchar)i] & WORDMSB) == WORDMSB)
      word_msb = 1;
    else
      word_msb = 0;
    out[(uchar)i] = in[(uchar)i] << 1;
    out[(uchar)i] = out[(uchar)i] ^ word_lsb;
    word_lsb = word_msb;
  }
}

void shift_left_times (char l, uchar in[], char times) {
  // plain old shift left of entire data a specified number of times
  char i,j;
  char word_msb = 0;
  char word_lsb = 0;
  
  for (j=1;j<=times;j++) {
    word_lsb = 0;
    // shift left, making sure of boundaries between words
    for (i=l-1;i>=0;i--) {
      if ((in[i] & WORDMSB) == WORDMSB)
	word_msb = 1;
      else
	word_msb = 0;
      in[i] = in[i] << 1;
      in[i] = in[i] ^ word_lsb;
      word_lsb = word_msb;
    }
  }
}

void gf_mul_comb_win_red (uchar a[], uchar b[], uchar c[]) {
  // WINDOW = 2 bits
  char k,j;
  uchar i;
  uchar b_shift[SEGS];
  uchar b3[SEGS];
  uchar c_long[2*SEGS];
  uchar temp;
  
  for (i=0;i<2*SEGS;i++)
    c_long[i] = 0;
  
  shift_left_copy(SEGS,b,b_shift);
  for (i=0;i<SEGS;i++)
    b3[i] = b[i] ^ b_shift[i];
  
  for (k=0; k<=3; k++) { // 3 is WORD/WINDOW -1 = 8/2 -1
    for (j=SEGS-1; j>=0; j--) {
      temp = a[j]  & (0xC0 >> (2*k));
      temp = temp >> (2*(3-k));
      
      if ( temp == 0x3) {
	for (i=j+1; i<=j+SEGS; i++)
	  c_long[i] = c_long[i] ^ b3[(uchar)(i-j-1)];
      }
      else if (temp == 0x02) {
	for (i=j+1; i<=j+SEGS; i++)
	  c_long[i] = c_long[i] ^ b_shift[(uchar)(i-j-1)];
      }
      else if (temp == 0x01) {
	for (i=j+1; i<=j+SEGS; i++)
	  c_long[i] = c_long[i] ^ b[(uchar)(i-j-1)];
      }
    }
    
    
    if (k!=3) {
      shift_left_times(2*SEGS,c_long,2);
    }
  }
  for (i=1;i<=10;i++) {
    temp = c_long[i];
    c_long[i+9] = c_long[i+9]   ^ (temp>>4) ^ (temp>>7);
    c_long[i+10] = c_long[i+10] ^ (temp<<4) ^ (temp<<1) ^ (temp>>1) ^ (temp>>3);
    c_long[i+11] = c_long[i+11] ^ (temp<<7) ^ (temp<<5);
  }
  temp = c_long[11] & 0xF8;
  c_long[20] = c_long[20] ^ (temp>>4) ^ (temp>>7);
  c_long[21] = c_long[21] ^ (temp<<4) ^ (temp<<1) ^ (temp>>1) ^ (temp>>3);
  
  c_long[11] = c_long[11] & 0x07;
  
  for (i=0;i<=10;i++)
    c[i] = c_long[11+i];
}

//-----------------------------------------------------
//
//               Communication support
//
//-----------------------------------------------------

void synchronize() {
  P3 = 0;
  while (P2) ;
}

uchar sync_slave(uchar v) {
  uchar r;
  while (P2 == 0) ;
  r  = P0;
  P1 = v;
  P3 = 1;
  return r;
}

void receive_seg(uchar n[]) {
  char i;
  for ( i=0 ; i<SEGS ; i++ ) {
    n[i] = sync_slave(1);
    synchronize();
  }
}

void send_seg(uchar n[]) {
  char  i;
  uchar r;
  
  for (i=0 ; i<SEGS ; i++ ) {
    r = sync_slave(n[i]);
    synchronize();
  }
}


//      master                slave
// 
//      synchronize           synchronize
//
//
//      master sends 5 to slave, master receives 6 from slave
//
//      r = sync_master(5);
//
//                            r = sync_slave(6);
//
//      synchronize;          synchronize;
//
//      r: 6                  r: 5


int main() {

  synchronize();

  receive_seg(a);
  receive_seg(b);
  gf_mul_comb_win_red(a, b, c);
  send_seg(c);

  P3 = 0x55;

  return 0;
}
