#define ARMCOSIM

//----------------------------------------------------------------------

#ifdef ARMCOSIM

volatile unsigned int *regid       = (volatile unsigned int *) 0x80000000;
volatile unsigned int *regopid     = (volatile unsigned int *) 0x80000004;
volatile unsigned int *regdata     = (volatile unsigned int *) 0x80000008;

volatile unsigned int *ptr_rfifodata   = (volatile unsigned int *) 0x80001000;
volatile unsigned int *ptr_rfifostatus = (volatile unsigned int *) 0x80001004;

volatile unsigned int *ptr_wfifodata   = (volatile unsigned int *) 0x80002000;
volatile unsigned int *ptr_wfifostatus = (volatile unsigned int *) 0x80002004;

int readreg(int id) {
  *regid = id;
  *regopid  = 2;  // read
  while (*regopid) ;
  return *regdata;
}

void writereg(int id, int n) {
  *regid = id;
  *regdata  = n;
  *regopid  = 1;
  while (*regopid) ;
}

void fifoinit() {
}

unsigned rfifostatus() {
  return *ptr_rfifostatus;
}

unsigned wfifostatus() {
  return *ptr_wfifostatus;
}

void wrfifo(unsigned v) {
  *ptr_wfifodata = v;
}

unsigned rdfifo() {
  return *ptr_rfifodata;
}

#define PRINT printf

// no DMA under ARMCOSIM
void dmatest() { }

#endif

//----------------------------------------------------------------------

#ifdef IPIFREG
#include "xparameters.h"
#include "GCDSTREAM.h"

void fifoinit() {
  GCDSTREAM_mResetWriteFIFO(XPAR_GCDSTREAM_0_BASEADDR);
  GCDSTREAM_mResetReadFIFO(XPAR_GCDSTREAM_0_BASEADDR);
}

unsigned rfifostatus() {
   return GCDSTREAM_mReadFIFOEmpty(XPAR_GCDSTREAM_0_BASEADDR);
}

unsigned wfifostatus() {
   return GCDSTREAM_mWriteFIFOFull(XPAR_GCDSTREAM_0_BASEADDR);
}

void wrfifo(unsigned v) {
  GCDSTREAM_mWriteToFIFO(XPAR_GCDSTREAM_0_BASEADDR, v);
}

unsigned rdfifo() {
  return GCDSTREAM_mReadFromFIFO(XPAR_GCDSTREAM_0_BASEADDR);
}

#define PRINT xil_printf

static Xuint32 __attribute__ ((aligned (64)))  SrcBuffer[128];  // m,n pairs
static Xuint32 __attribute__ ((aligned (64)))  DstBuffer[64];   // gcd result

void dmatest() {
  unsigned i, j;
  
   fifoinit();
   
   for (i=0; i<16; i++) {
     SrcBuffer[8*i+0] = 1;
     SrcBuffer[8*i+1] = 2;
     SrcBuffer[8*i+2] = 2;
     SrcBuffer[8*i+3] = 4;
     SrcBuffer[8*i+4] = 3;
     SrcBuffer[8*i+5] = 9;
     SrcBuffer[8*i+6] = 4;
     SrcBuffer[8*i+7] = 44;
   }	
   
   GCDSTREAM_mResetDMA0(XPAR_GCDSTREAM_0_BASEADDR);
   GCDSTREAM_mSetDMA0Control(XPAR_GCDSTREAM_0_BASEADDR, DMA_SINC_MASK | DMA_DLOCAL_MASK);
   GCDSTREAM_DMA0Transfer(XPAR_GCDSTREAM_0_BASEADDR, 
			  (Xuint32) SrcBuffer, 
			  XPAR_GCDSTREAM_0_BASEADDR + GCDSTREAM_WRFIFO_DATA_OFFSET,
			  32 * 4 );
   
   while ( ! GCDSTREAM_mDMA0Done(XPAR_GCDSTREAM_0_BASEADDR) ) {}
   if ( GCDSTREAM_mDMA0Error(XPAR_GCDSTREAM_0_BASEADDR) ) {
     xil_printf("   - DMA channel 0 transfer error \n");
     xil_printf("   - DMA channel 0 transfer error \n");
     xil_printf("   - DMA channel 0 transfer error \n");
     return;
   }
   
   GCDSTREAM_mResetDMA1(XPAR_GCDSTREAM_0_BASEADDR);
   GCDSTREAM_mSetDMA1Control(XPAR_GCDSTREAM_0_BASEADDR, DMA_DINC_MASK | DMA_SLOCAL_MASK);
   GCDSTREAM_DMA1Transfer(XPAR_GCDSTREAM_0_BASEADDR, 
			  XPAR_GCDSTREAM_0_BASEADDR+GCDSTREAM_RDFIFO_DATA_OFFSET, 
			  (Xuint32) DstBuffer, 
			  16 * 4);
   
   while ( ! GCDSTREAM_mDMA1Done(XPAR_GCDSTREAM_0_BASEADDR) ) {}
   if ( GCDSTREAM_mDMA1Error(XPAR_GCDSTREAM_0_BASEADDR) ) {
     xil_printf("   - DMA channel 1 transfer error \n");
     xil_printf("   - DMA channel 1 transfer error \n");
     xil_printf("   - DMA channel 1 transfer error \n");
   }
   
   
   for (i=0; i<16; i++)
     xil_printf("%d GCD %d %d = %d\n", i, SrcBuffer[2*i], SrcBuffer[2*i + 1], DstBuffer[i]);
} 

#endif

//----------------------------------------------------------------------

void fifotest() {
  unsigned i, a1, a2, a3, a4;
  
  fifoinit();
  
  for (i=0; i<2; i++) {
    wrfifo(1);
    wrfifo(2);
    
    wrfifo(2);
    wrfifo(4);
    
    wrfifo(3);
    wrfifo(9);
    
    wrfifo(4);
    wrfifo(44);
  }
  
  for (i=0; i<1; i++) {
    a1 = rdfifo();
    a2 = rdfifo();
    a3 = rdfifo();
    a4 = rdfifo();
    
    PRINT("GCD %d %d read fifo %5d\n", 1, 2, a1);
    PRINT("GCD %d %d read fifo %5d\n", 2, 4, a2);
    PRINT("GCD %d %d read fifo %5d\n", 3, 9, a3);
    PRINT("GCD %d %d read fifo %5d\n", 4, 44, a4);
  }
  
}

//---------------------------------------

int main() {
  //  dmatest();  
  fifotest();
}
