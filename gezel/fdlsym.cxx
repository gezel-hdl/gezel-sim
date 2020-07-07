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
// $Id: fdlsym.cxx 104 2009-05-19 14:35:17Z schaum $
//--------------------------------------------------------------

#include "fdlsym.h"
#include "fdlcode.h"

#include <string>

//void symnamecopy(char *to, char *from) {
//  if (strlen(from) > MAXSYMLEN) {
//    strncpy(to, from, MAXSYMLEN-1);
//    to[MAXSYMLEN-1] = 0;
//  } else {
//    strcpy(to, from);
//  }
//}

namesym::namesym(const char *_staticname) {
//  symnamecopy(nm, _staticname);
#ifdef MSC_VER
  nm = _strdup(_staticname);
#else
  nm = strdup(_staticname);
#endif
}

namesym::~namesym() {
  delete nm;
}

void namesym::show(ostream &os) {
  os << nm;
}

bool namesym::matchname(const char *s) {
  if (!strcmp(nm, s))
    return true;
  else
    return false;
}

void namesym::cgtracer(acodegen *CG, symid v) {
  CG->cgtracer(v);
}

void namesym::cgoption(acodegen *CG, symid v) {
  CG->cgoption(v, nm);
}

void namesym::cgfile(acodegen *CG, symid v) {
  CG->cgfile(v, nm);
}

void namesym::cgdp(acodegen *CG, symid v) {
  CG->cgdp(v, nm);
}

void namesym::cgdpclone(acodegen *CG, symid v, symid p) {
  CG->cgdpclone(v, nm, p);
}

void namesym::cgsfg(acodegen *CG, symid v) {
  CG->cgsfg(v, nm);
}

void namesym::cgipblock(acodegen *CG, symid v) {
  symbol *s = glbSymboltable.getsymbol(glbSymboltable.findiptype(v));
  if (s) {
    CG->cgipblock(v, nm, s->content()->getname());
  }
}

void namesym::cgipblockclone(acodegen *CG, symid v) {
  cgipblock(CG, v);
}

void namesym::cginput(acodegen *CG, symid v, symid dp) {
  symbol *s = glbSymboltable.getsymbol(glbSymboltable.findtype(v));
  if (s) {
    asym *t = s->content();
    CG->cginput(v, t->typew(), t->typesign(), dp, nm);
  }
}

void namesym::cgoutput(acodegen *CG, symid v, symid dp) {
  symbol *s = glbSymboltable.getsymbol(glbSymboltable.findtype(v));
  if (s) {
    asym *t = s->content();
    CG->cgoutput(v, t->typew(), t->typesign(), dp, nm);
  }
}

void namesym::cgipout(acodegen *CG, symid v, symid dp) {
  symbol *s = glbSymboltable.getsymbol(glbSymboltable.findtype(v));
  if (s) {
    asym *t = s->content();
    CG->cgipout(v, t->typew(), t->typesign(), dp, nm);
  }
}

void namesym::cgipparm(acodegen *CG, symid v, symid dp) {
  CG->cgipparm(v, nm, dp);
}

void namesym::cgreg(acodegen *CG, symid v, symid dp) {
  symbol *s = glbSymboltable.getsymbol(glbSymboltable.findtype(v));
  if (s) {
    asym *t = s->content();
    CG->cgreg(v, t->typew(), t->typesign(), dp, nm);
  }
}

void namesym::cgsig(acodegen *CG, symid v, symid dp) {
  symbol *s = glbSymboltable.getsymbol(glbSymboltable.findtype(v));
  if (s) {
    asym *t = s->content();
    CG->cgsig(v, t->typew(), t->typesign(), dp, nm);
  }
}

void namesym::cglutable(acodegen *CG, symid v, symid dp) {
  // locate type
  symbol *s = glbSymboltable.getsymbol(glbSymboltable.findtype(v));
  vector<char *> lucontent;
  glbSymboltable.findlutable(v, lucontent);
  if (s) {
    asym *t = s->content();
    CG->cglutable(v, t->typew(), t->typesign(), lucontent, dp, nm);
  }
}

void namesym::cglucontent(vector<char *> &luc) {
  luc.push_back(nm);
}

void namesym::cgconst(acodegen *CG, symid v) {
  CG->cgconst(v, nm);
}

void namesym::cgdispstr(acodegen *CG, symid dispid, symid ) {
  CG->cgdispstr(dispid, nm);
}

void namesym::cgfsm_state(acodegen *CG, symid v, symid c) {
  CG->cgfsm_state(v, c, nm);
}

void namesym::cgfsm_initial(acodegen *CG, symid v, symid c) {
  CG->cgfsm_initial(v, c, nm);
}

//--------------------------------------------------------------
intsym::intsym(int _v) : v(_v) {}

intsym::~intsym() {}

void intsym::show(ostream &os) {
  os << "(v=" << v << ")";
}

void intsym::cgdispbase(acodegen *CG, symid dispid, symid) {
  CG->cgdispbase(dispid, v);
}

//--------------------------------------------------------------

