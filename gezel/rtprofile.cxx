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

#include "rtprofile.h"
#include <cstdlib>
#include <sstream>

rtprofile *glbRTProfile = 0;

rtprofile_op::rtprofile_op(bool _upedgeMode, vector<unsigned> &opweights, symid id) {
  v    = make_gval(32,0);
  vand = make_gval(32,0);
  vxor = make_gval(32,0);
  v->assignulong(0);
  vand->assignulong(0);
  vxor->assignulong(0);
  evals      = 0;
  toggles    = 0;
  ones       = 0;
  zeroes     = 0;
  upedgeMode = _upedgeMode;
  weights    = opweights;
  thesym     = id;
}

void rtprofile_op::hammingupdate(gval *nv) {
  evals++;

  //  cerr << "eval " << nv << "\n";

  unsigned long h = nv->popcount();

  //    if (h) {
  //      glbSymboltable.showsymbol(cerr,thesym);
  //      cerr << "toggles - " << h << "\n";
  //   }
 
  ones   += h; // WILL NOT WORK FOR NEGATIVE NUMBERS
  zeroes += nv->getwordlength() - h;

  // weighted toggle counts
  if (weights.size() == 0) {
    if (upedgeMode) {
      vxor->xor_gval(*v, *nv);
      vand->and_gval(*vxor, *nv);
      toggles += vand->popcount();
    } else {
      toggles += v->hamdist(*nv);
    }
  } else {
    if (upedgeMode) {
      vxor->xor_gval(*v, *nv);
      vand->and_gval(*vxor, *nv);
      toggles += vand->weighted_popcount(weights);
    } else {
      toggles += v->weighted_hamdist(*nv,weights);
    }
  }

  v->valuecopy(*nv);
}

//---------------------------------------------------------------
rtprofile::rtprofile(bool _upedgeMode) {
  upedgeMode    = _upedgeMode;
  cycleMode     = false;
  opsMode       = false;
  makeop_enable = true;
  generation    = 0;
  totalevals    = 0;
  lastevals     = 0;
  totaltoggles  = 0;
  lasttoggles   = 0;
  currentones   = 0;
  currentzeroes = 0;
  totalones     = 0;
  totalzeroes   = 0;
  lastones      = 0;
  lastzeroes    = 0;
}

void rtprofile::include(string dp) {
  makeop_enable = false;
  includeset.insert(dp);
}

void rtprofile::toggleweights(string dp) {
  unsigned v;
  istringstream ci(dp.c_str());
  while (!ci.eof()) {
    ci >> v;
    if (! ci.fail()) 
      weights.push_back(v);
    else
      break;
  }
}

void rtprofile::togglevariation(string dp) {
  unsigned v;
  istringstream ci(dp.c_str());
  while (!ci.eof()) {
    ci >> v;
    if (! ci.fail()) 
      variation.push_back(v);
    else
      break;
  }
}

void rtprofile::check_include(string dp) {
  if (includeset.find(dp) != includeset.end())
    makeop_enable = true;
  else
    makeop_enable = false;

  // if no dp included, will include all of them by default
  if (includeset.size() == 0)
    makeop_enable = true;
}

void rtprofile::makeop(symid id) {
  if (id == NOSYMBOL) // is an intermediate symbol
    return;
  if (makeop_enable) {
    vector<unsigned> opws;

    if (ops.find(id) != ops.end()) return;

    //    glbSymboltable.showsymbol(cerr, id);
    //    cerr << ops.size() << "\n";

    if (variation.size() == 0) {
      opws = weights;
    } else {
      for (unsigned i=0; i<weights.size(); i++) {
	unsigned v = variation[i];
	if (v) 
	  opws.push_back(weights[i] + (unsigned) ((rand() % (2*v)) - v));
	else 
	  opws.push_back(weights[i]);
	}
    }
    //    for (unsigned i=0; i<opws.size(); i++) 
    //      cout << opws[i] << "\n";
    ops[id] = new rtprofile_op(upedgeMode, opws, id);
  }
}

void rtprofile::evalop(symid id, gval *v) {
  //  if (makeop_enable) {
  //    glbSymboltable.showsymbol(cerr, id);
  //    cerr << "--> " << makeop_enable << " evalop " << *v << "\n";
  //  }

  if (id == NOSYMBOL) // is an intermediate symbol
    return;

  if (ops.find(id) != ops.end()) 
    ops[id]->hammingupdate(v);
}

void rtprofile::accumulate(unsigned long g) {
  if (g != generation) {
    evals.clear();
    toggles.clear();
    totalevals   = 0;
    totaltoggles = 0;
    totalzeroes  = 0;
    totalones    = 0;
    // first collect statistics per operation type
    map<symid,  rtprofile_op *>::iterator i;
    for (i = ops.begin(); i != ops.end(); i++) {
      evals  [glbSymboltable.getsymbol(i->first)->type()] += i->second->evals;
      totalevals   += i->second->evals;
      toggles[glbSymboltable.getsymbol(i->first)->type()] += i->second->toggles;
      totaltoggles += i->second->toggles;
      totalzeroes  += i->second->zeroes;
      totalones    += i->second->ones;
    }
    currentzeroes  = totalzeroes - lastzeroes;
    lastzeroes     = totalzeroes;
    currentones    = totalones - lastones;
    lastones       = totalones;
    currentevals   = totalevals - lastevals;
    currenttoggles = totaltoggles - lasttoggles;
    lastevals      = totalevals;
    lasttoggles    = totaltoggles;
    generation = g;
  }
}

unsigned rtprofile::togglecount(unsigned long n) {
  accumulate(n);  // update toggle count for current cycle
  return currenttoggles;
}

unsigned rtprofile::onescount(unsigned long n) {
  accumulate(n);  // update ones count for current cycle
  return currentones;
}

unsigned rtprofile::zeroescount(unsigned long n) {
  accumulate(n);  // update zeroes count for current cycle
  return currentzeroes;
}

void rtprofile::tick(ostream &os, unsigned long n) {
  if (cycleMode) {
    accumulate(n);
    os << "Profile Cycle " << setw(15) << n << ": ";
    os << setw(15) << currentevals   << " evals, ";
    os << setw(15) << currenttoggles << " toggles, ";
    os << setw(15) << currentzeroes  << " zeroes, ";
    os << setw(15) << currentones    << " ones\n";
  }
}

void rtprofile::set_cycle_dispmode(bool _cycleMode) {
  cycleMode = _cycleMode;
}

void rtprofile::set_operations_dispmode(bool _opsMode) {
  opsMode = _opsMode;
}

void rtprofile::report(ostream &os) {
  if (opsMode) {
    accumulate((unsigned long) -1L);

    // header
    os << setw(15) << "Type";
    os << setw(15) << "Evals";
    os << setw(15) << "Toggles\n";
    
    map<symbol::_symtype, unsigned>::iterator j;
    map<symbol::_symtype, unsigned>::iterator k;
    for (j = evals.begin(), k = toggles.begin();
	 j != evals.end();
	 j++, k++) {
      os << setw(15);
      os << symbol::typestr(k->first);
      os << setw(15);
      os << j->second;
      os << setw(15);
      os << k->second;
      os << "\n";
    }
  }

}
