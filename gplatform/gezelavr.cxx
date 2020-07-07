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
// GEZEL integration of AVR module

#include "gezelavr.h"

vector<atm128core *> atm128list;

atm128core::atm128core(char *_name) : aipblock(_name) {
  sim = AvrFactory::instance().makeDevice("atmega128");
  sim->SetClockFreq(1000000000 / 4000000);  // default freq: 4MHz
  SystemClock::Instance().Add(sim);

  atm128list.push_back(this);

  xtalpin = sim->GetPin("XTAL");
  xtal = 0;

  // to print debbug messages
  sim->ReplaceIoRegister(0x22,
			 new RWReadFromFile(sim, "FREAD", "-"));
  sim->ReplaceIoRegister(0x20,
			 new RWWriteToFile(sim, "FWRITE", "-"));

}
 
atm128core::~atm128core() {
  if (tracefile.is_open())
    tracefile.close();
  glbRunningISS--;
}

// ipblock avr {
//    iptype "atm128core";
//    ipparm "exec=application"; // ELF application
//    ipparm "trace=1";
//    ipparm "tracefile=thisdata.txt";
// }

void atm128core::setparm(char *_name) {
  char *n;
  gval *v = make_gval(32,0);

  if ((n = matchparm(_name,"exec"))) {
    cerr << "atm128core: Load program " << n << "\n";
    sim->Load(n);
  } else if (matchparm(_name, "tracemode", *v)) {
    if (v->toulong() == 1) {
      sim->trace_on = 1;
      sysConHandler.SetTraceStream(&(std::cout));
    }
  } else if  ((n = matchparm(_name,"tracefile"))) {
    cerr << "Opening Trace File " << n << "\n";
    tracefile.open(n, ios_base::out);
    sysConHandler.SetTraceStream(&tracefile);
  } else {
    cerr <<"atm128core: Parameter not recognized: " << _name << "\n";
  }
}

void atm128core::run() {
  bool flag;
  SystemClock::Instance().Step(flag);
  xtalpin->SetInState((Pin::T_Pinstate) xtal);
  xtal = 1 - xtal;
}

bool atm128core::checkterminal(int n, char *tname, iodir dir) {
  return true; // no terminal
}

bool atm128core::cannotSleepTest() {
  return false;
}

bool atm128core::needsWakeupTest() {
  run();  // simulator continues to run in sleep mode
  return false;
}

void atm128core::blockincluded() {
  glbRunningISS++;
}

//------------------------------------------------------------

// ipblock avrpin(out data : ns(1)) {
//    iptype "atm128sourcepin";
//    ipparm "core=avr";
//    ipparm "pin=A2";
// }

atm128sourcepin::atm128sourcepin(char *_name) : aipblock(_name) {
  interfacewritten = false;
  hook = 0;
}

atm128sourcepin::~atm128sourcepin() {}

void atm128sourcepin::setparm(char *_name) {
  char *parm;
  
  if ((parm = matchparm(_name, "core"))) {
    vector<atm128core *>::iterator ai;
    for (ai = atm128list.begin();
	 ai != atm128list.end();
	 ai++) {
      if ((*ai)->blockisname(parm)) {
	hook = ((*ai)->sim);
	return;
      }
    }
    cerr << "atm128sourcepin: core " << parm << " not found\n";
    exit(0);
  } else if ((parm = matchparm(_name, "pin"))) {
    if (hook) {
      avrpin = hook->GetPin(parm);
      avrpin->RegisterCallback(this);
    } else {
      cerr << "atm128sourcepin: set core parameter before selecting pin\n";
      exit(0);
    }
  } else {
    cerr << "atm128sourcepin: parameter " << _name << " not understood\n";
  }
}

void atm128sourcepin::run() { }

bool atm128sourcepin::checkterminal(int n, char *tname, iodir dir) {
  switch (n) {
  case 0:
    return (isoutput(dir) && isname(tname, "data")); 
    break;
  }
  return false;
}

bool atm128sourcepin::needsWakeupTest() {
  bool v = interfacewritten;
  interfacewritten = false;
  return v;
}

