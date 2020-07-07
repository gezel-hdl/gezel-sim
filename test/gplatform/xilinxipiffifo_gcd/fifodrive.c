
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
  unsigned i, a1, a2, a3, a4;

  //                    read          write
  //
  //  register 0        spy_m
  //  register 1        spy_n
  //
  //  GCD operation: rdfifo = gcd(wrfifo1, wrfifo2)

  fifoinit();

  wrfifo(0x1235);
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());

  wrfifo(0x1234);
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());

  wrfifo(0x1234);
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());

  wrfifo(0x1234);
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());

  wrfifo(0x1234);
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());

  wrfifo(0x6);
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());

  wrfifo(0x6);
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());

  wrfifo(0x1234);
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());

  PRINT("Read fifo %5d ", rdfifo());
  PRINT(" status %5d", readreg(0));
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());

  PRINT("Read fifo %5d ", rdfifo());
  PRINT(" status %5d", readreg(0));
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());

  PRINT("Read fifo %5d ", rdfifo());
  PRINT(" status %5d", readreg(0));
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());

  PRINT("Read fifo %5d ", rdfifo());
  PRINT(" status %5d", readreg(0));
  PRINT(" rfifo status %5d", rfifostatus());
  PRINT(" wfifo status %5d\n", wfifostatus());

  wrfifo(0x1235);
  wrfifo(0x1234);

  wrfifo(0x1234);
  wrfifo(0x1234);

  wrfifo(0x1234);
  wrfifo(0x6);

  wrfifo(0x6);
  wrfifo(0x1234);

  a1 = rdfifo();
  a2 = rdfifo();
  a3 = rdfifo();
  a4 = rdfifo();

  PRINT("Read fifo %5d\n", a1);
  PRINT("Read fifo %5d\n", a2);
  PRINT("Read fifo %5d\n", a3);
  PRINT("Read fifo %5d\n", a4);

  return 0;
}
