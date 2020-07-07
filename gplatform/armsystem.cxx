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
// $Id: armsystem.cxx 92 2007-09-21 20:56:53Z schaum $
//--------------------------------------------------------------

#include "armsystem.h"
#include "platform.h"

// ipblock myarm {
//   iptype "armsystem";
//   ipparm "fpe_name = path_to_fpe";
//   ipparm "exec     = path_to_executable args_for_executable";
//   ipparm "verbose  = 1";
//   ipparm "period   = 5";
// }

vector<armsystem *> armsystemlist;

arm_simulator *findcore(char *pname) {
  vector<armsystem *>::iterator ai;
  for (ai = armsystemlist.begin();
       ai != armsystemlist.end();
       ai++) 
    if ((*ai)->blockisname(pname)) 
      return (*ai)->sim;
  return 0;
}

map <int, aipblock *>    armdevmap; 

void register_ipblock(int dev, aipblock *i) {
  armdevmap[dev] = i;
}

// for SFU units
static armsystem *findsystem(char *pname, aipblock *slave) {
  vector<armsystem *>::iterator ai;
  for (ai = armsystemlist.begin();
       ai != armsystemlist.end();
       ai++) 
    if ((*ai)->blockisname(pname)) {
      (*ai)->setslave(slave);
      return *ai;
    }
  return 0;
}

armsystem::armsystem(char *name) : aipblock(name) {
  armsystemlist.push_back(this);
  cerr << "core " << name << "\n";
  fpe        = NULL;
  sim        = new arm_simulator(false);
  simargc    = 0;
  period     = 1;
  period_cnt = period;
  slave_exec = 0;
}

void armsystem::setparm(char *_name) {
  gval *v = make_gval(32,0);
  char *n;
  
  if ((n = matchparm(_name, "fpe_name"))) {
    cerr << "armsystem: set fpe_name " << n << "\n";
    fpe = n;
  } else if ((n = matchparm(_name, "exec"))) {
    char *nn = n;
    while (*nn) {
      simargv[simargc++] = nn++;
      while (*nn && (*nn != ' ')) nn++;   // look for ws
      if (*nn) {                          // when ws found
	*nn++ = 0;                        // terminate sub string
	while (*nn && (*nn == ' ')) nn++; // and look for nonws
      }
    }
    cerr << "armsystem: loading executable [" << simargv[0] << "]\n";
    sim->load_program(simargv[0], simargc, simargv, *glbSimEnvp);
    sim->load_fpe(fpe);
    sim->initialize();
  } else if (matchparm(_name, "verbose", *v)) {
    if (v->toulong()) {
      sim->verbose=true;
    } else {
      sim->verbose=false;
    }
  } else if (matchparm(_name, "period", *v)) {
    period = v->toslong();
    if (period < 1)
      period = 1;
    period_cnt = period;
  } else {
    cerr << "armsystem: Parameter not recognized: " << _name << "\n";
  }
}

void armsystem::blockincluded() {
  if (sim->is_running()) {
    glbRunningISS++;
  }
}

bool armsystem::checkterminal(int n, char *tname, iodir dir) {
  return true; // no terminals
}

bool armsystem::cannotSleepTest() {
  return false;
}

void armsystem::run() {
  if (slave_exec == 0)
    realrun();
}

bool armsystem::needsWakeupTest() {
  run();
  return false;
}

void armsystem::stop() {
  sim->stop();
}

void armsystem::setslave(aipblock *s) {
  slave_exec = s;
}

void armsystem::slaverun(aipblock *s) {
  if (s == slave_exec) {
    //    cerr << "real run " << s << "\n";
    realrun();
  }
}

void armsystem::realrun() {
  if (sim->is_running()) {
    period_cnt--;
    if (period_cnt == 0) {
      period_cnt = period;       
      sim->clock_tick();
      if (! sim->is_running())
	glbRunningISS--;
    }
  }
}

//----------------------------------------------------------------

// ---- ARMSIMSINK --- (memory-mapped HWSW cosimulation)
armsystemsink::armsystemsink(char *name) : aipblock(name) {
  hook = 0;
}

