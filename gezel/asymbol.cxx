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
// $Id: asymbol.cxx 78 2007-07-14 19:00:16Z schaum $
//--------------------------------------------------------------

#include "asymbol.h"
#include "module.h"
#include "fdlcode.h"
#include <string.h>
#include <iomanip> // setw

#include "rterror.h"
#include "rtopt.h"

#define PRINTWIDTH 18

static symid glb_uniqid = 0;

symid makeuniqid() {
  return glb_uniqid++;
}

//--------------------------------------------------------------
asym::asym() {}

void asym::show(ostream &os) {
  os << setw(PRINTWIDTH);
  os << "<void>";
}


void asym::showlex(ostream &os) {
  os << "<void>";
}
  
//--------------------------------------------------------------
symbol::symbol(asym *value, _symtype T, symid _contextid) :
  c(value), 
  d(T), 
  contextid(_contextid) {
  uniqid    = glb_uniqid++;
  lineno    = fdl_lineno;
}

symbol::symbol(const symbol &S) : 
  c(S.c), 
  d(S.d), 
  contextid(S.contextid) {}

asym * symbol::content() {
  return c;
}

symid symbol::id() {
  return uniqid;
}

symid symbol::context() {
  return contextid;
}

long symbol::srcline() {
  return lineno;
}

void symbol::showheader(ostream &os) {
  os << setw(5)  << "Id";
  os << setw(15) << "Type";
  os << setw(PRINTWIDTH) << "Name";
  os << setw(10) << "Context";
}

char *symbol::typestr(_symtype T) {
  switch (T) {
  case datapath:       return "datapath"; break;
  case datapath_clone: return "datapath_clone"; break;
  case ipblock:        return "ipblock";  break;
  case ipblock_clone:  return "ipblock_clone";  break;
  case iptype:         return "iptype";  break;
  case ipoutput:       return "ipoutput"; break;
  case ipparm:         return "ipparm";  break;
  case dpinput:        return "dpinput";  break;
  case dpoutput:       return "dpoutput"; break;
  case sigtype:        return "sigtype";  break;
  case reg:            return "reg";      break;
  case sig:            return "sig" ;     break;
  case sfg:            return "sfg";      break;
  case sfg_always:     return "sfg_always"; break;
  case assign_op:      return "assign_op";break;
  case ior_op:         return "ior_op";   break;
  case xor_op:         return "xor_op";   break;
  case and_op:         return "and_op";   break;
  case shr_op:         return "shl_op";   break;
  case shl_op:         return "shr_op";   break;
  case add_op:         return "add_op";   break;
  case concat_op:      return "concat_op";   break;
  case sub_op:         return "sub_op";   break;
  case mul_op:         return "mul_op";   break;
  case mod_op:         return "mod_op";   break;
  case cast_op:        return "cast_op";  break;
  case not_op:         return "not_op";   break;
  case neg_op:         return "neg_op";   break;
  case const_op:       return "const_op"; break;
  case sel_op:         return "sel_op";   break;
  case hwire_ctl:      return "hwire_ctl";break;
  case fsm_ctl:        return "fsm_ctl";  break;
  case fsm_state:      return "fsm_state";break;
  case fsm_initial:return "fsm_initial";break;
  case fsm_trans:      return "fsm_trans";break;
  case ctl_true:       return "ctl_true"; break;
  case ctl_false:      return "ctl_false";break;
  case seq_ctl:        return "seq_ctl";  break;
  case ctlstep:        return "ctlstep";  break;
  case sfg_exec:       return "sfg_exec"; break;
  case system:         return "system";   break;
  case sysnet:         return "sysnet";   break;
  case syspin:         return "syspin";   break;
  case dfile:          return "dfile";    break;
  case display:        return "display";  break;
  case finish:         return "finish";   break;
  case tracer:         return "tracer";   break;
  case tracesig:       return "tracesig"; break;
  case display_str:    return "dispstr"; break;
  case display_var:    return "dispvar"; break;
  case display_cycle:  return "display_cycle"; break;
  case display_toggle: return "display_toggle"; break;
  case display_ones:   return "display_ones"; break;
  case display_zeroes: return "display_zeroes"; break;
  case display_changebase: return "display_base"; break;
  case display_dp:     return "display_dp"; break;
  case index:          return "index";    break;
  case net_index:      return "net_index"; break;
  case eq_op:    return "eq_op";    break;
  case ne_op:    return "ne_op";    break;
  case grt_op:   return "grt_op";   break;
  case smt_op:   return "smt_op";   break;
  case grteq_op: return "grteq_op"; break;
  case smteq_op: return "smteq_op"; break;
  case sysdp:    return "sysdp";    break;
  case com_lu:   return "com_lu";   break;
  case lu_op:    return "lu_op";    break;
  case dpc_arg:  return "dpc_arg";  break;
  case dpc_dp:   return "dpc_dp";   break;
  case dpc_ipblock: return "dpc_ipblock";   break;
  case option:   return "option";   break;
  }
  return "unknown";
}

