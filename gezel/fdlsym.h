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


#ifndef FDLSYM_H
#define FDLSYM_H

#include <iostream>

using namespace std;

#include "asymbol.h"

//-------------------------------------------------------------------

class namesym : public asym {
//  char nm[MAXSYMLEN];
  char *nm;
 public:
  namesym(const char *_staticname);
  char *getname() { return nm; }
  virtual ~namesym();
  bool matchname(const char *n);
  void show(ostream &os);
  void showlex(ostream &os) {
    os << nm;
  }
  asym * clone(symid ofs) {
    return new namesym(nm);
  }

  void cgoption     (acodegen *, symid v);
  void cgfile       (acodegen *, symid v);
  void cgdp         (acodegen *, symid v);
  void cgdpclone    (acodegen *, symid v, symid p);
  void cgipblock    (acodegen *, symid v);
  void cgipblockclone (acodegen *, symid v);
  void cgipout  (acodegen *, symid v, symid dp);
  void cgipparm (acodegen *, symid v, symid dp);
  void cgsfg   (acodegen *, symid v);
  void cginput (acodegen *, symid v, symid dp);
  void cgoutput(acodegen *, symid v, symid dp);
  void cgreg   (acodegen *, symid v, symid dp);
  void cgsig   (acodegen *, symid v, symid dp);
  void cgconst (acodegen *, symid v);
  void cglutable(acodegen *, symid v, symid dp);
  void cglucontent(vector<char *> &luc); // called as part of cglutable
  void cgtracer(acodegen *CG, symid v);  // $trace for controlllers
  
  void cgdispstr    (acodegen *, symid dispid, symid n);
  void cgfsm_state  (acodegen *, symid v,      symid c);
  void cgfsm_initial(acodegen *, symid v,      symid c);

};

//-------------------------------------------------------------------

class intsym : public asym {
  int      v;
 public:
  intsym(int _v);
  ~intsym();
  void show(ostream &os);
  asym * clone(symid ofs) {
    return new intsym(v);
  }
  void cgdispbase(acodegen *, symid dispid, symid c);
};

//-------------------------------------------------------------------

class sigtypesym : public asym {
 public:
  enum reptype {tc, ns};
 private:
  reptype rep;
  int      wlen;
 public:
  sigtypesym(reptype rep, int wlen);
  int  typew() {return wlen;}
  int  typesign() {return (rep==tc) ? 1 : 0;}
  ~sigtypesym();
  void show(ostream &os);
  asym * clone(symid ofs) {
    return new sigtypesym(rep, wlen);
  }
};

//-------------------------------------------------------------------

class ternaryopsym : public asym {
  symid left, middle, right;
 public:
  ternaryopsym(symid _left, symid _middle, symid _right);
  symid l() {return left;}
  symid m() {return middle;}
  symid r() {return right;}
  ~ternaryopsym();
  void show(ostream &os);
  void cgsel  (acodegen *, symid v);
  asym * clone(symid ofs) {
    return new ternaryopsym(left+ofs, middle+ofs, right+ofs);
  }
  void  findoperands(vector<symid> &ops);
};

//-------------------------------------------------------------------

class binaryopsym : public asym {
  symid left, right;
 public:
  binaryopsym(symid _left, symid _right);
  symid l() {return left;}
  symid r() {return right;}
  ~binaryopsym();
  void show(ostream &os);
  asym * clone(symid ofs) {
    return new binaryopsym(left+ofs, right+ofs);
  }
  void cgasgn (acodegen *, symid v, symid sfg);
  void cgior  (acodegen *, symid v);
  void cgxor  (acodegen *, symid v);
  void cgand  (acodegen *, symid v);
  void cgshl  (acodegen *, symid v);
  void cgshr  (acodegen *, symid v);
  void cgadd  (acodegen *, symid v);
  void cgconcat(acodegen *, symid v);
  void cgsub  (acodegen *, symid v);
  void cgmul  (acodegen *, symid v);
  void cgmod  (acodegen *, symid v);
  void cgcast (acodegen *, symid v);
  void cgeq   (acodegen *, symid v);
  void cgne   (acodegen *, symid v);
  void cggrt  (acodegen *, symid v);
  void cgsmt  (acodegen *, symid v);
  void cggrteq(acodegen *, symid v);
  void cgsmteq(acodegen *, symid v);
  void  findoperands(vector<symid> &ops);

};

//-------------------------------------------------------------------

class unaryopsym : public asym {
  symid left;
 public:
  unaryopsym(symid _left);
  symid l() {return left;}
  ~unaryopsym();
  void show(ostream &os);
  asym * clone(symid ofs) {
    return new unaryopsym(left+ofs);
  }
  void cgneg  (acodegen *, symid v);
  void cgnot  (acodegen *, symid v);
  void  findoperands(vector<symid> &ops);
};

//-------------------------------------------------------------------

class sfgexecsym : public asym {
  symid sfgsym;
  symid dpsym;
 public:
  sfgexecsym (symid sfgsym, symid dpsym);
  virtual ~sfgexecsym();
  void show  (ostream &os);
  asym * clone(symid ofs) {
    return new sfgexecsym(sfgsym+ofs, dpsym+ofs);
  }
  void cgexec(acodegen *, symid controller);
};

