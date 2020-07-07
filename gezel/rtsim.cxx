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

#include "rtsim.h"
#include "rtlookup.h"
#include "rtbinop.h"
#include "rterror.h"
#include "rtsleep.h"
#include "rtopt.h"

rtsimgen::rtsimgen() {
  vcd = 0;
  if (glbVCDDebug) vcd = new vcddumper ("TRACE.vcd");

}

rtsimgen::~rtsimgen() {

  for (map<symid, rtdp *>::iterator i1 = dplist.begin();
       i1 != dplist.end();
       ++i1)
    if ((i1->second))
      delete ((i1->second));
	
  for (map<symid, artsignal *>::iterator i2 = rtobj.begin();
       i2 != rtobj.end();
       ++i2)
    if ((i2->second))
      delete ((i2->second));

  for (map<symid, rtdirective *>::iterator i3 = rtdisp.begin();
       i3 != rtdisp.end();
       ++i3)
    if ((i3->second))
      delete ((i3->second));

  for (map<symid, artctl *>::iterator i4 = ctllist.begin();
       i4 != ctllist.end();
       ++i4)
    if (i4->second)
      delete i4->second;

  for (map<symid, rtlookuptable *>::iterator i5 = rtlut.begin();
       i5 != rtlut.end();
       ++i5)
    if (i5->second)
      delete i5->second;

  for (map<symid, aipblock *>::iterator i6 = iplist.begin();
       i6 != iplist.end();
       ++i6)
    if (i6->second)
      delete i6->second;

  for (map<symid, tracerblock *>::iterator i7 = tracerlist.begin();
       i7 != tracerlist.end();
       ++i7)
    if (i7->second)
      delete i7->second;

}

artsignal *rtsimgen::rt(symid v) {
  if (rtobj.find(v) == rtobj.end()) {
    fdlerror(ERROR_INTERNAL_NO_RTCODE, v);
  }
  return rtobj[v];
}

rtdp *rtsimgen::dp(symid v) {
  if (dplist.find(v) == dplist.end()) {
    fdlerror(ERROR_INTERNAL_DP_UNKNOWN, v);
  }
  return dplist[v];
}


aipblock *rtsimgen::ip(symid v) {
  if (iplist.find(v) == iplist.end()) {
    fdlerror(ERROR_INTERNAL_IP_UNKNOWN, v);
  }
  return iplist[v];
}

extern "C" {
RTMODE glbRTSimMode = RTSimMode_active;
}

void rtsimgen::reset() {
  RTCycle       = 0;
  glbRTSimMode = RTSimMode_active;

  if (glbVCDDebug) vcd->eval(RTCycle);

  for (map<symid, artctl *>::iterator i00 = ctllist.begin();
       i00 != ctllist.end();
       ++i00)
    ((i00->second))->reset();
}

void rtsimgen::showstats(ostream &os) {
  if (glbRTProfile)
    glbRTProfile->report(os);
}

void rtsimgen::stepto(unsigned long n) {
  // this function can be called when RTCycle << n
  // it assumes the simulator will not change state
  // between the current RTCycle and n
  // The first possible cycle where such change can happen is
  // at RTCycle + n

  if (n > RTCycle)
    advance(n - RTCycle );
}

void rtsimgen::advanceto(unsigned long n) {
  // this function simulates n consecutive cycles,
  // where it cannot be assumed that the simulator will be
  // inactive
  unsigned i;
  if (n > RTCycle) {
    for (i=0; i < (n-RTCycle); i++)
      advance(1);
  }
}

void rtsimgen::step() {
  advance(1);
}

