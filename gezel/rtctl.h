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

#ifndef RTCTL_H
#define RTCTL_H

#include "fdlsym.h"
#include "rtsignal.h"
#include "rtdp.h"
#include "rtio.h"
#include "rtopt.h"
#include <set>

// #####  ####### #     # ####### ######  ####### #       #       ####### ######
//#     # #     # ##    #    #    #     # #     # #       #       #       #     #
//#       #     # # #   #    #    #     # #     # #       #       #       #     #
//#       #     # #  #  #    #    ######  #     # #       #       #####   ######
//#       #     # #   # #    #    #   #   #     # #       #       #       #   #
//#     # #     # #    ##    #    #    #  #     # #       #       #       #    #
// #####  ####### #     #    #    #     # ####### ####### ####### ####### #     #

//--------------------------------------------------------------------------
// Simulation principle (above signal level)
//
// Each signal has a context (rtsigcontext) that decides which expressions
// are valid for updates of this signal in the current clock cycle.
//
// An rtsigcontext contains a list of sfg that provide these expressions
// as well as a reference to each of those expressions.
// In order to know which signal should be chosen, the rtsigcontext
// relies on information provided by an rtdpcontext
//
// rtdpcontext contains a list of sfg that are active within a particular
// clock cycle. 
//
// The set of instructions that are executed for one particular clock
// cycle is kept in dpinstruction. At the moment that this set must
// be executed, the appropriate sfg of an rtdpcontext are 'activated'
// by this object.
//
// Various types of controller objects are used to sequence
// dpinstructions in order to establish a cycle schedule.
//
//--------------------------------------------------------------------------


// DPINSTRUCTION manages a set of sfg that represent a datapath
// instruction. At the moment of execution, a call to
// seclect() is used to insert that set of sfg into
// a dpcontext

class dpinstruction {  //--> should share this one with seqctl
  set<symid> ins;
 public:
  dpinstruction();
  void addsfg(symid s);
  void select(rtdpcontext &dp);
  void show(ostream &os);
};

//------------------------------------------------
class artctl {
 protected:
  rtdpcontext &dp;
 public:
  artctl(rtdpcontext &);
  virtual ~artctl() {}

  virtual void addsfg(symid sfg)  {}
  virtual void addtracer()  {}
  virtual void program_nextstep(int seq, symid cond) {}
  virtual void run_nextstep(unsigned long) {}
  virtual void show(ostream &os) {}

  virtual void addstate(symid st) {}
  virtual void addinitial(symid st) {}
  virtual void program_truebranch(symid parent, symid t, artsignal *) {}
  virtual void program_falsebranch(symid parent, symid t, artsignal *) {}
  virtual void program_transition (symid src, symid tgt) {}
  virtual void reset() {}

};

//------------------------------------------------
//
// RTSEQCTL
// hardwired or sequencer control
// 
class seqrtctl : public artctl {
  int currentstep;
  vector<dpinstruction> seqctl;
  set<int>              tracer;
 public:
  seqrtctl(rtdpcontext &);
  ~seqrtctl() {}
  void addsfg(symid sfg);   // called for sfgexec symbols
  void addtracer();
  void program_nextstep(int seq, symid cond);  // called when a ctlstep symbol is encountered
  void run_nextstep(unsigned long);      // called during simulation
  void reset();
  void show(ostream &os);
};

//------------------------------------------------
//
// RTFSMCTL
// FSM control
// 

class fsmtrans {
  symid          tgtstate;
  artsignal     *condition;
  int            instructionid;
  gval          *zero;
 public:
  fsmtrans(symid _t, artsignal *_c, int _instructionid) :
    tgtstate(_t), condition(_c), instructionid(_instructionid) {
    zero = make_gval("0");
  }
  bool take(unsigned long g, symid &tgt, int &ins);
  void show(ostream &os);
};

class fsmrtctl : public artctl {
  symid parent;            // original fsm symbol

  symid initialstate;      // initial state
  symid currentstate;      // runtime info
  
  symid currentcondition;  // construction context info
  int   currentctlstep;    // construction context info

  int   prev_insid;        // most recent instruction entry out of instructions[]
  set<symid>                 states;
  typedef vector<fsmtrans *> transtyp;
  map<symid, transtyp>       transitions; // map address by source state
  map<int, dpinstruction>    instructions;
  map<symid, artsignal *>    conditions;
  set<artsignal *>           newsigs;
  set<int>                   tracer;

 public:

  fsmrtctl(rtdpcontext &, symid _parent);
  ~fsmrtctl();
  void addsfg             (symid sfg);   // called for sfgexec symbols
  void addtracer          ();
  void addstate           (symid st);
  void addinitial         (symid st);
  void program_nextstep   (int seq, symid cond);  // when a ctlstep symbol is encountered
  void program_truebranch (symid parent, symid t, artsignal *); 
                                                  // when ctl_true is encountered 
  void program_falsebranch(symid parent, symid t, artsignal *);  
                                                  // when ctl_false is encountered
  void program_transition (symid src, symid tgt);
  void reset              ();
  void run_nextstep       (unsigned long);      // called during simulation
  void show(ostream &os);
};

#endif
