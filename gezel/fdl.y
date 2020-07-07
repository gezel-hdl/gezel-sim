/* --------------------------------------------------------------
 FDL parser

 Copyright (C) 2003 Patrick Schaumont (schaum@ee.ucla.edu)                   
 University of California at Los Angeles
                                                                             
 This program is free software; you can redistribute it and/or modify        
 it under the terms of the GNU General Public License as published by        
 the Free Software Foundation; either version 2 of the License, or           
 (at your option) any later version.                                         
                                                                             
 This program is distributed in the hope that it will be useful,             
 but WITHOUT ANY WARRANTY; without even the implied warranty of              
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               
 GNU General Public License for more details.                                
                                                                             
 You should have received a copy of the GNU General Public License           
 along with this program; if not, write to the Free Software                 
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   

 $Id: fdl.y,v 1.2 2005/10/02 19:31:57 schaum Exp $
 -------------------------------------------------------------- */

/* $Id: fdl.y,v 1.2 2005/10/02 19:31:57 schaum Exp $ */
/* yacc spec for fsmd parser */

%{
#define YYDEBUG 1
#define YYMAXDEPTH 65536
#define YYINITDEPTH 4096

/* ----- yacc to lex interface & common error variables -- */

/* visual c++ 2005 and bison :
   "/c/Program Files/GnuWin32/bin/bison.exe" -d -pfdl -ofdlyacc.cc fdl.y
   mv fdlyacc.hh fdlyacc.h
*/

#include "fdlparse.h"

  long fdl_warningcount;
  int  fdl_pending_warning;
  long fdl_lineno;
  char fdl_error_message[255];

fdl_error fdl_error_id;

#include <iostream>

using namespace std;

/* first steps towards IOCCC ... */

#define __EXTERN_C__  // make yacc call yylex with C conventions

#include <stdio.h>

extern void complete_line();
void fdlerror(const char *) {
  long errline = fdl_lineno;

  while ((fdl_instream_valid())&&(errline == fdl_lineno))  { // scan to end of line 
    fdllex();
  }
  fdl_lineno = errline;

  if ((fdl_error_id == UNKNOWN)) {
    sprintf(fdl_error_message, "(line %d) Syntax Error\n", (int) fdl_lineno);
  }
}


/* ------ Symbol table management --------- */

#include "fdlsym.h"
#include "module.h"

int    fdl_storage_type;  // most recent storage token
int    fdl_io_dir;        // most recent io direction token
symid  fdl_topsymbol;     // symid @ start of current list
symid  fdl_datapath;      // current datapath
symid  fdl_inc_datapath;  // current datapath being included (hierarchy)
symid  fdl_sfg;           // current sfg
symid  fdl_always_sfg;    // default sfg for current datapath  
                          // -- set to NOSYMBOL when parsing a new datapath
                          //    initialized when parsing ALWAYS or sfg with name "always"
                          //    for each control step, add the always sfg just after
                          //    creating the control step.
symid  fdl_controller;    // current controller
symid  fdl_control_step;  // current control step
symid  fdl_ipblock_type;  // current ipblock type
int    fdl_control_step_count;  // current control step counter

#define MAX_FDL_STRING 1024

char   fdl_control_name[MAX_FDL_STRING]; // current controller name

symid  fdl_system;        // current system
int    fdl_pinno;         // argument position counter for sys interconnect
symid  fdl_net;           // current system net
symid  fdl_display;       // current display directive
symid  fdl_trace_sig_id;  // current signal argument of TRACE

symid  fdl_base;

int    fdl_posint;        // last scanned integer
int    fdl_index1;        // last scanned index1
int    fdl_index2;        // last scanned index2

symid  fdl_from_state;            // current 'from' state for fsm controller
bool   fdl_expect_only_registers; // logic flag to contrain check on expression variables
bool   fdl_iodeclares_ipblock;    // logic flag to indicate port list is for ipblock
vector<symid> fdl_condition;      // condition scope stack (for nested if .. then)

char   fdl_current_string[MAX_FDL_STRING];  // current string value

symid add_ternary_operation(symid left,
                            symid middle, 
                            symid right, 
                            symbol::_symtype T) {
  symbol *s = new symbol(new ternaryopsym(left, middle, right),
                         T,
                         fdl_sfg);
  glbSymboltable.append(s);
  return s->id();
}

symid add_binary_operation(symid left, 
                           symid right, 
                           symbol::_symtype T) {
  symbol *s = new symbol(new binaryopsym(left, right),
                         T,
                         fdl_sfg);
  glbSymboltable.append(s);
  return s->id();
}

symid add_unary_operation(symid left, 
                          symbol::_symtype T) {
  symbol *s = new symbol(new unaryopsym(left),
                         T,
                         fdl_sfg);
  glbSymboltable.append(s);
  return s->id();
}

symid add_control_step(symid c /* condition symbol */, int &refcnt, symid ctl) {
  symid r;
  symbol *ctlstep = new symbol(new ctlstepsym(refcnt, c), symbol::ctlstep, ctl);
  glbSymboltable.append(ctlstep);
  r = ctlstep->id();
  ++refcnt;
  return r;
}

/* ----------------------------------------- 
   The compile-time expression evaluationstack 
   is used to evaluate lookup table expressions.
   It uses the same type conversion rules as the 
   RT-simulator

   ------------------------------------------ */
#include "gval.h"

 map<long, gval *> ct_stack;

 char *ct_alloc_str() {
   char *s = new char[4096];
   return s;
 }
 
 long ct_insert(gval *s) {
   ct_stack[ct_stack.size()] = s;
   return (ct_stack.size() - 1);
 }
 
 long ct_insert(char *s) {
   gval *r = make_gval(s);
   return ct_insert(r);
 }
 
 char *ct_return(long c) {
   char *s = ct_alloc_str();
   ct_stack[c]->setbase(10);
   ct_stack[c]->writebuf(s);
   return s;
 }

 long ct_eval_ternary(long c, long t, long f) {
   gval *t1 = (ct_stack[c]);
   if (t1->isnz())
     return t;
   else
     return f;
 }

 long ct_eval_ior(long left, long right) {
   gval *t1 = (ct_stack[left]);
   gval *t2 = (ct_stack[right]);
   gval *r = make_gval(max(t1->getwordlength(),t2->getwordlength()), 
		       max(t1->getsign(),t2->getsign()));
   r->ior_gval(*t1, *t2);
   return ct_insert(r);
 }

 long ct_eval_xor(long left, long right) {
   gval *t1 = (ct_stack[left]);
   gval *t2 = (ct_stack[right]);
   gval *r =make_gval(max(t1->getwordlength(),t2->getwordlength()), 
		      max(t1->getsign(),t2->getsign()));
   r->xor_gval(*t1, *t2);
   return ct_insert(r);
 }

 long ct_eval_and(long left, long right) {
   gval *t1 = (ct_stack[left]);
   gval *t2 = (ct_stack[right]);
   gval *r = make_gval(max(t1->getwordlength(),t2->getwordlength()), 
		       max(t1->getsign(),t2->getsign()));
   r->and_gval(*t1, *t2);
   return ct_insert(r);
 }

 long ct_eval_eq(long left, long right) {
   gval *t1 = (ct_stack[left]);
   gval *t2 = (ct_stack[right]);
   gval *rt = make_gval(1,0,"1");
   gval *rf = make_gval(1,0,"0");
   if (*t1 == *t2)
     return ct_insert(rt);
   else
     return ct_insert(rf);
 }

 long ct_eval_ne(long left, long right) {
   gval *t1 = (ct_stack[left]);
   gval *t2 = (ct_stack[right]);
   gval *rt = make_gval(1,0,"1");
   gval *rf = make_gval(1,0,"0");
   if (*t1 != *t2)
     return ct_insert(rt);
   else
     return ct_insert(rf);
 }

 long ct_eval_smt(long left, long right) {
   gval *t1 = (ct_stack[left]);
   gval *t2 = (ct_stack[right]);
   gval *rt = make_gval(1,0,"1");
   gval *rf = make_gval(1,0,"0");
   if (*t1 < *t2)
     return ct_insert(rt);
   else
     return ct_insert(rf);
 }

 long ct_eval_grt(long left, long right) {
   gval *t1 = (ct_stack[left]);
   gval *t2 = (ct_stack[right]);
   gval *rt = make_gval(1,0,"1");
   gval *rf = make_gval(1,0,"0");
   if (*t1 > *t2)
     return ct_insert(rt);
   else
     return ct_insert(rf);
 }

 long ct_eval_smteq(long left, long right) {
   gval *t1 = (ct_stack[left]);
   gval *t2 = (ct_stack[right]);
   gval *rt = make_gval(1,0,"1");
   gval *rf = make_gval(1,0,"0");
   if (*t1 <= *t2)
     return ct_insert(rt);
   else
     return ct_insert(rf);
 }

 long ct_eval_grteq(long left, long right) {
   gval *t1 = (ct_stack[left]);
   gval *t2 = (ct_stack[right]);
   gval *rt = make_gval(1,0,"1");
   gval *rf = make_gval(1,0,"0");
   if (*t1 >= *t2)
     return ct_insert(rt);
   else
     return ct_insert(rf);
 }

 long ct_eval_shl(long left, long right) {
   gval *t1 = (ct_stack[left]);
   gval *t2 = (ct_stack[right]);
   gval *r = make_gval(t1->getwordlength() + (1 << t2->getwordlength()), 
		       t1->getsign());
   r->shl_gval(*t1, *t2);
   return ct_insert(r);
 }

 long ct_eval_shr(long left, long right) {
   gval *t1 = (ct_stack[left]);
   gval *t2 = (ct_stack[right]);
   gval *r = make_gval(max(t1->getwordlength(),t2->getwordlength()), 
		       t1->getsign());
   r->shr_gval(*t1, *t2);
   return ct_insert(r);
 }

 long ct_eval_add(long left, long right) {
   gval *t1 = (ct_stack[left]);
   gval *t2 = (ct_stack[right]);
   gval *r  = make_gval(max(t1->getwordlength(),t2->getwordlength()), 
			max(t1->getsign(),t2->getsign()));
   r->add_gval(*t1, *t2);
   return ct_insert(r);
 }

 long ct_eval_sub(long left, long right) {
   gval *t1 = (ct_stack[left]);
   gval *t2 = (ct_stack[right]);
   gval *r = make_gval(max(t1->getwordlength(),t2->getwordlength()), 
		       max(t1->getsign(),t2->getsign()));
   r->sub_gval(*t1, *t2);
   return ct_insert(r);
 }

 long ct_eval_concat(long left, long right) {
   gval *t1 = (ct_stack[left]);
   gval *t2 = (ct_stack[right]);
   gval *r = make_gval(t1->getwordlength()+t2->getwordlength(), 
		       t1->getsign());
   r->concat_gval(*t1, *t2);
   return ct_insert(r);
 }

 long ct_eval_mul(long left, long right) {
   gval *t1 = (ct_stack[left]);
   gval *t2 = (ct_stack[right]);
   gval *r = make_gval(t1->getwordlength()+t2->getwordlength(), 
		       max(t1->getsign(),t2->getsign()));
   r->mul_gval(*t1, *t2);
   return ct_insert(r);
 }

 long ct_eval_mod(long left, long right) {
   gval *t1 = (ct_stack[left]);
   gval *t2 = (ct_stack[right]);
   gval *r = make_gval(t1->getwordlength(), t1->getsign());
   r->mod_gval(*t1, *t2);
   return ct_insert(r);
 }

 long ct_eval_cast(long left, long right) {
   gval *t1 = (ct_stack[left]);
   gval *t2 = (ct_stack[right]);
   gval *r = make_gval(t1->getwordlength(), t1->getsign());
   //   cerr << "cast wl " << t1->getwordlength() << " " << t1->getsign() << "\n";
   *r = *t2;
   //   cerr << "cast from " << *t2 << " to " << *r << "\n";
   return ct_insert(r);
 }

 long ct_eval_neg(long left) {
   gval *t = (ct_stack[left]);
   gval *r = make_gval(t->getwordlength(), t->getsign());
   r->neg_gval(*t);
   return ct_insert(r);
 }

 long ct_eval_not(long left) {
   gval *t = (ct_stack[left]);
   gval *r = make_gval(t->getwordlength(), t->getsign());
   r->not_gval(*t);
   return ct_insert(r);
 }

 long ct_eval_posint(char *w) {
   // allocate new entry
   char *s = ct_alloc_str();
   strcpy(s, w);
   return ct_insert(s);
 }

 long ct_eval_type(int wl, int sign) {
   // allocate new entry
   gval *w = make_gval(wl, sign);
   return ct_insert(w);
 }

%}