sigtypesym::sigtypesym(reptype _rep, int _wlen) : 
  rep(_rep), wlen(_wlen) {}

sigtypesym::~sigtypesym() {}

void sigtypesym::show(ostream &os) {
  os << "(rep=";
  if (rep == tc)
    os << "tc";
  else
    os << "ns";
  os << ",w=" << wlen << ")"; 
}

//--------------------------------------------------------------
ternaryopsym::ternaryopsym(symid _left, symid _middle, symid _right) :
  left(_left), middle(_middle), right(_right) {}

ternaryopsym::~ternaryopsym() {}

void ternaryopsym::show(ostream &os) {
  os << "(L=" << left << ", M=" << middle << ", R=" << right << ")"; 
}

void ternaryopsym::cgsel(acodegen *CG, symid v) {
  CG->cgsel(v, left, middle, right);
}

void ternaryopsym::findoperands(vector<symid> &ops) {
  ops.push_back(left);
  ops.push_back(middle);
  ops.push_back(right);
}

//--------------------------------------------------------------
binaryopsym::binaryopsym(symid _left, symid _right) :
  left(_left), right(_right) {}

binaryopsym::~binaryopsym() {}

void binaryopsym::show(ostream &os) {
  os << "(L=" << left << ", R=" << right << ")"; 
}

void binaryopsym::cgasgn(acodegen *CG, symid v, symid sfgid) {
  CG->cgassign(v, sfgid, left, right);
}

void binaryopsym::cgior(acodegen *CG, symid v) {
  CG->cgior(v, left, right);
}

void binaryopsym::cgxor(acodegen *CG, symid v) {
  CG->cgxor(v, left, right);
}

void binaryopsym::cgand(acodegen *CG, symid v) {
  CG->cgand(v, left, right);
}

void binaryopsym::cgshl(acodegen *CG, symid v) {
  CG->cgshl(v, left, right);
}

void binaryopsym::cgshr(acodegen *CG, symid v) {
  CG->cgshr(v, left, right);
}

void binaryopsym::cgadd(acodegen *CG, symid v) {
  CG->cgadd(v, left, right);
}

void binaryopsym::cgconcat(acodegen *CG, symid v) {
  CG->cgconcat(v, left, right);
}

void binaryopsym::cgsub(acodegen *CG, symid v) {
  CG->cgsub(v, left, right);
}

void binaryopsym::cgmul(acodegen *CG, symid v) {
  CG->cgmul(v, left, right);
}

void binaryopsym::cgmod(acodegen *CG, symid v) {
  CG->cgmod(v, left, right);
}

void binaryopsym::cgeq(acodegen *CG, symid v) {
  CG->cgeq(v, left, right);
}

void binaryopsym::cgne(acodegen *CG, symid v) {
  CG->cgne(v, left, right);
}

void binaryopsym::cggrt(acodegen *CG, symid v) {
  CG->cggrt(v, left, right);
}

void binaryopsym::cgsmt(acodegen *CG, symid v) {
  CG->cgsmt(v, left, right);
}

void binaryopsym::cggrteq(acodegen *CG, symid v) {
  CG->cggrteq(v, left, right);
}

void binaryopsym::cgsmteq(acodegen *CG, symid v) {
  CG->cgsmteq(v, left, right);
}

void binaryopsym::cgcast(acodegen *CG, symid v) {
  // first find type info
  symbol *s = glbSymboltable.getsymbol(left);
  if (s) {
    asym *t = s->content();
    CG->cgcast(v, right, t->typew(), t->typesign());
  }
}

void binaryopsym::findoperands(vector<symid> &ops) {
  ops.push_back(left);
  ops.push_back(right);
}

//--------------------------------------------------------------
unaryopsym::unaryopsym(symid _left) :
  left(_left) {}

unaryopsym::~unaryopsym() {}

void unaryopsym::show(ostream &os) {
  os << "(L=" << left << ")";
}

void unaryopsym::cgneg(acodegen *CG, symid v) {
  CG->cgneg(v, left);
}

void unaryopsym::cgnot(acodegen *CG, symid v) {
  CG->cgnot(v, left);
}

void unaryopsym::findoperands(vector<symid> &ops) {
  ops.push_back(left);
}

//--------------------------------------------------------------
sfgexecsym::sfgexecsym(symid _sfgsym, symid _dpsym) : 
  sfgsym(_sfgsym), dpsym(_dpsym) {}

sfgexecsym::~sfgexecsym() {}

void sfgexecsym::show(ostream &os) {
  os << "(D=" << dpsym << ",S=" << sfgsym << ")";
}

void sfgexecsym::cgexec(acodegen *CG, symid controller) {
  CG->cgexec(controller, dpsym, sfgsym);
}

//--------------------------------------------------------------
pinsym::pinsym(symid _dpio, symid _dpsym) :
  dpio(_dpio), dpsym(_dpsym) {}

pinsym::~pinsym() {}

void pinsym::show(ostream &os) {
  os << "(D=" << dpsym << ", P=" << dpio << ")"; 
}

bool pinsym::pinpoints(symid p) {
  return (dpio == p);
}

