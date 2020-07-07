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

#include "rtunop.h"
#include "rtopt.h"

rtunop::rtunop(symid _id, 
	       artsignal *_left) :
  artsignal(_id),
  left(_left) {
  v = make_gval(_left->wlenreq(), _left->signreq());
  if (glbRTProfile)
    glbRTProfile->makeop(_id);
}

rtunop::rtunop(symid _id, 
	       artsignal *_left, int wlen, int sign) :
  artsignal(_id),
  left(_left) {
  v = make_gval(wlen, sign);
  if (glbRTProfile)
    glbRTProfile->makeop(_id);
}

gval& rtunop::peekval() {
  return *v;
}

int rtunop::wlenreq() {
  return left->wlenreq();
}

unsigned rtunop::signreq() {
  return left->signreq();
}

gval & rtnot::eval(unsigned long g) {
  v->not_gval(left->eval(g));
  if (glbRTProfile)
    glbRTProfile->evalop(id, v);
  return *v;
}

gval & rtneg::eval(unsigned long g) {
  v->neg_gval(left->eval(g));
  if (glbRTProfile)
    glbRTProfile->evalop(id, v);
  return *v;
}

gval & rtcast::eval(unsigned long g) {
  // note: casting happens as a side effect of the assignment
  //  cerr << "eval cast w " << wlenreq() << " s " << signreq() << "\n";
  *v = left->eval(g);
  //  cerr << "Cast LHS: ";
  //  v->dbgprint();
  //  cerr << "Cast RHS: ";
  //  left->eval(g).dbgprint();
  if (glbRTProfile)
    glbRTProfile->evalop(id, v);
  return *v;
}

int rtcast::wlenreq() {
  return v->getwordlength();
}

unsigned rtcast::signreq() {
  return v->getsign();
}







