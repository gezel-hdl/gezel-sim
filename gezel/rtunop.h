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
#ifndef RTUNOP_H
#define RTUNOP_H

#include "artsignal.h"

//------------------------------------------
// #     # #     #    #    ######  #     #
// #     # ##    #   # #   #     #  #   #
// #     # # #   #  #   #  #     #   # #
// #     # #  #  # #     # ######     #
// #     # #   # # ####### #   #      #
// #     # #    ## #     # #    #     #
//  #####  #     # #     # #     #    #

class rtunop : public artsignal {
 protected:
  gval *v;
  artsignal *left;
 public:
  rtunop(symid _id, artsignal *_left);
  rtunop(symid _id, artsignal *_left, int wlen, int sign);
  virtual gval & eval(unsigned long g) = 0;
  virtual gval & peekval();
  virtual int wlenreq();
  virtual unsigned signreq();
};

class rtnot : public rtunop {
 public:
  rtnot(symid _id, artsignal *_left) :
    rtunop(_id, _left) {}
  gval & eval(unsigned long g);
};

class rtneg : public rtunop {
 public:
  rtneg(symid _id, artsignal *_left) :
    rtunop(_id, _left, _left->wlenreq(), 1) {} // always signed
  gval & eval(unsigned long g);
  unsigned signreq() { return 1; }
};

class rtcast : public rtunop {
 public:
  rtcast(symid _id, int wlen, int sign, artsignal *_left) :
    rtunop(_id, _left, wlen, sign) {
  }
  gval & eval(unsigned long g);
  int wlenreq();
  unsigned signreq();
};

#endif
