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

#ifndef ARTSIGNAL_H
#define ARTSIGNAL_H

#include "fdlsym.h"
#include "gval.h"
#include "rtprofile.h"

//------------------------------------------
class artsignal {
 protected:
  unsigned long generation; // generation counter used in simulation
  enum          _evalstate {STABLE, EVALUATION, EVALUATION_CHECK};
  _evalstate    currentstate;
//  symid         id;
 public:
  symid id;
  artsignal(symid _id) : id(_id) {
    generation = 0;
    currentstate = STABLE;
  }
  virtual ~artsignal() {}

  // SIMULATION: read value and update
  virtual gval & eval(unsigned long target_generation) = 0;
  virtual void   asgn(gval& ) {}
  virtual gval& peekval() = 0;
  virtual int      wlenreq() { return 0;}
  virtual unsigned signreq() { return 0;}

  virtual void show(ostream &os) {
    os << peekval();
    //    cout << "Warning: Missing show() method: ";
    //   dbginfo(cout);
    //   cout << "\n";
  }

  virtual void setbase(int base) {}
  virtual bool isgeneration(unsigned long generation) {return false;}

  // CONSTRUCTION: declare a driver under context of sfg
  // (only used for non-constant terminals
  virtual void newdef(symid _sfg, artsignal * _definer) {}
  virtual void defdriver(artsignal *p) {}

  // debuginfo
  void dbginfo(ostream &os);
  
  //  // usleep_m() returns true if the signal has changed value in the
  //  // previous clock cycle - default true unless can be proven
  //  // otherwise (by derived classes)
  //  virtual bool    usleep_m() {
  //   // cerr << "** eval artsig sleep .. " << id << "\n";
  //    return true;
  //  }

};

#endif
