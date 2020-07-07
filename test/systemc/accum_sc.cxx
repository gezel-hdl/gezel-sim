
#include "systemc_itf.h"

int sc_main(int argc, char **argv) {

  sc_signal<bool>         clk;
  sc_signal<sc_int<32> >  data_1;
  sc_signal<sc_int<32> >  data_2;

  int i;

  glbIpblockcreate = systemc_ipblockcreate;

  // instantiate the gezel parser to scan in a module
  gezel_module  G ("gezel_block",   "accum.fdl");
  G.clk(clk);

  // instantiate an input channel to gezel
  // this will connect to an ipblock of type 'systemcsource'
  // the channel name is 'var1'
  gezel_inport  O1("gezel_inport",  "var1");
  O1.datain(data_1);

  // instantiate an output channel from gezel
  // this will connect to an ipblock of type 'systemcsink'
  // the channel name is 'var2'
  gezel_outport O2("gezel_outport", "var2");
  O2.dataout(data_2);

  sc_initialize();
  cerr << "Sim starts\n";

  for (i=0; i<10; i++) {
    clk = false;
    data_1 = i;
    cout << "data_2 value is " << data_2.read() << "\n";
    sc_start(50);
    clk = true;
    sc_start(50);
  }

  return 0;
}
