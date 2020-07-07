//--------------------------------------------------------------
// Copyright (C) 2003-2005 P. Schaumont, D. Ching 
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
// $Id: systemc_itf.cxx 78 2007-07-14 19:00:16Z schaum $
//--------------------------------------------------------------

#include "systemc_itf.h"

#include "ipconfig.h" // static_ipblockcreate, dynamic_ipblockcreate

#define CREATE(S) if (!strcmp(tname, #S)) return new S(instname) 

aipblock *systemc_ipblockcreate(char *instname, char *tname) {
  aipblock *r;

  r = static_ipblockcreate(instname, tname);
  if (r) return r;

  CREATE(systemcsource);
  CREATE(systemcsink);

  r = dynamic_ipblockcreate(instname, tname);
  if (r) return r;

  cerr << "armcosim: unknown user block: " << tname << "\n";
  return new aipblock(instname);
}

//-------------------------------------------------------

map <string, systemcsource *>       systemcsourcelist;
map <string, systemcsink   *>       systemcsinklist;

// for sc_int<32> outport
typedef sc_out < sc_int<32> > *    otptr;
typedef map <systemcsink *, otptr > otmap;
otmap   outputmap;
typedef otmap::iterator omap_iter;

// for sc_uint<32> outport
typedef sc_out < sc_uint<32> > *    otptr_u32;
typedef map <systemcsink *, otptr_u32 > otmap_u32;
otmap_u32   outputmap_u32;
typedef otmap_u32::iterator omap_iter_u32;


//----------------------------------------------------------------

systemcsource::systemcsource(char *name) : aipblock(name), interfacewritten(false) {}

void systemcsource::setparm(char *_name) {
  char *sysvar;
  if ((sysvar = matchparm(_name, "var"))) {
    cerr << "systemcsource: set variable " << sysvar << "\n";
    systemcsourcelist[string(sysvar)] = this;
  }
}

void systemcsource::run() {}

bool systemcsource::checkterminal(int n, char *tname, aipblock::iodir d) {
  switch (n) {
  case 0:
    return (isoutput(d) && isname(tname, "data"));
    break;
  }
  return false;
}

bool systemcsource::needsWakeupTest() {
  bool v = interfacewritten;
  interfacewritten = false;
  return v;
}

bool systemcsource::cannotSleepTest() {
  return true;
  //  bool v = interfacewritten;
  //  interfacewritten = false;
  //  return v;
}

void systemcsource::touch() {
  interfacewritten = true;
}

//----------------------------------------------------------------

systemcsink::systemcsink(char *name) : aipblock(name) {}

void systemcsink::setparm(char *_name) {
  char *sysvar;
  if ((sysvar = matchparm(_name, "var"))) {
    cerr << "systemcsink: set variable " << sysvar << "\n";
    systemcsinklist[string(sysvar)] = this;
  }
}

void systemcsink::run() {}

bool systemcsink::checkterminal(int n, char *tname, aipblock::iodir d) {
  switch (n) {
  case 0:
    return (isinput(d) && isname(tname, "data"));
    break;
  }
  return false;
}

bool systemcsink::cannotSleepTest() {
  return false;
}

//----------------------------------------------------------------

gezel_module::gezel_module(sc_module_name scname, char *fdlname) : sc_module(scname) {
  myrtsimgen = 0;
  if (call_parser(fdlname)) {
    cerr << "Could not open GEZEL file " << fdlname << "\n";
    exit(0);
  }
  glbAllowSimulatorSleep = 1;
  myrtsimgen = new rtsimgen;
  glbSymboltable.createcode(myrtsimgen);
  myrtsimgen->reset();    
  SC_METHOD(run);
  sensitive_pos << clk;
}

void gezel_module::run() {
  myrtsimgen->step();
  // all signed output ports
  for (omap_iter i = outputmap.begin(); i != outputmap.end(); i++) {
    i->second->write(i->first->ioval[0]->toslong());
  }
  // all unsigned output ports
  for (omap_iter_u32 j = outputmap_u32.begin(); j != outputmap_u32.end(); j++) {
    j->second->write(j->first->ioval[0]->toulong());
  }
}

//----------------------------------------------------------------

gezel_inport::gezel_inport(sc_module_name scname, char *fdlsigname) : sc_module(scname) {
  
  gzlmodule = 0;
  
  if (systemcsourcelist.find(string(fdlsigname)) != systemcsourcelist.end()) {
    gzlmodule = systemcsourcelist[string(fdlsigname)];
  } else {
    cerr << "Warning - Gezel inport does not include signal " << fdlsigname << "\n";
  }
  
  SC_METHOD(run);
  sensitive << datain;
}

void gezel_inport::run() {
  //  cerr << "%%% gezel inport runs .. ";
  if (gzlmodule) {
    gzlmodule->ioval[0]->assignslong(datain.read().to_int());
    //    cerr << "ioval set to " << datain.read().to_int() << "\n";
    gzlmodule->touch();
  }
}

//----------------------------------------------------------------

gezel_inport_u32::gezel_inport_u32(sc_module_name scname, char *fdlsigname) : sc_module(scname) {
  
  gzlmodule = 0;
  
  if (systemcsourcelist.find(string(fdlsigname)) != systemcsourcelist.end()) {
    gzlmodule = systemcsourcelist[string(fdlsigname)];
  } else {
    cerr << "Warning - Gezel inport does not include signal " << fdlsigname << "\n";
  }
  
  SC_METHOD(run);
  sensitive << datain;
}

void gezel_inport_u32::run() {
  //  cerr << "%%% gezel inport runs .. ";
  if (gzlmodule) {
    gzlmodule->ioval[0]->assignulong(datain.read().to_uint());
    //    cerr << "ioval set to " << datain.read().to_int() << "\n";
    gzlmodule->touch();
  }
}

//----------------------------------------------------------------

gezel_outport::gezel_outport(sc_module_name scname, char *fdlsigname) : sc_module(scname) {
    gzlmodule = 0;
    if (systemcsinklist.find(string(fdlsigname)) != systemcsinklist.end()) {
      gzlmodule = systemcsinklist[string(fdlsigname)];
    } else {
      cerr << "Warning - Gezel outport does not include signal " << fdlsigname << "\n";
    }
    outputmap[gzlmodule] = &dataout;  
}

//----------------------------------------------------------------

gezel_outport_u32::gezel_outport_u32(sc_module_name scname, char *fdlsigname) : sc_module(scname) {
    gzlmodule = 0;
    if (systemcsinklist.find(string(fdlsigname)) != systemcsinklist.end()) {
      gzlmodule = systemcsinklist[string(fdlsigname)];
    } else {
      cerr << "Warning - Gezel outport does not include signal " << fdlsigname << "\n";
    }
    outputmap_u32[gzlmodule] = &dataout;  
}

