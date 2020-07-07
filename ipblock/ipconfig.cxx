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
#include "ipdtu.h"
#include "ipuser.h"
#include "ipconfig.h"
#include "nallatechfsb.h"

// ------- ALLOCATOR --------
//
// decode typename and dynamically allocate 
// aipblock-inherited specific block

#define CREATE(S) if (!strcmp(tname, #S)) return new S(instname) 

//----------------------------
// Dynamically load an ipblock
extern "C" {
  bool      dynloader(char *tname);
  aipblock *dyncreate(char *instname, char *tname);
}

aipblock * stdipblockcreate(char *instname, char *tname) {
  aipblock *r;

  r = static_ipblockcreate(instname, tname);
  if (r) return r;

  r = dynamic_ipblockcreate(instname, tname);
  if (r) return r;

  fdlerror(ERROR_USER_IPBLOCK_UNKNOWN, tname);
  return new aipblock(instname);

}

#ifndef _MSC_VER
aipblock * dynamic_ipblockcreate(char *instname, char *tname) {
  // try to find the block as a dynamic library
  if (dynloader(tname)) return dyncreate(instname, tname);

  return 0;
}
#else
aipblock * dynamic_ipblockcreate(char *instname, char *tname) {
  return 0;
}
#endif

aipblock * static_ipblockcreate(char *instname, char *tname) {
  CREATE(ram);
  CREATE(wideram);
  CREATE(rom);
  CREATE(ijvm);
  CREATE(ctr32);
  CREATE(gctr32);
  CREATE(tracer);
  CREATE(filesource);
  CREATE(gfmul16);
  CREATE(rand16);

  CREATE(nalla_regio);
  CREATE(nalla_spl);

  return 0;
}

#ifndef _MSC_VER
#include <dlfcn.h>

map<string, void *> dynlibs;

extern "C" bool dynloader(char *tname) {
  string buf;

  // look for './lib<block>.so' 

  // with the pathname './' we assume that the ipblock implementation
  // will live in the same directory as the GEZEL source file.  This
  // may not be desirable for big projects.

  buf = "./lib" + string(tname) + ".so";

  //  cerr << "looking for [" << buf.c_str() << "]\n";

  void *dlib = dlopen(buf.c_str(), RTLD_LAZY);

  if (!dlib) {
    cerr << dlerror() << "\n";
    return false;
  }

  dynlibs[string(tname)] = dlib;
  return true;
}

typedef aipblock *aipblock_create_t(char *instname);

extern "C" aipblock *dyncreate(char *instname, char *tname) {
  // first locate the factory function of the dynamic ipblock
  // this function must be called 'create_<block>'
  string buf;
  buf = "create_" + string(tname);

  if (dynlibs.find(string(tname)) == dynlibs.end()) {
    cerr << "*** Error: dyncreate: library did not load: " << tname << "\n";
    return 0;  // dyncreate can only be called when lib is loaded
  }

  aipblock_create_t *f = (aipblock_create_t *) dlsym(dynlibs[string(tname)],buf.c_str());

  return f(instname);
}

#endif
