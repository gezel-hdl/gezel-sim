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


#ifndef RTPROFILE_H
#define RTPROFILE_H

#include <map>
#include <set>
#include "gval.h"
#include <iomanip>
#include "asymbol.h"
#include <assert.h>

// using namespace std;
// using std::setw;

class rtprofile_op {
  gval *v;
  gval *vand;
  gval *vxor;
  bool upedgeMode;
  vector<unsigned>  weights;

  symid thesym;

 protected:
  unsigned evals;
  unsigned toggles;
  unsigned ones;
  unsigned zeroes;
 public:
  friend class rtprofile;
  rtprofile_op(bool _upedgeMode, vector<unsigned> &opweights, symid id);
  void hammingupdate(gval *nv);
};

class rtprofile {
  map<symid,  rtprofile_op *>     ops;

  map<symbol::_symtype, unsigned> evals;
  map<symbol::_symtype, unsigned> toggles;

  unsigned currentevals;
  unsigned currenttoggles;  // toggles, evals in current clock cycle (generation)
  unsigned totalevals;

  unsigned totaltoggles;    // total toggles, evals since cycle 0
  unsigned lastevals;
  unsigned lasttoggles;     // total toggles, evals in previous clock cycle

  unsigned long generation;

  unsigned currentzeroes;
  unsigned lastzeroes;
  unsigned totalzeroes;     // total number of '0' nets over entire simulation

  unsigned currentones;
  unsigned lastones;
  unsigned totalones;       // total number of '1' nets over entire simulation

  bool opsMode;
  bool cycleMode;
  bool upedgeMode;

  set<string > includeset;
  bool         makeop_enable;

  vector<unsigned>  weights;
  vector<unsigned>  variation;

  void accumulate(unsigned long);

 public:
  rtprofile(bool _upedgeMode);
  void set_cycle_dispmode(bool _cycleMode);
  void set_operations_dispmode(bool _opsMode);
  void include(string dp);
  void check_include(string dp);
  void toggleweights(string dp);
  void togglevariation(string dp);
  void makeop(symid id);
  void evalop(symid id, gval *v);
  void tick(ostream &os, unsigned long n);
  void report(ostream &os);
  unsigned togglecount(unsigned long n); // toggle count in cycle n
  unsigned onescount(unsigned long n);   // ones count in cycle n
  unsigned zeroescount(unsigned long n); // zeroes count in cycle n
};

extern rtprofile *glbRTProfile;

#endif