void rtsimgen::advance(unsigned long n) {

  RTCycle += n;

  if (glbRTDebug) {
    if (glbRTSimMode == RTSimMode_active) {
      cout << "> Cycle " << glbRTCycle+n << "\n";
    }
  }

  glbRTCycle = RTCycle;
  //  if (glbRTDebug)
  //    cout << "-- " << glbRTCycle << "\n";

  if (glbRTSimMode == RTSimMode_sleep) {
    // ---------------- sleep mode ---------------
    bool wakeup = false;

    // call ip blocks (for output-less ip blocks)
    for (map<symid,aipblock *>::iterator i11 = iplist.begin();
	 i11 != iplist.end();
	 ++i11)
      wakeup |= ((i11->second))->needsWakeupTest();
    
    if (wakeup) {
      glbRTSimMode = RTSimMode_active;
      if (glbRTDebug)
	cout << "*** INFO: Cycle " << RTCycle << ": FDL Wakeup\n";
    } else {
      glbRTSimMode = RTSimMode_sleep;
    }
  }

  else if (glbRTSimMode == RTSimMode_active) {
    // ---------------- active mode --------------
    glbRTSleep.reset();
    
    if (glbRTDebug)
      cout << "*** INFO: Cycle " << RTCycle << ": RTCTL\n";

    // set correct controllers
    for (map<symid, artctl *>::iterator i0 = ctllist.begin();
	 i0 != ctllist.end();
	 ++i0)
      ((i0->second))->run_nextstep(RTCycle);

    if (glbRTDebug)
      cout << "*** INFO: Cycle " << RTCycle << ": eval_out ip\n";

    // evaluated registered outputs of ipblocks
    for (map<symid,aipblock *>::iterator i00 = iplist.begin();
	 i00 != iplist.end();
	 ++i00)
      ((i00->second))->eval_out(RTCycle);

    if (glbRTDebug)
      cout << "*** INFO: Cycle " << RTCycle << ": eval\n";

    // eval
    for (map<symid,rtdp *>::iterator i1 = dplist.begin();
	 i1 != dplist.end();
	 ++i1)
      ((i1->second))->eval(RTCycle);
    
    if (glbRTDebug)
      cout << "*** INFO: Cycle " << RTCycle << ": eval ip\n";

    // call ip blocks (for output-less ip blocks)
    for (map<symid,aipblock *>::iterator i11 = iplist.begin();
	 i11 != iplist.end();
	 ++i11)
      ((i11->second))->eval(RTCycle);
    
    if (glbRTDebug)
      cout << "*** INFO: Cycle " << RTCycle << ": disp\n";

    // display & finish
    for (map<symid, rtdirective *>::iterator i3 = rtdisp.begin();
	 i3 != rtdisp.end();
	 ++i3) {
      if ((i3->second)->canshow()) 
	((i3->second))->show(cout,RTCycle);
    }
    
    // call tracer blocks
    for (map<symid, tracerblock *>::iterator i7 = tracerlist.begin();
	 i7 != tracerlist.end();
	 ++i7) {
      (i7->second)->eval(RTCycle);
    }
   
    // call vcd dump every clock cycle
    if (glbVCDDebug)  vcd->eval(RTCycle);
 
    // tick
    for (map<symid,rtdp *>::iterator i2 = dplist.begin();
	 i2 != dplist.end();
	 ++i2) {
      ((i2->second))->tick();
    }
    if (glbRTProfile)
      glbRTProfile->tick(cout, RTCycle);

    if (glbRTSleep.testCannotSleep()) {
      glbRTSimMode = RTSimMode_active;
    } else {
      if (glbAllowSimulatorSleep) {
	glbRTSimMode = RTSimMode_sleep;
	if (glbRTDebug)
	  cout << "*** INFO: Cycle " << RTCycle << ": FDL Sleep\n";
      }
    }
  }
}

void rtsimgen::run(long cycles) {
  reset();
  if (cycles < 0) {
    while (1) step();
  } else {
    for (long c=1; c <= cycles; ++c) step();
  }
}

void rtsimgen::cghwire_ctl (symid v, symid dp) {
  if (dplist.find(dp) == dplist.end()) {
    fdlerror(ERROR_INTERNAL_DP_NOT_IN_HWIRE_CTL, dp, v);
  }
  artctl *c = new seqrtctl(dplist[dp]->csfgset);
  ctllist[v] = c;
  c->program_nextstep(0, NOSYMBOL);
}

void rtsimgen::cgseq_ctl (symid v, symid dp) {
  if (dplist.find(dp) == dplist.end()) {
    fdlerror(ERROR_INTERNAL_DP_NOT_IN_SEQ_CTL, dp, v);
    exit(0);
  }
  artctl *c = new seqrtctl(dplist[dp]->csfgset);
  ctllist[v] = c;
}

