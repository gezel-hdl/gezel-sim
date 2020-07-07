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
// $Id: i8051system.cxx 52 2006-05-28 19:33:23Z schaum $
//--------------------------------------------------------------

#include "rtsim.h"
#include "i8051system.h"

// ipblock my8051 {
//   iptype "i8051system";
//   ipparm "exec     = hex_file";
//   ipparm "verbose  = 1";
//   ipparm "period   = 5";
// }

vector<i8051system *> i8051systemlist;

i8051system::i8051system(char *n) : aipblock(n) {
  period     = 1;
  period_cnt = period;  
  i8051systemlist.push_back(this);
}

void i8051system::setparm(char *_name) {
  gval *v = make_gval(32,0);
  char *n;

  if ((n = matchparm(_name, "exec"))) {
    cerr << "i8051system: loading executable [" << n << "]\n";
    sim.LoadHex(n);
    sim.Init8051();
  } else if (matchparm(_name, "period", *v)) {
    period = v->toslong();
    if (period < 1)
      period = 1;
    period_cnt = period;
  } else if (matchparm(_name, "verbose", *v)) {
    if (v->toulong()) {
      sim.VerboseMode = true;
    } else {
      sim.VerboseMode = false;
    }
  } else {
    cerr << "i8051system: Parameter not recognized: " << _name << "\n";
  }
}

void i8051system::run() {
  if (sim.IsRunning()) {
    period_cnt--;
    if (period_cnt == 0) {
      period_cnt = period;      
      sim.ClockTick();
      if (! sim.IsRunning())
	glbRunningISS--;
    }
  }
}

void i8051system::blockincluded() {
  if (sim.IsRunning()) {
    glbRunningISS++;
  }
}

bool i8051system::checkterminal(int n, char *tname, iodir dir) {
  return true; // no terminals
}

bool i8051system::cannotSleepTest() {
  return false; // the ISS will continue to run in sleep mode (cfr needsWakeupTest).
                // so we can also return 'OK to sleep'
}

bool i8051system::needsWakeupTest() {
  run();
  return false;
}

void i8051system::stop() {
  sim.Stop();
}

//-----------------------------------------------
// ipblock my8051sink(in data : ns(8)) {
//   iptype "i8051systemsink";
//   ipparm "core = my8051";
//     ipparm "port = P0";
// }

i8051systemsink::i8051systemsink(char *name) : aipblock(name) {
  hook        = 0;
  address     = 0;
}

i8051systemsink::~i8051systemsink() {}

void i8051systemsink::setparm(char *_name) {
  char *parm;

  if ((parm = matchparm(_name, "core"))) {
    vector<i8051system *>::iterator ai;
    for (ai = i8051systemlist.begin();
	 ai != i8051systemlist.end();
	 ai++) {
      if ((*ai)->blockisname(parm)) {
	hook = & ((*ai)->sim);
	return;
      }
    }
    cerr << "i8051systemsink: core " << parm << " not found\n";
    exit(0);
  } else if ((parm = matchparm(_name, "port"))) {
    if (hook) {
      //    port -> contains port
      if (strcmp(parm, "P0") == 0) {
	address = hook->P0;
      } else if (strcmp(parm, "P1") == 0) {
	address = hook->P1;
      } else if (strcmp(parm, "P2") == 0) {
	address = hook->P2;
      } else if (strcmp(parm, "P3") == 0) {
	address = hook->P3;
      } else {
	cerr << "i8051systemsink: unmatched port name (choose one of P0, P1, P2, P3)\n";
	exit(0);
      }
    } else {
      cerr << "i8051systemsink: set core parameter before selecting port\n";
      exit(0);
    }
    return;
  }
  cerr << "i8051systemsink: parameter " << _name << " not understood\n";
}

void i8051systemsink::run() {
  if (hook) {
    hook->writeRAM(address, ioval[0]->toulong());
  }
}

bool i8051systemsink::checkterminal(int n, char *tname, aipblock::iodir d) {
  switch (n) {
  case 0:
    return (isinput(d) && isname(tname, "data"));
    break;
  }
  return false;
}

bool i8051systemsink::cannotSleepTest() {
  return false;
}


//-----------------------------------------------
// ipblock my8051source(out data : ns(8)) {
//   iptype "i8051systemsource";
//   ipparm "core = my8051";
//     ipparm "port = P0";
//   or
//     ipparm "xbus = address";
// }

map<int, i8051systemsource *> i8051devmap;

i8051systemsource::i8051systemsource(char *name) : 
  aipblock(name), 
  interfacewritten(false) {
  address = 0;
}

i8051systemsource::~i8051systemsource() {
}

