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

#include "rtio.h"
#include "rtctl.h"
#include "rterror.h"
#include "rtsleep.h"
#include "rtopt.h"
#include <cstdlib>

//----------------------------------------------------
rtoutput::rtoutput(rtdpcontext &_rtdpc, symid _id, int wlen, int sign) :
  artsignal(_id),
  s(_rtdpc, _id, wlen, sign) {
  if (glbRTProfile)
    glbRTProfile->makeop(_id);
}

void rtoutput::newdef(symid _sfg, artsignal * _definer) {
  s.newdef(_sfg, _definer);
}

void rtoutput::defdriver(artsignal* _driver) {
  s.defdriver(_driver);
}

gval & rtoutput::eval(unsigned long g) {
  //  cerr << "out_eval\n";
  gval & rv = s.eval(g);
  return rv;
}

void rtoutput::asgn(gval &nv) {
  //  cerr << "out_asgn\n";
  s.asgn(nv);
}
 
gval& rtoutput::peekval() {
  cerr << "out_peekval\n";
  return s.peekval();
}

int rtoutput::wlenreq() {
  return s.wlenreq();
}

unsigned rtoutput::signreq() {
  return s.signreq();
}

void rtoutput::show(ostream &os) {
  s.show(os);
}

bool rtoutput::isgeneration(unsigned long generation) {
  return s.isgeneration(generation);
}

void rtoutput::setbase(int base) {
  s.setbase(base);
}

//----------------------------------------------------
rtfile::rtfile(symid _id, char *filename) :
  artsignal(_id),
  is(filename) {
  v = make_gval(128,1);// LIMIT OF RTFILE: 128 BIT NUMBERS
  if (is.fail()) {
    fdlerror(ERROR_USER_COULD_NOT_OPEN_FILE, filename);
  }
}

gval & rtfile::eval(unsigned long g) {

  if (generation != g) {
    if (is.fail()) {
      fdlerror(ERROR_USER_FAIL_ON_INPUT);
    }
    is >> *v;
    generation = g;
  }

  // this implies that, as soon as we use an rtfile,
  // we will never be able to go into sleep mode
  // (safe and pessimistic)
  glbRTSleep.cannotSleep();

  return *v;
}

gval &rtfile::peekval() {
  return *v;
}

//----------------------------------------------------
rtinput::rtinput(symid _id, int wlen, int sign) :
  artsignal(_id) {
  driver = 0;
  v = make_gval(wlen, sign);
}

void rtinput::defdriver(artsignal *p) {
  driver = p;
}

gval & rtinput::eval(unsigned long g) {
  if (driver) {
    generation = g;
    *v = driver->eval(g);
  } else {
    fdlerror(ERROR_USER_SIGNAL_HAS_NO_DRIVER, id);
  }
  return *v;
}

gval &rtinput::peekval() {
  return *v;
}

int rtinput::wlenreq() {
  return v->getwordlength();
}

unsigned rtinput::signreq() {
  return v->getsign();
}

void rtinput::show(ostream &os) {
  os << peekval();
}

void rtinput::setbase(int base) {
  v->setbase(base);
}

bool rtinput::isgeneration(unsigned long g) {
  return (generation == g); 
}

//bool rtinput::usleep_m() {
//  // the 'microsleep_modified' function is determined by 
//  // the output that drives this input.
//  // if the output is modified, then this input will be
//  // modified as well - and this function will return true
//  if (driver) 
//    return driver->usleep_m();
//  else 
//    fdlerror(ERROR_USER_SIGNAL_HAS_NO_DRIVER, id);
//  return true;
//}

//----------------------------------------------------
rtsigdisp::rtsigdisp(artsignal *_s) :
  s(_s) {}

void rtsigdisp::setbase(int base) {
  s->setbase(base);
}

void rtsigdisp::show(ostream &os, unsigned long g) {
  if (s->isgeneration(g)) {
    //    os << "show for " << g << "\n";
    s->show(os);
  } else {
    //    os << "eval and show for " << g << "\n";
    s->eval(g);
    s->show(os);
  }

  //    os << "<not current value: ";
  //    s->dbginfo(os); os << ">";
  
}

