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

#ifndef ASYMBOL_H
#define ASYMBOL_H

#include "fdlparse.h" 
#include <cstring>
#include <iostream>
#include <vector>
#include <map>

using namespace std;

typedef long symid;
class acodegen ;
#define NOSYMBOL -1

symid makeuniqid();

class asym {
  // abstract symbol value
 public:
  enum reptype {tc, ns};
  asym();
  virtual ~asym() {}
  virtual bool matchname(const char *) {return false;}
  virtual void show(ostream &os);
  virtual void showlex(ostream &os);
  virtual void relocate(symid id) {} // relocates symbol target to id (for hierarchy expansion)
  virtual asym * clone(symid ofs) {
    return new asym;
  }

  // OPS
  virtual void cgneg    (acodegen *, symid v)                  {}
  virtual void cgnot    (acodegen *, symid v)                  {}
  virtual void cgconst  (acodegen *, symid v)                  {}
  virtual void cgasgn   (acodegen *, symid v, symid sfg)       {}
  virtual void cgior    (acodegen *, symid v)                  {}
  virtual void cgxor    (acodegen *, symid v)                  {}
  virtual void cgand    (acodegen *, symid v)                  {}
  virtual void cgshl    (acodegen *, symid v)                  {}
  virtual void cgshr    (acodegen *, symid v)                  {}
  virtual void cgadd    (acodegen *, symid v)                  {}
  virtual void cgconcat (acodegen *, symid v)                  {}
  virtual void cgsub    (acodegen *, symid v)                  {}
  virtual void cgmul    (acodegen *, symid v)                  {}
  virtual void cgmod    (acodegen *, symid v)                  {}
  virtual void cgcast   (acodegen *, symid v)                  {}
  virtual void cgeq     (acodegen *, symid v)                  {}
  virtual void cgne     (acodegen *, symid v)                  {}
  virtual void cgsmt    (acodegen *, symid v)                  {}
  virtual void cggrt    (acodegen *, symid v)                  {}
  virtual void cgsmteq  (acodegen *, symid v)                  {}
  virtual void cggrteq  (acodegen *, symid v)                  {}

  virtual void cgexec     (acodegen *, symid controller)       {}
  virtual void cgtracer   (acodegen *, symid controller)       {}
  virtual void cgctlstep  (acodegen *, symid v, symid control) {}
  virtual void cgtracesig (acodegen *CG, symid v)              {}
  virtual void cgseq_ctl  (acodegen *CG, symid v)              {}
  virtual void cghwire_ctl(acodegen *CG, symid v)              {}
  virtual void cgfsm_ctl  (acodegen *CG, symid v)              {}
  virtual void cgfsm_state(acodegen *CG, symid v, symid c)     {}
  virtual void cgfsm_initial(acodegen *CG, symid v, symid c)   {}
  virtual void cgctltrue  (acodegen *, symid v, symid c, symid parentc) {}
  virtual void cgctlfalse (acodegen *, symid v, symid c, symid parentc) {}
  virtual void cgfsm_trans(acodegen *CG, symid v, symid c)     {}

  virtual void cgoption     (acodegen *, symid v)                {}
  virtual void cgsel        (acodegen *, symid v)                {}
  virtual void cgfile       (acodegen *, symid v)                {}
  virtual void cgdp         (acodegen *, symid v)                {}
  virtual void cgdpclone    (acodegen *, symid v, symid p)       {}
  virtual void cgipblock    (acodegen *, symid v)                {}
  virtual void cgipblockclone(acodegen *, symid v)                {}
  virtual void cgipblocktype(acodegen *, char *, symid v)      {}
  virtual void cgipparm     (acodegen *, symid v, symid dp)      {}
  virtual void cgipout      (acodegen *, symid v, symid dp)      {}
  virtual void cgsfg        (acodegen *, symid v)                {}
  virtual void cginput      (acodegen *, symid v, symid dp)      {}
  virtual void cgoutput     (acodegen *, symid v, symid dp)      {}
  virtual void cgreg        (acodegen *, symid v, symid dp)      {}
  virtual void cgsig        (acodegen *, symid v, symid dp)      {}
  virtual void cgcom_reg    (acodegen *, symid v, symid dp)      {}
  virtual void cgcom_sig    (acodegen *, symid v, symid dp)      {}
  virtual void cglocal_reg  (acodegen *, symid v, symid dp)      {}
  virtual void cglocal_sig  (acodegen *, symid v, symid dp)      {}
  virtual void cglutable    (acodegen *, symid v, symid dp)      {}
  virtual void cglucontent  (vector<char *> &luc) {} // called as part of cglutable
  virtual void cgluop       (acodegen *, symid v, symid dp)      {}
  virtual void cgindex      (acodegen *, symid v, symid basevar) {}
  virtual void cgdispvar    (acodegen *, symid dispid, symid v)  {}
  virtual void cgdispstr    (acodegen *, symid dispid, symid n)  {}
  virtual void cgsysdp      (acodegen *, symid v)                {}
  virtual void cgdispbase   (acodegen *, symid dispid, symid n)  {}
  virtual void cgdpc_arg    (acodegen *, symid parentdp, symid parentpin) {}
  virtual void cgdpc_dp     (acodegen *, symid v, symid parentdp)         {}
  virtual void cgdpc_ipblock(acodegen *, symid v, symid parentdp)         {}

  // TYPE QUERY
  virtual int   typew()            {return 0;}
  virtual int   typesign()         {return 0;}
  virtual bool  pinpoints(symid p) {return 0;}
  virtual symid unref()            {return NOSYMBOL;}
  virtual void  findoperands(vector<symid> &ops) {}
  virtual bool  ctlofdp(symid p)   {return false;} // true if this symbol is the controller of dp

