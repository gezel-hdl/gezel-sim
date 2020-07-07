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

#ifndef RTTRACE_H
#define RTTRACE_H

#include "artsignal.h"
#include "rtopt.h"
#include <fstream>

class tracerdumper {
  vector<gval *> buffer;
  ofstream os;
  const unsigned vlen; // #values in trace before flush
  unsigned wptr;
 public:
  tracerdumper(char *file);
  ~tracerdumper();
  void addval(gval *v);
  void setwl(int wl);
};

class vcddumper {
  const char *fname;
  vector<gval *> signals;
  int change_flag;
  int idnum;     // contains number of trace objects registered
  ofstream os;

 public:
  vcddumper(const char *_fname);
  int reg_trace (artsignal *_sig);
  void update (int id, gval *v);
  void eval(unsigned long);
};


class tracerblock {
  char        *fname;
  artsignal   *sig;
  vcddumper   *vcd;
  tracerdumper td;
  int id;

 public:
  tracerblock(char *_fname, artsignal *_sig);
  tracerblock(char *_fname, artsignal *_sig, vcddumper *_vcd);
  bool isdupfile(char *_fname);
  void eval(unsigned long);
};


#endif