void symbol::show(ostream &os) {
  os << setw(5) << uniqid;
  os << setw(15);
  os << typestr(d);
  os << setw(PRINTWIDTH);
  c->show(os);
  os << setw(10) << contextid;
}

// note: cleanup of c is not done ins destructor of
// symbol as all c will be dynamically allocated and
// can be deleted only once. The deletecontent function
// is called when the global symbol table is destroyed;
// the deletes all symbol (dynamically allocated) and also
// all dynamic content INSIDE of those symbols (in this case c)
void symbol::deletecontent() {
  if (c)
    delete c;
}

bool symbol::clone_iter_stop() {
  if ((d == datapath)  || 
      (d == datapath_clone) || 
      (d == ipblock)   ||
      (d == ipblock_clone)   ||
      (d == system)    ||
      (d == hwire_ctl) ||
      (d == fsm_ctl)   ||
      (d == seq_ctl)) {
    return true;
  }
  return false;
}

symbol * symbol::clone(symid ofs) {
  if (contextid != NOSYMBOL)
    return new symbol(c->clone(ofs), d, contextid + ofs);
  else 
    return new symbol(c->clone(ofs), d, NOSYMBOL);
}

void symbol::converttypetoclone(symid CHK) {
  // this method has only effect on datapath / ipblock
  if (d == datapath) {
    d = datapath_clone;
    contextid = CHK;
  } else if (d == ipblock) {
    d = ipblock_clone;
    contextid = CHK;
  }
}

void symbol::createcode(acodegen *CG) {
  static symid currentsfg, currentctl, currentdisp, currentdp;

  switch(d) {
  case datapath:
    currentdp = id();
    content()->cgdp(CG,id());
    break; 
  case datapath_clone:
    currentdp = id();
    content()->cgdpclone(CG,id(), context());
    break;
  case ipblock:
    currentdp = id();
    content()->cgipblock(CG,id());
    break;
  case ipblock_clone:
    currentdp = id();
    content()->cgipblockclone(CG,id());
    break;
  case iptype:
    break;
  case ipoutput:
    content()->cgipout(CG, id(), currentdp);
    break;
  case ipparm:
    content()->cgipparm(CG, id(), currentdp);
    break;
  case sfg:
    currentsfg = id();
    content()->cgsfg(CG,id());
    break;
  case sfg_always:
    currentsfg = id();
    content()->cgsfg(CG,id());
    break;
  case dpinput:
    content()->cginput(CG, id(), currentdp);
    break;
  case dpoutput:
    content()->cgoutput(CG, id(), currentdp);
    break;
  case sigtype:
    break;
  case reg:
    content()->cgreg(CG, id(), currentdp);
    break;
  case sig:
    content()->cgsig(CG, id(), currentdp);
    break;
  case assign_op:
    content()->cgasgn(CG, id(), currentsfg);
    break;
  case ior_op:
    content()->cgior(CG,id());
    break;
  case xor_op:
    content()->cgxor(CG,id());
    break;
  case and_op:
    content()->cgand(CG,id());
    break;
  case shl_op:
    content()->cgshl(CG,id());
    break;
  case shr_op:
    content()->cgshr(CG,id());
    break;
  case add_op:
    content()->cgadd(CG,id());
    break;
  case concat_op:
    content()->cgconcat(CG,id());
    break;
  case sub_op:
    content()->cgsub(CG,id());
    break;
  case mul_op:
    content()->cgmul(CG,id());
    break;
  case mod_op:
    content()->cgmod(CG,id());
    break;
  case cast_op:
    content()->cgcast(CG,id());
    break;
  case eq_op:
    content()->cgeq(CG,id());
    break;
  case ne_op:
    content()->cgne(CG,id());
    break;
  case grt_op:
    content()->cggrt(CG,id());
    break;
  case smt_op:
    content()->cgsmt(CG,id());
    break;
  case grteq_op:
    content()->cggrteq(CG,id());
    break;
  case smteq_op:
    content()->cgsmteq(CG,id());
    break;
  case neg_op:
    content()->cgneg(CG, id());
    break;
  case not_op:
    content()->cgnot(CG, id());
    break;
  case const_op:
    content()->cgconst(CG, id());
    break;
  case sel_op:
    content()->cgsel(CG, id());
    break;
  case hwire_ctl:
    currentctl = id();
    content()->cghwire_ctl(CG, id());
    break;
  case seq_ctl:
    currentctl = id();
    content()->cgseq_ctl(CG, id());
    break;
  case fsm_ctl:
    currentctl = id();
    content()->cgfsm_ctl(CG, id());
    break;
  case tracesig:
    content()->cgtracesig(CG, id());
    break;
  case fsm_state:
    content()->cgfsm_state(CG, id(), currentctl);
    break;
  case fsm_trans:
    content()->cgfsm_trans(CG, id(), currentctl);
    break; 
  case fsm_initial:
    content()->cgfsm_initial(CG, id(), currentctl);
    break;
  case ctlstep:
    content()->cgctlstep(CG, id(), currentctl);
    break;
  case ctl_true:
    content()->cgctltrue(CG, id(), currentctl, context());
    break;
  case ctl_false:
    content()->cgctlfalse(CG, id(), currentctl, context());
    break;
  case system:
    break;
  case sfg_exec:
    content()->cgexec(CG, currentctl);
    break;
  case tracer:
    content()->cgtracer(CG, currentctl);
  case syspin:
    break;
  case dfile:      
    content()->cgfile(CG, id());
    break;
  case sysnet:
    break;  // check symboltable::createcode
  case net_index:
    break;  // check symboltable::createcode
  case index:
    content()->cgindex(CG, id(), context());
    break;
  case display:
    currentdisp = id();
    CG->cgdisp(id(), currentsfg, currentdp);  /* asym types can be called directly */
    break;
  case finish:
    CG->cgfinish(id(), currentsfg, currentdp);
    break;
  case display_var:
    content()->cgdispvar(CG, currentdisp, id());
    break;
  case display_dp:
    CG->cgdispdp(currentdisp);
    break;
  case display_str:
    content()->cgdispstr(CG, currentdisp, id());
    break;
  case display_cycle:
    CG->cgdispcycle(currentdisp);
    break;
  case display_toggle:
    CG->cgdisptoggle(currentdisp);
    break;
  case display_ones:
    CG->cgdispones(currentdisp);
    break;
  case display_zeroes:
    CG->cgdispzeroes(currentdisp);
    break;
  case display_changebase: 
    content()->cgdispbase(CG, currentdisp, id());
    break;
  case sysdp:
    content()->cgsysdp(CG, id());
    break;
  case com_lu:
    content()->cglutable(CG, id(), currentdp);
    break;
  case lu_op:
    content()->cgluop(CG, id(), currentdp);
    break;
  case dpc_arg:
    content()->cgdpc_arg(CG, currentdp, context());
    break;
  case dpc_dp:
    content()->cgdpc_dp(CG, id(), context()); 
    break;
  case dpc_ipblock:
    content()->cgdpc_ipblock(CG, id(), context()); 
    break;
  case option:
    content()->cgoption(CG, id());
    break;
  }
}

