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

#ifndef RTBINOP_H
#define RTBINOP_H

#include "artsignal.h"

//------------------------------------------------
//    #     #####   #####    ###    #####  #     #
//   # #   #     # #     #    #    #     # ##    #
//  #   #  #       #          #    #       # #   #
// #     #  #####   #####     #    #  #### #  #  #
// #######       #       #    #    #     # #   # #
// #     # #     # #     #    #    #     # #    ##
// #     #  #####   #####    ###    #####  #     #
class rtassign : public artsignal {
  gval *v;
  artsignal *left, *right;
 public:
  rtassign(symid _id, 
	   symid context,
	   artsignal *_left, artsignal *_right);
  gval & eval(unsigned long g);
  gval & peekval();
  int wlenreq();
  unsigned signreq();
};

//------------------------------------------
// ####### ####### ######  #     #    #    ######  #     #
//    #    #       #     # ##    #   # #   #     #  #   #
//    #    #       #     # # #   #  #   #  #     #   # #
//    #    #####   ######  #  #  # #     # ######     #
//    #    #       #   #   #   # # ####### #   #      #
//    #    #       #    #  #    ## #     # #    #     #
//    #    ####### #     # #     # #     # #     #    #
class rtterop : public artsignal {
 protected:
  gval *v, *gval_zero;
  artsignal *left, *middle, *right;
 public:
  rtterop(symid _id, artsignal *_left, artsignal *_middle,artsignal *_right);
  virtual gval & eval(unsigned long g) = 0;
  virtual gval & peekval();
};

class rtsel : public rtterop {
 public:
  rtsel(symid _id, 
	artsignal *_left, artsignal *_middle, artsignal *_right) :
    rtterop(_id, _left, _middle, _right) {}
  gval & eval(unsigned long g);
  int wlenreq();
  unsigned signreq();
};

//------------------------------------------
// ######    ###   #     #    #    ######  #     #
// #     #    #    ##    #   # #   #     #  #   #
// #     #    #    # #   #  #   #  #     #   # #
// ######     #    #  #  # #     # ######     #
// #     #    #    #   # # ####### #   #      #
// #     #    #    #    ## #     # #    #     #
// ######    ###   #     # #     # #     #    #

class rtbinop : public artsignal {
 protected:
  gval *v;
  artsignal *left, *right;
 public:
  rtbinop(symid _id, artsignal *_left, artsignal *_right);
  rtbinop(symid _id, artsignal *_left, artsignal *_right, int wlen, int sign);
  virtual gval & eval(unsigned long g) = 0;
  virtual gval & peekval();
  virtual int wlenreq();
  virtual unsigned signreq();
};

class rtand : public rtbinop {
 public:
  rtand(symid _id, 
	artsignal *_left, artsignal *_right) :
    rtbinop(_id, _left, _right) {}
  gval & eval(unsigned long g);
};

class rtior : public rtbinop {
 public:
  rtior(symid _id, 
	artsignal *_left, artsignal *_right) :
    rtbinop(_id, _left, _right) {}
  gval & eval(unsigned long g);
};

class rtxor : public rtbinop {
 public:
  rtxor(symid _id, 
	artsignal *_left, artsignal *_right) :
    rtbinop(_id, _left, _right) {}
  gval & eval(unsigned long g);
};

class rtshl : public rtbinop {
 public:
  rtshl(symid _id, 
	artsignal *_left, artsignal *_right) :
    rtbinop(_id, _left, _right, 
	    _left->wlenreq() + ( 1 << _right->wlenreq()), //pessimistic & safe
	    _left->signreq()) {}
  gval & eval(unsigned long g);
  int wlenreq();
  unsigned signreq();
};

class rtshr : public rtbinop {
 public:
  rtshr(symid _id, 
	artsignal *_left, artsignal *_right) :
    rtbinop(_id, _left, _right) {}
  gval & eval(unsigned long g);
};

class rtadd : public rtbinop {
 public:
  rtadd(symid _id, 
	artsignal *_left, artsignal *_right) :
    rtbinop(_id, _left, _right) {}
  gval & eval(unsigned long g);
};

