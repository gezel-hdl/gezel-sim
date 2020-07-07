//--------------------------------------------------------------
// Copyright (C) 2003-2010 P. Schaumont 
//                                                                             
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2 of
// the License, or (at your option) any later version.
//                                                                             
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//                                                                             
// You should have received a copy of the GNU Lesser General Public
// License along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
//--------------------------------------------------------------

#ifndef SYSTEMC_ITF_H
#define SYSTEMC_ITF_H

#include "rtsim.h"
#include "systemc.h"

#include "ipblock.h" // aipblock

aipblock *systemc_ipblockcreate(char *instname, char *tname);

class systemcsource : public aipblock {
  bool  interfacewritten;
public:
  systemcsource(char *name);
  void setparm (char *_name);
  void run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool needsWakeupTest();
  bool cannotSleepTest();
  void touch();
};

class systemcsink : public aipblock {
public:
  systemcsink(char *name);
  void setparm (char *_name);
  void run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool cannotSleepTest();
};

SC_MODULE(gezel_module) {
  rtsimgen *myrtsimgen;
  sc_in <bool> clk;
  SC_HAS_PROCESS(gezel_module);
  gezel_module(sc_module_name scname, char *fdlname);
  void run();
};

SC_MODULE(gezel_inport) {
  sc_in < sc_int<32> > datain;
  systemcsource *gzlmodule;
  SC_HAS_PROCESS(gezel_inport);
  gezel_inport(sc_module_name scname, char *fdlsigname);
  void run();
};

SC_MODULE(gezel_outport) {
  sc_out < sc_int<32> > dataout;
  systemcsink *gzlmodule;
  SC_HAS_PROCESS(gezel_outport);
  gezel_outport(sc_module_name scname, char *fdlsigname);
};

SC_MODULE(gezel_inport_u32) {
  sc_in < sc_uint<32> > datain;
  systemcsource *gzlmodule;
  SC_HAS_PROCESS(gezel_inport_u32);
  gezel_inport_u32(sc_module_name scname, char *fdlsigname);
  void run();
};

SC_MODULE(gezel_outport_u32) {
  sc_out < sc_uint<32> > dataout;
  systemcsink *gzlmodule;
  SC_HAS_PROCESS(gezel_outport_u32);
  gezel_outport_u32(sc_module_name scname, char *fdlsigname);
};

#endif
