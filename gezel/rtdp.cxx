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

#include "rtdp.h"
#include "rtopt.h"
#include "rterror.h"
#include "rtio.h"
#include "ipblock.h"
#include <algorithm>


rtdpcontext::rtdpcontext() {
  emptyflag = false;
  multipleflag = false;
  xsect = NOSYMBOL;
}

void rtdpcontext::activate(symid s) {
  activesfg.insert(s);
}

bool rtdpcontext::isactivesfg(symid sfg) {
  return (activesfg.find(sfg) != activesfg.end());
}

void rtdpcontext::clearall() {
  activesfg.clear();
}

bool rtdpcontext::empty() {
  return emptyflag;
}

symid rtdpcontext::single() {
  return xsect;
}

bool rtdpcontext::multiple() {
  return multipleflag;
}

void rtdpcontext::lookup(set<symid> &sigsfg) {
  emptyflag = false;
  multipleflag = false;
  xsect = NOSYMBOL;

  // for all of the active sfg, look which are also
  // defining a signal (sfg present in sigsfg)
  // This should be exactly one. If zero or more then one,
  // return NOSYMBOL as an error marker. The rtdpcontext
  // object can then be queried witht the reportconflicts
  // function
  // The multiple flag is used to indicate multiple defs are
  // available
  set<symid> xsectset;
  set_intersection(activesfg.begin(), activesfg.end(),
		   sigsfg.begin(), sigsfg.end(),
		   inserter(xsectset, xsectset.begin()));
  if (xsectset.size() == 0)
    emptyflag = true;
  else if (xsectset.size() > 1)
    multipleflag = true;
  else // size is 1
    xsect = *(xsectset.begin());
}

void rtdpcontext::reportconflicts(ostream &os, symid v, set<symid> &sigsfg) {
  set<symid> xsect;
  set_intersection(activesfg.begin(), activesfg.end(),
		   sigsfg.begin(), sigsfg.end(),
		   inserter(xsect, xsect.begin()));
  if (xsect.size() > 0) {
    fdlerror(ERROR_USER_MULTIPLE_ASSIGNMENT_MULTIPLE_SFG, v, xsect);
  } else if (xsect.size() == 0) {
    fdlerror(ERROR_USER_MISSING_ASSINGMENT, v);
  }
}

void rtdpcontext::setctlstep(int m) {
  currentctlstep = m;
}

int rtdpcontext::getctlstep() {
  return currentctlstep;
}

//----------------------------------------------------
rtsigcontext::rtsigcontext(rtdpcontext &_dpc, symid _lhssig) : 
  dpc(_dpc), lhssig(_lhssig)  {
#ifdef RTSIGDEFCACHE
  int i;
  for (i=0; i<32; ++i) {
    defcache_ctlstep[i] = -1;
    defcache_signal[i]  = 0;
  }
#endif
}

void rtsigcontext::newdef(symid _sfg, artsignal * _definer) {
  if (defsfg.find(_sfg) != defsfg.end()) {
    fdlerror(ERROR_USER_MULTIPLE_ASSIGNMENT_SAME_SFG, lhssig, _sfg);
  } else {
    defsfg.insert(_sfg);
    definitions[_sfg] = _definer;
  }
}

artsignal * rtsigcontext::curdef() {  

#ifdef RTSIGDEFCACHE
  int k = dpc.getctlstep();

  if (defcache_ctlstep[k & 0x1f] != k) {
    // definer is not in cache. Do a class lookup
    // by cross-sectioning the set of sfg that define this signal
    // with the set of currently active sfg in the datapath context dpc
    dpc.lookup(defsfg);
    if (dpc.empty() || dpc.multiple()) {
      dpc.reportconflicts(cout, lhssig, defsfg);
      return (0); // reportconflicts() terminates
    } else {
      artsignal *definer = definitions[dpc.single()];
      defcache_signal [k & 0x1f] = definer;
      defcache_ctlstep[k & 0x1f] = dpc.getctlstep();
      return definer; 
    }
  } else {
    // definer is available in cache
    return defcache_signal [k & 0x1f];
  }

#else // RTSIGDEFCACHE

  dpc.lookup(defsfg);
  if (dpc.empty() || dpc.multiple()) {
    dpc.reportconflicts(cout, lhssig, defsfg);
    return (0); // reportconflicts() terminates
  } else {
    artsignal *definer = definitions[dpc.single()];
    return definer; 
  }

#endif // RTSIGDEFCACHE

}

bool rtsigcontext::hasdef() {

#ifdef RTSIGDEFCACHE
  int k = dpc.getctlstep();

  if (defcache_ctlstep[k & 0x1f] != k) {

    dpc.lookup(defsfg);

    if (dpc.multiple()) {
      dpc.reportconflicts(cout, lhssig, defsfg);
      return false;
    }

    if (dpc.empty())
      return false;

    artsignal *definer = definitions[dpc.single()];
    defcache_signal [k & 0x1f] = definer;
    defcache_ctlstep[k & 0x1f] = dpc.getctlstep();

    return true;

  } else {
    // definer is in cache
    return true;
  }

#else // RTSIGDEFCACHE

  dpc.lookup(defsfg);
  
  if (dpc.multiple()) {
    dpc.reportconflicts(cout, lhssig, defsfg);
    return false;
  }
  
  if (dpc.empty())
    return false;
  
  return true;

#endif // RTSIGDEFCACHE

}

//----------------------------------------------------
rtdp::rtdp(symid v) {
  insystem = false;
  id       = v;
}

void rtdp::present() {
  // turn on present flag for this dp and all hierarchically included ones
  insystem = true;
  for (vector<rtdp *>::iterator i = incdp.begin();
       i != incdp.end();
       ++i) 
    (*i)->present();
  for (vector<aipblock *>::iterator j = incip.begin();
       j != incip.end();
       ++j) 
    (*j)->present();
}

bool rtdp::ispresent() {
  return insystem;
}

void rtdp::appendoutput(rtoutput *v) {
  outputs.push_back(v);
}

void rtdp::includedp(rtdp *p) {
  incdp.push_back(p);
}

void rtdp::includeip(aipblock *p) {
  incip.push_back(p);
}

void rtdp::appendinput(rtinput *v) {
  inputs.push_back(v);
}

bool rtdp::hasinput(artsignal *v) {
  for (vector<rtinput *>::iterator i = inputs.begin();
       i != inputs.end();
       ++i) 
    if ((artsignal *) *i == v)
      return true;
  return false;
}

void rtdp::appendregister(rtregister *v) {
  regs.push_back(v);
}


void rtdp::eval(unsigned long g) {

  if (! insystem)
    return;

#ifdef RTACTIVITY
  glbRTDPEval++;
#endif

  // normal simulation
  for (vector<rtoutput *>::iterator qq = outputs.begin();
       qq != outputs.end();
       ++qq)
    (*qq)->eval(g);
  for (vector<rtregister *>::iterator i = regs.begin();
       i != regs.end();
       ++i) {
    (*i)->updatein(g);
  }
  
}

void rtdp::tick() {
  if (insystem) {
    
    for (vector<rtregister *>::iterator i = regs.begin();
	 i != regs.end();
	 ++i) {
      (*i)->tick();
    }
    
  }
}

bool rtdp::isactivesfg(symid sfg) {
  return csfgset.isactivesfg(sfg);
}