//------------------------------------
symboltable::symboltable() {}

symboltable::~symboltable() {
  // destroy dynamically allocated symbols
  for (table_iter i = table.begin(); i != table.end(); ++i) {
    (i->second)->deletecontent();
    delete i->second;
  }
}

bool symboltable::append(symbol *S) {
  // returns true if append OK
  // false if symbol already defined or table full
  if (!S)
    return false;
  if (table.find(S->id()) != table.end())
    return false;
  table[S->id()] = S;
  return true;
}

// enters a name in the dp index as a datapath name
// only useful for fast lookup of a symid!
void symboltable::idx_markdp(char *dpname, symid dpid) {
  idx_dpmap[dpname] = dpid;

}

// enters a name in the dp index as a datapath name
// only useful for fast lookup of a symid!
void symboltable::idx_markcomsig(symid dp, char *signame, symid s) {
  (idx_comsigmap[dp])[signame] = s;
}

void symboltable::show(ostream &os) {
  os << "Symbol Table holds " << (unsigned int) table.size() << " entries\n";
  for (table_iter i = table.begin(); i != table.end(); ++i) {
    if (i == table.begin()) {
      symbol::showheader(os);
      os << "\n";
    }
    (i->second)->show(os);
    os << "\n";
  }
}

void symboltable::showsymbol(ostream &os, symid id) {
  // this prints symbols plus context (for error messages)
  if (table.find(id) != table.end()) {
    os << ">"; table[id]->show(os); os << "\n";
    if (table[id]->context() != NOSYMBOL) {
      showsymbol(os, table[id]->context());
    }
  } else {
    os << "> ** UNKNOWN ** (id=" << id << ")\n";
  }  
}

// prints textual representation of symbol
void symboltable::showlex(ostream &os, symid id) {
  if (table.find(id) != table.end()) {
    // first print context symbol
    if (table[id]->context() != NOSYMBOL) {
      showlex(os, table[id]->context());
      os << ".";
    }
    table[id]->content()->showlex(os);
  } else {
    os << "> ** UNKNOWN ** (id=" << id << ")\n";
  }  
}

// prints source code line that refers to symbol
void symboltable::showline(ostream &os, symid id) {
  if (table.find(id) != table.end()) {
    unsigned long h = table[id]->srcline();
    unsigned long low = (h > 3) ? h - 3 : 0;
    unsigned long high = ((unsigned long ) h >= ((unsigned long) glbProgramtext.size() - 3)) ? (unsigned long) glbProgramtext.size() - 1 : h + 3;
    for (unsigned long i = low; i < high; i++) {
      if (i == h) {
	os << "(" << i << ") >>> ";
      } else {
	os << "(" << i << ")     ";
      }
      os << glbProgramtext[i] << "\n";
    }
  } else {
    os << "> ** UNKNOWN ** (id=" << id << ")\n";
  }        
}