void armsystemsink::setparm(char *_name) {
  gval *v = make_gval(32,0);
  char *pname;

  if ((pname = matchparm(_name, "core"))) {
    if ((hook = findcore(pname)))
	return;
    cerr << "armsystemsink: core " << pname << " not found\n";
    exit(0);
  } else if (matchparm(_name, "address", *v)) {
    cerr << "armsystemsink: set address " << v->toulong() << "\n";
    if (hook) {
      deviceid = hook->mem->register_addr(v->toulong()); 
      register_ipblock(deviceid, this);
    } else {
      cerr << "armsystemsink: set core parameter before selecting address\n";
      exit(0);
    }
    return;
  }
  cerr << "armsystemsink: parameter " << _name << " not understood\n";
}

void armsystemsink::run() {
  //  cout << "armsystemsink value " << ioval[0].toulong() << "\n";
}

bool armsystemsink::checkterminal(int n, char *tname, aipblock::iodir d) {
  switch (n) {
  case 0:
    return (isinput(d) && isname(tname, "data"));
    break;
  }
  return false;
}

bool armsystemsink::cannotSleepTest() {
  return false;
}

unsigned long armsystemsink::read_device(int dev) {
  return ioval[0]->toulong();
}

// ---- ARMSYSTEMSOURCE --- (memory-mapped HWSW cosimulation)

armsystemsource::armsystemsource(char *name) : 
  aipblock(name), 
  interfacewritten(false) {
  hook = 0;
}

void armsystemsource::setparm(char *_name) {
  gval *v = make_gval(32,0);
  char *pname;

  if ((pname = matchparm(_name, "core"))) {
    if ((hook = findcore(pname)))
	return;
    cerr << "armsystemsource: core " << pname << " not found\n";
    exit(0);
  } else if (matchparm(_name, "address", *v)) {
    if (hook) {
      deviceid = hook->mem->register_addr(v->toulong()); 
      register_ipblock(deviceid, this);
    } else {
      cerr << "armsystemsource: set core parameter before selecting address\n";
      exit(0);
    }
    return;
  }
  cerr << "armsystemsink: parameter " << _name << " not understood\n";
}

void armsystemsource::run() {
  //  cout << "armsystemsink value " << ioval[0].toulong() << "\n";
}

bool armsystemsource::checkterminal(int n, char *tname, aipblock::iodir d) {
  switch (n) {
  case 0:
    return (isoutput(d) && isname(tname, "data"));
    break;
  }
  return false;
}

bool armsystemsource::needsWakeupTest() {
  bool v = interfacewritten;
  interfacewritten = false;
  return v;
}

bool armsystemsource::cannotSleepTest() {
  bool v = interfacewritten;
  interfacewritten = false;
  return v;
}

void armsystemsource::write_device(int dev, unsigned long n) {
  ioval[0]->assignulong(n);
  interfacewritten = true;
}



//----------------------------------------------------
//  ipblock fsl1(out data   : ns(32);
//               out exists : ns(1);
//               in  read   : ns(1)) {
//    iptype "armfslslave";
//    ipparm "core=arm1";
//    ipparm "write=0x80000000";
//    ipparm "status=0x80000008";
//  }

armfslslave::armfslslave(char *name) : 
  aipblock(name), 
  interfacewritten(false),
  tokenavailable(false) {
  hook     = 0;
  write_deviceid = 0;
  status_deviceid = 0;
}

armfslslave::~armfslslave() {}

void armfslslave::setparm(char *name) {
  gval *v = make_gval(32,0);
  char *pname;

  if ((pname = matchparm(name, "core"))) {
    if ((hook = findcore(pname)))
      return;
    cerr << "armfslslave: core " << pname << " not found\n";
    exit(0);
  } else if (matchparm(name, "write", *v)) {
    if (hook) {
      write_deviceid = hook->mem->register_addr(v->toulong()); 
      register_ipblock(write_deviceid, this);
    } else {
      cerr << "armfslslave: set core parameter before selecting address\n";
      exit(0);
    }
    return;
  } else if (matchparm(name, "status", *v)) {
    if (hook) {
      status_deviceid = hook->mem->register_addr(v->toulong()); 
      register_ipblock(status_deviceid, this);
    } else {
      cerr << "armfslslave: set core parameter before selecting address\n";
      exit(0);
    }
    return;
  }
  cerr << "armfslslave: parameter " << name << " not understood\n";
}

