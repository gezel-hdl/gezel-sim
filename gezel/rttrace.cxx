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

#include "rttrace.h"
#include <cstdlib>

//-----------------------------------------------------

tracerdumper::tracerdumper(char *file) : os(file), vlen(1024) {
  if (os.fail()) {
    cout << "Error: Unable to open file " << file << " for writing\n";
    exit(0);
  }
  for (unsigned i=0; i<vlen; ++i) {
    buffer.push_back(make_gval("0xdeadbeefdeadbeef"));
  }
  wptr = 0;
}

void tracerdumper::setwl(int wl) {
  for (unsigned i=0; i<vlen; ++i) {
    buffer[i]->setwordlength(wl);
    buffer[i]->setbase(2);         // binary
  }
}

tracerdumper::~tracerdumper() {
  for (unsigned i=0; i<wptr; ++i) { // flush
    os << *buffer[i] << "\n";
  }
  os.close();
}

void tracerdumper::addval(gval *v) {
  *(buffer[wptr++]) = *v;
  //  cerr << wptr << "\n";
  if (wptr >= vlen) { // flush
    for (unsigned i=0; i<vlen; ++i) {
      //      cerr << "flush " << i << "\n";
      os << *buffer[i] << "\n";
    }
    wptr = 0;
  }
}

//-----------------------------------------------------

tracerblock::tracerblock(char *_fname, artsignal *_sig) :
  fname(_fname), sig(_sig), td(_fname) {
  td.setwl(_sig->wlenreq());
}


tracerblock::tracerblock(char *_fname, artsignal *_sig, vcddumper *_vcd) :
  fname(_fname), sig(_sig), vcd(_vcd), td(_fname) {
  td.setwl(_sig->wlenreq());
  if (vcd)  id = vcd->reg_trace(sig);
}

bool tracerblock::isdupfile(char *_fname) {
  return (strcmp(_fname, fname) == 0);
}

void tracerblock::eval(unsigned long C) {
  td.addval(&(sig->eval(C)) );
  if (vcd) vcd->update(id, &(sig->eval(C)));
}


//-----------------------------------------------------

vcddumper::vcddumper (const char*_fname) :
  fname(_fname), os(_fname)  {
  if (os.fail()) {
    cout << "Error: Unable to open file " << fname << " for writing\n";
    exit(0);
  }
  idnum = 0;
  for (unsigned i=0; i<93; ++i) {
    signals.push_back(make_gval("0b0"));
  }
} 


int vcddumper::reg_trace (artsignal *_sig) {
  char buffA[255];
  char buffB[255];
  symid n;

  n = _sig->id;
  strcpy(buffB, glbSymboltable.getsymbol(n)->content()->getname());
  while (1) {
    n = glbSymboltable.getsymbol(n)->context();
    if (n == -1) break;
    strcpy(buffA, glbSymboltable.getsymbol(n)->content()->getname());
    strcat(buffA, ".");
    strcat(buffA,buffB);
    strcpy(buffB,buffA);
  }

  os << "$var reg \t" << _sig->wlenreq() 
     << " " << char(idnum+33) << "\t" 
     << buffA
     << "  $end\n"; 
  (signals[idnum])->setwordlength( _sig->wlenreq());
  return (idnum++);
}


void vcddumper::update (int id, gval *v) {
  if (*(signals[id]) == *v ) return;
  *(signals[id]) = *v ;
  change_flag = 1;
}



void vcddumper::eval (long unsigned int _cycle) {
  if (_cycle ==0) 
    os << "$enddefinitions $end \n";

  if (change_flag==0) return;

  change_flag = 0;
  os << "#" << _cycle << endl;
  for (int i=0; i<idnum; i++){
    os << "b"; 
    os << *(signals[i]); 
    os << " " << char(i+33) << endl;
  }
}