symid symboltable::topsymbol() {
  //
  // !!! topsymbol will only be the topmost entry if
  //     NO additional increments to glb_uniqid have been generated
  //     (such as would happen with a call to makeuniqid())
  //
  return glb_uniqid;
}

void symboltable::clone_dp(symid currentdp, symid parentdp) {

  // find the controller corresponding to parentdp
  table_iter c_loc;
  for (c_loc = table.begin();
       (c_loc != table.end());
       c_loc++) {
    if (c_loc->second->content()->ctlofdp(parentdp)) {
      break;
    }
  }

  if (c_loc == table.end()) {
    fdlerror(ERROR_USER_CLONE_WITHOUT_CTL, parentdp);
  }

  // now clone parent dp
  symid ofs = glb_uniqid - parentdp - 1;

  if (table.find(parentdp) == table.end()) {
    fdlerror(ERROR_INTERNAL_SYMBOL_NOT_FOUND, parentdp);
  }

  table_iter q = table.find(parentdp);
  ++q;
  while ((q != table.end()) && ( ! (q->second->clone_iter_stop()))) {
    append(q->second->clone(ofs));
    ++q;
  }
  
  // now clone controller dp

  // gkb_uniqid points to the next 'available' space
  // currentdp points to the dp id of the current datapath (that is being clone)
  // parentdp points to the dp id of the datapath that is being clone
  // c_loc->id() points the parent controller
  //
  // there are two 'offsets':
  //   currentdp - parentdp - 1 = the offset between parent and copied datapath
  //   glbuniqid - c_loc->id() - 1 = the offset between parent controller and copied controller
  //
  // because a controller can never be defined earlier then a datapath,
  // AND a cloned datapath cannot be created if the parent datapath is absent
  // , this means glbuniqid will always have to be incremented
  //
  //             +---------------+ 
  //             | parent dp     |
  //             +---------------+
  //             |    GAP        |
  //             +---------------+ <--- c_loc
  //             | parent ctrl   |
  //             +---------------+
  //             | ANOTHER GAP   |
  //             +---------------+
  //             | cloned dp     |
  //             +---------------+
  //             -> glbuniqid points just after cloned dp
  //
  // To simplify the copy operation, we will adjust glb_uniqid
  // so that we maintain a similar GAP between the cloned dp/ctrl as
  // with the parent.Thus, we will insert 'GAP' after cloned dp
  //

  glb_uniqid = currentdp - parentdp + c_loc->second->id();

  // first simply is the ctl symbol. Always copy this one
  append(c_loc->second->clone(ofs));

  ++c_loc;
  while ((c_loc != table.end()) && ( ! (c_loc->second->clone_iter_stop()))) {
    append(c_loc->second->clone(ofs));
    ++c_loc;
  }

  // finally, perform hierarchy expansion.
  // all included datapaths (symbols of type dpc_dp) should be copied
  // because cloning of an included datapth creates an alias
  // i.e.
  //   dp A includes B
  //
  // then clone_of_A : A
  //
  //  will create:
  //
  //   dp clone_of_A includes B
  //
  // We transform this to:
  //
  //   dp clone_of_A includes clone_of_B
  //
  // We scan over the newly created cloned dp looking for dpc_dp
  // symbols. If we find them, then
  //   (a) we create a clone of the included dp
  //   (b) we relocate the reference in dpc_dp to this new dp


   if (table.find(currentdp) == table.end()) {
     fdlerror(ERROR_INTERNAL_SYMBOL_NOT_FOUND, currentdp);
   }
   q = table.find(currentdp);
   ++q;
   while ((q != table.end()) && ( ! (q->second->clone_iter_stop()))) {

     if ((q->second->istype(symbol::dpc_dp)) ||
	 q->second->istype(symbol::dpc_ipblock)) {
       symid srcdp          = q->second->content()->unref(); // should make a copy of this dp | ipblock
       symbol *newdp_symbol = getsymbol(srcdp)->clone(0);

       append(newdp_symbol);    // create a copy of the datapath symbol
       glbModuleTable.addmodule(newdp_symbol->id()); // is a new module
       glbModuleTable.defineparentof(newdp_symbol->id(), currentdp);

       if (q->second->istype(symbol::dpc_dp)) {
	 newdp_symbol->converttypetoclone(srcdp);  // clone datapath
	 clone_dp(newdp_symbol->id(), srcdp);
       } else {
	 newdp_symbol->converttypetoclone(srcdp);  // clone ipblock
	 clone_ipblock(newdp_symbol->id(), srcdp);
       }

       // relocate the reference to the newly created dp
       q->second->content()->relocate(newdp_symbol->id()); 

       // also relocate any dpc_arg that follow this dpc_dp (or dpc_ipblock)
       ++q;
       while ((q != table.end()) && q->second->istype(symbol::dpc_arg)) {
	 q->second->content()->relocate(newdp_symbol->id());
	 ++q;
       }
       --q;
     }
     ++q;
   }

}