void rtsimgen::cgfsm_ctl (symid v, symid dp) {
  if (dplist.find(dp) == dplist.end()) {
    fdlerror(ERROR_INTERNAL_DP_NOT_IN_FSM_CTL, dp, v);
    exit(0);
  }
  artctl *c = new fsmrtctl(dplist[dp]->csfgset, v);
  ctllist[v] = c;
}

void rtsimgen::cgfsm_state(symid v, symid c, char *str) {
  if (ctllist.find(c) == ctllist.end()) {
    fdlerror(ERROR_INTERNAL_CANNOT_ADD_STATE, v, c);
  }
  ctllist[c]->addstate(v);
}

void rtsimgen::cgfsm_initial(symid v, symid c, char *str) {
  if (ctllist.find(c) == ctllist.end()) {
    fdlerror(ERROR_INTERNAL_CANNOT_ADD_STATE, v, c);
  }
  ctllist[c]->addinitial(v);
}

void rtsimgen::cgctltrue(symid v, symid c, symid expr, symid parentexpr) {
  if (ctllist.find(c) == ctllist.end()) {
    fdlerror(ERROR_INTERNAL_CANNOT_ADD_CONDITION, v, c);
  }
  if (rtobj.find(expr) == rtobj.end()) {
    fdlerror(ERROR_INTERNAL_NO_CODE_FOR_CONDITION, v, expr);
  }
  ctllist[c]->program_truebranch(parentexpr, v, rtobj[expr]);
}

void rtsimgen::cgctlfalse(symid v, symid c, symid expr, symid parentexpr) {
  if (ctllist.find(c) == ctllist.end()) {
    fdlerror(ERROR_INTERNAL_CANNOT_ADD_CONDITION, v, c);
  }
  if (rtobj.find(expr) == rtobj.end()) {
    fdlerror(ERROR_INTERNAL_NO_CODE_FOR_CONDITION, v, expr);
  }
  ctllist[c]->program_falsebranch(parentexpr, v, rtobj[expr]);
}

void rtsimgen::cgfsm_trans  (symid v, symid c, symid from, symid to) {
  if (ctllist.find(c) == ctllist.end()) {
    fdlerror(ERROR_INTERNAL_CANNOT_ADD_TRANSITION, v, c);
  }
  ctllist[c]->program_transition(from, to);
}

void rtsimgen::cgctlstep (symid v, symid c, int n, symid condition) {
  if (ctllist.find(c) == ctllist.end()) {
    fdlerror(ERROR_INTERNAL_CANNOT_ADD_CTLSTEP, v, c);
  }
  ctllist[c]->program_nextstep(n, condition);
}

void rtsimgen::cgexec  (symid ctl, symid dp, symid sfg) {
  if (ctllist.find(ctl) == ctllist.end()) {
    fdlerror(ERROR_INTERNAL_CANNOT_FIND_CTL_CGEXEC, sfg, ctl);
  }
  ctllist[ctl]->addsfg(sfg);
}

void rtsimgen::cgtracer (symid ctl) {
  if (ctllist.find(ctl) == ctllist.end()) {
    fdlerror(ERROR_INTERNAL_CANNOT_FIND_CTL_CGTRACER, ctl);
  }
  ctllist[ctl]->addtracer();
}

void rtsimgen::cgtracesig (symid v, symid thesig, char *filename) {
  if (rtobj.find(thesig) == rtobj.end()) {
    fdlerror(ERROR_INTERNAL_TRACE_SIGNAL_NOT_FOUND, thesig);
  }

  for (map<symid, tracerblock *>::iterator i1 = tracerlist.begin();
       i1 != tracerlist.end();
       ++i1)
    if (i1->second->isdupfile(filename)) {
      fdlerror(ERROR_MULTIPLE_TRACES_NOT_SUPPORTED, v);
    }
  tracerlist[v] = new tracerblock(filename, rtobj[thesig], vcd);
}

void rtsimgen::cgfile  (symid v, char *filename) {
  rtfile *i = new rtfile(v, filename);
  rtobj[v] = i;
}

