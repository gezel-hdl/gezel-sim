
#define ARMCOSIM

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

#endif

#ifdef IPIFREG
#include "xparameters.h"
#include "fifocoproc.h"

volatile unsigned *reg1 = (volatile unsigned *) 0x41600000;
volatile unsigned *reg2 = (volatile unsigned *) 0x41600004;

int readreg(int id) {
  return id ? *reg1 : *reg2;
}

void writereg(int id, int n) {
  if (id)
    *reg1 = n;
  else
    *reg2 = n;
}

void fifoinit() {
  FIFOCOPROC_mResetWriteFIFO(XPAR_FIFOCOPROC_0_BASEADDR);
  FIFOCOPROC_mResetReadFIFO(XPAR_FIFOCOPROC_0_BASEADDR);
}

unsigned rfifostatus() {
   return FIFOCOPROC_mReadFIFOEmpty(XPAR_FIFOCOPROC_0_BASEADDR);
}

unsigned wfifostatus() {
   return FIFOCOPROC_mWriteFIFOFull(XPAR_FIFOCOPROC_0_BASEADDR);
}

void wrfifo(unsigned v) {
  FIFOCOPROC_mWriteToFIFO(XPAR_FIFOCOPROC_0_BASEADDR, v);
}

unsigned rdfifo() {
  return FIFOCOPROC_mReadFromFIFO(XPAR_FIFOCOPROC_0_BASEADDR);
}

#define PRINT xil_printf

#endif

int main() {
  unsigned i;

  //                    read          write
  //
  //  register 0        status         ctl
  //
  //  register 1          b             a
  //
  // status[0] RFIFO2IP_WrAck
  // status[1] RFIFO2IP_Full
  // status[2] RFIFO2IP_AlmostFull
  // status[3] always 0
  // status[4] WFIFO2IP_RdAck
  // status[5] WFIFO2IP_Empty
  // status[6] WFIFO2IP_AlmostEmpty
  //
  // ctl[0] write a into rfifo  (on upedge)
  // ctl[1] write wfifo into b  (on upedge)


  fifoinit();
  PRINT("status %d\n", readreg(0));
  PRINT("rfifo status %d\n", rfifostatus());
  PRINT("wfifo status %d\n", wfifostatus());

  PRINT("--\n");

  writereg(1, 0x1234);

  writereg(0, 0x1);  // copy a into rfifo
  writereg(0, 0x0);

  PRINT("status %5d", readreg(0));
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());
    
  writereg(1, 0x5678);

  writereg(0, 0x1);  // copy a into rfifo
  writereg(0, 0x0);

  PRINT("status %5d", readreg(0));
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());
    
  writereg(1, 0x9ABC);

  writereg(0, 0x1);  // copy a into rfifo
  writereg(0, 0x0);
    
  PRINT("status %5d", readreg(0));
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());
    
  writereg(1, 0xDEF0);

  writereg(0, 0x1);  // copy a into rfifo
  writereg(0, 0x0);

  PRINT("status %5d", readreg(0));
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());
        
  PRINT("Read fifo %x ", rdfifo());
  PRINT(" status %5d", readreg(0));
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());
            
  PRINT("Read fifo %x ", rdfifo());
  PRINT(" status %5d", readreg(0));
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());

  PRINT("Read fifo %x ", rdfifo());
  PRINT(" status %5d", readreg(0));
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());

  PRINT("Read fifo %x ", rdfifo());
  PRINT(" status %5d", readreg(0));
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());

  PRINT("---------------------------------\n");

  for (i=0; i<10; i++) {
    writereg(1, i);
    
    writereg(0, 0x1);  // copy a into rfifo
    writereg(0, 0x0);
    
    PRINT("status %5d", readreg(0));
    PRINT(" rfifo status %5d", rfifostatus());
    PRINT(" wfifo status %5d\n", wfifostatus());
    
  }

  PRINT("---------------------------------\n");

  for (i=0; i<10; i++) {

    PRINT("Read fifo %x ", rdfifo());
    PRINT(" status %5d", readreg(0));
    PRINT(" rfifo status %5d", rfifostatus());
    PRINT(" wfifo status %5d\n", wfifostatus());

  }

  PRINT("---------------------------------\n");

  wrfifo(0x1234);

  PRINT("status %5d", readreg(0));
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());
        
  wrfifo(0x5678);

  PRINT("status %5d", readreg(0));
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());
        
  wrfifo(0x9ABC);

  PRINT("status %5d", readreg(0));
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());
        
  wrfifo(0xDEF0);

  PRINT("status %5d", readreg(0));
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());
        
  writereg(0, 0x2);  // copy wfifo into b
  writereg(0, 0x0);

  PRINT("Read b %x ", readreg(1));
  PRINT(" status %5d", readreg(0));
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());

  writereg(0, 0x2);  // copy wfifo into b
  writereg(0, 0x0);

  PRINT("Read b %x ", readreg(1));
  PRINT(" status %5d", readreg(0));
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());

  writereg(0, 0x2);  // copy wfifo into b
  writereg(0, 0x0);

  PRINT("Read b %x ", readreg(1));
  PRINT(" status %5d", readreg(0));
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());

  writereg(0, 0x2);  // copy wfifo into b
  writereg(0, 0x0);

  PRINT("Read b %x ", readreg(1));
  PRINT(" status %5d", readreg(0));
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());

  PRINT("---------------------------------\n");

  for (i=0; i<10; i++) {

    wrfifo(i);
    
    PRINT("status %5d", readreg(0));
    PRINT(" rfifo status %5d", rfifostatus());
    PRINT(" wfifo status %5d\n", wfifostatus());
    
  }

  PRINT("---------------------------------\n");

  for (i=0; i<10; i++) {

    writereg(0, 0x2);  // copy wfifo into b
    writereg(0, 0x0);
    PRINT("Read b %x ", readreg(1));
    PRINT(" status %5d", readreg(0));
    PRINT(" rfifo status %5d", rfifostatus());
    PRINT(" wfifo status %5d\n", wfifostatus());
    
  }

  return 0;
}
