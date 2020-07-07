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


#include "GezelInport.h"
#include "rtsim.h"
#include "java_itf.h"
#include <string>

//--- gezel side
class javasource;

map <string, javasource *> javasourcelist;

javasource::javasource(char *name) : aipblock(name), interfacewritten(false) {}

void javasource::setparm(char *_name) {
  char *sysvar;
  if ((sysvar = matchparm(_name, "var"))) {
    cerr << "javasource: set variable " << sysvar << "\n";
    javasourcelist[string(sysvar)] = this;
  }
}

void javasource::run() {}

bool javasource::checkterminal(int n, char *tname, aipblock::iodir d) {
  switch (n) {
  case 0:
    return (isoutput(d) && isname(tname, "data"));
    break;
  }
  return false;
}

bool javasource::needsWakeupTest() {
  bool v = interfacewritten;
  interfacewritten = false;
  return v;
}

bool javasource::cannotSleepTest() {
  bool v = interfacewritten;
  interfacewritten = false;
  return v;
}

void javasource::touch() {
  interfacewritten = true;
}

//--- java side

map <jint, javasource *> javainportlist;

JNIEXPORT void JNICALL Java_GezelInport_portname(JNIEnv *env, jobject obj, jstring str) {

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

  if (javasourcelist.find(string(portname)) != javasourcelist.end()) {
    javainportlist[myPortId] = javasourcelist[string(portname)];
  } else {
    cerr << "Warning - GEZEL inport does not include signal " << portname << "\n";
  }
  
  env->ReleaseStringUTFChars(str, portname);
}

JNIEXPORT void JNICALL Java_GezelInport_write(JNIEnv *env, jobject obj, jint n) {
  jclass   cls      = env->GetObjectClass(obj);
  jfieldID jfid     = env->GetFieldID(cls, "portId", "I");
  jint     myPortId = env->GetIntField(obj, jfid);

  javainportlist[myPortId]->ioval[0]->assignslong(n);
  javainportlist[myPortId]->touch();
}