//-----------------------------------------------------------------

void symboltable::clone_ipblock(symid currentdp, symid parentdp) {

  // now clone ipblock
  symid ofs = glb_uniqid - parentdp - 1;

  if (table.find(parentdp) == table.end()) {
    fdlerror(ERROR_INTERNAL_SYMBOL_NOT_FOUND, parentdp);
  }

  table_iter q = table.find(parentdp);
  ++q;
  while ((q != table.end()) && ( ! (q->second->clone_iter_stop()))) {
    append(q->second->clone(ofs));
    ++q;
  }  
}

//-------------------------------------------------------------
// general notes with symbol location
//
// The symbol table is traversed back to front, thus
// crossing first the most-recently defined symbols.
// It is *assumed* that this way the inner levels of a
// program are exposed before the outer ones
// As long as we do not support some form of declaration
// of sfg (and thus that there exist only definitions of sfg)
// this will be indeed the case.

//--------------------------------------------------------
symid symboltable::findtypedname(char *n, symbol::_symtype T) {
  for (table_riter i = table.rbegin();
       i != table.rend();
       ++i) {
    if ((i->second->istype(T)) &&
	(i->second->content()->matchname(n))) {
      return i->second->id();
    }
  } 
  return NOSYMBOL;
}

//--------------------------------------------------------
symid symboltable::findiptype(symid ipb) {
  if (table.find(ipb) == table.end()) {
    fdlerror(ERROR_INTERNAL_IP_UNKNOWN, ipb);
  }
  if (! (table[ipb])->istype(symbol::ipblock)) {
    if (! (table[ipb])->istype(symbol::ipblock_clone)) {
      fdlerror(ERROR_INTERNAL_IP_UNKNOWN, ipb);
    }
  }
  table_iter j = table.find(ipb);
  while (j != table.end()) {
    if (j->second->istype(symbol::iptype))
      return j->first;
    ++j;
  }
  fdlerror(ERROR_USER_IPBLOCK_HAS_NO_TYPEDEF, ipb);
  return NOSYMBOL;
}

//--------------------------------------------------------
symid symboltable::findipparm(symid ipb, symid parmstart) {
  if (table.find(ipb) == table.end()) {
    fdlerror(ERROR_INTERNAL_IP_UNKNOWN, ipb);
  }
  if (! (table[ipb])->istype(symbol::ipblock)) {
    if (! (table[ipb])->istype(symbol::ipblock_clone)) {
      fdlerror(ERROR_INTERNAL_IP_UNKNOWN, ipb);
    }
  }
  table_iter j = table.find(ipb);
  while (j != table.end()) {
    if ((j->second->istype(symbol::ipparm)) &&
        (j->second->context() == ipb)       &&
        (j->first > parmstart))
      return j->first;
    ++j;
  }
  return NOSYMBOL;
}

//--------------------------------------------------------
// check if system interconnect name is unique
symid symboltable::findsystem(char *n) {
  return findtypedname(n, symbol::system);
}

//--------------------------------------------------------
// check if controller name is unique
// controller is hwire_ctl or seq_ctl
symid symboltable::findctl(char *n) {
  symid v;
  v = findtypedname(n, symbol::hwire_ctl);
  if (v == NOSYMBOL) {
    v = findtypedname(n, symbol::seq_ctl);
  }
  if (v == NOSYMBOL) {
    v = findtypedname(n, symbol::fsm_ctl);
  }
  return v;
}

//--------------------------------------------------------
// check if datapath name is unique
symid symboltable::finddp(char *n) {

#ifdef PARSERINDEXTABLES
  idx_dpmap_it e = idx_dpmap.find(n);
  if (e != idx_dpmap.end()) {
    return e->second;
  }
  return NOSYMBOL;
#else
  // direct implementation without index
  symid v;
  v = findtypedname(n, symbol::datapath);
  if (v == NOSYMBOL)
    v = findtypedname(n, symbol::ipblock);
  return v;
#endif
}

//--------------------------------------------------------
// check if net name is unique
symid symboltable::findnet(char *n) {
  return findtypedname(n, symbol::sysnet);
}

//--------------------------------------------------------
// check if sfg name is unique in current datapath
symid symboltable::findsfg_backward(char *n, symid fdl_datapath) {
  for (table_riter i = table.rbegin();
       i != table.rend();
       ++i) {
    if ((i->second->istype(symbol::sfg) || i->second->istype(symbol::sfg_always)) && 
        (i->second->context() == fdl_datapath) &&
	(i->second->content()->matchname(n))) {
      return i->second->id();
    }
  } 
  return NOSYMBOL;
}

//--------------------------------------------------------
// check if sfg name is unique in current datapath
symid symboltable::findsfg_forward(char *n, symid fdl_datapath) {
  for (table_iter i = table.begin();
       i != table.end();
       ++i) {
    if ((i->second->istype(symbol::sfg) || i->second->istype(symbol::sfg_always)) && 
        (i->second->context() == fdl_datapath) &&
	(i->second->content()->matchname(n))) {
      return i->second->id();
    }
  } 
  return NOSYMBOL;
}

