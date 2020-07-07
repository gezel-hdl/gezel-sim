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


#include "ipblock.h"
#include "rterror.h"
#include "java_itf.h"

// ------- ALLOCATOR --------
//
// decode typename and dynamically allocate 
// aipblock-inherited specific block

#define CREATE(S) if (!strcmp(tname, #S)) return new S(instname) 

aipblock * ipblockcreate(char *instname, char *tname) {
  CREATE(ram);
  CREATE(ctr32);
  CREATE(gctr32);
  CREATE(javasource);
  CREATE(javasink);
  cerr << "Error: Unknown ipblock type: " << tname << "\n";
  return new aipblock(instname);
}