%union {
  char *   posint;       /* unsigned integer, hex or decimal */
  int      cmd;          /* command value */
  char *   str;          /* string buf */
  long     shash;        /* symbol hash value - parser internal */
  long     cttree;       /* compile-time expression eval stack index */
}


%token <posint> POSINT
%token <str>    ID 
%token <str>    ERRTOK
%token <str>    QSTRING
%token <cmd>    BRACE_OPEN 
%token <cmd>    BRACE_CLOSE 
%token <cmd>    COMMA
%token <cmd>    RBRACKET_OPEN 
%token <cmd>    RBRACKET_CLOSE
%token <cmd>    BRACKET_OPEN 
%token <cmd>    BRACKET_CLOSE
%token <cmd>    SEMICOLON 
%token <cmd>    COLON
%token <cmd>    DP 
%token <cmd>    TC 
%token <cmd>    NS 
%token <cmd>    IN 
%token <cmd>    OUT 
%token <cmd>    SIG 
%token <cmd>    REG 
%token <cmd>    SFG
%token <cmd>    ALWAYS
%token <cmd>    ASSIGN 
%token <cmd>    QMARK
%token <cmd>    AT
%token <cmd>    IOR
%token <cmd>    XOR 
%token <cmd>    AND 
%token <cmd>    SHL 
%token <cmd>    SHR 
%token <cmd>    ADD 
%token <cmd>    HASH
%token <cmd>    SUB 
%token <cmd>    MUL 
%token <cmd>    MOD
%token <cmd>    NOT
%token <cmd>    HARDWIRED
%token <cmd>    SEQUENCER
%token <cmd>    DOT
%token <cmd>    SYSTEM
%token <cmd>    STIMULUS
%token <cmd>    DISPLAY
%token <cmd>    FINISH
%token <cmd>    HEXDISPLAY
%token <cmd>    DECDISPLAY
%token <cmd>    BINDISPLAY
%token <cmd>    EQ
%token <cmd>    NE
%token <cmd>    GRT
%token <cmd>    GRTEQ
%token <cmd>    SMT
%token <cmd>    SMTEQ
%token <cmd>    SFGNAME
%token <cmd>    CYCLENUM
%token <cmd>    TOGGLENUM
%token <cmd>    ONESNUM
%token <cmd>    ZEROESNUM
%token <cmd>    TRACE
%token <cmd>    OPTION
%token <cmd>    DPNAME
%token <cmd>    TARGET
%token <cmd>    STATE
%token <cmd>    INITIALSTATE
%token <cmd>    IF
%token <cmd>    THEN
%token <cmd>    ELSE
%token <cmd>    FSM
%token <cmd>    LOOKUP
%token <cmd>    IPBLOCK
%token <cmd>    IPTYPE
%token <cmd>    IPPARM
%token <cmd>    USE

%type <posint> ct_eval_conditional_expr
%type <cttree> ct_conditional_expr
%type <cttree> ct_ior_expr
%type <cttree> ct_xor_expr
%type <cttree> ct_and_expr
%type <cttree> ct_equal_expr 
%type <cttree> ct_shift_expr
%type <cttree> ct_add_expr
%type <cttree> ct_mul_expr
%type <cttree> ct_cast_expr 
%type <cttree> ct_unary_expr
%type <cttree> ct_primary_expr
%type <cttree> ct_postfix_expr
%type <cttree> ct_sig_type

%type <cmd>     storage_type;
%type <str>     dp_id;
%type <str>     port_id;
%type <str>     string_term;
%type <cmd>     sign_type;
%type <shash>   expr;
%type <shash>   assignment_expr;
%type <shash>   conditional_expr;
%type <shash>   ior_expr;
%type <shash>   xor_expr;
%type <shash>   and_expr;
%type <shash>   shift_expr;
%type <shash>   equal_expr;
%type <shash>   add_expr;
%type <shash>   mul_expr;
%type <shash>   cast_expr;
%type <shash>   unary_expr;
%type <shash>   primary_expr;
%type <shash>   postfix_expr;
%type <shash>   sig_type;
%type <shash>   transition_condition;
%start fdl_description

%%

fdl_description: fdl_element
                  | fdl_description fdl_element
                  ;