  // This member function is - strictly speaking - a cheating function
  // as it allows to query the content of derived symbols (namesym)
  // It is an 'easy hack' that supports code generation and symbol table indexing functions
  virtual char *getname() {return 0;}
};

class acodegen;

class symbol {
 public:
  enum _symtype {
    datapath,    
    datapath_clone, 
    sfg,       
    sfg_always,
    dpinput,   
    dpoutput,  
    sigtype,
    reg,
    sig,
    assign_op,
    ior_op,      
    xor_op,    
    and_op,    
    shl_op,    
    shr_op,    
    add_op,
    sub_op,      
    concat_op, 
    mul_op,    
    mod_op,    
    cast_op,   
    neg_op,    
    not_op,
    const_op,    
    sel_op,    
    hwire_ctl, 
    system,    
    sfg_exec,
    sysnet,      
    syspin,    
    dfile,     
    finish,    
    display,   
    display_str,
    display_var,
    display_dp,
    display_changebase,
    index,     
    eq_op,     
    ne_op,     
    smt_op,    
    grt_op, 
    smteq_op,    
    grteq_op,
    seq_ctl,     
    ctlstep,
    fsm_ctl,     
    fsm_state, 
    fsm_initial, 
    ctl_true, 
    ctl_false, 
    fsm_trans,
    display_cycle, 
    display_toggle, 
    display_ones, 
    display_zeroes, 
    net_index, 
    sysdp, 
    com_lu, 
    lu_op, 
    ipblock, 
    ipblock_clone, 
    iptype, 
    ipparm,
    ipoutput,    
    dpc_arg,   
    dpc_dp, 
    dpc_ipblock,
    tracer,     
    tracesig,
    option
  };
 private:
  asym     *c;        // actual symbol content; variable depending on kind
  _symtype d;         // type info
  symid uniqid;       // hash value
  symid contextid;    // hash to context symbol
  long  lineno;       // line number in program text where this symbol is generated
 public:
  symbol(asym *_value, _symtype T, symid _contextid);
  symbol(const symbol &S);
  ~symbol() {}
  asym *  content();
  void    deletecontent();
  symid   id();
  symid   context();
  symid   srcline();
  bool    istype(_symtype _d) { return (d == _d); }
  _symtype type() {return d;}
  static const char *typestr(_symtype T);
  void    show(ostream &os);
  static  void showheader(ostream &os);

  void    createcode(acodegen *);
  bool    clone_iter_stop();
  symbol *clone(symid ofs);
  void    converttypetoclone(symid j);
};


class symboltable {
  map<symid, symbol *> table;

  typedef map<symid, symbol *> maps1;
  typedef maps1::iterator table_iter;
  typedef maps1::reverse_iterator table_riter;
  symid   findtypedname(const char *n, symbol::_symtype T);

  bool    netsource(symid net, symid s);             // true if net driven by s
  bool    netsink  (symid net, symid s, symid &idx); // true if net sunken by s

  // idx members and functions work as index functions
  // for fast lookup of symid given the name/context of a symbol
  struct idx_ltstr {
    bool operator()(const char* s1, const char* s2) const {
      return strcmp(s1, s2) < 0;
    }
  };

  typedef map<const char *, symid, idx_ltstr>    idx_type;

  idx_type                                 idx_dpmap;   
  typedef idx_type::iterator               idx_dpmap_it;

  typedef map <symid , idx_type>           idx_comsigmap_type;
  idx_comsigmap_type                       idx_comsigmap;
  typedef idx_comsigmap_type::iterator     idx_comsigmap_it;


 public:
  symboltable        ();
  ~symboltable       ();
  symid topsymbol    ();
  bool append        (symbol *);
  void idx_markdp    (const char *, symid);
  void idx_markcomsig(symid, char *, symid);
  void show          (ostream &os);
  void showsymbol    (ostream &os, symid id);
  void showlex       (ostream &os, symid id);
  void showline      (ostream &os, symid id);

  void clone_dp(symid datapath, symid base); // for datapath (includes copy of ctl)
  void clone_ipblock(symid datapath, symid base); // for ipblock

  // QUERY
  symid findlocalsig (const char *n, symid sfgcontext);
  symid findcomsig   (const char *n, symid dpcontext);
  symid findsigdef   (const char *n, symid sfgcontext, symid dpcontext);
  symid findsfg_backward(const char *n, symid fdl_datapath);
  symid findsfg_forward(const char *n, symid fdl_datapath);
  symid findincdp    (const char *n, symid fdl_datapath);
  symid findincdp    (const char *n);
  symid findstate    (const char *n, symid fsmcontext);
  symid finddp       (const char *n);
  symid findctl      (const char *n);
  symid findsystem   (const char *n);
  symid findnet      (const char *n);
  symid findactparm  (int n, symid dpcontext);
  symid findtype     (symid sidid);
  void  findlutable  (symid v, vector<char *> &lucontent);
  symid findiptype   (symid ipb);
  symid findipparm   (symid ipb, symid parmstart);
  void  findoperands (symid e, vector<symid> &ops);

  // DIRECT SYMBOL ACCESS
  symbol* getsymbol  (symid id);

  // CODE GENERATION
  void createcode    (acodegen *CG);
  void createcode_dp (acodegen *CG, char *dpname);

};

extern symboltable       glbSymboltable;
extern map<long, string> glbProgramtext;

#endif


