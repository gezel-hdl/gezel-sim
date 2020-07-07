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

#include "rtbinop.h"
#include "rtopt.h"

//----------------------------------------------------
rtassign::rtassign(symid _id, symid context,
		   artsignal *_left, artsignal *_right) :
  artsignal(_id),
  left(_left),
  right(_right) {
  left->newdef(context,this);
  v = make_gval(_right->wlenreq(), _right->signreq());
  if (glbRTProfile)
    glbRTProfile->makeop(_id);
}

gval & rtassign::eval(unsigned long g) {
#ifdef RTVALUECOPY
  v->valuecopy(right->eval(g));
#else
  *v = right->eval(g);
#endif

  //  cerr << "rtassign " << id << "\n";
  left->asgn(*v);
  if (glbRTProfile)
    glbRTProfile->evalop(id, v);

  return *v;
}

gval& rtassign::peekval() {
  return *v;
}

int rtassign::wlenreq() {
  return right->wlenreq();
}

unsigned rtassign::signreq() {
  return right->signreq();
}

//----------------------------------------------------
rtterop::rtterop(symid _id, 
		 artsignal *_left, artsignal *_middle,artsignal *_right) :
  artsignal(_id),
  left(_left),
  middle(_middle),
  right(_right) {
  v = make_gval(max(_middle->wlenreq(), _right->wlenreq()), max(_middle->signreq(), _right->signreq()));
  gval_zero = make_gval("0");
  if (glbRTProfile)
    glbRTProfile->makeop(_id);
}

gval& rtterop::peekval() {
    return *v;
}

gval & rtsel::eval(unsigned long g) {

  gval & c = left->eval(g);

  //  gval & l = middle->eval(g);
  //  gval & r = right->eval(g);

#ifdef RTVALUECOPY
  if (c != *gval_zero) {
    gval & l = middle->eval(g);
    v->valuecopy(l);
    //    cout << "condition true\n";
    //    cout << "CV " << c << "\n";
    //    cout << "CZ " << *gval_zero << "\n";
  } else {
    gval & r = right->eval(g);
    v->valuecopy(r);
    //    cout << "condition false\n";
    //    cout << "CV " << c << "\n";
  }
#else
  if (c != *gval_zero) {
    gval & l = middle->eval(g);
    *v = l;
  }  else {
    gval & r = right->eval(g);
    *v = r;
  }
#endif

  if (glbRTProfile)
    glbRTProfile->evalop(id, v);

  return *v;
}

int rtsel::wlenreq() {
  return max(middle->wlenreq(),right->wlenreq());
}

unsigned rtsel::signreq() {
  return max(middle->signreq(),right->signreq());
}

//----------------------------------------------------
rtbinop::rtbinop(symid _id, 
		 artsignal *_left, artsignal *_right) :
  artsignal(_id),
  left(_left),
  right(_right) {
  //  cout << "make rtbinop\n";
  v = make_gval(wlenreq(),signreq());

  if (glbRTProfile)
    glbRTProfile->makeop(_id);
}

rtbinop::rtbinop(symid _id, 
		 artsignal *_left, artsignal *_right,
		 int wlen, int sign) :
  artsignal(_id),
  left(_left),
  right(_right) {
  v = make_gval(wlen, sign);

  if (glbRTProfile)
    glbRTProfile->makeop(_id);
}

gval& rtbinop::peekval() {
    return *v;
}

int rtbinop::wlenreq() {
  return max(left->wlenreq(),right->wlenreq());
}

unsigned rtbinop::signreq() {
  return max(left->signreq(),right->signreq());
}

gval & rtand::eval(unsigned long g) {
  v->and_gval(left->eval(g),right->eval(g));
  if (glbRTProfile)
    glbRTProfile->evalop(id, v);
  return *v;
}

gval & rtior::eval(unsigned long g) {
  v->ior_gval(left->eval(g),right->eval(g));
  if (glbRTProfile)
    glbRTProfile->evalop(id, v);
  return *v;
}

gval & rtxor::eval(unsigned long g) {
  v->xor_gval(left->eval(g), right->eval(g));

  //  cerr << "XOR " << "L " << left->eval(g) << " R " <<  right->eval(g) << " ";
  //  cerr << *v << "\n";

  if (glbRTProfile)
    glbRTProfile->evalop(id, v);
  return *v;
}

gval & rtshr::eval(unsigned long g) {
  v->shr_gval(left->eval(g), right->eval(g));
  if (glbRTProfile)
    glbRTProfile->evalop(id, v);
  return *v;
}

gval & rtshl::eval(unsigned long g) {
  v->shl_gval(left->eval(g), right->eval(g));
  if (glbRTProfile)
    glbRTProfile->evalop(id, v);
  return *v;
}

int rtshl::wlenreq() {
  return (left->wlenreq() + ( 1 << right->wlenreq()));
}

unsigned rtshl::signreq() {
  return left->signreq();
}

