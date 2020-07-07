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

#ifndef IPBLOCK_H
#define IPBLOCK_H

#include "gval.h"
#include "rtsignal.h"
#include "rtio.h"
#include "rttrace.h"

//-------------------------------------------------------------------------
//
// Operation of needsWakeupTest and cannotSleepTest
// After each run, cannotSleepTest is called and can return true/false
//    True: The block requires the next clock cycle to be simulated
//          because it either has an output that will change, or else
//          internal state that will change (like a counter).
//          By default, blocks can never sleep. The user must
//          explicitly overload cannotSleepTest and determine the
//          appropriate conditions under which false will be returned.
//    False: The block does not need simulation in the next clock cycle.
//           This is for example the case for purely combinatorial blocks
// When the simulator is in sleep mode, the simulator will call needsWakeupTest
// for each ip block in the simulation.
//    True: This clock cycle is a wakeup clock cycle, and the simulator must
//          transition to active mode.
//    False: This clock cycle is no wakeup cycle. By default, user blocks will
//          stay in sleep mode, and the needsWakeupTest needs to be explicitly
//          called and return true;
//
//-------------------------------------------------------------------------
//
// registered output support:
//
// aipblock can indicate certain outputs to be 'registered'
// this means that these outputs can be evaluate within a certain
// clock cycle without knowledge on the values of the input.
// The use of registered output allows loops between ipblocks,
// i.e. A->B and B->A with '->' a connection and A and B ipblocks
//
// When the user constructs an ipblock, the user must call:
// aipblock::outputregister(n), with n the number of the output
// that can be evaluated upfront.
// At runtime, before calling 'run' or 'eval', the system scheduler will
// call 'prepareregoutputs()', which will inturn call-back to
// the user ipblock with 'updateoutput(n)'
//
// The user will these see, each clock cycle, a call 'updateoutput()' for
// each call to outputregister() that the user has made.
//
//-------------------------------------------------------------------------

extern vector<aipblock *> glbIpblocks;

extern "C" {
  // ipblock creation function
  typedef aipblock * (*glbIpblockcreate_t)(char *instname, char *tname);
  extern glbIpblockcreate_t glbIpblockcreate;
}

class ipblockout;

class aipblock {

  set   <artsignal  *>           inputs;   // input signals
  set   <ipblockout *>           outputs;  // output signals
  map   <artsignal *, unsigned>  iomap;    // maps signal to IO port #
  map   <unsigned, ipblockout *> invomap;  // maps port # to output signals
  vector<ipblockout *> indep_out;          // stores independent outputs (i.e. those that
                                           // are known at the start of a clock cycle)
  set<unsigned> indep_out_todo;            // stores indices of calls to regOutput
                                           // when addoutput was not done yet
  long generation;
  bool ispresent;
  bool evalloop;

 public:
  vector<gval *> ioval;

 protected:
  char *name;
  enum iodir {input, output};
  bool isinput  (iodir d);
  bool isoutput (iodir d);
  bool isname   (char *, const char *n);
  char * matchparm(char *orig, const char *tgt);
  bool matchparm(char *orig, const char *tgt, gval &v);

 public:

  aipblock(char *_name);
  virtual ~aipblock();

  void addoutput   (ipblockout *, char *);
  void addinput    (artsignal *, char *, int, unsigned );
  bool hasinput    (artsignal  *);

  void regOutput   (unsigned n);    // called by used to indicated a reg'ed/ind. output
  void present     ();
  bool blockisname (char *n);

  void eval        (long g);  
  void eval_out    (unsigned long); // called by system scheduler

  //---------------- user-def -----------------
  virtual void run            ();
  virtual void out_run        ();
  virtual void setparm        (char *_name);
  virtual bool checkterminal  (int n, char *tname, iodir dir);
  virtual bool needsWakeupTest();
  virtual bool cannotSleepTest();
  virtual void write_device(int dev, unsigned long n);
  virtual unsigned long read_device(int dev);
  virtual void touch          ();
  // virtual void r_touch        (int ); -- replaced by read_device
  virtual void probe          (char *);
  virtual void stop           ();
  virtual void blockincluded  ();
};

typedef rtinput ipblockin;

class ipblockout : public artsignal {
  aipblock *owner;
  gval     *data;
 public:
  ipblockout(symid _id, aipblock *owner, int wlenreq, unsigned signreq);
  gval &    eval(unsigned long g);
  gval &    peekval();
  int       wlenreq();
  unsigned  signreq();
  void      updatedto(unsigned long);
};

#endif