void rtsimgen::cgoption(symid v, char *option) {
  if (string(option)==string("debug")) {
    glbRTDebug = 1;
  } else if (string(option)==string("vcd")) {
    glbVCDDebug = 1;
    if (vcd == 0) {
      vcd = new vcddumper ("TRACE.vcd");
    }
  } else if (string(option)==string("profile_toggle_alledge_operations")) {
    cerr << "profile_toggle_alledge_operations no longer supported\n";
  } else if (string(option)==string("profile_toggle_alledge_cycles")) {
    cerr << "profile_toggle_alledge_cycles no longer supported\n";
  } else if (string(option)==string("profile_toggle_upedge_operations")) {
    cerr << "profile_toggle_upedge_operations no longer supported\n";
  } else if (string(option)==string("profile_toggle_upedge_cycles")) {
    cerr << "profile_toggle_upedge_cycles no longer supported\n";
  } else if (string(option)==string("profile_toggle_upedge")) {
    if (glbRTProfile) {
      cerr << "Option " << option << " ignored - only a single profiling option per simulation\n";
    } else {
      glbRTProfile = new rtprofile(true);
    }
  } else if (string(option)==string("profile_toggle_alledge")) {
    if (glbRTProfile) {
      cerr << "Option " << option << " ignored - only a single profiling option per simulation\n";
    } else {
      glbRTProfile = new rtprofile(false);
    }
  } else if (string(option)==string("profile_display_cycles")) {
    if (glbRTProfile) {
      glbRTProfile->set_cycle_dispmode(true);
    }
  } else if (string(option)==string("profile_display_operations")) {
    if (glbRTProfile) {
      glbRTProfile->set_operations_dispmode(true);
    }
  } else if (string(option).find("toggle_include") == 0) {
    string o = string(option);
    if (o.size() >= 16) {
      string dpid = o.substr(15, o.size()-16+1);
      if (glbRTProfile) {
	glbRTProfile->include(dpid);
      }
    }
  } else if (string(option).find("toggle_weights") == 0) {
    string o = string(option);
    if (o.size() >= 16) {
      string dpid = o.substr(15, o.size()-16+1);
      if (glbRTProfile) {
	glbRTProfile->toggleweights(dpid);
      }
    }                                    
  } else if (string(option).find("toggle_variation") == 0) {
    string o = string(option);
    if (o.size() >= 18) {
      string dpid = o.substr(17, o.size()-18+1);
      if (glbRTProfile) {
    	glbRTProfile->togglevariation(dpid);
      }
    }
  } else if (string(option).find("generic") == 0) {
    // this is for vhdl code gen
  } else {
    cerr << "rtsimgen: option " << option << " is unsupported\n";
  }
}

// pin1 is output, pin2 is input; cfr fdlsym.cc
void rtsimgen::cgcon   (symid pin1, symid pin2) {
  if (rtobj.find(pin2) == rtobj.end()) {
    fdlerror(ERROR_INTERNAL_PIN_IS_NOT_INPUT, pin2);
  }
  rtobj[pin2]->defdriver(rtobj[pin1]);
}

void rtsimgen::cgdpc_ipblock(symid v, symid parentdp) {

  if (iplist.find(v) == iplist.end()) {
      fdlerror(ERROR_USER_UNKNOWN_CHILD_IPBLOCK, v, parentdp);
  }

  if (dplist.find(parentdp) == dplist.end()) {
    // give child dp as context
    fdlerror(ERROR_USER_UNKNOWN_PARENT_DATAPATH, v, parentdp);
  }

  dp(parentdp)->includeip(ip(v));
}

void rtsimgen::cgdpc_dp(symid v, symid parentdp) {
  //  cerr << "cgdpc_dp for " << v << " parent " << parentdp << "\n";
  // add this dp hierarchically
  // rtdp::includedp(rtdp *p)
  // the inlcude flag is used lateron to turn on the 'present' flag
  // hiearchically

  if (dplist.find(v) == dplist.end()) {
    // give parent dp as context
      fdlerror(ERROR_USER_UNKNOWN_CHILD_DATAPATH, v, parentdp);
  }

  if (dplist.find(parentdp) == dplist.end()) {
    // give child dp as context
    fdlerror(ERROR_USER_UNKNOWN_PARENT_DATAPATH, v, parentdp);
  }

  dp(parentdp)->includedp(dp(v));
}

