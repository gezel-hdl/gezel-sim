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
// $Id: GezelOutportImp.cxx 21 2005-11-15 19:08:03Z schaum $
//--------------------------------------------------------------

#include "GezelOutport.h"
#include "rtsim.h"
#include "java_itf.h"
#include <string>

//-- gezel side
class javasink;

map <string, javasink *> javasinklist;

javasink::javasink(char *name) : aipblock(name) {}

void javasink::setparm(char *_name) {
  char *sysvar;
  if ((sysvar = matchparm(_name, "var"))) {
    cerr << "javasink: set variable " << sysvar << "\n";
    javasinklist[string(sysvar)] = this;
  }
}

void javasink::run() {
  //  cerr << "javasink run\n";
}

bool javasink::checkterminal(int n, char *tname, aipblock::iodir d) {
  switch (n) {
  case 0:
    return (isinput(d) && isname(tname, "data"));
    break;
  }
  return false;
}

bool javasink::cannotSleepTest() {
  return false;
}

//-- java side

map <jint, javasink *> javaoutportlist;

JNIEXPORT void JNICALL Java_GezelOutport_portname(JNIEnv *env, jobject obj, jstring str) {

  const char *portname = env->GetStringUTFChars(str, 0);

  // the field 'portId' must contain an object-unique identifier that can be retrieved
  // later in the read() method. This object-unique identifier allows to get back
  // to the corresponding GEZEL port.
  // Ideally, we would want to use the jobject parameter for this, instead of creating
  // these identifier articificially. However, the jobject argument does not seem to
  // be constant over different native method invocations !!

  jclass   cls      = env->GetObjectClass(obj);
  jfieldID jfid     = env->GetFieldID(cls, "portId", "I");
  jint     myPortId = env->GetIntField(obj, jfid);

  if (javasinklist.find(string(portname)) != javasinklist.end()) {
    javaoutportlist[myPortId] = javasinklist[string(portname)];
  } else {
    cerr << "Warning - GEZEL outport does not include signal " << portname << "\n";
  }
  
  env->ReleaseStringUTFChars(str, portname);
}

JNIEXPORT jint JNICALL Java_GezelOutport_read(JNIEnv *env, jobject obj) {
  jclass   cls      = env->GetObjectClass(obj);
  jfieldID jfid     = env->GetFieldID(cls, "portId", "I");
  jint     myPortId = env->GetIntField(obj, jfid);

  if (javaoutportlist.find(myPortId) == javaoutportlist.end()) {
    cerr << "Error - Did not find port for " << myPortId << "\n";
    return 0;
  } else {
    return (jint) javaoutportlist[myPortId]->ioval[0]->toslong();
  }
}