// fdl_element:    datapath
//               | hardwiredcontrol
//               | sequencercontrol
//               | fsmcontrol
//               | ipblock
//               | system
//               | optionspec
//               ;

fdl_element:    fsmd
              | ipblock
              | system
              | optionspec
              ;

// Goal: tighten the link between a dp and the controller for it
// Detect when we create a dp without controller, and add a default
// hardwired controller
fsmd:   datapath hardwiredcontrol
      | datapath sequencercontrol
      | datapath fsmcontrol
      | datapath_clone
      | datapath
      {
        // is a datapath without controller
	symbol *hwctl = new symbol(new ctlsym("noname", fdl_datapath),
				   symbol::hwire_ctl, 
				   NOSYMBOL);
	glbSymboltable.append(hwctl);
	fdl_controller = hwctl->id();
	// add 'always' sfg as sfg_exec
        if (fdl_always_sfg != NOSYMBOL) {
	   symbol *asym = new symbol(new sfgexecsym(fdl_always_sfg, fdl_datapath),
				     symbol::sfg_exec,
				     fdl_control_step);
	   glbSymboltable.append(asym);
	}
      }
      ;

/* --- optional strings --------------- */

optionspec: OPTION option_string {}
          ;

option_string: string_term {
        symbol *prm = new symbol(new namesym(fdl_current_string), 
				 symbol::option, 
                                 NOSYMBOL);
	// options are modules because they are top-level symbols
	glbModuleTable.addmodule(prm->id());
        glbSymboltable.append(prm);
        }
        ;

/* --- system interconnect ----------------------- */

system : SYSTEM system_decl BRACE_OPEN system_def BRACE_CLOSE {}
              ;

system_decl : ID {
	      symid CHK = glbSymboltable.findsystem($1);
              if (CHK != NOSYMBOL) {
		sprintf(fdl_error_message,
			"(line %d) Error: Multiple definition of system %s", 
			(int) fdl_lineno,  $1);
		fdl_error_id = MULTIPLE_SYSTEMS;
		YYERROR;
	      }
              symbol *sym = new symbol(new namesym($1),
                                       symbol::system, NOSYMBOL);
              glbSymboltable.append(sym);
	      glbModuleTable.addmodule(sym->id());
              fdl_system = sym->id();
            }
            ;

system_def : system_def_el
           | system_def system_def_el
           ;

system_def_el : system_dp
              | system_stimulus
              ;

system_dp : system_dp_id RBRACKET_OPEN system_dp_pin_list RBRACKET_CLOSE SEMICOLON
           | /* DP with empty IO spec */ system_dp_id SEMICOLON
           ;

system_dp_id : ID {
	      symid CHK = glbSymboltable.finddp($1);
              if (CHK == NOSYMBOL) {
		sprintf(fdl_error_message,
			"(line %d) Error: Unknown datapath %s in system interconnect ", 
			(int) fdl_lineno,  $1);
		fdl_error_id = BAD_DP_NAME;
		YYERROR;
	      }
              fdl_datapath = CHK;
              symbol *sym = new symbol(new refsym(CHK), symbol::sysdp, fdl_system);
              glbSymboltable.append(sym);      
              fdl_pinno    = 0;
	      glbModuleTable.defineparentof(CHK, fdl_system);
             }
             ;

system_dp_pin_list : system_dp_pin_postfix
                   | system_dp_pin_list COMMA system_dp_pin_postfix
                   ;

system_dp_pin_postfix: system_dp_pin {
                        // add pin connection id
                       symid actualpin = glbSymboltable.findactparm(fdl_pinno, fdl_datapath);
                       symbol *sym = new symbol(new pinsym(actualpin, fdl_datapath),
                                                symbol::syspin, fdl_net);
                       glbSymboltable.append(sym);
                       ++fdl_pinno;
                     }
                     | system_dp_pin BRACKET_OPEN rangeexp BRACKET_CLOSE {
                         int ib = (fdl_index1 > fdl_index2) ? fdl_index2 : fdl_index1;
                         int ie = (fdl_index1 > fdl_index2) ? fdl_index1 : fdl_index2;
                         symbol *isym = new symbol(new indexsym(ib, ie-ib+1, fdl_net), 
						   symbol::net_index, fdl_net);
                         glbSymboltable.append(isym);
                         symid indexer = isym->id();
                         // --- pin connection
                         symid actualpin = glbSymboltable.findactparm(fdl_pinno, fdl_datapath);
                         symbol *psym = new symbol(new pinsym(actualpin, fdl_datapath),
                                                  symbol::syspin, indexer);
                         glbSymboltable.append(psym);
                         ++fdl_pinno;
                     }
                     ;

system_dp_pin : ID {
		// add system net
                symid CHK = glbSymboltable.findnet($1);
                if (CHK == NOSYMBOL) {
                   symbol *sym = new symbol(new namesym($1),
                                            symbol::sysnet, fdl_system);
                   glbSymboltable.append(sym);
	           CHK = sym->id();
                }
                fdl_net = CHK;

              }
              ;

system_stimulus : STIMULUS RBRACKET_OPEN stim_pin COMMA 
		  stim_fname RBRACKET_CLOSE SEMICOLON {}
                ;

stim_pin : ID {
           // add system net
           fdl_net = glbSymboltable.findnet($1);
           if (fdl_net == NOSYMBOL) {
              symbol *sym = new symbol(new namesym($1),
                                       symbol::sysnet, fdl_system);
              glbSymboltable.append(sym);
	      fdl_net = sym->id();
           }
         }
         ;

stim_fname : string_term { 
           // define a driver file symbol out of file for fdl_net
           symbol *sym = new symbol(new namesym(fdl_current_string), 
				    symbol::dfile, 
				    fdl_net);
           glbSymboltable.append(sym);
           }
           ;

/* --- instructions ---------------------------- */
/* assumes that fdl_control_step is pointing to the appropriate controller
   symbol, the next few rules retrieve an 'instruction'. 
   For dpid, the existence of the datapath is
   checked and the var fdl_datapath is initialized with that token.
   For sfgid, the existence of the sfg is checked and a new token 
   'sfgexec' is created 
 */

/* instruction: instr_dp_id DOT instr_sfg_id 
               ;
 */

instr_dp_id: ID {
	// must be an existing datapath
	symid CHK = glbSymboltable.finddp($1);
        if (CHK == NOSYMBOL) {
	  sprintf(fdl_error_message,
		  "(line %d) Error: Unknown datapath identifier %s ", 
		  (int) fdl_lineno,  $1);
	  fdl_error_id = BAD_DP_NAME;
	  YYERROR;
	}
        fdl_datapath = CHK;
        }
        ;

instr_sfg_id: ID {
        // must be an existing sfg within datapath fdl_datapath
	symid CHK = glbSymboltable.findsfg_backward($1, fdl_datapath);
        if (CHK == NOSYMBOL) {
	  sprintf(fdl_error_message,
		  "(line %d) Error: Unknown sfg identifier %s ", 
		  (int) fdl_lineno,  $1);
	  fdl_error_id = BAD_DP_NAME;
	  YYERROR;
	}
        symbol *ctlsfg = new symbol(new sfgexecsym(CHK, fdl_datapath),
				    symbol::sfg_exec, 
				    fdl_control_step);
	glbSymboltable.append(ctlsfg);
        } |
        TRACE {
        // a trace instruction is a help for debugging
        symbol *trac = new symbol(new namesym("tracer"), symbol::tracer, fdl_controller);
	glbSymboltable.append(trac);
        }
        ;

ctl_dpdef: RBRACKET_OPEN instr_dp_id RBRACKET_CLOSE {}
         ;

ctl_name: ID {
             if (strlen($1) > MAX_FDL_STRING) {
               strncpy(fdl_control_name, $1, MAX_FDL_STRING-1);
               fdl_control_name[MAX_FDL_STRING-1] = 0;
             } else {
               strcpy(fdl_control_name, $1);
             }
           }
           ;

/* --- hardwired control ----------------------- */
/* note: the format 'controller(datapath)' requires us to
   make a copy of the controller name before the parser can proceed
   to the datapath string token. At that moment both the contrlller
   name and datapth name have been checked and a controller
   symbol can be created. The controller name is first copied into
   fdl_control_name for this reason.
   But it is a messy solution after all ...
 */