class rtconcat : public rtbinop {
 public:
  rtconcat(symid _id, 
	artsignal *_left, artsignal *_right) :
    rtbinop(_id, _left, _right,
	    _left->wlenreq() + _right->wlenreq(),
	    _left->signreq()) {}
  gval & eval(unsigned long g);
  int wlenreq();
  unsigned signreq();
};

class rtsub : public rtbinop {
 public:
  rtsub(symid _id, 
	artsignal *_left, artsignal *_right) :
    rtbinop(_id, _left, _right) {}
  gval & eval(unsigned long g);
};

class rtmod : public rtbinop {
 public:
  rtmod(symid _id, 
	artsignal *_left, artsignal *_right) :
    rtbinop(_id, _left, _right) {}
  gval & eval(unsigned long g);
};

class rtmul : public rtbinop {
 public:
  rtmul(symid _id, 
	artsignal *_left, artsignal *_right) :
    rtbinop(_id, _left, _right, 
	    _left->wlenreq() + _right->wlenreq(), 
		(_left->signreq() > _right->signreq()) ? _left->signreq() :  _right->signreq() ) {}
  gval & eval(unsigned long g);
  int wlenreq();
  unsigned signreq();
};

class rteq : public rtbinop {
  gval *gvaltrue ;
  gval *gvalfalse;
 public:
  rteq(symid _id, 
	artsignal *_left, artsignal *_right) :
    rtbinop(_id, _left, _right, 1, 0) {
    gvaltrue  = make_gval(1,0,"1");
    gvalfalse = make_gval(1,0,"0");
  }
  gval & eval(unsigned long g);
  int wlenreq();
  unsigned signreq();
};

class rtne : public rtbinop {
  gval *gvaltrue ;
  gval *gvalfalse;
 public:
  rtne(symid _id, 
	artsignal *_left, artsignal *_right) :
    rtbinop(_id, _left, _right, 1, 0) {
    gvaltrue  = make_gval(1,0,"1");
    gvalfalse = make_gval(1,0,"0");
  }
  gval & eval(unsigned long g);
  int wlenreq();
  unsigned signreq();
};

class rtgrt : public rtbinop {
  gval *gvaltrue ;
  gval *gvalfalse;
 public:
  rtgrt(symid _id, 
	artsignal *_left, artsignal *_right) :
    rtbinop(_id, _left, _right, 1, 0) {
    gvaltrue  = make_gval(1,0,"1");
    gvalfalse = make_gval(1,0,"0");
  }
  gval & eval(unsigned long g);
  int wlenreq();
  unsigned signreq();
};

class rtsmt : public rtbinop {
  gval *gvaltrue ;
  gval *gvalfalse;
 public:
  rtsmt(symid _id, 
	artsignal *_left, artsignal *_right) :
    rtbinop(_id, _left, _right, 1, 0) {
    gvaltrue  = make_gval(1,0,"1");
    gvalfalse = make_gval(1,0,"0");
  }
  gval & eval(unsigned long g);
  int wlenreq();
  unsigned signreq();
};

class rtgrteq : public rtbinop {
  gval *gvaltrue ;
  gval *gvalfalse;
 public:
  rtgrteq(symid _id, 
	artsignal *_left, artsignal *_right) :
    rtbinop(_id, _left, _right, 1, 0) {
    gvaltrue  = make_gval(1,0,"1");
    gvalfalse = make_gval(1,0,"0");
  }
  gval & eval(unsigned long g);
  int wlenreq();
  unsigned signreq();
};

class rtsmteq : public rtbinop {
  gval *gvaltrue ;
  gval *gvalfalse;
 public:
  rtsmteq(symid _id, 
	artsignal *_left, artsignal *_right) :
    rtbinop(_id, _left, _right, 1, 0) {
    gvaltrue  = make_gval(1,0,"1");
    gvalfalse = make_gval(1,0,"0");
  }
  gval & eval(unsigned long g);
  int wlenreq();
  unsigned signreq();
};

#endif