void rtsimgen::cgsysdp(symid v) {
  if (dplist.find(v) != dplist.end())
    dp(v)->present();
  else if (iplist.find(v) != iplist.end())
    ip(v)->present();
  else {
    fdlerror(ERROR_INTERNAL_CANNOT_FIND_DATAPATH, v);
  }
}

void rtsimgen::cgdpc_arg  (symid parentdp, symid parentpin, symid childdp, symid childpin) {
  if (dplist.find(childdp) == dplist.end()) {
    if (iplist.find(childdp) == iplist.end())
      fdlerror(ERROR_INTERNAL_CANNOT_FIND_DATAPATH, childdp);
  }
  if (dplist.find(parentdp) == dplist.end()) {
    fdlerror(ERROR_INTERNAL_CANNOT_FIND_DATAPATH, parentdp);
  }
  if (rtobj.find(parentpin) == rtobj.end()) {
    fdlerror(ERROR_INTERNAL_CANNOT_FIND_DATAPATH_PIN, parentpin);
  }
  if (rtobj.find(childpin) == rtobj.end()) {
    fdlerror(ERROR_INTERNAL_CANNOT_FIND_DATAPATH_PIN, childpin);
  }
  if (dplist.find(childdp) != dplist.end()) {
    if (dp(childdp)->hasinput(rtobj[childpin])) {
      rtobj[childpin]->defdriver(rtobj[parentpin]);
    } else {
      rtobj[parentpin]->defdriver(rtobj[childpin]);
    }
  } else {
    if (ip(childdp)->hasinput(rtobj[childpin])) {
      //      cerr << "$$$ connect " << rtobj[childpin] << " driven by " << rtobj[parentpin] << "\n";
      rtobj[childpin]->defdriver(rtobj[parentpin]);
    } else {
      //      cerr << "$$$ connect " << rtobj[parentpin] << " driven by " << rtobj[childpin] << "\n";
      rtobj[parentpin]->defdriver(rtobj[childpin]);
    }
  } 
}

void rtsimgen::cgdisp(symid disp, symid sfg, symid _dp) {
  rtdp *d = dp(_dp);
  rtdirective *t;
  if (rtdisp.find(disp) != rtdisp.end()) {
    t = rtdisp[disp];
  } else {
    t = new rtdisplay(disp, sfg, d);
    rtdisp[disp] = t;
  }
}

/* add for finish */
void rtsimgen::cgfinish(symid disp, symid sfg, symid _dp) {
	rtdp *d = dp(_dp);
	rtdirective *t;
	t = new rtfinish(disp, sfg, d);
	rtdisp[disp] = t;
}

void rtsimgen::cgdispbase (symid disp, int v) {
  if (rtdisp.find(disp) == rtdisp.end()) {
    fdlerror(ERROR_INTERNAL_UNKNOWN_DISPLAY_ID, disp);
  }
  rtdisp[disp]->changebase(v);
}

void rtsimgen::cgdispcycle(symid disp) {
  if (rtdisp.find(disp) == rtdisp.end()) {
    fdlerror(ERROR_INTERNAL_UNKNOWN_DISPLAY_ID, disp);
  }
  rtdisp[disp]->addcycle();
}

void rtsimgen::cgdisptoggle(symid disp) {
  if (rtdisp.find(disp) == rtdisp.end()) {
    fdlerror(ERROR_INTERNAL_UNKNOWN_DISPLAY_ID, disp);
  }
  rtdisp[disp]->addtoggle();
}

void rtsimgen::cgdispones(symid disp) {
  if (rtdisp.find(disp) == rtdisp.end()) {
    fdlerror(ERROR_INTERNAL_UNKNOWN_DISPLAY_ID, disp);
  }
  rtdisp[disp]->addones();
}

void rtsimgen::cgdispzeroes(symid disp) {
  if (rtdisp.find(disp) == rtdisp.end()) {
    fdlerror(ERROR_INTERNAL_UNKNOWN_DISPLAY_ID, disp);
  }
  rtdisp[disp]->addzeroes();
}

