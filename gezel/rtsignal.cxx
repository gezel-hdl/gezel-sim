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

#include "rtsignal.h"
#include <algorithm>
#include "rtunop.h" // to create conditions
#include "rtbinop.h" 
#include "rterror.h"
#include "rtsleep.h"
#include <iomanip> // setw

int max(int v1, int v2) {
  return (v1 > v2) ? v1 : v2;
}

//----------------------------------------------------
// rtsignals can be defined (driven) in two ways:
// - inside of an sfg, where a mechanism of contexts is used
// - by using a 'driver' in which case always the same signal/expression is
//   used to define this signal. this situation occurs when using structural
//   hierarchy (where the outputs of an included block) drive signals
//   in the upper level.
rtsignal::rtsignal(rtdpcontext & _rtdpc,
		   symid _id, 
		   int wlen, 
		   int sign) :
  artsignal(_id), 
  context  (_rtdpc,  _id),
  driver   (0) {
  v = make_gval( wlen , sign);
  if (glbRTProfile)
    glbRTProfile->makeop(_id);
}

void rtsignal::newdef(symid _sfg, artsignal * _definer) {
  if (driver) {
    // this signal is already driven, no sfg definitions can be done
    fdlerror(ERROR_USER_DRIVEN_SIGNAL_REASSIGNED, id, _sfg);
  }
  context.newdef(_sfg, _definer);
}

void rtsignal::defdriver(artsignal * _driver) {
  driver = _driver;
}

gval & rtsignal::eval(unsigned long target_generation) {

  if (driver) {
    // this is a driven signal, for which no assignments inside of an sfg (context)
    // can be done.
    //    cerr << (artsignal *) this << " isdrivenby " << (artsignal *) driver << "\n";

    bool profile = false;
    if (generation != target_generation)
      profile = true;

    generation = target_generation;
    *v = driver->eval(target_generation);

    if (glbRTProfile && profile)
      glbRTProfile->evalop(id, v);
  }

  if ((currentstate == STABLE) && (generation != target_generation)) {
    // first-time visit
    currentstate = EVALUATION_CHECK;
    *v = context.curdef()->eval(target_generation);
    generation = target_generation; // can take arbitrary jumps
    currentstate = STABLE;
  }

  if ((currentstate == EVALUATION_CHECK) && (generation != target_generation)) {
    currentstate = EVALUATION;
    fdlwarn(ERROR_USER_COMBINATORIAL_LOOP, id);
    *v = context.curdef()->eval(target_generation);
  }

  if ((currentstate == EVALUATION) && (generation != target_generation)) {
    // second-time visit - must be combinational dependency
    fdlerror(ERROR_USER_COMBINATORIAL_LOOP, id);
  }
  
  return *v;
}

void rtsignal::asgn(gval &nv) {

  //  cerr << "asgn rhs wl " << nv.getwordlength() << " sgn ";
  //  cerr << nv.getsign() << " value ";  nv.writestream(cerr);
  //  cerr << " lhs WL " << v->getwordlength() << " sgn ";
  //  cerr << v->getsign() << " value "; v->writestream(cerr);
  //  cerr << "\n";

  *v = nv;

  if (glbRTProfile)
    glbRTProfile->evalop(id, v);

}

gval& rtsignal::peekval() {
  return *v;
}

int rtsignal::wlenreq() {
  return v->getwordlength(); 
}

unsigned rtsignal::signreq() {
  return v->getsign(); 
}

bool rtsignal::hasdef() {
  return context.hasdef();
}

void rtsignal::show(ostream &os) {
  os << peekval();
}

void rtsignal::setbase(int base) {
  v->setbase(base);
}

bool rtsignal::isgeneration(unsigned long g) {
  return (g == generation);
}

//----------------------------------------------------
rtregister::rtregister(rtdpcontext &_rtdpc, symid _id, int wlen, int sign) :
  artsignal(_id),
  ri(_rtdpc, _id, wlen, sign) {
#ifdef RTACTIVITY
  glbRTNumRegisters++;
#endif
  ro = make_gval(wlen, sign);

}

// for the difference between drivers and definers: see explanation at rtsignal
void rtregister::newdef(symid _sfg, artsignal * _definer) {
  //  cout << "define def " << this << " - sfg " << _sfg << " - definer " << _definer << "\n";
  ri.newdef(_sfg, _definer);
}

void rtregister::defdriver(artsignal *driver) {
  ri.defdriver(driver);
}

gval & rtregister::eval(unsigned long g) {
  generation = g;

  return *ro;
}

void rtregister::updatein(unsigned long g) {
  if (ri.hasdef()) 
    ri.eval(g);     // because register can be left undefined in a particular clock
  generation = g; // always advance to current generation
}

void rtregister::asgn(gval &nv) {
  ri.asgn(nv);
}
 
gval& rtregister::peekval() {
  return *ro;
}

void rtregister::tick() {
  //  show(cout);

  // for global sleep
  if (ri.peekval() != *ro) {
    glbRTSleep.cannotSleep();
    //    cerr << "Reg ticks\n";

    if (glbRTDebug) {
      cout << setw(20) << peekval();
      cout << setw(20) << ri.peekval() << " ";
      glbSymboltable.showlex(cout, id);
      cout << "\n";
    }

  }

#ifdef RTVALUECOPY
  ro->valuecopy(ri.peekval());
#else
  *ro = ri.peekval();
#endif


}

void rtregister::show(ostream &os) {
  os << peekval() << "/" <<  ri.peekval();
}

void rtregister::setbase(int base) {
  ro->setbase(base);
  ri.setbase(base);
}

bool rtregister::isgeneration(unsigned long g) {
  return (g == generation);
}

int rtregister::wlenreq() {
  return ri.wlenreq();
}

unsigned rtregister::signreq() {
  return ri.signreq(); 
}

//----------------------------------------------------
rtregmgr::rtregmgr() {}

void rtregmgr::addreg(rtregister *s) {
  allregs.insert(s);
}

void rtregmgr::remreg(rtregister *s) {
  set<rtregister *>::iterator I = allregs.find(s);
  if (I != allregs.end())
    allregs.erase(I);
}

//----------------------------------------------------
// NOTE: Current proxies ONLY work RHS !
rtproxy::rtproxy(symid _id, artsignal *_b, int _idx, int _len) :
  artsignal(_id), 
  b(_b), 
  idx(_idx),
  len(_len) {

  v = make_gval(_len,0);

  // for simulation purposes,
  // slice operations require to have the wordlength of the source
  // operand, i.e. a[1] is 8 bits if 'a' is a bits.
  //  v = make_gval(b->wlenreq(),0);
}

void rtproxy::newdef(symid _sfg, artsignal * _definer) {
  fdlerror(ERROR_USER_PROXY_ASSIGN_NOT_SUPPORTED, id);
}

void rtproxy::defdriver(artsignal *d) {
  fdlerror(ERROR_USER_PROXY_ASSIGN_NOT_SUPPORTED, id);
}

gval & rtproxy::eval(unsigned long g) {
  v->shr_ui_gval(b->eval(g), (unsigned) idx);
  return *v;
}

gval &rtproxy::peekval() {
  return *v;
}

int rtproxy::wlenreq() {
  //  return b->wlenreq();
  return len;
}

unsigned rtproxy::signreq() {
  return 0; // unsigned
}

void rtproxy::setbase(int base) {
  v->setbase(base);
}

//----------------------------------------------------
