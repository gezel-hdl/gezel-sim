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

#ifndef RTSIGNAL_H
#define RTSIGNAL_H

#include <iostream>

using namespace std;

#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include "artsignal.h"
#include "rtdp.h"
#include "rtopt.h"

//------------------------------------------
// ####### ####### ######  #     #   ###   #     #    #    #
//    #    #       #     # ##   ##    #    ##    #   # #   #
//    #    #       #     # # # # #    #    # #   #  #   #  #
//    #    #####   ######  #  #  #    #    #  #  # #     # #
//    #    #       #   #   #     #    #    #   # # ####### #
//    #    #       #    #  #     #    #    #    ## #     # #
//    #    ####### #     # #     #   ###   #     # #     # #######
class rtconstant : public artsignal {
  gval *v;
 public:
  rtconstant(symid _id, char *value) :
    artsignal(_id) {
    v = make_gval(value);
  }
  int wlenreq() {
    return v->getwordlength();
  }
  unsigned signreq() {
    return 0;
  }
  gval & eval(unsigned long) {
    return *v; // constants are always valid
  }
  gval & peekval() {
    return *v;
  }
};

//-------------------------------------------------
class rtsignal : public artsignal {
  rtsigcontext    context;
  artsignal      *driver; // for continous assignments, drivers are used iso contexts
  gval           *v;       // value 
 public:
  rtsignal(rtdpcontext & _rtdpc, symid _id, int wlen, int sign);
  void            newdef(symid _sfg, artsignal * _definer);
  void            defdriver(artsignal *p);
  gval  & eval(unsigned long target_generation);
  void    asgn(gval &nv);
  gval &  peekval();
  bool            hasdef();
  int             wlenreq();
  unsigned        signreq();
  void            show(ostream &os);
  void            setbase(int base);
  bool            isgeneration(unsigned long generation);
};

class rtregister : public artsignal {
  rtsignal ri; // reg_input
  gval    *ro; // reg output
 public:
  rtregister(rtdpcontext & _rtdpc, symid _id, int wlen, int sign);
  void    newdef(symid _sfg, artsignal * _definer);
  void    defdriver(artsignal * _driver);
  gval & eval(unsigned long g);
  void asgn(gval &nv);
  gval & peekval();
  int wlenreq();
  unsigned signreq();
  void    updatein(unsigned long g);
  void    tick();
  void    show(ostream &os);
  void    setbase(int base);
  bool    isgeneration(unsigned long generation);
};

class rtregmgr {
  set<rtregister *> allregs;
 public:
  rtregmgr();
  void addreg(rtregister *);
  void remreg(rtregister *);
  void tick();
};

class rtproxy : public artsignal {
  artsignal *b; // base signal
  gval      *v; // value
  int        idx;
  int        len;
 public:
  rtproxy(symid _id, artsignal *_b, int _idx, int _len);
  void newdef(symid _sfg, artsignal * _definer);
  void defdriver(artsignal * _driver);
  gval & eval(unsigned long g);
  gval& peekval();
  void  setbase(int base);
  int wlenreq();
  unsigned signreq();
};
#endif