hardwiredcontrol: HARDWIRED ctl_name ctl_dpdef {
  	            symid CHK = glbSymboltable.findctl(fdl_control_name);
                    if (CHK != NOSYMBOL) {
		      sprintf(fdl_error_message,
			      "(line %d) Error: Multiple definition of controller %s ", 
			      (int) fdl_lineno, fdl_control_name );
		      fdl_error_id = BAD_DP_NAME;
		      YYERROR;
		    } 
                    symbol *hwctl = new symbol(new ctlsym(fdl_control_name, fdl_datapath), 
				               symbol::hwire_ctl, NOSYMBOL);
	            glbSymboltable.append(hwctl);
                    fdl_controller = hwctl->id();

                    fdl_control_step = hwctl->id(); // hardwired control has one single control step
		    // add 'always' sfg as sfg_exec
		    if (fdl_always_sfg != NOSYMBOL) {
		      symbol *asym = new symbol(new sfgexecsym(fdl_always_sfg, fdl_datapath),
					     symbol::sfg_exec,
					     fdl_control_step);
		      glbSymboltable.append(asym);
		    }

                } BRACE_OPEN hw_def BRACE_CLOSE {}
	        ;

hw_def: hw_def_el
      | hw_def hw_def_el
      ;

hw_def_el: instr_sfg_id SEMICOLON
         ;

/* ----- sequencer control ----------------------- */
/* see the note with hardwired control ... */

sequencercontrol : SEQUENCER ctl_name ctl_dpdef {
  	            symid CHK = glbSymboltable.findctl(fdl_control_name);
                    if (CHK != NOSYMBOL) {
		      sprintf(fdl_error_message,
			      "(line %d) Error: Multiple definition of controller %s ", 
			      (int) fdl_lineno, fdl_control_name );
		      fdl_error_id = BAD_DP_NAME;
		      YYERROR;
		    }
                    symbol *seqctl = new symbol(new ctlsym(fdl_control_name, fdl_datapath), 
				                     symbol::seq_ctl, NOSYMBOL);
	            glbSymboltable.append(seqctl);
                    fdl_controller = seqctl->id();

                 } BRACE_OPEN seq_def BRACE_CLOSE
                 ;

seq_def : /* Control step symbols precede each set of sfg_exec symbols */ {
          fdl_control_step = add_control_step(NOSYMBOL, fdl_control_step_count, fdl_controller);
	  // add 'always' sfg as sfg_exec
	  if (fdl_always_sfg != NOSYMBOL) {
	    symbol *asym = new symbol(new sfgexecsym(fdl_always_sfg, fdl_datapath),
				      symbol::sfg_exec,
				      fdl_control_step);
	    glbSymboltable.append(asym);
	  }
        } seq_def_el
        | seq_def {
          fdl_control_step = add_control_step(NOSYMBOL, fdl_control_step_count, fdl_controller);
	  // add 'always' sfg as sfg_exec
	  if (fdl_always_sfg != NOSYMBOL) {
	    symbol *asym = new symbol(new sfgexecsym(fdl_always_sfg, fdl_datapath),
				      symbol::sfg_exec,
				      fdl_control_step);
	    glbSymboltable.append(asym);
	  }
        } seq_def_el
        ;

seq_def_el : simple_seq_def SEMICOLON
           | composite_seq_def SEMICOLON
           ;

simple_seq_def : instr_sfg_id
               ;

composite_seq_def : RBRACKET_OPEN composite_seq_def_contents RBRACKET_CLOSE {}
                  ;

composite_seq_def_contents : composite_seq_def_el 
                           | composite_seq_def_contents COMMA composite_seq_def_el
                           ;

composite_seq_def_el : instr_sfg_id
                     ;
/* --- fsm control ----------------------- */

fsmcontrol: FSM ctl_name ctl_dpdef {
  	        symid CHK = glbSymboltable.findctl(fdl_control_name);
                if (CHK != NOSYMBOL) {
		  sprintf(fdl_error_message,
			  "(line %d) Error: Multiple definition of controller %s ", 
			  (int) fdl_lineno, fdl_control_name );
		  fdl_error_id = BAD_DP_NAME;
		  YYERROR;
		}
                symbol *hwctl = new symbol(new ctlsym(fdl_control_name, fdl_datapath), 
				               symbol::fsm_ctl, NOSYMBOL);
	        glbSymboltable.append(hwctl);

                fdl_controller = hwctl->id();
                fdl_control_step_count = 0;

                /* fdl_sfg is the context variable (symid) for expressions, and points
                   to the most recently create SFG symbol. For control
                   steps, the context variable is the current control step,
                   and fdl_sfg is made to point to that control step */

                fdl_sfg = fdl_controller;

          } BRACE_OPEN fsm_def BRACE_CLOSE
          ;

fsm_def: fsm_def_el
      | fsm_def fsm_def_el
      ;

fsm_def_el: state_id
         | transition_id
         ;

state_id: STATE state_id_list SEMICOLON {}
        | INITIALSTATE initial_state_id SEMICOLON {}
        ;

state_id_list: state_id
             | state_id_list COMMA state_id
             ;

initial_state_id: ID {
             symid rval = glbSymboltable.findstate($1, fdl_controller);
             if (rval != NOSYMBOL) {
	       sprintf(fdl_error_message,
		       "(line %d) Error: Multiple definition of state %s ", 
		       (int) fdl_lineno, $1);
	       fdl_error_id = BAD_DP_NAME;
	       YYERROR;
	     }
             symbol *s = new symbol(new namesym($1), symbol::fsm_initial, fdl_controller);
             glbSymboltable.append(s);
             }
             ;

state_id: ID {
          symid rval = glbSymboltable.findstate($1, fdl_controller);
          if (rval != NOSYMBOL) {
	    sprintf(fdl_error_message,
		    "(line %d) Error: Multiple definition of state %s ", 
		    (int) fdl_lineno, $1);
	    fdl_error_id = BAD_DP_NAME;
	    YYERROR;
	  }
          symbol *s = new symbol(new namesym($1), symbol::fsm_state, fdl_controller);
          glbSymboltable.append(s);
        }
        ;

transition_id: AT state_from_id {fdl_condition.push_back(NOSYMBOL);} target_transition_descr
             ;

state_from_id: ID {
             symid rval =  glbSymboltable.findstate($1, fdl_controller);
             if (rval == NOSYMBOL) {
	       sprintf(fdl_error_message,
		       "(line %d) Error: Unknown starting state %s ", 
		       (int) fdl_lineno, $1);
	       fdl_error_id = BAD_DP_NAME;
	       YYERROR;
	     }
             fdl_from_state = rval;
             }
             ;

target_transition_descr: /* Control step symbols precede each set of sfg_exec symbols */ {
                         fdl_control_step = add_control_step(fdl_condition.back(), 
							     fdl_control_step_count, 
                                                             fdl_controller);
			   // add 'always' sfg as sfg_exec
			   if (fdl_always_sfg != NOSYMBOL) {
			     symbol *asym = new symbol(new sfgexecsym(fdl_always_sfg, fdl_datapath),
			 			       symbol::sfg_exec,
						       fdl_control_step);
			     glbSymboltable.append(asym);
			   }
                         }
                         unconditional_transition_descr
                       | IF 
                            {fdl_expect_only_registers = true; } 
			 transition_condition
                             { fdl_expect_only_registers = false; 
                               symbol *sym = new symbol(new refsym($3), 
                                                        symbol::ctl_true, 
                                                        fdl_condition.back());
                               glbSymboltable.append(sym); 
                               fdl_condition.push_back(sym->id());
                             }
                         THEN target_transition_descr
                         ELSE
                             {
                               fdl_condition.pop_back();

                               symbol *sym = new symbol(new refsym($3), 
                                                      symbol::ctl_false, 
                                                      fdl_condition.back());
                               glbSymboltable.append(sym);
                               fdl_condition.push_back(sym->id());
                             }
                         target_transition_descr
                             {
                               fdl_condition.pop_back();
                             }
                         ;

transition_condition: RBRACKET_OPEN  
                           conditional_expr 
                      RBRACKET_CLOSE {$$ = $2;} 
                    ;

unconditional_transition_descr: simple_sfg_set TARGET state_to_id SEMICOLON
                              | composite_sfg_set TARGET state_to_id SEMICOLON 
                              ;