void rtsimgen::cgdispvar(symid disp, symid var) {
  if (rtdisp.find(disp) == rtdisp.end()) {
    fdlerror(ERROR_INTERNAL_UNKNOWN_DISPLAY_ID, disp);
  }
  if (rtobj.find(var) == rtobj.end()) {
    fdlerror(ERROR_INTERNAL_UNKNOWN_DISPLAY_VAR_ID, disp, var);
  }
  rtdisp[disp]->addvar(rtobj[var]);
}

void rtsimgen::cgdispdp(symid disp) {
  if (rtdisp.find(disp) == rtdisp.end()) {
    fdlerror(ERROR_INTERNAL_UNKNOWN_DISPLAY_ID, disp);
  }
  rtdisp[disp]->adddp();
}

void rtsimgen::cgdispstr(symid disp, char * var) {
  if (rtdisp.find(disp) == rtdisp.end()) {
    fdlerror(ERROR_INTERNAL_UNKNOWN_DISPLAY_ID, disp);
  }
  rtdisp[disp]->addstring(var);
}

// SFG/ SIGNAL
void rtsimgen::cgipblock(symid v, char *instname, char *tname) {
  if (glbIpblockcreate)
    iplist[v] = glbIpblockcreate(instname, tname);
  else {
    cerr << "** Error: rtsimgen: glbIpblockcreate not initialized\n";
    exit(0);
  }
}

void rtsimgen::cgdp    (symid v, char *n) {
  // provide datapath name to profiler
  // if datapath name is in excluded set,
  // no operations will be added to te profiler
  if (glbRTProfile)
    glbRTProfile->check_include(string(n));

  dplist[v] = new rtdp(v);  
}

void rtsimgen::cgdpclone (symid v, char *n, symid) {
  if (glbRTProfile)
    glbRTProfile->check_include(string(n));

  dplist[v] = new rtdp(v);  
}

void rtsimgen::cgsfg   (symid v, char *) {}

void rtsimgen::cginput (symid v, int wlen, int sign, symid _dp, char *portname) {
  rtinput *i = new rtinput(v, wlen, sign);
  rtobj[v] = i;

  if (dplist.find(_dp) != dplist.end()) {
    dplist[_dp]->appendinput(i);
  }

  if (iplist.find(_dp) != iplist.end()) {
    iplist[_dp]->addinput(i, portname, wlen, sign);
  }
}

void rtsimgen::cgipout(symid v, int wlen, int sign, symid _dp, char *name) {
  aipblock *p = ip(_dp);
  ipblockout *i = new ipblockout(v, p, wlen, sign);
  rtobj[v] = i;
  p->addoutput(i, name);
}

void rtsimgen::cgipparm(symid v, char *parmname, symid _dp) {
  aipblock *p = ip(_dp);
  p->setparm(parmname);
}

void rtsimgen::cgoutput(symid v, int wlen, int sign, symid _dp, char *portname) {
  rtdp * d = dp(_dp);
  rtoutput *i = new rtoutput(d->csfgset, v, wlen, sign);
  rtobj[v] = i;
  d->appendoutput(i);
}

void rtsimgen::cgreg   (symid v, int wlen, int sign, symid _dp, char *) {
  rtdp * d = dp(_dp);
  rtregister *i = new rtregister(d->csfgset, v, wlen, sign);
  rtobj[v] = i;
  d->appendregister(i);
}

void rtsimgen::cgsig   (symid v, int wlen, int sign, symid _dp, char *) {
  rtdp * d = dp(_dp);
  rtsignal *i = new rtsignal(d->csfgset, v, wlen, sign);
  rtobj[v] = i;
}

// #include <strstream>

void rtsimgen::cgconst (symid v, char *val) {
  rtconstant *i = new rtconstant(v, val);
  rtobj[v] = i;
}

void rtsimgen::cgassign(symid v, symid sfg, symid lhs, symid rhs) {
  rtassign *i = new rtassign(v, sfg, rt(lhs), rt(rhs));
  rtobj[v] = i;
}

void rtsimgen::cgindex(symid v, symid b, int idx, int len) {
  rtproxy *i = new rtproxy(v, rt(b), idx, len);
  rtobj[v] = i;
}