//--------------------------------------------------------
// check if local signal is unique in current sfg
symid symboltable::findlocalsig(char *n, symid sfgcontext) {
  register bool issig;
  for (table_riter i = table.rbegin();
       i != table.rend();
       ++i) {
    issig = (i->second->istype(symbol::reg)   ||
	     i->second->istype(symbol::sig));
    if (issig &&
	(i->second->context() == sfgcontext) &&
	(i->second->content()->matchname(n))) {
      return i->second->id();
    }
  }
  return NOSYMBOL;
}

//--------------------------------------------------------
// check for state definition in FSM
symid symboltable::findstate(char *n, symid fsmcontext) {
  register bool isstate;
   for (table_riter i = table.rbegin();
       i != table.rend();
       ++i) {
     isstate = (   (i->second->istype(symbol::fsm_state))
                || (i->second->istype(symbol::fsm_initial)));
     if (isstate && 
	(i->second->context() == fsmcontext) &&
	(i->second->content()->matchname(n))) {
       return i->second->id();
     }
   }
   return NOSYMBOL;
}

//--------------------------------------------------------
// check if common signal and/or port name is unique in 
// current datapath
symid symboltable::findcomsig(char *n, symid fdl_datapath) {

#ifdef PARSERINDEXTABLES

  idx_comsigmap_it e = idx_comsigmap.find(fdl_datapath);
  idx_dpmap_it e2;

  if (e != idx_comsigmap.end()) {
    e2 = e->second.find(n);
    if (e2 != e->second.end())
      return e2->second;
  }
  return NOSYMBOL;

#else

  register bool issig;
  for (table_riter i = table.rbegin();
       i != table.rend();
       ++i) {
   issig = (i->second->istype(symbol::reg)   ||
  	     i->second->istype(symbol::sig)   ||
             i->second->istype(symbol::com_lu)    ||
            i->second->istype(symbol::dpinput)   ||
             i->second->istype(symbol::dpoutput));
    if (issig &&
  	(i->second->context() == fdl_datapath) &&
  	(i->second->content()->matchname(n))) {
      return i->second->id();
    }
  }
  return NOSYMBOL;

#endif

}

//--------------------------------------------------------
// check if a datapath is included by any other one in a direct hierarchy relationship
symid symboltable::findincdp(char *n) {
  symid CHK = finddp(n);
  if (CHK == NOSYMBOL)
    return CHK;
  for (table_iter i = table.begin();
       i != table.end();
       ++i) {
    if (i->second->istype(symbol::dpc_dp)) {
      if (i->second->content()->unref() == CHK)
	return CHK;
    }
  }
  // if we get here search was unsuccesful
  return NOSYMBOL;
}

//--------------------------------------------------------
// check if a datapath includes another one in a direct hierarchy relationship
symid symboltable::findincdp(char *n, symid fdl_datapath) {
  symid CHK = finddp(n);
  if (CHK == NOSYMBOL)
    return CHK;
  // start a search from fdl_datapath, up to next datapath symbol or end-of-table
  // and see if CHK appears as a dpc_dp (datapath-call) symbol, indicating that
  // it was indeed included
  table_iter i = table.find(fdl_datapath);
  if (i == table.end()) // bad fdl_datapath value
    return NOSYMBOL;
  while ((i != table.end())) {
    if (i->second->istype(symbol::dpc_dp)) {
      if (i->second->content()->unref() == CHK)
	return CHK;
    }
    ++i;
  }
  // if we get here search was unsuccesful
  return NOSYMBOL;
}

//--------------------------------------------------------
// check if signal name is unique in current scope
symid symboltable::findsigdef(char *n, symid fdl_sfg, symid fdl_datapath) {
  register bool issig;
  for (table_riter i = table.rbegin();
       i != table.rend();
       ++i) {
    issig = (i->second->istype(symbol::reg) ||
	     i->second->istype(symbol::sig) ||
             i->second->istype(symbol::dpinput)   ||
             i->second->istype(symbol::com_lu)    ||
             i->second->istype(symbol::dpoutput));
    if (issig) {
      if ((i->second->context() == fdl_sfg) &&
	  (i->second->content()->matchname(n))) {
	return i->second->id();
      }
      if ((i->second->context() == fdl_datapath) &&
	  (i->second->content()->matchname(n))) {
	return i->second->id();
      }
    }
  }
  // not found!!
  return NOSYMBOL;
}

//--------------------------------------------------------
// given a symbol of dpinput, dpoutput, load/com_sig or reg,
// find the correspinding type definition
// the type definition symbol is found as the symbol
// following the signal symbol declaration symbol.
//
symid symboltable::findtype(symid sig) {
  table_iter k = table.find(sig);
  if (k == table.end())
    return NOSYMBOL;
  for (table_iter i = k; i != table.end(); ++i) {
    if (i->second->istype(symbol::sigtype))
      return (i->second->id());
  }
  return NOSYMBOL;
}

