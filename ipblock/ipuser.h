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

#ifndef IPUSER_H
#define IPUSER_H

#include "ipblock.h"

//  #     #  #####  ####### ######
//  #     # #     # #       #     #
//  #     # #       #       #     #
//  #     #  #####  #####   ######
//  #     #       # #       #   #
//  #     # #     # #       #    #
//   #####   #####  ####### #     #

class tracer : public aipblock {
  tracerdumper *td;
 public:
  tracer(char *);
  ~tracer();
  void setparm(char *_name);
  void run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool cannotSleepTest();
};

class filesource : public aipblock {
  ifstream *f;
  gval *rv;
  int  nfields;
 public:
  filesource(char *);
  ~filesource();
  void setparm(char *_name);
  void run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool cannotSleepTest();
};

class ram : public aipblock {
  vector<gval *> content;
  gval *outputreg;
  int wl;

  ifstream *f;
  gval *rv;

  unsigned probestart;
  unsigned probestop;
 public:
  ram(char *);
  void setparm(char *_name);
  void out_run();
  void run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool cannotSleepTest();
  void probe(char *);
  void showcontents(unsigned from, unsigned to);
  void load_file();
};

class wideram : public aipblock {
  vector<gval *> content;
  int  wl;
  int  width;
 public:
  wideram(char *);
  void setparm(char *_name);
  void run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool cannotSleepTest();
};

class gctr32 : public aipblock {
  gval *value, *one;
 public:
  gctr32(char *);
  void run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
};

class ctr32 : public aipblock {
  gval *value, *one;
 public:
  ctr32(char *);
  void run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
};

class gfmul16 : public aipblock {
  unsigned lasta, lastb;
 public:
  gfmul16(char *);
  void run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
};

class rand16 : public aipblock {
  unsigned lasta, lastb;
 public:
  rand16(char *);
  void run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
};

#endif