state_to_id: ID {
             symid rval =  glbSymboltable.findstate($1, fdl_controller);
             if (rval == NOSYMBOL) {
	       sprintf(fdl_error_message,
		       "(line %d) Error: Unknown target state %s ", 
		       (int) fdl_lineno, $1);
	       fdl_error_id = BAD_DP_NAME;
	       YYERROR;
	     }
             // create transition from fdl_from_state to this one
             symbol *s = new symbol(new transitionsym(fdl_from_state, rval), 
				    symbol::fsm_trans, 
                                    fdl_control_step);
             glbSymboltable.append(s); 
           }
           ;

simple_sfg_set: instr_sfg_id
              ;

composite_sfg_set: RBRACKET_OPEN composite_sfg_set_contents RBRACKET_CLOSE {}
                 ;

composite_sfg_set_contents: composite_sfg_set_el
                          | composite_sfg_set_contents COMMA composite_sfg_set_el
                          ;

composite_sfg_set_el: instr_sfg_id
                    ;

/* -- ip blocks (external interface) ----------------- */
ipblock: IPBLOCK ipblock_id ipblock_io BRACE_OPEN ipblock_def BRACE_CLOSE {}
       | IPBLOCK ipblock_id COLON ipblock_id_copy {}
       ;

ipblock_id: ID {
	     symid CHK = glbSymboltable.finddp($1);
             if (CHK != NOSYMBOL) {
	       sprintf(fdl_error_message,
		       "(line %d) Error: Multiple definition of datapath/ipblock %s ", 
		       (int) fdl_lineno, $1);
	       fdl_error_id = BAD_DP_NAME;
	       YYERROR;
	     } 
             symbol *block = new symbol(new namesym($1), 
			         symbol::ipblock, NOSYMBOL);
	     glbSymboltable.append(block);
             fdl_ipblock_type = NOSYMBOL;
	     fdl_datapath = block->id();
	     glbSymboltable.idx_markdp(block->content()->getname(), block->id());
	     glbModuleTable.addmodule(block->id());
	    } 
            ;

ipblock_id_copy: ID {
	     symid CHK = glbSymboltable.finddp($1);
             if (CHK == NOSYMBOL) {
               sprintf(fdl_error_message,
                       "(line %d) Error: Ipblock to copy %s not found",
                       (int) fdl_lineno, $1);
               fdl_error_id = BAD_DP_NAME;
               YYERROR;
             }

             // first, convert type of last entered datapath into datapath_clone
             glbSymboltable.getsymbol(fdl_datapath)->converttypetoclone(CHK);
	     // now perform cloning operation

	     glbSymboltable.clone_ipblock(fdl_datapath, CHK);
             };

ipblock_io:
          | RBRACKET_OPEN {fdl_iodeclares_ipblock = true;} port_io_list RBRACKET_CLOSE
          ;

ipblock_def: ipblock_def_el
           | ipblock_def_el ipblock_def
           ;

ipblock_def_el: iptype
              | ipparm
              ;

iptype: IPTYPE string_term {
           if (fdl_ipblock_type != NOSYMBOL) {
	     sprintf(fdl_error_message,
		  "(line %d) Error: Can have only a single iptype per ipblock", 
		  (int) fdl_lineno);
	     fdl_error_id = BAD_DP_NAME;
	     YYERROR;
           }
           symbol *typ = new symbol(new namesym(fdl_current_string), 
				    symbol::iptype, 
                                    fdl_datapath);
	   glbSymboltable.append(typ);
	   fdl_ipblock_type = typ->id();
      } SEMICOLON
      ;

ipparm: IPPARM string_term {
           symbol *prm = new symbol(new namesym(fdl_current_string), 
				    symbol::ipparm, 
                                    fdl_datapath);
	   glbSymboltable.append(prm);
      } SEMICOLON
      ;

/* -- datapath --------------------------------------- */

datapath: DP dp_id dp_io BRACE_OPEN dp_def BRACE_CLOSE {}
          ;

datapath_clone: DP dp_id COLON dp_id_copy {}
                ;

dp_io:    /* empty io spec */
     |    RBRACKET_OPEN RBRACKET_CLOSE
     |    RBRACKET_OPEN {fdl_iodeclares_ipblock = false;} port_io_list RBRACKET_CLOSE {}
     ;

dp_id : ID {
           symid CHK = glbSymboltable.finddp($1);
           if (CHK != NOSYMBOL) {
 	     sprintf(fdl_error_message,
		  "(line %d) Error: Multiple definition of datapath/ipblock %s ",
		 (int) fdl_lineno, $1);
	     fdl_error_id = BAD_DP_NAME;
	     YYERROR;
             }
           symbol *datapath = new symbol(new namesym($1),
	                                 symbol::datapath, NOSYMBOL);
           glbSymboltable.append(datapath);
           fdl_datapath   = datapath->id();
           fdl_always_sfg = NOSYMBOL;
           glbSymboltable.idx_markdp(datapath->content()->getname(), datapath->id());
           glbModuleTable.addmodule(datapath->id());
           }
           ;

dp_id_copy : ID {
	     symid CHK = glbSymboltable.finddp($1);

             if (CHK == NOSYMBOL) {
               sprintf(fdl_error_message,
                       "(line %d) Error: Datapath to copy %s not found",
                       (int) fdl_lineno, $1);
               fdl_error_id = BAD_DP_NAME;
               YYERROR;
             }
             // first, convert type of last entered datapath into datapath_clone
             glbSymboltable.getsymbol(fdl_datapath)->converttypetoclone(CHK);
	     // now perform cloning operation
	     glbSymboltable.clone_dp(fdl_datapath, CHK);
        } 
        ;

dp_def:  dp_el
      |  dp_def dp_el
      ;

dp_el:   com_sig_el
      |  sfg_el 
      |  dp_inc_el
      |  trace_sig_el 
      ;

com_sig_el: storage_type 
             {fdl_storage_type = $1; 
              fdl_topsymbol    = glbSymboltable.topsymbol(); } 
           com_sig_id_list COLON sig_type SEMICOLON
          | LOOKUP {
            fdl_topsymbol = glbSymboltable.topsymbol();
          } com_lut_id_list COLON sig_type ASSIGN lut_content SEMICOLON
          ;

trace_sig_el:
	  TRACE RBRACKET_OPEN trace_sig_def COMMA trace_file_def RBRACKET_CLOSE SEMICOLON
          {}
	  ;

trace_file_def:
	  string_term {
	    if (fdl_trace_sig_id != NOSYMBOL) {
              symbol *s = new symbol(new attrsym(fdl_current_string, fdl_trace_sig_id), 
				     symbol::tracesig, 
				     fdl_datapath);
	      glbSymboltable.append(s);
	    }
	  }
	  ;

trace_sig_def:
	  conditional_expr {
	    fdl_trace_sig_id = $1;
          } 
          ;

/* -------- lookup table ------- */

com_lut_id_list: com_lut_id
           | com_lut_id_list COMMA com_lut_id
           ;

com_lut_id: ID {
        symid CHK = glbSymboltable.findcomsig($1, fdl_datapath);
	if (CHK != NOSYMBOL) {
	  sprintf(fdl_error_message,
		  "(line %d) Error: Multiple definition of common name %s ", 
		  (int) fdl_lineno, $1);
	  fdl_error_id = BAD_DP_NAME;
	  YYERROR;
	}
        symbol *s = new symbol(new namesym($1), symbol::com_lu, fdl_datapath);
	glbSymboltable.append(s);
	glbSymboltable.idx_markcomsig(fdl_datapath, s->content()->getname(), s->id());
      }
      ;

lut_content: BRACE_OPEN lut_def BRACE_CLOSE {}
           ;

lut_def: lut_def_el
       | lut_def COMMA lut_def_el
       ;

lut_def_el: ct_eval_conditional_expr {
      	  symbol *s = new symbol(new namesym($1), symbol::const_op, fdl_topsymbol);
      	  glbSymboltable.append(s);
          } 
          ;

/* -------- port declarations ------- */

port_io_list: port_io_el
          | port_io_list SEMICOLON port_io_el
          ;

port_io_el: io_dir {fdl_topsymbol = glbSymboltable.topsymbol();} 
                     port_id_list COLON sig_type
          ;
 
port_id_list: port_id {} | 
              port_id_list COMMA port_id {}
            ;

