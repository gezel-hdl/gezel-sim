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

#ifndef RTDP_H
#define RTDP_H

#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include "artsignal.h"

//------------------------------------------------
// RTDPCONTEXT is an object that manages which SFG are active in a
// particular clock cycle of operation The member functions activate
// and deactivate are used to turn on and off sfg At any time, a
// signal can query the rtdpcontext to check if any of

class rtdpcontext {
  set<symid> activesfg;
  bool emptyflag, multipleflag;
  symid xsect;

  int            currentctlstep;

 public:
  rtdpcontext();
  void  activate(symid s);
  void  clearall();
  bool  empty();
  bool  multiple();
  symid single();
  void  lookup(set<symid> &);
  void  reportconflicts(ostream &os, symid thissig, set<symid> &);
  bool  isactivesfg(symid sfg);
  void  setctlstep(int);
  int   getctlstep();
};

//------------------------------------------------
// RTSIGCONTEXT is an object that manages assignments to a signal
// within one sfg. Given a current RTDPCONTEXT it returns the proper
// defining expression for the signal If multiple definitions or no
// definition is available at a particular clock cycle, an error is
// signalled.  Otherwise, a ptr to the definit artsignal is returned.

class rtsigcontext {
  rtdpcontext &dpc;
  symid      lhssig;
  set<symid> defsfg;
  map<symid, artsignal *> definitions;


  // definer cache keeps track of signal definitions depending on control step
  int        defcache_ctlstep[32]; // do not change size without adapting curdef/hasdef !!
  artsignal *defcache_signal[32];  // do not change size without adapting curdef/hasdef !!

 public:
  rtsigcontext(rtdpcontext &_dpc, symid _lhssig);
  void        newdef(symid _sfg, artsignal * _definer);
  artsignal * curdef();
  bool        hasdef();
};

//------------------------------------------------
//
// RTDP
// top level object that contorls the execution of
// a set of outputs and registers.
// In addition an RTDP maintains a 'currentsfg' variable
// that can be used by rt-assignment statements to identify
// which sfg is under execution

class rtoutput;
class rtinput;
class rtregister;
class aipblock;

class rtdp {
  vector<rtoutput   *>    outputs;
  vector<rtinput    *>    inputs;
  vector<rtregister *>    regs;
  vector<rtdp       *>    incdp;
  vector<aipblock   *>    incip;
  bool                    insystem;
  symid                   id;
 public:
  rtdp(symid d);
  void appendoutput  (rtoutput   *);
  void appendinput   (rtinput    *);
  void includedp     (rtdp       *);
  void includeip     (aipblock   *);
  void appendregister(rtregister *);
  bool hasinput      (artsignal  *);

  symid getid() { return id; }
  void  present();
  bool  ispresent();

  rtdpcontext csfgset;
  void  eval(unsigned long);
  void  tick();
  bool  isactivesfg(symid sfg);

};

#endif





