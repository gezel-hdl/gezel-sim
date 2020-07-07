#include "ipblock.h"
#include "ipconfig.h"    // static_ipblockcreate, dynamic_ipblockcreate

#include "systemc_itf.h" // systemc ipblocks
#include "armsystem.h"   // arm ipblocks

#define CREATE(S) if (!strcmp(tname, #S)) return new S(instname) 

aipblock * ipblockcreate(char *instname, char *tname) {
  aipblock *r;

  r = static_ipblockcreate(instname, tname);
  if (r) return r;

  CREATE(armsystem);
  CREATE(armsystemsource);
  CREATE(armsystemsink);
  CREATE(armsystemprobe);
  CREATE(armbuffer);
  CREATE(armfslslave);
  CREATE(armfslmaster);
  CREATE(systemcsource);
  CREATE(systemcsink);

  r = dynamic_ipblockcreate(instname, tname);
  if (r) return r;

  cerr << "Unknown ipblock type: " << tname << "\n";
  return new aipblock(instname);
}

//----------------------------------------------
int sc_main(int argc, char **argv) {

  sc_signal<bool>         clk;
  sc_signal<sc_uint<32> >  data_1;
  sc_signal<sc_uint<32> >  data_2;

  int i;
  unsigned old2 = 0;

  glbIpblockcreate = ipblockcreate;

  // instantiate the gezel parser to scan in a module
  gezel_module  G ("gezel_block",   "accum.fdl");
  G.clk(clk);

  // instantiate an input channel to gezel
  // this will connect to an ipblock of type 'systemcsource'
  // the channel name is 'var1'
  gezel_inport_u32  O1("gezel_inport_u32",  "var1");
  O1.datain(data_1);

  // instantiate an output channel from gezel
  // this will connect to an ipblock of type 'systemcsink'
  // the channel name is 'var2'
  gezel_outport_u32 O2("gezel_outport_u32", "var2");
  O2.dataout(data_2);

  sc_initialize();
  cerr << "Sim starts\n";

  for (i=0; i<100000; i++) {
    clk = false;

    // send clock cycle count as data to arm
    data_1 = 0x80000000 + i;

    // print data_2 input whenever it changes
    if (old2 != data_2.read()) {
      old2 = data_2.read();
      cout << i << " SystemC: data_2 value read is ";
      cout << hex << data_2.read() << "\n";
    }

    sc_start(50);
    clk = true;
    sc_start(50);

  }

  return 0;
}