port_id : ID {
           symid CHK = glbSymboltable.findcomsig($1, fdl_datapath);
	   if (CHK != NOSYMBOL) {
	     sprintf(fdl_error_message,
		     "(line %d) Error: Multiple Definition of global signal or port %s ", 
		     (int) fdl_lineno, $1);
	     fdl_error_id = BAD_DP_NAME;
	     YYERROR;
	   } 
	   if (fdl_io_dir == IN) {
              symbol *ioport = new symbol(new namesym($1), 
				          symbol::dpinput, 
				          fdl_datapath);
              glbSymboltable.append(ioport);
	      glbSymboltable.idx_markcomsig(fdl_datapath, ioport->content()->getname(), ioport->id());
           } else if (fdl_io_dir == OUT) {
               symbol *ioport = new symbol(new namesym($1), 
 					   (fdl_iodeclares_ipblock) ? symbol::ipoutput : symbol::dpoutput, 
					   fdl_datapath);
               glbSymboltable.append(ioport);
 	       glbSymboltable.idx_markcomsig(fdl_datapath, ioport->content()->getname(), ioport->id());
           } else {
               sprintf(fdl_error_message,
                       "(line %d) Error: Port Type %s Unknown, Parser Error",
                       (int) fdl_lineno, (char *) $1);
               fdl_error_id = BAD_IO_TYPE;
               YYERROR;
           }
         }
        ;

io_dir: IN  {fdl_io_dir = $1;}
      | OUT {fdl_io_dir = $1;}
      | ID { sprintf(fdl_error_message,
		     "(line %d) Error: Unrecognized IO type %s",
		     (int) fdl_lineno, $1);
             fdl_error_id = BAD_IO_TYPE;
	     YYERROR;
           } 
      ;

/* -------- signal declarations ------- */

local_sig_id_list: local_sig_id
           | local_sig_id_list COMMA local_sig_id 
           ;

local_sig_id: ID {
	   symid CHK = glbSymboltable.findlocalsig($1, fdl_sfg);
           if (CHK != NOSYMBOL) {
	     sprintf(fdl_error_message,
		     "(line %d) Error: Multiple Definition of local signal %s ", 
		     (int) fdl_lineno, $1);
	     fdl_error_id = BAD_DP_NAME;
	     YYERROR;
	   }
           symbol *s = new symbol(new namesym($1),
                                  (fdl_storage_type == REG) ? 
                                       symbol::reg : 
                                       symbol::sig,
                                  fdl_sfg);
           glbSymboltable.append(s);
           }
           ;

com_sig_id_list: com_sig_id
           | com_sig_id_list COMMA com_sig_id 
           ;

com_sig_id: ID {
           symid CHK = glbSymboltable.findcomsig($1, fdl_datapath);
	   if (CHK != NOSYMBOL) {
	     sprintf(fdl_error_message,
		     "(line %d) Error: Multiple Definition of global signal or port %s ", 
		     (int) fdl_lineno, $1);
	     fdl_error_id = BAD_DP_NAME;
	     YYERROR;
	   } 
           symbol *s = new symbol(new namesym($1),
                                  (fdl_storage_type == REG) ? 
					symbol::reg : 
				        symbol::sig,
                                  fdl_datapath);
           glbSymboltable.append(s);
	   glbSymboltable.idx_markcomsig(fdl_datapath, s->content()->getname(), s->id());
           }
          ;

/* -------- sfg declarations -------- */

sfg_el: SFG ID {
         symid CHK = glbSymboltable.findsfg_backward($2, fdl_datapath);
	 if (CHK != NOSYMBOL) {
	   sprintf(fdl_error_message,
		   "(line %d) Error: Multiple Definition of sfg %s ", 
		   (int) fdl_lineno, $2);
	   fdl_error_id = BAD_DP_NAME;
	   YYERROR;
	 } 
         symbol *s = new symbol(new namesym($2), symbol::sfg, fdl_datapath);
	 glbSymboltable.append(s);          
         fdl_sfg   = s->id();
        } BRACE_OPEN sfg_internals BRACE_CLOSE
        | 
        ALWAYS {
	  symid CHK = glbSymboltable.findsfg_backward("always", fdl_datapath);
	  if (CHK != NOSYMBOL) {
	    sprintf(fdl_error_message,
		    "(line %d) Error: Multiple Definition of sfg %s ", 
		    (int) fdl_lineno, "always");
	    fdl_error_id = BAD_DP_NAME;
	    YYERROR;
	  } 
	  symbol *s = new symbol(new namesym("always"), symbol::sfg_always, fdl_datapath);
	  glbSymboltable.append(s);
	  fdl_sfg   = s->id();
          fdl_always_sfg = s->id();
        } BRACE_OPEN sfg_internals BRACE_CLOSE
        | 
        SFG ALWAYS {
	  symid CHK = glbSymboltable.findsfg_backward("always", fdl_datapath);
	  if (CHK != NOSYMBOL) {
	    sprintf(fdl_error_message,
		    "(line %d) Error: Multiple Definition of sfg %s ", 
		    (int) fdl_lineno, "always");
	    fdl_error_id = BAD_DP_NAME;
	    YYERROR;
	  } 
	  symbol *s = new symbol(new namesym("always"), symbol::sfg_always, fdl_datapath);
	  glbSymboltable.append(s);
	  fdl_sfg   = s->id();
          fdl_always_sfg = s->id();
        } BRACE_OPEN sfg_internals BRACE_CLOSE
        ;

sfg_internals: local_sig_el SEMICOLON expression_list
             | expression_list
             | 
             ;

local_sig_el: storage_type { 
               fdl_storage_type = $1; 
               fdl_topsymbol    = glbSymboltable.topsymbol(); 
             } local_sig_id_list COLON sig_type 
            ;


/* -------- directives ---------- */

directive: display_directive 
	 | finish_directive
	 ;

display_directive: DISPLAY {
		symbol * s = new symbol(new asym, symbol::display, fdl_sfg);
		glbSymboltable.append(s);
		fdl_display = s->id();
		} RBRACKET_OPEN display_el_list RBRACKET_CLOSE
                ;
finish_directive: FINISH {
		symbol * s = new symbol(new asym, symbol::finish, fdl_sfg);
		glbSymboltable.append(s);
		} ;

display_el_list: display_el
               | display_el_list COMMA display_el
               ;

display_el: string_term {
           // define a name symbol 
           symbol *sym = new symbol(new namesym(fdl_current_string), 
				    symbol::display_str, 
				    fdl_display);
           glbSymboltable.append(sym);
          }
          | conditional_expr {
           symbol *rval = glbSymboltable.getsymbol($1);
           symbol *sym = new symbol(new refsym(rval->id()), 
				    symbol::display_var, 
				    fdl_display);
           glbSymboltable.append(sym);
          } 
          | DPNAME {
            symbol *sym = new symbol(new asym, 
				     symbol::display_dp, 
				     fdl_display);
	    glbSymboltable.append(sym);
          }
          | SFGNAME {
	   symbol *symsfg = glbSymboltable.getsymbol(fdl_sfg);
           symbol *sym   = new symbol(symsfg->content()->clone(0),  
			              symbol::display_str, 
                                      fdl_display);
	   glbSymboltable.append(sym);           
          }
          | CYCLENUM {
           symbol *sym   = new symbol(new asym,  
			              symbol::display_cycle, 
                                      fdl_display);
	   glbSymboltable.append(sym);
          }
          | TOGGLENUM {
           symbol *sym   = new symbol(new asym,  
			              symbol::display_toggle, 
                                      fdl_display);
	   glbSymboltable.append(sym);
          }
          | ONESNUM {
           symbol *sym   = new symbol(new asym,  
			              symbol::display_ones, 
                                      fdl_display);
	   glbSymboltable.append(sym);
          }
          | ZEROESNUM {
           symbol *sym   = new symbol(new asym,  
			              symbol::display_zeroes, 
                                      fdl_display);
	   glbSymboltable.append(sym);
          }
	  | HEXDISPLAY {
	     symbol *sym = new symbol(new intsym(16), 
				      symbol::display_changebase, 
				      fdl_display);
             glbSymboltable.append(sym);
          }
          | BINDISPLAY {
	     symbol *sym = new symbol(new intsym(2), 
				      symbol::display_changebase, 
				      fdl_display);
             glbSymboltable.append(sym);
          }
          | DECDISPLAY {
	     symbol *sym = new symbol(new intsym(10), 
				      symbol::display_changebase, 
				      fdl_display);
             glbSymboltable.append(sym);
          }
          ;

/* -------- behavioral/structural datapath hierarchy --------- */

/*--- this part is used globally for a datapath */

dp_inc_el:  USE dp_inc_id RBRACKET_OPEN dp_inc_pin_list RBRACKET_CLOSE SEMICOLON {}
            |  USE dp_inc_id SEMICOLON {}
         ;