//-------------------------------------------------------------------

class pinsym : public asym {
  symid dpio;
  symid dpsym;
 public:
  pinsym(symid _dpio, symid _dpsym);
  virtual ~pinsym();
  void show(ostream &os);
  asym * clone(symid ofs) {
    return new pinsym(dpio, dpsym);
  }
  bool pinpoints(symid p);
  void relocate(symid id) { // pinsym are not cloned but relocated explicitly
                            // i.e. new offset for dpsym is calculated by the code generator
                            // note: a pinsym is used for either 'use' statements or
                            // for 'system' statements. The relocate is used for 'use'
                            // statements only.
    int newofs = (id - dpsym);
    dpsym = id;
    dpio += newofs;
  }
  void cgdpc_arg(acodegen *, symid parentdp, symid parentpin);
};

//-------------------------------------------------------------------

class refsym : public asym {
  symid ref;
 public:
  refsym(symid _ref);
  virtual ~refsym();
  void show(ostream &os);
  asym * clone(symid ofs) {
    return new refsym(ref+ofs);
  }
  symid unref() {
    return ref;
  }
  void cgdispvar  (acodegen *, symid dispid, symid v);
  void cgctltrue  (acodegen *, symid v,   symid c, symid parentc);
  void cgctlfalse (acodegen *, symid v,   symid c, symid parentc);
  void cgsysdp    (acodegen *, symid v);
  void findoperands(vector<symid> &ops);
};

//-------------------------------------------------------------------

class hrefsym : public asym {
  symid ref;
 public:
  hrefsym(symid _ref);
  virtual ~hrefsym();
  void show(ostream &os);
  asym * clone(symid ofs) {
    return new hrefsym(ref);
  }
  symid unref() {
    return ref;
  }
  void relocate(symid id) {
    ref = id;
  }
  void cgdpc_dp     (acodegen *, symid v, symid parentdp);
  void cgdpc_ipblock(acodegen *, symid v, symid parentdp);
};

//-------------------------------------------------------------------

class indexsym : public asym {
  int idx;
  int len;
  symid parent;
 public:
  indexsym(int _idx, int _len, symid _parent);
  virtual ~indexsym();
  void show(ostream &os);
  asym * clone(symid ofs) {
    return new indexsym(idx, len, parent+ofs);
  }
  void cgindex(acodegen *, symid dispid, symid v);
  void findoperands(vector<symid> &ops);
};

//-------------------------------------------------------------------

class lusym : public asym {
  symid ref;
  symid parent;
 public:
  lusym(symid ref, symid _parent);
  virtual ~lusym();
  void show(ostream &os);
  asym * clone(symid ofs) {
    return new lusym(ref+ofs, parent+ofs);
  }
  void cgluop     (acodegen *, symid v, symid dp);
  void findoperands(vector<symid> &ops);
};

//-------------------------------------------------------------------

class ctlsym : public asym {
  symid dp;
//  char nm[MAXSYMLEN];
  char *nm;
 public:
  ctlsym(const char *_staticname, symid _dp);
  virtual ~ctlsym();
  void show(ostream &os);
  void showlex(ostream &os) {
    os << nm;
  }
  asym * clone(symid ofs) {
    return new ctlsym(nm, dp+ofs);
  }
  bool ctlofdp(symid p) {
    //    cerr << "out: " << dp << " and " << p << "\n";
    return (p == dp);
  }
  void cgseq_ctl  (acodegen *CG, symid v);
  void cghwire_ctl(acodegen *CG, symid v);
  void cgfsm_ctl  (acodegen *CG, symid v);
};

//-------------------------------------------------------------------

class attrsym : public asym {
  symid ref;
//  char nm[MAXSYMLEN];
  char *nm;
 public:
  attrsym(char *_staticname, symid _dp);
  virtual ~attrsym();
  void show(ostream &os);
  void showlex(ostream &os) {
    os << nm;
  }
  asym * clone(symid ofs) {
    return new attrsym(nm, ref+ofs);
  }
  void cgtracesig (acodegen *CG, symid v);
};

//-------------------------------------------------------------------

class ctlstepsym : public asym {
  int idx;
  int condition;
 public:
  ctlstepsym(int _idx, symid _condition);
  virtual ~ctlstepsym();
  void show(ostream &os);
  asym * clone(symid ofs) {
    return new ctlstepsym(idx, (condition == NOSYMBOL) ? NOSYMBOL : condition+ofs);
  }
  void cgctlstep(acodegen *, symid, symid);
};

//-------------------------------------------------------------------

class transitionsym : public asym {
  symid from, to;
 public:
  transitionsym(symid from, symid to);
  virtual ~transitionsym();
  void show(ostream &os);
  asym * clone(symid ofs) {
    return new transitionsym(from+ofs, to+ofs);
  }
  void cgfsm_trans(acodegen *CG, symid v, symid c);
};


#endif