bool atm128sourcepin::cannotSleepTest() {
  bool v = interfacewritten;
  interfacewritten = false;
  return v;
}

void atm128sourcepin::PinStateHasChanged(Pin *p) {
  ioval[0]->assignulong((bool)*p);
  interfacewritten = true;
}

//------------------------------------------------------------

// ipblock avrpin(in data : ns(1)) {
//    iptype "atm128sinkpin";
//    ipparm "core=avr";
//    ipparm "pin=A2";
// }


atm128sinkpin::atm128sinkpin(char *_name) : aipblock(_name) {
  avrpin = 0;
  hook   = 0;
}

atm128sinkpin::~atm128sinkpin() {}

void atm128sinkpin::setparm(char *_name) {
  char *parm;
  
  if ((parm = matchparm(_name, "core"))) {
    vector<atm128core *>::iterator ai;
    for (ai = atm128list.begin();
	 ai != atm128list.end();
	 ai++) {
      if ((*ai)->blockisname(parm)) {
	hook = ((*ai)->sim);
	return;
      }
    }
    cerr << "atm128sinkpin: core " << parm << " not found\n";
    exit(0);
  } else if ((parm = matchparm(_name, "pin"))) {
    if (hook) {
      avrpin = hook->GetPin(parm);
    } else {
      cerr << "atm128sinkpin: set core parameter before selecting pin\n";
      exit(0);
    }
  } else {
    cerr << "atm128sinkpin: parameter " << _name << " not understood\n";
  }
}

void atm128sinkpin::run() { 
  avrpin->SetInState((Pin::T_Pinstate) ioval[0]->toulong());
}

bool atm128sinkpin::checkterminal(int n, char *tname, iodir dir) {
  switch (n) {
  case 0:
    return (isinput(dir) && isname(tname, "data")); 
    break;
  }
  return false;
}

bool atm128sinkpin::needsWakeupTest() {
  return false;
}

bool atm128sinkpin::cannotSleepTest() {
  return false;
}


//------------------------------------------------------------

// ipblock adpin(in data : ns(10)) {
//    iptype "atm128sinkpin";
//    ipparm "core=avr";
//    ipparm "aref=5000000";
//    ipparm "pin=F0";
// }

atm128adpin::atm128adpin(char *_name) : aipblock(_name), Pin(Pin::ANALOG) {
  hook = 0;
}

atm128adpin::~atm128adpin() {}

void atm128adpin::setparm(char *_name) {
  char *parm;
  
  if ((parm = matchparm(_name, "core"))) {
    vector<atm128core *>::iterator ai;
    for (ai = atm128list.begin();
	 ai != atm128list.end();
	 ai++) {
      if ((*ai)->blockisname(parm)) {
	hook = ((*ai)->sim);
	return;
      }
    }
    cerr << "atm128adpin: core " << parm << " not found\n";
    exit(0);
  } else if ((parm = matchparm(_name, "pin"))) {
    if (hook) {
      connection.Add(this);
      connection.Add(hook->GetPin(parm));
      // cerr << "atmpinptr " << hook->GetPin(parm) << "\n";
    } else {
      cerr << "atm128adpin: set core parameter before selecting pin\n";
      exit(0);
    }
  } else {
    cerr << "atm128adpin: parameter " << _name << " not understood\n";
  }
}

void atm128adpin::run() { 
  //  cerr << "atmadpin analog " << ioval[0]->toulong() << "\n";
  analogValue = ioval[0]->toulong();
  // cerr << "chk " << GetAnalog() << "\n";
  connectedTo->CalcNet();
  // cerr << "chk " << GetAnalog() << "\n";
}

bool atm128adpin::checkterminal(int n, char *tname, iodir dir) {
  switch (n) {
  case 0:
    return (isinput(dir) && isname(tname, "data")); 
    break;
  }
  return false;
}

bool atm128adpin::needsWakeupTest() {
  return false;
}

bool atm128adpin::cannotSleepTest() {
  return false;
}