gval & rtadd::eval(unsigned long g) {
  v->add_gval(left->eval(g), right->eval(g));

  //  cerr << "ADD " << "L " << left->eval(g) << " R " <<  right->eval(g) << " ";
  //  cerr << *v << "\n";

  //  cerr << "wleft "  << left->wlenreq()  << " sleft "  << left->signreq();
  //  cerr << " wright " << right->wlenreq() << " sright " << right->signreq();
  //  cerr << " wsum "   << v->getwordlength()     << " ssum "   << v->getsign() << "\n";

  if (glbRTProfile)
    glbRTProfile->evalop(id, v);
  return *v;
}

gval & rtconcat::eval(unsigned long g) {
  v->concat_gval(left->eval(g), right->eval(g));
  if (glbRTProfile)
    glbRTProfile->evalop(id, v);
  return *v;
}

int rtconcat::wlenreq() {
  return (left->wlenreq() + right->wlenreq());
}

unsigned rtconcat::signreq() {
  return left->signreq();
}

gval & rtsub::eval(unsigned long g) {
  v->sub_gval(left->eval(g), right->eval(g));
  if (glbRTProfile)
    glbRTProfile->evalop(id, v);
  return *v;
}

gval & rtmod::eval(unsigned long g) {
  v->mod_gval(left->eval(g), right->eval(g));
  if (glbRTProfile)
    glbRTProfile->evalop(id, v);
  return *v;
}

gval & rtmul::eval(unsigned long g) {
  v->mul_gval(left->eval(g), right->eval(g));
  if (glbRTProfile)
    glbRTProfile->evalop(id, v);
  return *v;
}

int rtmul::wlenreq() {
  return (left->wlenreq() + right->wlenreq());
}

unsigned rtmul::signreq() {
  return max(left->signreq(), right->signreq());
}

//----------------------
gval & rteq::eval(unsigned long g) {

#ifdef RTVALUECOPY
  if (left->eval(g) == right->eval(g))
    v->valuecopy(*gvaltrue);
  else
    v->valuecopy(*gvalfalse);
#else
  if (left->eval(g) == right->eval(g))
    *v = gvaltrue;
  else
    *v = gvalfalse;
#endif

  if (glbRTProfile)
    glbRTProfile->evalop(id, v);
  return *v;
}

int rteq::wlenreq() {
  return 1;
}

unsigned rteq::signreq() {
  return 0;
}
//------------------------
gval & rtne::eval(unsigned long g) {
#ifdef RTVALUECOPY
  if (left->eval(g) != right->eval(g))
    v->valuecopy(*gvaltrue);
  else
    v->valuecopy(*gvalfalse);
#else
  if (left->eval(g) != right->eval(g))
    *v = gvaltrue;
  else
    *v = gvalfalse;
#endif

  if (glbRTProfile)
    glbRTProfile->evalop(id, v);
  return *v;
}

int rtne::wlenreq() {
  return 1;
}

unsigned rtne::signreq() {
  return 0;
}
//------------------------
gval & rtgrt::eval(unsigned long g) {
#ifdef RTVALUECOPY
  if (left->eval(g) > right->eval(g))
    v->valuecopy(*gvaltrue);
  else
    v->valuecopy(*gvalfalse);
#else
  if (left->eval(g) > right->eval(g))
    *v = gvaltrue;
  else
    *v = gvalfalse;
#endif
  if (glbRTProfile)
    glbRTProfile->evalop(id, v);
  return *v;
}

int rtgrt::wlenreq() {
  return 1;
}

unsigned rtgrt::signreq() {
  return 0;
}
//------------------------
gval & rtsmt::eval(unsigned long g) {
#ifdef RTVALUECOPY
  if (left->eval(g) < right->eval(g))
    v->valuecopy(*gvaltrue);
  else
    v->valuecopy(*gvalfalse);
#else
  if (left->eval(g) < right->eval(g))
    *v = gvaltrue;
  else
    *v = gvalfalse;
#endif
  if (glbRTProfile)
    glbRTProfile->evalop(id, v);
  return *v;
}

int rtsmt::wlenreq() {
  return 1;
}

unsigned rtsmt::signreq() {
  return 0;
}
//------------------------
gval & rtgrteq::eval(unsigned long g) {
#ifdef RTVALUECOPY
  if (left->eval(g) >= right->eval(g))
    v->valuecopy(*gvaltrue);
  else
    v->valuecopy(*gvalfalse);
#else
  if (left->eval(g) >= right->eval(g))
    *v = gvaltrue;
  else
    *v = gvalfalse;
#endif
  if (glbRTProfile)
    glbRTProfile->evalop(id, v);
  return *v;
}

int rtgrteq::wlenreq() {
  return 1;
}

unsigned rtgrteq::signreq() {
  return 0;
}

gval & rtsmteq::eval(unsigned long g) {
#ifdef RTVALUECOPY
  if (left->eval(g) <= right->eval(g))
    v->valuecopy(*gvaltrue);
  else
    v->valuecopy(*gvalfalse);
#else
  if (left->eval(g) <= right->eval(g))
    *v = gvaltrue;
  else
    *v = gvalfalse;
#endif
  if (glbRTProfile)
    glbRTProfile->evalop(id, v);
  return *v;
}

int rtsmteq::wlenreq() {
  return 1;
}

unsigned rtsmteq::signreq() {
  return 0;
}