void pinsym::cgdpc_arg(acodegen *CG, 
		       symid parentdp, symid parentpin) {
  CG->cgdpc_arg(parentdp, parentpin, dpsym, dpio);
}

//--------------------------------------------------------------
refsym::refsym(symid _ref) :
  ref(_ref) {}

refsym::~refsym() {}

void refsym::show(ostream &os) {
  os << "(V=" << ref << ")"; 
}

void refsym::cgdispvar(acodegen *CG, symid dispid, symid v) {
  CG->cgdispvar(dispid, ref);
}

void refsym::cgctltrue (acodegen *CG, symid v,   symid c, symid parentc) {
  CG->cgctltrue(v, c, ref, parentc);
}

void refsym::cgctlfalse(acodegen *CG, symid v,   symid c, symid parentc) {
  CG->cgctlfalse(v, c, ref, parentc);
}

void refsym::cgsysdp(acodegen *CG, symid v) {
  CG->cgsysdp(ref);
}

void refsym::findoperands(vector<symid> &ops) {
  ops.push_back(ref);
}

//--------------------------------------------------------------
hrefsym::hrefsym(symid _ref) :
  ref(_ref) {}

hrefsym::~hrefsym() {}

void hrefsym::show(ostream &os) {
  os << "(V=" << ref << ")"; 
}

void hrefsym::cgdpc_dp(acodegen *CG, symid v, symid parentdp) {
  CG->cgdpc_dp(ref, parentdp);
}

void hrefsym::cgdpc_ipblock(acodegen *CG, symid v, symid parentdp) {
  CG->cgdpc_ipblock(ref, parentdp);
}


//--------------------------------------------------------------
indexsym::indexsym(int _idx, int _len, symid _parent) :
  idx(_idx), len(_len), parent(_parent) {}

indexsym::~indexsym() {}

void indexsym::show(ostream &os) {
  os << "[" << idx;
  if (len > 1) {
    os << ":" << idx+len-1 << "]"; 
  } else {
    os << "]"; 
  }
  os << " " << "P=" << parent;
}

void indexsym::cgindex(acodegen *CG, symid v, symid basevar) {
  CG->cgindex(v, basevar, idx, len);
}

void indexsym::findoperands(vector<symid> &ops) {
  ops.push_back(parent);
}


//--------------------------------------------------------------
lusym::lusym(symid _ref, symid _parent) : ref(_ref), parent(_parent) {}

lusym::~lusym() {}

void lusym::show(ostream &os) {
  os << "(I=" << ref << ", Base=" << parent << ")";
}

void lusym::cgluop     (acodegen *CG, symid v, symid dp) {
  CG->cgluop(v, ref, parent);
}

void lusym::findoperands(vector<symid> &ops) {
  ops.push_back(parent);
  ops.push_back(ref);
}

//--------------------------------------------------------------
ctlsym::ctlsym(const char *_staticname, symid _dp) : dp(_dp) {
//  symnamecopy(nm, _staticname);
#ifdef MSC_VER
  nm = _strdup(_staticname);
#else
  nm = strdup(_staticname);
#endif
}

ctlsym::~ctlsym() {
  delete nm;
}

void ctlsym::show(ostream &os) {
  os << "(" << nm << ", dp=" << dp << ")"; 
}

void ctlsym::cgseq_ctl(acodegen *CG, symid v) {
  CG->cgseq_ctl(v, dp);
}

void ctlsym::cghwire_ctl(acodegen *CG, symid v) {
  CG->cghwire_ctl(v, dp);
}

void ctlsym::cgfsm_ctl(acodegen *CG, symid v) {
  CG->cgfsm_ctl(v, dp);
}


//--------------------------------------------------------------
attrsym::attrsym(char *_staticname, symid _ref) : ref(_ref) {
//  symnamecopy(nm, _staticname);
#ifdef MSC_VER
  nm = _strdup(_staticname);
#else
  nm = strdup(_staticname);
#endif
}

attrsym::~attrsym() {
  delete nm;
}

void attrsym::show(ostream &os) {
  os << "(" << nm << ", ref=" << ref << ")"; 
}

void attrsym::cgtracesig(acodegen *CG, symid v) {
  CG->cgtracesig(v, ref, nm); 
}

//--------------------------------------------------------------
ctlstepsym::ctlstepsym(int _idx, symid _c) :
  idx(_idx), condition(_c) {}

ctlstepsym::~ctlstepsym() {}

void ctlstepsym::show(ostream &os) {
  os << "(#=" << idx << ", C=" << condition << ")";
}

void ctlstepsym::cgctlstep(acodegen *CG, symid v, symid c) {
  CG->cgctlstep(v, c, idx, condition); // context points to condition ref or NOSYMBOL
}

//--------------------------------------------------------------
transitionsym::transitionsym(symid _from, symid _to) : from(_from), to(_to) {}

transitionsym::~transitionsym() {}

void transitionsym::show(ostream &os) {
  os << from << "->" << to; 
}

void transitionsym::cgfsm_trans(acodegen *CG, symid v, symid c) {
  CG->cgfsm_trans(v, c, from, to);
}