void rtsimgen::cglutable(symid v, int wlen, int sign, vector<char *> &content, symid _dp, char *) {
  rtlookuptable *t = new rtlookuptable(wlen, sign, content);
  rtlut[v] = t;
}

void rtsimgen::cgluop(symid v, symid L, symid table) {
  if (rtlut.find(table) == rtlut.end()) {
    fdlerror(ERROR_INTERNAL_LOOKUP_TABLE_UNKOWN);
  }
  rtlookup * i = new rtlookup(v, rt(L), rtlut[table]);
  rtobj[v] = i;
}

// OPERATIONS
void rtsimgen::cgadd   (symid v, symid L, symid R) {
  rtadd *i = new rtadd(v, rt(L), rt(R));
  rtobj[v] = i;
}

void rtsimgen::cgconcat (symid v, symid L, symid R) {
  rtconcat *i = new rtconcat(v, rt(L), rt(R));
  rtobj[v] = i;
}

void rtsimgen::cgsub   (symid v, symid L, symid R) {
  rtsub *i = new rtsub(v, rt(L), rt(R));
  rtobj[v] = i;
}

void rtsimgen::cgior   (symid v, symid L, symid R) {
  rtior *i = new rtior(v, rt(L), rt(R));
  rtobj[v] = i;
}

void rtsimgen::cgxor   (symid v, symid L, symid R) {
  rtxor *i = new rtxor(v, rt(L), rt(R));
  rtobj[v] = i;
}

void rtsimgen::cgand   (symid v, symid L, symid R) {
  rtand *i = new rtand(v, rt(L), rt(R));
  rtobj[v] = i;
}

void rtsimgen::cgshr   (symid v, symid L, symid R) {
  rtshr *i = new rtshr(v, rt(L), rt(R));
  rtobj[v] = i;
}

void rtsimgen::cgshl   (symid v, symid L, symid R) {
  rtshl *i = new rtshl(v, rt(L), rt(R));
  rtobj[v] = i;
}

void rtsimgen::cgmul   (symid v, symid L, symid R) {
  rtmul *i = new rtmul(v, rt(L), rt(R));
  rtobj[v] = i;
}

void rtsimgen::cgmod   (symid v, symid L, symid R) {
  rtmod *i = new rtmod(v, rt(L), rt(R));
  rtobj[v] = i;
}

void rtsimgen::cgeq(symid v, symid L, symid R) {
  rteq *i = new rteq(v, rt(L), rt(R));
  rtobj[v] = i;
}

void rtsimgen::cgne(symid v, symid L, symid R) {
  rtne *i = new rtne(v, rt(L), rt(R));
  rtobj[v] = i;
}

void rtsimgen::cgsmt(symid v, symid L, symid R) {
  rtsmt *i = new rtsmt(v, rt(L), rt(R));
  rtobj[v] = i;
}

void rtsimgen::cggrt(symid v, symid L, symid R) {
  rtgrt *i = new rtgrt(v, rt(L), rt(R));
  rtobj[v] = i;
}

void rtsimgen::cgsmteq(symid v, symid L, symid R) {
  rtsmteq *i = new rtsmteq(v, rt(L), rt(R));
  rtobj[v] = i;
}

void rtsimgen::cggrteq(symid v, symid L, symid R) {
  rtgrteq *i = new rtgrteq(v, rt(L), rt(R));
  rtobj[v] = i;
}

void rtsimgen::cgcast  (symid v, symid L, int wlen, int sign) {
  rtcast *i = new rtcast(v, wlen, sign, rt(L));
  rtobj[v] = i;
}

void rtsimgen::cgnot   (symid v, symid L) {
  rtnot *i = new rtnot(v, rt(L));
  rtobj[v] = i;
}

void rtsimgen::cgneg   (symid v, symid L) {
  rtneg *i = new rtneg(v, rt(L));
  rtobj[v] = i;
}

void rtsimgen::cgsel   (symid v, symid L, symid M, symid R) {
  rtsel *i = new rtsel(v, rt(L), rt(M), rt(R));
  rtobj[v] = i;
}