//--------------------------------------------------------
// find parameter n in datapath dpcontext
// to find it, look for dp definition and then find the nth
// dpinput or dpoutput. If new dp symbol find before nth occurs,
// then nth parameter does not exist
symid symboltable::findactparm (int n, symid dpcontext) {
  int current = 0;
  table_iter k = table.find(dpcontext);
  if (k == table.end())
    return NOSYMBOL;
  for (table_iter i = k; i != table.end(); ++i) {
    if (i->second->istype(symbol::dpinput) || i->second->istype(symbol::dpoutput)
        || i->second->istype(symbol::ipoutput)) {
      if (current == n)
	return (i->second->id());
      ++current;
    }
  }
  return NOSYMBOL;
}

//--------------------------------------------------------
void symboltable::findlutable(symid v, vector<char *> &lucontent) {
  table_iter k = table.find(v);
  if (k == table.end()) {
    fdlerror(ERROR_INTERNAL_LOOKUP_TABLE_UNKOWN, v);
  }
  // find first constant
  while ((k != table.end()) && (! k->second->istype(symbol::const_op))) 
    ++k;
  // loop over all constants, filling lucontent
  while ((k != table.end()) && (k->second->istype(symbol::const_op))) {
    k->second->content()->cglucontent(lucontent);
    ++k;
  }
//    cout << "lutable for " << v << "\n";
//    for (vector<char *>::iterator p = lucontent.begin();
//         p != lucontent.end();
//         ++p) 
//      cout << *p << "\n";
}

//--------------------------------------------------------
//
 void symboltable::findoperands(symid e, vector<symid> &ops) {

   // finds operands that define expression e
   // operands are primary symbols: registers, datapath inputs,
   // or constants. findoperands travels recursively into the
   // expression tree that defines e to find these primary
   // symbols

   vector<symid> e_ops;
   typedef vector<symid> vectorsymid;
   typedef vectorsymid::iterator vsidi;

   if (e == NOSYMBOL)
     return;

   getsymbol(e)->content()->findoperands(e_ops);

   //
   // pseudocode:
   //   if (e_ops) empty
   //	   add e to ops if not already in
   //   else
   //      for all op from e_ops
   //		findoperadns(op, ops)
   //
   if (e_ops.empty()) {
     for (vsidi k = ops.begin();
	  k != ops.end();
	  k++)
       if (e == *k)
	 return; // e is alread in
     ops.push_back(e);
     return;
   } else {
     for (vsidi k = e_ops.begin();
	  k != e_ops.end();
	  k++)
       findoperands(*k, ops);
   }
}

//--------------------------------------------------------
symbol* symboltable::getsymbol(symid id) {
  table_iter k = table.find(id);
  if (k == table.end()) {
    fdlerror(ERROR_INTERNAL_UNKNOWN_SYMBOL, id);
  }
  return k->second;
}

symboltable       glbSymboltable;
map<long, string> glbProgramtext;

//--------------------------------------------------
bool symboltable::netsource(symid net, symid s) {
  // s should be a stimuli file
  if ((getsymbol(s)->context() == net) &&
      (getsymbol(s)->istype(symbol::dfile)))
    return true;

  // s should be a dpoutput or an ipoutput
  // there should be a syspin on net
  // syspin should point to s
  if (getsymbol(s)->istype(symbol::dpoutput) || getsymbol(s)->istype(symbol::ipoutput) ) {
    for (table_iter i=table.begin(); i!=table.end(); ++i) {
      if (i->second->istype(symbol::syspin) && 
	  (i->second->context() == net)) {
	if (i->second->content()->pinpoints(s))
	  return true;
      }
    }
  }

  // if s is a netindex, then it SHOULD NOT source net
  // since this is currently not supported.
  if (getsymbol(s)->istype(symbol::net_index)) {
    for (table_iter i=table.begin(); i!=table.end(); ++i) {
      if (i->second->istype(symbol::syspin) &&
	  (i->second->context() == s)) {
	// there should be no dpoutput or ipoutput connected to this syspin
	for (table_iter j=table.begin(); j!=table.end(); ++j) {
	  if ((j->second->istype(symbol::dpoutput)||j->second->istype(symbol::ipoutput) ) &&
	      i->second->content()->pinpoints(j->first)) {
	    fdlerror(ERROR_USER_NO_SOURCE_PROXY, i->second->id());
	  }
	}
      }
    }
  }

  return false;
}
//--------------------------------------------------
bool symboltable::netsink(symid net, symid s, symid &index) {
  // s should be a dpinput
  // there shold be a syspin on net
  // syspin should point to s
  if (getsymbol(s)->istype(symbol::dpinput)) {

    index = NOSYMBOL;

    for (table_iter i=table.begin(); i!=table.end(); ++i) {

      if (i->second->istype(symbol::syspin) && 
	  (i->second->context() == net)) {
	if (i->second->content()->pinpoints(s))
	  return true;
      }
      
      if (i->second->istype(symbol::syspin)) {
	if (getsymbol(i->second->context())->istype(symbol::net_index)) {
	  if (getsymbol(i->second->context())->context() == net) {
	    if (i->second->content()->pinpoints(s)) {
	      index = i->second->context();
	      return true;
	    }
	  }
	}
      }
      
    }
  }

  return false;
}
//--------------------------------------------------
// #include <vector>
// void adddefaultcontrollers() {
//   table_iter tableid;
//   // add default controllers for all dp that have no controller defined
//   for (tableid = table.begin(); tableid != table.end(); ++tableid) {
//     if (tableid->second->istype(symbol::datapath)) {
//       // search of a controller between tableid and the end of table
//       table_iter ctlid;
//       bool found = false;
//       for (ctlid = tableid; ctlid != table.end(); ++ctlid) {
// 	if (ctlid->second->istype(symbol::fsm_ctl  )   || 
// 	    ctlid->second->istype(symbol::hwire_ctl)   || 
// 	    ctlid->second->istype(symbol::seq_ctl  ) ) {
// 	  if (ctlid->second->content()->ctlofdp(tableid->second->id())) {
// 	    found = true;
// 	    break;
// 	  }
// 	}
//       }
//       if (! found) {
// 	// add default controller for tableid->id()
// 	symbol *hwctl = new symbol(new ctlsym("noname", tableid->second->id()),
// 				   symbol::hwire_ctl, 
// 				   NOSYMBOL);
// 	append(hwctl);
// 	// find the 'always' sfg in this datapath
// 	symid sfgid = findsfg_forward("always", tableid->second->id());
// 	// and add it if it exists
// 	if (sfgid != NOSYMBOL) {
// 	  symbol *asym = new symbol(new sfgexecsym(sfgid, tableid->second->id()),
// 				    symbol::sfg_exec,
// 				    hwctl->id());
// 	  append(asym);
// 	}  
//       }
//     }
//   }
// }