rtstrdisp::rtstrdisp(char *_s) :
  s(_s) {}

void rtstrdisp::show(ostream &os, unsigned long) {
  os << s;
}

//------------------------------------------------------
rtcycledisp::rtcycledisp() {}

void rtcycledisp::show(ostream &os, unsigned long g) {
  os << g-1; // display durrent generation, not target generation
}

//------------------------------------------------------
rttoggledisp::rttoggledisp() {}

void rttoggledisp::show(ostream &os, unsigned long g) {
  if (glbRTProfile)
    os << glbRTProfile->togglecount(g);
  else
    os << "NA";
}

//------------------------------------------------------
rtonesdisp::rtonesdisp() {}

void rtonesdisp::show(ostream &os, unsigned long g) {
  if (glbRTProfile)
    os << glbRTProfile->onescount(g);
  else
    os << "NA";
}

//------------------------------------------------------
rtzeroesdisp::rtzeroesdisp() {}

void rtzeroesdisp::show(ostream &os, unsigned long g) {
  if (glbRTProfile)
    os << glbRTProfile->zeroescount(g);
  else
    os << "NA";
}

//------------------------------------------------------
rtdpdisp::rtdpdisp(rtdp *_dp) {
  dp = _dp;
}

void rtdpdisp::show(ostream &os, unsigned long) {
  glbSymboltable.getsymbol(dp->getid())->content()->show(os);
}

//------------------------------------------------------
rtdisplay::rtdisplay(symid _id, symid _sfg, rtdp *_dp) : 
  id(_id), sfg(_sfg), dp(_dp), defaultbase(16) {}

rtdisplay::~rtdisplay() {
  for (vector<rtadisp *>::iterator i = displaylist.begin();
       i != displaylist.end();
       ++i) {
    if (*i)
      delete (*i);
  }
}

void rtdisplay::addvar(artsignal *s) {
  displaylist.push_back(new rtsigdisp(s));
  currentbase.push_back(defaultbase);
}

void rtdisplay::addstring(char *s) {
  displaylist.push_back(new rtstrdisp(s));
  currentbase.push_back(defaultbase);
}

void rtdisplay::changebase(int v) {
  defaultbase = v;
}

void rtdisplay::addcycle() {
  displaylist.push_back(new rtcycledisp());
  currentbase.push_back(defaultbase);
}

void rtdisplay::addtoggle() {
  displaylist.push_back(new rttoggledisp());
  currentbase.push_back(defaultbase);
}

void rtdisplay::addones() {
  displaylist.push_back(new rtonesdisp());
  currentbase.push_back(defaultbase);
}

void rtdisplay::addzeroes() {
  displaylist.push_back(new rtzeroesdisp());
  currentbase.push_back(defaultbase);
}

void rtdisplay::adddp() {
  displaylist.push_back(new rtdpdisp(dp));
  currentbase.push_back(defaultbase);
}

bool rtdisplay::canshow() {
  if (dp->ispresent() && dp->isactivesfg(sfg))
    return true;
  return false;
}

void rtdisplay::show(ostream &os, unsigned long g) {
  vector<int>::iterator basep = currentbase.begin();
  for (vector<rtadisp *>::iterator i = displaylist.begin();
       i != displaylist.end();
       ++i) {
    (*i)->setbase(*basep);
    ++basep;
    (*i)->show(os,g);
  }
  os << "\n";
}

//------------------------------------------------------
rtfinish::rtfinish(symid _id, symid _sfg, rtdp *_dp) : 
  id(_id), sfg(_sfg), dp(_dp) {}

rtfinish::~rtfinish() {
  //   delete (dp);
}

void rtfinish::show(ostream &os, unsigned long g) {
	os<<"finish reached !"<<endl;
	exit(0);
}

bool rtfinish::canshow() {
	if (dp->ispresent() && dp->isactivesfg(sfg))
		return true;
	return false;
}
