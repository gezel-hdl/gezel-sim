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

#ifndef RTMODULE_H
#define RTMODULE_H

#include "asymbol.h"

class moduletable {
  vector<symid> mtable;                      // bottom-up module table
  map<symid, symid> parentmap;               // child-to-parent map
  map<symid, vector<symid> > iparentmap;     // parent-to-child map
  typedef map<symid, symid>::iterator parentmap_it;
  typedef vector<symid>::iterator     mtable_it;
  unsigned mtable_iter;
  unsigned symbol_iter;
  void hierarchysort(); 
  void hierarchysort_rec(symid); 
 public:
  moduletable            ();
  void addmodule         (symid);
  void defineparentof    (symid child, symid parent);
  void show              ();

  symid bottomup_iterator_init();
  symid bottomup_iterator_next();

  bool  ismodule(symid);

};


extern moduletable glbModuleTable;

#endif