void armfslslave::run() {

  // data value ioval[0] is filled out in ext_write_xxx functions
  if (ioval[2]->toulong() == 1) 
    tokenavailable = 0;

  if (tokenavailable)
    ioval[1]->assignulong(1); // exists <- 1
  else
    ioval[1]->assignulong(0); // exists <- 0
}

bool armfslslave::checkterminal(int n, char *tname, aipblock::iodir d) {
  switch (n) {
  case 0:
    return (isoutput(d) && isname(tname, "data"));
    break;
  case 1:
    return (isoutput(d) && isname(tname, "exists"));
    break;
  case 2:
    return (isinput(d) && isname(tname, "read"));
    break;
  }
  return false;
}

bool armfslslave::needsWakeupTest() {
  bool v = interfacewritten;
  interfacewritten = false;
  //  cout << "WU..";
  return v;
}

bool armfslslave::cannotSleepTest() {
  bool v = interfacewritten;
  interfacewritten = false;
  return v;
}

void armfslslave::write_device(int dev, unsigned long n) {
  ioval[0]->assignulong((long) n);
  interfacewritten = true;
  tokenavailable = true;
  //  cout << "T\n";
}

unsigned long armfslslave::read_device(int deviceid) {
  if (deviceid == status_deviceid) {
    if (tokenavailable) 
      ioval[0]->assignulong(1);
    else
      ioval[0]->assignulong(0);
  }
  return ioval[0]->toulong();
  //  cout << "RT\n";
}

//----------------------------------------------------
//ipblock fsl2(in  data   : ns(32);
//             out full   : ns(1);
//             in  write  : ns(1)) {
//  iptype "armfslmaster";
//  ipparm "core=arm1";
//  ipparm "read=0x80000004";
//  ipparm "status=0x80000008";
//}

armfslmaster::armfslmaster(char *name) : 
  aipblock(name) {
  hook     = 0;
  read_deviceid = 0;
  status_deviceid = 0;
  tokenavailable = false;
  lastdata = 0;
}

armfslmaster::~armfslmaster() {}

void armfslmaster::setparm(char *name) {
  gval *v = make_gval(32,0);
  char *pname;

  if ((pname = matchparm(name, "core"))) {
    if ((hook = findcore(pname)))
      return;
    cerr << "armfslmaster: core " << pname << " not found\n";
    exit(0);
  } else if (matchparm(name, "read", *v)) {
    if (hook) {
      read_deviceid = hook->mem->register_addr(v->toulong()); 
      register_ipblock(read_deviceid, this);
    } else {
      cerr << "armfslmaster: set core parameter before selecting address\n";
      exit(0);
    }
    return;
  } else if (matchparm(name, "status", *v)) {
    if (hook) {
      status_deviceid = hook->mem->register_addr(v->toulong()); 
      register_ipblock(status_deviceid, this);
    } else {
      cerr << "armfslmaster: set core parameter before selecting address\n";
      exit(0);
    }
    return;
  }
  cerr << "armfslslave: parameter " << name << " not understood\n";
}

void armfslmaster::run() {
  // sets up a one-place fifp
  // (full flag goes up after a single write)
  if ((tokenavailable==false) && (ioval[2]->toulong()==1)) {
    lastdata = ioval[0]->toulong(); // current data
    tokenavailable = true;
  }
  if (tokenavailable) 
    ioval[1]->assignulong(1);
  else
    ioval[1]->assignulong(0);
}

bool armfslmaster::checkterminal(int n, char *tname, aipblock::iodir d) {
  switch (n) {
  case 0:
    return (isinput(d) && isname(tname, "data"));
    break;
  case 1:
    return (isoutput(d) && isname(tname, "full"));
    break;
  case 2:
    return (isinput(d) && isname(tname, "write"));
    break;
  }
  return false;
}

