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
// $Id$
//--------------------------------------------------------------

#include "rtctl.h"
#include "rtbinop.h"
#include "rtunop.h"
#include "rterror.h"
#include <algorithm>
#include "rtsleep.h"
#include "rtopt.h"

//----------------------------------------------------
dpinstruction::dpinstruction() {}

void dpinstruction::addsfg(symid s) {
  ins.insert(s);
}

void dpinstruction::select(rtdpcontext &dp) {

  dp.clearall(); // current sfg
    
  for (set<symid>::iterator i = ins.begin(); i != ins.end(); ++i) {
    dp.activate(*i);
  }

}

void dpinstruction::show(ostream &os) {
  for (set<symid>::iterator i = ins.begin(); i != ins.end(); ++i)
    os << *i << " ";
  os << "\n";
}

//----------------------------------------------------
artctl::artctl(rtdpcontext &_dp) : dp(_dp) {}

//----------------------------------------------------
seqrtctl::seqrtctl(rtdpcontext &_dp) : artctl(_dp) {
  currentstep     = 0;
}

void seqrtctl::addsfg(symid sfg) {
  if (seqctl.size() == 0) {
    fdlerror(ERROR_INTERNAL_UNKNOWNCONTROLSTEP, __FILE__, __LINE__);
  }
  seqctl.back().addsfg(sfg);
}

void seqrtctl::addtracer() {
  tracer.insert(currentstep);
}


void seqrtctl::program_nextstep(int , symid ) { 
  dpinstruction T;
  seqctl.push_back(T);
}

void seqrtctl::run_nextstep(unsigned long) {

  dp.setctlstep(currentstep);
  seqctl[currentstep].select(dp);

  // for global sleep
  if (seqctl.size() > 1)
    glbRTSleep.cannotSleep();

  if (tracer.find(currentstep) != tracer.end()) {
    cout << "Sequencer step " << currentstep << "\n";
  }
  
  currentstep = (currentstep + 1) % ((unsigned) seqctl.size());
}

void seqrtctl::show(ostream &os) {
  os << "Controller content\n";
  int stepc = 0;
  for (vector<dpinstruction>::iterator j = seqctl.begin(); j != seqctl.end(); ++j) {
    os << "--- Step " << stepc++ << "\n";
    (*j).show(os);
  }
}

void seqrtctl::reset() {
  currentstep = 0;
}

//----------------------------------------------------
bool fsmtrans::take(unsigned long g, symid &tgt, int &ins) {
  tgt = tgtstate;
  ins = instructionid;
  if (condition) {
    // cout << "Condition " << condition->eval(g) << "\n";
    if (condition->eval(g) != *zero)
      return true;
    else
      return false;
  } else
    return true;
}

void fsmtrans::show(ostream &os) {
  os << "   target_state=" << tgtstate;
  os << " condition=" << condition;
  os << " ins=" << instructionid << "\n";
}

//----------------------------------------------------
fsmrtctl::fsmrtctl(rtdpcontext &_dp, symid _p) : artctl(_dp), parent(_p) {
  initialstate   = currentstate = NOSYMBOL;
  currentcondition = NOSYMBOL;
  currentctlstep   = 0;
}

void fsmrtctl::show(ostream &os) {
  os << "Fsm has " << (unsigned) states.size() << " states with id ";
  for (set<symid>::iterator i = states.begin(); i != states.end(); ++i)
    os << *i <<" ";
  os << "\n";
  os << "Initial state is id " << initialstate << "\n";
  for (map<symid, transtyp>::iterator k = transitions.begin(); k != transitions.end(); ++k) {
    os << "Transitions out of " << k->first << ":\n";
    for (transtyp::iterator j = (k->second).begin(); j != (k->second).end(); ++j) {
      (*j)->show(os);
      os << "\n";
    }
  }
  os << "Fsm has " << (unsigned) instructions.size() << " instructions\n";
  for (map<int,  dpinstruction>::iterator l = instructions.begin(); l != instructions.end(); ++l) {
    os << "Instruction id " << l->first << " contains following sfg: ";
    (l->second).show(os);
  }
}

fsmrtctl::~fsmrtctl() {
  for (set<artsignal *>::iterator i = newsigs.begin();
       i != newsigs.end();
       ++i)
    delete (*i);

  for (map<symid, transtyp>::iterator j = transitions.begin();
       j != transitions.end();
       ++j)
    for (vector<fsmtrans *>::iterator k = (j->second).begin();
	 k != (j->second).end();
	 ++k)
      delete *k;

}