void i8051systemsource::setparm(char *_name) {
  char *parm;
  
  if ((parm = matchparm(_name, "core"))) {
    vector<i8051system *>::iterator ai;
    for (ai = i8051systemlist.begin();
	 ai != i8051systemlist.end();
	 ai++) {
      if ((*ai)->blockisname(parm)) {
	hook = & ((*ai)->sim);
	return;
      }
    }
    cerr << "i8051systemsource: core " << parm << " not found\n";
    exit(0);
  } else if ((parm = matchparm(_name, "port"))) {
    if (hook) {
      //    port -> contains port
      if (strcmp(parm, "P0") == 0) {
	address = hook->P0;
	i8051devmap[hook->register_addr(hook->P0)] = this;
      } else if (strcmp(parm, "P1") == 0) {
	address = hook->P1;
	i8051devmap[hook->register_addr(hook->P1)] = this;
      } else if (strcmp(parm, "P2") == 0) {
	address = hook->P2;
	i8051devmap[hook->register_addr(hook->P2)] = this;
      } else if (strcmp(parm, "P3") == 0) {
	address = hook->P3;
	i8051devmap[hook->register_addr(hook->P3)] = this;
      } else {
	cerr << "i8051source: unmatched port name (choose one of P0, P1, P2, P3)\n";
	exit(0);
      }
    } else {
      cerr << "i8051systemsource: set core parameter before selecting port\n";
      exit(0);
    }
    return;
  }
  cerr << "i8051systemsource: parameter " << _name << " not understood\n";
}

void i8051systemsource::run() {
}

bool i8051systemsource::checkterminal(int n, char *tname, aipblock::iodir d) {
  switch (n) {
  case 0:
    return (isoutput(d) && isname(tname, "data"));
    break;
  }
  return false;
}

bool i8051systemsource::needsWakeupTest() {
  bool v = interfacewritten;
  interfacewritten = false;
  return v;
}

bool i8051systemsource::cannotSleepTest() {
  bool v = interfacewritten;
  interfacewritten = false;
  return v;
}

void i8051systemsource::touch() {
  interfacewritten = true;
}

//-----------------------------------------------
// ipblock my8051buffer(in  idata : ns(8);
//                      out odata : ns(8);
//                      in  address : ns(x);
//                      in  wr    : ns(1)) {
//   iptype "i8051buffer";
//   ipparm "core   = my8051";
//   ipparm "xbus   = addr";
//   ipparm "xrange = range";
// }
//
// This is a shared memory in external address space
// from xbus to xbus + xrange
//
// The coprocessor can access it as a dual port
// buffer using a local address 0 to xrange-1

i8051buffer::i8051buffer(char *name) : aipblock(name) {
  hook        = 0;
  address     = 0;
  range       = 0;
}

i8051buffer::~i8051buffer() {}

void i8051buffer::setparm(char *_name) {
  char *parm;
  gval *v = make_gval(32,0);

  if ((parm = matchparm(_name, "core"))) {
    vector<i8051system *>::iterator ai;
    for (ai = i8051systemlist.begin();
	 ai != i8051systemlist.end();
	 ai++) {
      if ((*ai)->blockisname(parm)) {
	hook = & ((*ai)->sim);
	return;
      }
    }
    cerr << "i8051buffer: core " << parm << " not found\n";
    exit(0);
  } else if (matchparm(_name, "xbus", *v)) {
    if (hook) {
      address = v->toulong();
    } else {
      cerr << "i8051buffer: set core parameter before selecting port\n";
      exit(0);
    }
    return;
  } else if (matchparm(_name, "xrange", *v)) {
    if (hook) {
      range = v->toulong();
    } else {
      cerr << "i8051buffer: set core parameter before selecting port\n";
      exit(0);
    }
    return;
  }
  cerr << "i8051buffer: parameter " << _name << " not understood\n";
}

void i8051buffer::run() {
  if (hook) {
    // implement a read/write cycle for ram
    // ioval[0]  idata
    // ioval[1]  odata
    // ioval[2]  address
    // ioval[3]  wr
    if (ioval[3]->toulong()) {
      // is a write cycle
      if (ioval[2]->toulong() < range) 
	hook->writeXRAM(address + ioval[2]->toulong(), ioval[0]->toulong());
    } else {
      // is a read cycle
      if (ioval[2]->toulong() < range) 
	ioval[1]->assignulong(hook->readXRAM(address + ioval[2]->toulong()));
    }
  }
}

bool i8051buffer::checkterminal(int n, char *tname, aipblock::iodir d) {
  switch (n) {
  case 0:
    return (isinput(d) && isname(tname, "idata"));
    break;
  case 1:
    return (isoutput(d) && isname(tname, "odata"));
    break;
  case 2:
    return (isinput(d) && isname(tname, "address"));
    break;
  case 3:
    return (isinput(d) && isname(tname, "wr"));
    break;
  }
  return false;
}

bool i8051buffer::cannotSleepTest() {
  return false;
}

//---------------------------

void i8051externalwrite(int dev, unsigned char d) {
  i8051devmap[dev]->ioval[0]->assignulong((long) d);
  i8051devmap[dev]->touch();
}