unsigned long armfslmaster::read_device(int deviceid) {
  // this code was inherited from the former r_touch function
  // probably, the data to returned does not need to be copied into ioval[0]
  if (deviceid == read_deviceid) {
    ioval[0]->assignulong(lastdata);
    tokenavailable = false;
  } else {
    // status id
    if (tokenavailable) 
      ioval[0]->assignulong(1);
    else
      ioval[0]->assignulong(0);
  }
  return ioval[0]->toulong();
}

bool armfslmaster::cannotSleepTest() {
  return tokenavailable;
}

#include "emumem.h"

//--- ARMBUFFER --------------------------------------
// ipblock myarmbuffer(in  idata : ns(32);
//                     out odata : ns(32);
//                     in  address : ns(x);
//                     in  wr    : ns(1)) {
//   iptype "armbuffer";
//   ipparm "core   = myarm";
//   ipparm "address= addr";
//   ipparm "range  = range";
// }
//
// This is a shared memory in external address space
// from address to address + range
//
// The coprocessor can access it as a dual port
// buffer using a local address 0 to xrange-1

armbuffer::armbuffer(char *name) : aipblock(name) {
  hook        = 0;
  address     = 0;
  range       = 0;
}

armbuffer::~armbuffer() {}

void armbuffer::setparm(char *_name) {
  char *parm;
  gval *v = make_gval(32,0);

  if ((parm = matchparm(_name, "core"))) {
    if ((hook = findcore(parm)))
      return;
    cerr << "armbuffer: core " << parm << " not found\n";
    exit(0);
  } else if (matchparm(_name, "address", *v)) {
    if (hook) {
      address = v->toulong();
    } else {
      cerr << "armbuffer: set core parameter before selecting port\n";
      exit(0);
    }
    return;
  } else if (matchparm(_name, "range", *v)) {
    if (hook) {
      range = v->toulong();
    } else {
      cerr << "armbuffer: set core parameter before selecting port\n";
      exit(0);
    }
    return;
  }
  cerr << "armbuffer: parameter " << _name << " not understood\n";
}

void armbuffer::run() {
  if (hook) {
    // implement a read/write cycle for ram
    // ioval[0]  idata
    // ioval[1]  odata
    // ioval[2]  address
    // ioval[3]  wr
    if (ioval[3]->toulong()) {
      // is a write cycle
      if (ioval[2]->toulong() < range) 
	hook->mem->write_word(address + ioval[2]->toulong(), ioval[0]->toulong());
    } else {
      // is a read cycle
      if (ioval[2]->toulong() < range) 
	ioval[1]->assignulong(hook->mem->read_word(address + ioval[2]->toulong()));
    }
  }
}

