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
// $Id: rtio.h 78 2007-07-14 19:00:16Z schaum $
//--------------------------------------------------------------

#ifndef RTIO_H
#define RTIO_H

#include "rtsignal.h"

//------------------------------------------------
//  ###   #######
//   #    #     #
//   #    #     #
//   #    #     #
//   #    #     #
//   #    #     #
//  ###   #######

class rtoutput : public artsignal {
  rtsignal      s;
 public:
  rtoutput(rtdpcontext & _rtdpc, symid _id, int wlen, int sign);
  void    newdef(symid _sfg, artsignal * _definer);
  void    defdriver(artsignal* _driver);
  virtual gval & eval(unsigned long g);
  virtual void    asgn(gval &nv);
  virtual gval &  peekval();
  virtual int wlenreq();
  virtual unsigned signreq();
  void    show(ostream &os);
  void    setbase(int base);
  bool    isgeneration(unsigned long generation);
};

class rtfile : public artsignal {
  gval *v;
  ifstream is;
 public:
  rtfile(symid _id, char *_filename);
  virtual gval & eval(unsigned long g);
  virtual gval &  peekval();
};

class rtinput : public artsignal {
  gval *v;
  artsignal *driver;
 public:
  rtinput(symid _id, int wlen, int sign);
  void defdriver(artsignal *p);
  virtual gval & eval(unsigned long g);
  virtual gval & peekval();
  virtual int wlenreq();
  virtual unsigned signreq();
  void    show(ostream &os);
  void    setbase(int base);
  bool    isgeneration(unsigned long generation);
  //  bool    usleep_m();
};

//------------------------------------------------
// debug level object
class rtadisp {
 public:
  rtadisp() {}
  virtual ~rtadisp() {}
  virtual void show(ostream &os, unsigned long) {}
  virtual void setbase(int base) {}
};

class rtsigdisp : public rtadisp {
  artsignal *s;
 public:
  rtsigdisp(artsignal *);
  void setbase(int base);
  void show(ostream &os, unsigned long);
};

class rtstrdisp : public rtadisp {
  char *s;
 public:
  rtstrdisp(char *);
  void show(ostream &os, unsigned long);
};

class rtcycledisp : public rtadisp {
 public:
  rtcycledisp();
  void show(ostream &os, unsigned long);
};

class rttoggledisp : public rtadisp {
 public:
  rttoggledisp();
  void show(ostream &os, unsigned long);
};

class rtonesdisp : public rtadisp {
 public:
  rtonesdisp();
  void show(ostream &os, unsigned long);
};

class rtzeroesdisp : public rtadisp {
 public:
  rtzeroesdisp();
  void show(ostream &os, unsigned long);
};

class rtdp ;

class rtdpdisp : public rtadisp {
  rtdp *dp;
 public:
  rtdpdisp(rtdp *_dp);
  void show(ostream &os, unsigned long);
};


class rtdirective {
 public:
  rtdirective() {}
  virtual ~rtdirective() {}
  virtual void addvar(artsignal *s) {}
  virtual void addstring(char *c) {}
  virtual void addcycle() {}
  virtual void addtoggle() {}
  virtual void addones() {}
  virtual void addzeroes() {}
  virtual void adddp() {}
  virtual void changebase(int v) {}
  virtual bool canshow() {return true;}
  virtual void show(ostream &os, unsigned long g) {}
};

class rtdisplay : public rtdirective {
  symid id, sfg;
  rtdp *dp;
  int defaultbase;
  vector<rtadisp *> displaylist;
  vector<int> currentbase;
 public:
  rtdisplay(symid _id, symid _sfg, rtdp *_dp);
  ~rtdisplay();
  void addvar(artsignal *s);
  void adddp ();
  void addstring(char *c);
  void addcycle();
  void addtoggle();
  void addones();
  void addzeroes();
  void changebase(int v);
  bool canshow();
  void show(ostream &os, unsigned long g);
};

class rtfinish  : public rtdirective {
  symid id, sfg;
  rtdp *dp;
public:
  rtfinish(symid _id, symid _sfg, rtdp *_dp);
  ~rtfinish();

  void addvar(artsignal *s) {}
  void addstring(char *c) {}
  void addcycle() {}
  void addtoggle() {}
  void addones() {}
  void addzeroes() {}
  void changebase(int v) {}


  bool canshow();
  void show(ostream& os, unsigned long g);
};

#endif