dp_inc_id : ID {
	  symid CHK = glbSymboltable.finddp($1);
	  if (CHK == NOSYMBOL) {
	    sprintf(fdl_error_message,
		    "(line %d) Error: Unknown datapath %s",
		    (int) fdl_lineno, $1);
	    fdl_error_id = BAD_DP_NAME;
	    YYERROR;
	  }
          // check if this datapath was already include somewhere
          symid CHK2 = glbSymboltable.findincdp($1);
          if (CHK2 != NOSYMBOL) {
	    sprintf(fdl_error_message,
		    "(line %d) Error: Multiple use of datapath %s",
		    (int) fdl_lineno, $1);
	    fdl_error_id = DP_MULTIPLE_USE;
	    YYERROR;
	  }

	  symbol *sym;
	  if (glbSymboltable.getsymbol(CHK)->istype(symbol::ipblock) ||
	      glbSymboltable.getsymbol(CHK)->istype(symbol::ipblock_clone)) {
	    sym = new symbol(new hrefsym(CHK), symbol::dpc_ipblock, fdl_datapath);
	  } else {
	    sym = new symbol(new hrefsym(CHK), symbol::dpc_dp, fdl_datapath);
	  }

          fdl_pinno = 0;
	  
          glbSymboltable.append(sym);
	  glbModuleTable.defineparentof(CHK, fdl_datapath);

/*
          fdl_inc_datapath = sym->id();
*/
	  fdl_inc_datapath = CHK;
          }
          ;

dp_inc_pin_list : dp_inc_pin_postfix
                 | dp_inc_pin_list COMMA dp_inc_pin_postfix
                 ;

dp_inc_pin_postfix : primary_expr {
/*
                symbol *sym = new symbol(new refsym($1), symbol::dpc_arg, fdl_inc_datapath);
                glbSymboltable.append(sym);
 */
        symid actualpin = glbSymboltable.findactparm(fdl_pinno, fdl_inc_datapath);
	if (actualpin == NOSYMBOL) {
            sprintf(fdl_error_message, "\n(line %d) Warning: Pin number %d of dp id %d out of range", 
			(int) fdl_lineno, (int) fdl_pinno, (int) fdl_inc_datapath);
	    fdl_error_id = BAD_DP_NAME;
	    YYERROR;
	}
        symbol *sym = new symbol(new pinsym(actualpin, fdl_inc_datapath),
                                 symbol::dpc_arg, $1);
        glbSymboltable.append(sym);
        ++fdl_pinno;
        }
        ;

/* -------- expressions --------- */
expression_list:  expression_list semicol_expr
               | semicol_expr
               ;

semicol_expr: directive SEMICOLON
            | expr SEMICOLON   {}
            ;

expr: assignment_expr 
    | expr COMMA assignment_expr
    ;

assignment_expr: conditional_expr 
    | unary_expr assignment_op assignment_expr {
      if (glbSymboltable.getsymbol($1)->istype(symbol::dpinput)) {
	sprintf(fdl_error_message,
		"(line %d) Error: Cannot assign datapath input",
		(int) fdl_lineno);
	fdl_error_id = UNKNOWN_IDENTIFIER;

	long errline = fdl_lineno;
	while (errline == fdl_lineno)  { // scan to end of line 
	  fdllex();
	}
	fdl_lineno = errline;

	YYERROR;
      }
      $$ = add_binary_operation($1, $3, symbol::assign_op);
    }
    ;

assignment_op: ASSIGN /* for in-place operations, assignment_op
                         must be extended */ {}
               ;


/*--------------------------------------------------------------
                    compile time expressions

   ct_eval_conditional_expr returns a string (similar to posint)
   ct_conditional_expr returns an index into the compile-time eval stack
   ct_ior_expr         returns an index into the compile-time eval stack
   ct_xor_expr         returns an index into the compile-time eval stack
   ct_and_expr         returns an index into the compile-time eval stack
   ct_equal_expr       returns an index into the compile-time eval stack
   ct_shift_expr       returns an index into the compile-time eval stack
   ct_add_expr         returns an index into the compile-time eval stack
   ct_mul_expr         returns an index into the compile-time eval stack
   ct_cast_expr        returns an index into the compile-time eval stack
   ct_unary_expr       returns an index into the compile-time eval stack
   ct_primary_expr     returns an index into the compile-time eval stack
   ct_sig_type         returns an index into the compile-time eval stack

  --------------------------------------------------------------*/

ct_eval_conditional_expr: ct_conditional_expr {
                  $$ = ct_return($1);
                }
                ;

ct_conditional_expr: ct_ior_expr 
                | ct_ior_expr QMARK ct_conditional_expr COLON ct_conditional_expr {
                   $$ = ct_eval_ternary($1, $3, $5);
                }
                ;

ct_ior_expr: ct_xor_expr 
        | ct_ior_expr IOR ct_xor_expr {
          $$ = ct_eval_ior($1, $3);
        }
        ;

ct_xor_expr: ct_and_expr 
        | ct_xor_expr XOR ct_and_expr {
            $$ = ct_eval_xor($1, $3);
        }
        ;

ct_and_expr: ct_equal_expr 
        | ct_and_expr AND ct_equal_expr {
            $$ = ct_eval_and($1, $3);
        }
        ;

ct_equal_expr: ct_shift_expr
        | ct_equal_expr EQ ct_shift_expr {
            $$ = ct_eval_eq($1, $3);
        }
        | ct_equal_expr NE ct_shift_expr {
            $$ = ct_eval_ne($1, $3);
        }
        | ct_equal_expr SMT ct_shift_expr {
            $$ = ct_eval_smt($1, $3);
        }
        | ct_equal_expr GRT ct_shift_expr {
            $$ = ct_eval_grt($1, $3);
        }
        | ct_equal_expr SMTEQ ct_shift_expr {
            $$ = ct_eval_smteq($1, $3);
        }
        | ct_equal_expr GRTEQ ct_shift_expr {
            $$ = ct_eval_grteq($1, $3);
        }
	;


ct_shift_expr: ct_add_expr 
        | ct_shift_expr SHL ct_add_expr {
            $$ = ct_eval_shl($1, $3);
        }
        | ct_shift_expr SHR ct_add_expr {
            $$ = ct_eval_shr($1, $3);
        }
        ;

ct_add_expr: ct_mul_expr 
        | ct_add_expr ADD ct_mul_expr {
            $$ = ct_eval_add($1, $3);
        }
        | ct_add_expr SUB ct_mul_expr{
            $$ = ct_eval_sub($1, $3);
        }
        | ct_add_expr HASH ct_mul_expr {
            $$ = ct_eval_concat($1, $3);
        }
        ;

ct_mul_expr: ct_cast_expr 
        | ct_mul_expr MUL ct_cast_expr {
            $$ = ct_eval_mul($1, $3);
        }
        | ct_mul_expr MOD ct_cast_expr {
            $$ = ct_eval_mod($1, $3);
        }
        ;

ct_cast_expr: ct_unary_expr 
        | RBRACKET_OPEN ct_sig_type RBRACKET_CLOSE ct_cast_expr {
            $$ = ct_eval_cast($2, $4);
         }
        ;

ct_unary_expr: ct_postfix_expr  
        | SUB ct_cast_expr  {$$ = ct_eval_neg($2);}
        | NOT ct_cast_expr  {$$ = ct_eval_not($2);}
        ;

ct_postfix_expr: ct_primary_expr
        { $$ = $1; }
        ;

ct_primary_expr: POSINT {
          $$ = ct_eval_posint($1);
         }
        | RBRACKET_OPEN ct_conditional_expr RBRACKET_CLOSE {
	    $$ = $2;
         }
        ;

ct_sig_type: sign_type RBRACKET_OPEN ct_eval_conditional_expr RBRACKET_CLOSE {
              if ($1 == TC)
		$$ = ct_eval_type(atoi($3), 1);
              else
		$$ = ct_eval_type(atoi($3), 0);
          }
         ;


/*--------------------------------------------------------------
                     runtime expressions
  --------------------------------------------------------------*/

conditional_expr: ior_expr 
                | ior_expr QMARK expr COLON conditional_expr {
                  $$ = add_ternary_operation($1, $3, $5, symbol::sel_op);
                }
                ;

ior_expr: xor_expr 
        | ior_expr IOR xor_expr {
          $$ = add_binary_operation($1, $3, symbol::ior_op);
        }
        ;

