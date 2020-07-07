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

#include "module.h"
#include <iostream>

moduletable::moduletable() {}

void moduletable::addmodule(symid _mod) {
  parentmap[_mod] = NOSYMBOL;
}

void moduletable::defineparentof(symid child, symid parent) {
  parentmap[child] = parent;
}

void moduletable::show() {
  cerr << "Module table\n";
  for (parentmap_it j = parentmap.begin(); j != parentmap.end(); j++) {
    cerr << j->first << " parent " << j->second << "\n";
  }

}

void moduletable::hierarchysort_rec(symid parent) {
  if (iparentmap.find(parent) != iparentmap.end()) {
    // need to make lower-level visits first
    for (mtable_it v = iparentmap[parent].begin();
	 v != iparentmap[parent].end();
	 v++) {
      hierarchysort_rec(*v);
    }
  }
  // finally, insert the parent
  if (parent != NOSYMBOL) mtable.push_back(parent);
}

void moduletable::hierarchysort() {
  mtable.clear();

  // first, invert parent map

  for (parentmap_it j = parentmap.begin(); j != parentmap.end(); j++) 
    iparentmap[j->second].push_back(j->first);

  // next, recursively visit the inverted parent map, starting with the root (-1)
  hierarchysort_rec(NOSYMBOL);

}

symid moduletable::bottomup_iterator_init() {
  //  show();
  hierarchysort();
  mtable_iter = 0;
  symbol_iter = mtable[mtable_iter];
  return symbol_iter;
}

symid moduletable::bottomup_iterator_next() {
  symbol_iter++;
  if (parentmap.find(symbol_iter) != parentmap.end()) {
    // this is a new module. switch to new mtable_iter
    mtable_iter++;
    if (mtable_iter >= mtable.size()) { // EOT
      symbol_iter = (unsigned) NOSYMBOL;
    } else {
      symbol_iter = mtable[mtable_iter];
    }
  }
  return symbol_iter;
}

bool moduletable::ismodule(symid k) {
  if (parentmap.find(k) != parentmap.end())
    return true;
  else
    return false;
}

moduletable glbModuleTable;
