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

#include "rtlookup.h"
#include "rterror.h"

rtlookuptable::rtlookuptable(int _wlen, 
			     int _sign, 
			     vector<char *> &luc) : wlen(_wlen), sign(_sign) {
  if (luc.size() == 0) 
    cout << "Warning: Empty lookup table\n";
  for (unsigned i=0; i< luc.size(); ++i) {
    table.push_back(make_gval(luc[i]));
  }
  for (unsigned i=0; i< luc.size(); ++i) {
    table[i]->setwordlength(wlen);
  }
  //  for (unsigned i=0; i< luc.size(); ++i) {
  //    cerr << *table[i] << "\n";
  //  }
  // NOTE!! SIGN NOT USED
  size_t t   = (luc.size()-1);
  int wls = 0;
  while (t > 0) {
    ++wls;
    t = t >> 1;
  }
}

int rtlookuptable::wlenreq() {
  return wlen;
}

unsigned rtlookuptable::signreq() {
  return sign;
}

gval &rtlookuptable::lookup(gval &adr, symid k) {
  //  cerr << adr.toulong() << " " << table.size() << "\n";
  if (adr.toulong() >= table.size()) {
    fdlerror(ERROR_UNDEFINED_LOCATION_IN_LOOKUP_TABLE, k);
  }
  return *table[adr.toulong()];
}

rtlookup::rtlookup(symid _id, artsignal *_left, rtlookuptable *Tparm) :
  rtunop(_id, _left, Tparm->wlenreq(), Tparm->signreq()), T(Tparm) {}


gval & rtlookup::eval(unsigned long g) {
#ifdef RTVALUECOPY
  v.valuecopy(T->lookup(left->eval(g)), id);
#else
  *v = T->lookup(left->eval(g), id);
#endif
  return *v;
}

int rtlookup::wlenreq() {
  return v->getwordlength();
}

unsigned rtlookup::signreq() {
  return v->getsign();
}