xor_expr: and_expr 
        | xor_expr XOR and_expr {
          $$ = add_binary_operation($1, $3, symbol::xor_op);
        }
        ;

and_expr: equal_expr 
        | and_expr AND equal_expr {
          $$ = add_binary_operation($1, $3, symbol::and_op);
        }
        ;

equal_expr: shift_expr
        | equal_expr EQ shift_expr {
          $$ = add_binary_operation($1, $3, symbol::eq_op);
        }
        | equal_expr NE shift_expr {
          $$ = add_binary_operation($1, $3, symbol::ne_op);
        }
        | equal_expr SMT shift_expr {
          $$ = add_binary_operation($1, $3, symbol::smt_op);
        }
        | equal_expr GRT shift_expr {
          $$ = add_binary_operation($1, $3, symbol::grt_op);
        }
        | equal_expr SMTEQ shift_expr {
          $$ = add_binary_operation($1, $3, symbol::smteq_op);
        }
        | equal_expr GRTEQ shift_expr {
          $$ = add_binary_operation($1, $3, symbol::grteq_op);
        }
	;

shift_expr: add_expr 
        | shift_expr SHL add_expr {
          $$ = add_binary_operation($1, $3, symbol::shl_op);
        }
        | shift_expr SHR add_expr {
          $$ = add_binary_operation($1, $3, symbol::shr_op);
        }
        ;

add_expr: mul_expr 
        | add_expr ADD mul_expr {
          $$ = add_binary_operation($1, $3, symbol::add_op);
        }
        | add_expr SUB mul_expr{
          $$ = add_binary_operation($1, $3, symbol::sub_op);
        }
        | add_expr HASH mul_expr {
          $$ = add_binary_operation($1, $3, symbol::concat_op);
        }
        ;

mul_expr: cast_expr 
        | mul_expr MUL cast_expr {
          $$ = add_binary_operation($1, $3, symbol::mul_op);
        }
        | mul_expr MOD cast_expr {
          $$ = add_binary_operation($1, $3, symbol::mod_op);
        }
        ;

cast_expr: unary_expr 
        | RBRACKET_OPEN sig_type RBRACKET_CLOSE cast_expr {
          $$ = add_binary_operation($2, $4, symbol::cast_op);
         }
        ;

unary_expr: postfix_expr  /* pre-increment and pre-decrement inplace should be added here */
        | SUB cast_expr  {$$ = add_unary_operation($2, symbol::neg_op);}
        | NOT cast_expr  {$$ = add_unary_operation($2, symbol::not_op);}
        ;

postfix_expr: primary_expr
           | postfix_expr { fdl_base = $1; } BRACKET_OPEN rangeexp BRACKET_CLOSE {
              int ib = (fdl_index1 > fdl_index2) ? fdl_index2 : fdl_index1;
              int ie = (fdl_index1 > fdl_index2) ? fdl_index1 : fdl_index2;
              symbol *sym = new symbol(new indexsym(ib, ie-ib+1, fdl_base), symbol::index, fdl_base);
              glbSymboltable.append(sym);
	      $$ = sym->id(); } 
           | postfix_expr { fdl_base = $1; } RBRACKET_OPEN conditional_expr RBRACKET_CLOSE {
              symbol *sym = new symbol(new lusym($4, fdl_base), symbol::lu_op, fdl_base); 
              glbSymboltable.append(sym);
	      $$ = sym->id();
           }
           ;

rangeexp: ct_eval_conditional_expr {
          fdl_index1 = atoi($1);
          fdl_index2 = fdl_index1;
        }
        | rangeexp COLON ct_eval_conditional_expr {
          fdl_index2 = atoi($3); 
        }
        ;

primary_expr: ID {
         symid rval = glbSymboltable.findsigdef($1, fdl_sfg, fdl_datapath);
         if (rval == NOSYMBOL) {
             sprintf(fdl_error_message,
		     "(line %d) Error: Unknown identifier %s",
		     (int) fdl_lineno, $1);
             fdl_error_id = UNKNOWN_IDENTIFIER;

	     long errline = fdl_lineno;
	     while (errline == fdl_lineno)  { // scan to end of line 
	       fdllex();
	     }
	     fdl_lineno = errline;

	     YYERROR;
	 } else {
	   if (fdl_expect_only_registers &
	       ! (glbSymboltable.getsymbol(rval)->istype(symbol::reg) )) {
	     sprintf(fdl_error_message, 
		     "\n(line %d) State Transition Condition variable %s should be a register", 
		     (int) fdl_lineno, $1);
	     fdl_error_id = BAD_DP_NAME;
	   }
	   $$ = rval;
	 }
        }
        | POSINT {
      	  symbol *s = new symbol(new namesym($1), symbol::const_op, fdl_sfg);
      	  glbSymboltable.append(s);
	  /* printf("*%s*\n", $1); */
          $$ = s->id();
         }
        | RBRACKET_OPEN expr RBRACKET_CLOSE {
	    $$ = $2;
         }
        ;

string_term: string_term QSTRING {
 	       char buf[512]; char *buf2 = buf;
               strcpy(buf2, $1);
	       buf2++; buf2[strlen(buf2)-1] = 0; // drop quotes
	       if ((strlen(buf2) + strlen(fdl_current_string)) < MAX_FDL_STRING) 
		 strcat(fdl_current_string, buf2);
	       else {
		 sprintf(fdl_error_message,
			 "(line %d) Error: string to long",
			 (int) fdl_lineno);
		 fdl_error_id = STRING_TO_LONG;
		 YYERROR;
	       }
             }
             | QSTRING {
 	       char buf[512]; char *buf2 = buf;
               strcpy(buf2, $1);
	       buf2++; buf2[strlen(buf2)-1] = 0; // drop quotes
	       if (strlen(buf2) < MAX_FDL_STRING) 
		 strcpy(fdl_current_string, buf2);
	       else {
		 sprintf(fdl_error_message,
			 "(line %d) Error: string to long",
			 (int) fdl_lineno);
		 fdl_error_id = STRING_TO_LONG;
		 YYERROR;
	       }
             }
             ;

/* ------------ Type declarations ------------- */

storage_type: SIG
            | REG 
	    ;

sig_type: sign_type RBRACKET_OPEN ct_eval_conditional_expr {fdl_posint = atoi($3);} RBRACKET_CLOSE {
             symbol *s = new symbol(
                          new sigtypesym(($1 == TC) ? 
					   sigtypesym::tc : 
					   sigtypesym::ns, fdl_posint), 
		          symbol::sigtype, 
                          fdl_topsymbol);
             glbSymboltable.append(s);
             $$ = s->id();
          }
         ;

sign_type: TC
         | NS
         ;

%%

/*--------------------------*/

int call_parser(char *filename) {
  int retval;

  fdl_warningcount = 0;
  fdl_lineno       = 1;
  fdl_error_id     = UNKNOWN;
  fdl_expect_only_registers = false;

  if (fdl_openstream(filename) != 0) {
    fdl_error_id = BAD_FILE_NAME;
    sprintf(fdl_error_message,"Could not open file %s\n", filename);
    return fdl_error_id;
  }

  retval = fdlparse();
  destroy_fdl_lexer();

  if (retval) {

    int i;
    int n = (fdl_lineno > 3) ? fdl_lineno-3 : fdl_lineno;
    cerr << "\n*** " << fdl_error_message << "\n";
    for (i = n; i<=fdl_lineno; i++) {
      if (i==fdl_lineno) {
	cerr << "(" << i << ") >>>";
      } else {
	cerr << "(" << i << ")    ";
      }
      cerr << glbProgramtext[i] << "\n";
    }
    cerr << "\n";
  }

  // not needed - fsmd terminal will add default controllers during parsing
  //  // add hardwired controllers for dp wo any controller
  //
  //  //  glbSymboltable.adddefaultcontrollers();

  return retval;
}

void record_line(long lineno, string linetxt) {
  glbProgramtext[lineno] = linetxt;
  //  cerr << lineno << ": " << linetxt << "\n";
}

void issue_pending_warning() {
  if (fdl_pending_warning) {
    fdl_warningcount++;
    cerr << fdl_error_message << "\n\n";
    cerr << "(" << fdl_lineno << ") >>> " << glbProgramtext[fdl_lineno] << "\n\n";
    fdl_pending_warning = 0;
  }
  
}

 void lex_fdl_warning() {
   if (fdl_pending_warning) {
     issue_pending_warning();
   }
   fdl_pending_warning++;
 }