bool armbuffer::checkterminal(int n, char *tname, aipblock::iodir d) {
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

bool armbuffer::cannotSleepTest() {
  return false;
}

//-- ARMSYSTEMPROBE ----------------------------------

armsystemprobe::armsystemprobe(char *name) : aipblock(name) {
  hook             = 0;
  probedblock      = 0;
  interfacewritten = false;
  runonce          = false;
  // make a 'virtual output'
  ioval.push_back(make_gval(32, 0));
}

void armsystemprobe::setparm(char *_name) {
  gval *v = make_gval(32,0);
  char *blockname, *pname;

  if (matchparm(_name, "probe", *v)) {
    cerr << "armsystemprobe: set address " << v->toulong() << "\n";
    if (hook) {
      deviceid = hook->mem->register_addr(v->toulong()); 
      register_ipblock(deviceid, this);
    } else {
      cerr << "armsystemprobe: must set core parameter before setting probe parameter\n";
      exit(0);
    }
    return;
  } else if ((blockname = matchparm(_name, "block"))) {
    vector<aipblock *>::iterator p;
    for (p = glbIpblocks.begin(); p != glbIpblocks.end(); p++) {
      if ((*p)->blockisname(blockname)) {
	probedblock = *p;
      }
    }
    if (probedblock == 0) {
      cerr << "armsystemprobe: block name " << blockname << " not found\n";
    }
    return;
  } else if ((pname = matchparm(_name, "core"))) {
    if ((hook = findcore(pname)))
      return;
    cerr << "armsystemsink: core " << pname << " not found\n";
    exit(0);
  }
  cerr << "armsystemprobe: parameter " << _name << " not understood\n";
}

void armsystemprobe::run() {
  if (runonce) {
    runonce = false;
    char buf[255];
    unsigned memloc = ioval[0]->toulong();
    char *ptr = buf;
    while ((*ptr++ = hook->mem->read_byte((target_addr_t) memloc++))) {}
    *ptr = 0;
    if (probedblock) {
      probedblock->probe(buf);
    } else {
      cerr << "armsystemprobe: missing block to probe\n";
    }
  }
}

bool armsystemprobe::checkterminal(int n, char *tname, aipblock::iodir d) {
  return false;
}

bool armsystemprobe::cannotSleepTest() {
  bool v = interfacewritten;
  interfacewritten = false;
  return v;
}

bool armsystemprobe::needsWakeupTest() {
  bool v = interfacewritten;
  interfacewritten = false;
  return v;
}

void armsystemprobe::write_device(int dev, unsigned long value) {
  ioval[0]->assignulong(value);
  interfacewritten = true;
  runonce = true;
}


//------------------------------------

byte_t     ext_read_byte     (int dev) {
  aipblock *n = armdevmap[dev];
  if (n)
    return n->read_device(dev);
  else {
    cerr << "*** ext_read: no ipblock registered\n";
    exit(0);
  }

  //  armdevmap[dev]->r_touch(dev);
  //  return armdevmap[dev]->ioval[0]->toulong();
}

halfword_t ext_read_half_word(int dev) {
  aipblock *n = armdevmap[dev];
  if (n)
    return n->read_device(dev);
  else {
    cerr << "*** ext_read: no ipblock registered\n";
    exit(0);
  }

  //  armdevmap[dev]->r_touch(dev);
  //  return armdevmap[dev]->ioval[0]->toulong();
}

word_t     ext_read_word     (int dev) {
  aipblock *n = armdevmap[dev];
  if (n)
    return n->read_device(dev);
  else {
    cerr << "*** ext_read: no ipblock registered\n";
    exit(0);
  }

  //  armdevmap[dev]->r_touch(dev);
  //  return armdevmap[dev]->ioval[0]->toulong();
}

dword_t    ext_read_dword    (int dev) {
  aipblock *n = armdevmap[dev];
  if (n)
    return n->read_device(dev);
  else {
    cerr << "*** ext_read: no ipblock registered\n";
    exit(0);
  }

  //  armdevmap[dev]->r_touch(dev);
  //  return armdevmap[dev]->ioval[0]->toulong();
}

void ext_write_byte     (int dev, byte_t value) {
  aipblock *n = armdevmap[dev];
  if (n)
    return n->write_device(dev, (unsigned long) value);
  else {
    cerr << "*** ext_write: no ipblock registered\n";
    exit(0);
  }

  //  armdevmap[dev]->ioval[0]->assignulong((long) value);
  //  armdevmap[dev]->touch();
}

void ext_write_half_word(int dev, halfword_t value) {
  aipblock *n = armdevmap[dev];
  if (n)
    return n->write_device(dev, (unsigned long) value);
  else {
    cerr << "*** ext_write: no ipblock registered\n";
    exit(0);
  }

  //  armdevmap[dev]->ioval[0]->assignulong((long) value);
  //  armdevmap[dev]->touch();
}

void ext_write_word     (int dev, word_t  value) {
  aipblock *n = armdevmap[dev];
  if (n)
    return n->write_device(dev, (unsigned long) value);
  else {
    cerr << "*** ext_write: no ipblock registered\n";
    exit(0);
  }

  //  armdevmap[dev]->ioval[0]->assignulong((long) value);
  //  armdevmap[dev]->touch();
}

void ext_write_dword    (int dev, dword_t value) {
  aipblock *n = armdevmap[dev];
  if (n)
    return n->write_device(dev, (unsigned long) value);
  else {
    cerr << "*** ext_write: no ipblock registered\n";
    exit(0);
  }

  //  armdevmap[dev]->ioval[0]->assignulong((long) value);
  //  armdevmap[dev]->touch();
}

//----------------------------------------------------------------
// ARM Special Function Units

#ifdef SUPPORTSIMITSFU

map <int, aipblock *> sfu2x2map;
map <int, aipblock *> sfu2x1map;
map <int, aipblock *> sfu3x1map;

// ipblock mysfu(out d1 : ns(32); out d2 : ns(32);
//               in  q1 : ns(32); in  q2 : ns(32)) {
//   iptype "armsfu2x2";
//   ipparm "core = mycore"; -- core to hook into
//   ipparm "device = 0";    -- 2 possible devices per sfu type
// }

armsfu2x2::armsfu2x2(char *name) : aipblock(name) {
  deviceid = 0;
  hook = 0;
  myarm = 0;
  interfacewritten = false;
  regOutput(0);  // d1 and d2 are registered
  regOutput(1);
}

void armsfu2x2::setparm(char *_name) {
  gval *v = make_gval(32,0);
  char *pname;
  
  if ((pname = matchparm(_name, "core"))) {
    if ((myarm = findsystem(pname, this))) {
      hook = myarm->sim;
      return;
    }
    cerr << "armsfu2x2: core " << pname << " not found\n";
    exit(0);
  } else if (matchparm(_name, "device", *v)) {
    if (hook) {
      if (v->toulong() < 2) {
	sfu2x2map[v->toulong()] = this;
	deviceid = v->toulong();
      }
    } else {
      cerr << "armsfu2x2: set core parameter before selecting device id\n";
      exit(0);
    }
    return;
  }
  cerr << "armsfu2x2: parameter " << _name << " not understood\n";
}

void armsfu2x2::run() {
  // copy simit port variables to our ioval[]
  hook->sfu22_d1[deviceid] = ioval[2]->toulong();
  hook->sfu22_d2[deviceid] = ioval[3]->toulong();
  //  cerr << "slaverun 2x2\n";
  myarm->slaverun(this);
}

void armsfu2x2::out_run() {
  ioval[0]->assignulong(hook->sfu22_s1[deviceid]);
  ioval[1]->assignulong(hook->sfu22_s2[deviceid]);
}

bool armsfu2x2::checkterminal(int n, char *tname, aipblock::iodir d) {
  switch (n) {
  case 0:
    return (isoutput(d) && isname(tname, "d1"));
    break;
  case 1:
    return (isoutput(d) && isname(tname, "d2"));
    break;
  case 2:
    return (isinput(d) && isname(tname, "q1"));
    break;
  case 3:
    return (isinput(d) && isname(tname, "q2"));
    break;
  }
  return false;
}

bool armsfu2x2::cannotSleepTest() {
  bool v = interfacewritten;
  interfacewritten = false;
  return v;
}

bool armsfu2x2::needsWakeupTest() {
  bool v = interfacewritten;
  interfacewritten = false;
  if (!v) myarm->slaverun(this);
  return v;
}

void armsfu2x2::touch() {
  interfacewritten = true;
}

//--------------------------------------------------------------

armsfu3x1::armsfu3x1(char *name) : aipblock(name) {
  deviceid = 0;
  hook = 0;
  myarm = 0;
  interfacewritten = false;
  regOutput(0);  // d1, d2 and d3 are registered
  regOutput(1);
  regOutput(2);
}

void armsfu3x1::setparm(char *_name) {
  gval *v = make_gval(32,0);
  char *pname;
  
  if ((pname = matchparm(_name, "core"))) {
    if ((myarm = findsystem(pname, this))) {
      hook = myarm->sim;
      return;
    }
    cerr << "armsfu3x1: core " << pname << " not found\n";
    exit(0);
  } else if (matchparm(_name, "device", *v)) {
    if (hook) {
      if (v->toulong() < 2) {
	sfu3x1map[v->toulong()] = this;
	deviceid = v->toulong();
      }
    } else {
      cerr << "armsfu3x1: set core parameter before selecting device id\n";
      exit(0);
    }
    return;
  }
  cerr << "armsfu3x1: parameter " << _name << " not understood\n";
}

void armsfu3x1::run() {
  // copy simit port variables to our ioval[]
  hook->sfu31_d1[deviceid] = ioval[3]->toulong();
  //  cerr << "slaverun 3x1\n";
  myarm->slaverun(this);
}

void armsfu3x1::out_run() {
  ioval[0]->assignulong(hook->sfu31_s1[deviceid]);
  ioval[1]->assignulong(hook->sfu31_s2[deviceid]);
  ioval[2]->assignulong(hook->sfu31_s3[deviceid]);
}

bool armsfu3x1::checkterminal(int n, char *tname, aipblock::iodir d) {
  switch (n) {
  case 0:
    return (isoutput(d) && isname(tname, "d1"));
    break;
  case 1:
    return (isoutput(d) && isname(tname, "d2"));
    break;
  case 2:
    return (isoutput(d) && isname(tname, "d3"));
    break;
  case 3:
    return (isinput(d) && isname(tname, "q1"));
    break;
  }
  return false;
}

bool armsfu3x1::cannotSleepTest() {
  bool v = interfacewritten;
  interfacewritten = false;
  return v;
}

bool armsfu3x1::needsWakeupTest() {
  bool v = interfacewritten;
  interfacewritten = false;
  if (!v) myarm->slaverun(this);
  return v;
}

void armsfu3x1::touch() {
  interfacewritten = true;
}


//----------------------------------------------------------------
armsfu2x1::armsfu2x1(char *name) : aipblock(name) {
  deviceid = 0;
  hook = 0;
  myarm = 0;
  interfacewritten = false;
  regOutput(0);  // d1 and d2 are registered
  regOutput(1);
}

void armsfu2x1::setparm(char *_name) {
  gval *v = make_gval(32,0);
  char *pname;
  
  if ((pname = matchparm(_name, "core"))) {
    if ((myarm = findsystem(pname, this))) {
      hook = myarm->sim;
      return;
    }
    cerr << "armsfu2x1: core " << pname << " not found\n";
    exit(0);
  } else if (matchparm(_name, "device", *v)) {
    if (hook) {
      if (v->toulong() < 2) {
	sfu2x1map[v->toulong()] = this;
	deviceid = v->toulong();
      }
    } else {
      cerr << "armsfu2x1: set core parameter before selecting device id\n";
      exit(0);
    }
    return;
  }
  cerr << "armsfu2x1: parameter " << _name << " not understood\n";
}

void armsfu2x1::run() {
  // copy simit port variables to our ioval[]
  hook->sfu21_d1[deviceid] = ioval[2]->toulong();
  myarm->slaverun(this);
}

void armsfu2x1::out_run() {
  ioval[0]->assignulong(hook->sfu21_s1[deviceid]);
  ioval[1]->assignulong(hook->sfu21_s2[deviceid]);
}

bool armsfu2x1::checkterminal(int n, char *tname, aipblock::iodir d) {
  switch (n) {
  case 0:
    return (isoutput(d) && isname(tname, "d1"));
    break;
  case 1:
    return (isoutput(d) && isname(tname, "d2"));
    break;
  case 2:
    return (isinput(d) && isname(tname, "q1"));
    break;
  }
  return false;
}

bool armsfu2x1::cannotSleepTest() {
  bool v = interfacewritten;
  interfacewritten = false;
  return v;
}

bool armsfu2x1::needsWakeupTest() {
  bool v = interfacewritten;
  interfacewritten = false;
  if (!v) myarm->slaverun(this);
  return v;
}

void armsfu2x1::touch() {
  interfacewritten = true;
}


//----------------------------------------------------------------
// SFU write operation
void ext_write_sfu2x2(unsigned index) {
  if (sfu2x2map.find(index) != sfu2x2map.end()) {
    sfu2x2map[index]->touch();
  }
}

void ext_write_sfu2x1(unsigned index) {
  if (sfu2x1map.find(index) != sfu2x1map.end()) {
    sfu2x1map[index]->touch();
  }
}

void ext_write_sfu3x1(unsigned index) {
  if (sfu3x1map.find(index) != sfu3x1map.end()) {
    sfu3x1map[index]->touch();
  }
}

#endif  // SUPPORTSIMITSFU

void ext_show_clock() {
  cerr << "Cycle Count " << glbRTCycle << "\n";
}
