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
// $Id: GezelModuleImp.cxx 21 2005-11-15 19:08:03Z schaum $
//--------------------------------------------------------------

#include "GezelModule.h"
#include "rtsim.h"

static rtsimgen *myrtsimgen;

#include "ipblock.h"     // glbIpblockcreate
#include "ipconfig.h"    // static_ipblockcreate, dynamic_ipblockcreate
#include "java_itf.h" // ipblocks for 8051

#define CREATE(S) if (!strcmp(tname, #S)) return new S(instname) 

aipblock * java_ipblockcreate(char *instname, char *tname) {
  aipblock *r;

  r = static_ipblockcreate(instname, tname);
  if (r) return r;

  CREATE(javasource);
  CREATE(javasink);

  cerr << "java_ipblockcreate: Unknown ipblock type: " << tname << "\n";
  return new aipblock(instname);
}

JNIEXPORT void JNICALL Java_GezelModule_loadfile(JNIEnv *env, 
						 jobject obj, 
						 jstring fdl) {

  const char *fdlname = env->GetStringUTFChars(fdl, 0);

  glbIpblockcreate = java_ipblockcreate;
  
  myrtsimgen = 0;
  if (call_parser((char *) fdlname)) {
    cerr << "Could not open GEZEL file " << fdlname << "\n";
  } else {
    glbAllowSimulatorSleep = 1;
    myrtsimgen = new rtsimgen;
    glbSymboltable.createcode(myrtsimgen);
    myrtsimgen->reset();    
  }

  env->ReleaseStringUTFChars(fdl, fdlname);
}

JNIEXPORT void JNICALL Java_GezelModule_tick(JNIEnv * env, jobject obj) {
  myrtsimgen->step();
}