void symboltable::createcode(acodegen *CG) {
  table_iter tableid;

  // create code while respecting the module hierarchy
  for (symid i = glbModuleTable.bottomup_iterator_init();
       (i != NOSYMBOL);
       i = glbModuleTable.bottomup_iterator_next()) {
    if (table.find(i) == table.end())
      break;
    table[i]->createcode(CG);
  }

  // verify if there is a system block defined in the description
  bool hassystem = false;
  for (table_riter ri = table.rbegin();
       ri != table.rend();
       ++ri) {
    if ((ri->second->istype(symbol::system))) {
      hassystem = true;
      break;
    }    
  } 
  if (! hassystem) {
    // issue a warning for this
    cerr << "*** Warning: FDL Description has no System block\n";
  }

  // connect system nets:
  // find all nets
  vector<symid> nets;
  for (tableid= table.begin(); tableid != table.end(); ++tableid)
    if (tableid->second->istype(symbol::sysnet)) {
      nets.push_back(tableid->first);
    }
  
  typedef vector<symid> vectorsymid;
  vectorsymid driver;
  typedef vectorsymid::iterator vsidi;
  vsidi netid;

   // for each system net, find the driving pin
  for (netid = nets.begin(); netid != nets.end(); ++netid) {
    int driverfound = 0;
    for (tableid = table.begin(); tableid != table.end(); ++tableid) {
      if (netsource(*netid,tableid->first)) {
	if (!driverfound) {
	  driver.push_back(tableid->first);
	  driverfound = 1;
	} else {
	  fdlerror(ERROR_USER_NET_HAS_MULTIPLE_DRIVERS, *netid);
	}
      }
    }
    if (!driverfound) {
      fdlerror(ERROR_USER_SIGNAL_HAS_NO_DRIVER, *netid);
    }
  }

  // cout << "Number of driven system nets " << driver.size() << "\n";
  if (driver.size() != nets.size()) {
    fdlerror(ERROR_INTERNAL_UNKNOWN, __FILE__, __LINE__);
  }

  // for each driver, find all listeners
  for (unsigned j=0; j<driver.size(); ++j) {
    for (tableid = table.begin(); tableid != table.end(); ++tableid) {
      symid idx;
      if (netsink(nets[j], tableid->first, idx)) {
	// cout << "Connect " << driver[j] << "/" << tableid->first << "\n";
	if (idx == NOSYMBOL) {
	  CG->cgcon (driver[j], tableid->first);
	} else { 
	  getsymbol(idx)->content()->cgindex(CG, idx, driver[j]);
	  CG->cgcon (idx, tableid->first);
	}	
      }
    }
  }

}

void symboltable::createcode_dp(acodegen *CG, char *dpname) {
  symid dp = finddp(dpname);

  if (dp == NOSYMBOL) {
    fdlerror(ERROR_USER_DP_NOT_PRESENT, dpname);
    exit(0);
  }

  for (symid i = dp; 
       (i != NOSYMBOL) && ((i == dp) || (!glbModuleTable.ismodule(i))); 
       i++) {
    table[i]->createcode(CG);
  }

}