void fsmrtctl::addsfg(symid s) {
  instructions[currentctlstep].addsfg(s);
}

void fsmrtctl::addtracer() {
  tracer.insert(currentctlstep);
}

void fsmrtctl::addstate(symid st) {
  states.insert(st);
}

void fsmrtctl::addinitial(symid st) {
  initialstate = st;
  states.insert(st);
}

void fsmrtctl::program_nextstep(int seq, symid cond) {
  currentctlstep   = seq;
  currentcondition = cond;
}

void fsmrtctl::reset() {
  if (initialstate == NOSYMBOL) {
    fdlerror(ERROR_USER_MISSING_INITIAL_STATE, parent);
  }
  currentstate = initialstate;
  prev_insid   = -1; // hold most recently executed instructions[] index
}

void fsmrtctl::program_truebranch(symid parent, symid t, artsignal *n) {
  artsignal *a;
  if (parent != NOSYMBOL)
    a = new rtand(NOSYMBOL,conditions[parent],n);
  else
    // this should be a 'pass' oepration
    a  = new rtproxy(NOSYMBOL, n, 0, 1);
  newsigs.insert(a);
  conditions[t] = a;
}

void fsmrtctl::program_falsebranch(symid parent, symid t, artsignal *n) {
  artsignal *s1, *a;
  if (parent != NOSYMBOL) {
    s1 = new rtnot(NOSYMBOL,n);
    newsigs.insert(s1);
    a = new rtand(NOSYMBOL,conditions[parent], s1);
  } else {
    // this should be a 'pass' oepration
    a = new rtnot(NOSYMBOL, n);
  }
  newsigs.insert(a);
  conditions[t] = a;
}

void fsmrtctl::program_transition(symid src, symid tgt) {
  // creates the state transition belonging to the
  // currentcondition and currentctlstep
  if (states.find(src) == states.end()) {
    // in the error message, we give the taget state as context symbol (since
    // the source state symbol does not exist
    fdlerror(ERROR_USER_NO_SOURCESTATE_FSM, parent, tgt);
  }
  if (states.find(tgt) == states.end()) {
    // in the error message, we give the source state as context symbol (since
    // the target state symbol does not exist
    fdlerror(ERROR_USER_NO_TARGETSTATE_FSM, parent, src);
  }
  // create transition
  fsmtrans *p;
  if (currentcondition == NOSYMBOL) {
    p = new fsmtrans(tgt,0, currentctlstep);
  } else {
    if (conditions.find(currentcondition) == conditions.end()) {
      fdlerror(ERROR_INTERNAL_NO_CURRENT_CONDITION, __FILE__, __LINE__);
    }
    p = new fsmtrans(tgt, conditions[currentcondition], currentctlstep);
  }
  transitions[src].push_back(p);
}

void fsmrtctl::run_nextstep(unsigned long g) {
  if (transitions.find(currentstate) == transitions.end()) {
    fdlerror(ERROR_USER_NO_TRANSITIONS, currentstate);
  }

  // cout << "currentstate: " << currentstate << "\n";
  for (vector<fsmtrans *>::iterator i = transitions[currentstate].begin();
       i != transitions[currentstate].end();
       ++i) {
    symid tgtstate;
    int   insid;
    if ((*i)->take(g, tgtstate, insid)) {

      if (instructions.find(insid) == instructions.end()) {
	// DOES NOT SEEM TO TRIGGER FOR UNKNOWN INSTRUCTIONS ??
	fdlerror(ERROR_USER_UNKNOWN_INSTRUCTION, currentstate);
      }

      dp.setctlstep(insid);
      instructions[insid].select(dp); // switch dpcontext to execute sfg in this instruction

      // trace facility
      if (tracer.find(insid) != tracer.end()) {
	glbSymboltable.showlex(cout, parent);
	cout << ": ";
	glbSymboltable.showlex(cout, currentstate);
	cout << " -> ";
	glbSymboltable.showlex(cout, tgtstate);
	cout << "\n";
      }

      // debug facility (need to integrate with trace)
      if (glbRTDebug) {
	glbSymboltable.showlex(cout, parent);
	cout << ": ";
	glbSymboltable.showlex(cout, currentstate);
	cout << " -> ";
	glbSymboltable.showlex(cout, tgtstate);
	cout << "\n";
      }

      // for global sleep
      if (insid != prev_insid) {
	//	cerr << "fsm ticks from " << currentstate << " to " << tgtstate << "\n";
	glbRTSleep.cannotSleep();
      }

      currentstate = tgtstate;
      prev_insid = insid;
      return;
    }
  }
}


