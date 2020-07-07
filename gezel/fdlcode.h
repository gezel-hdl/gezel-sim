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
// $Id: fdlcode.h 78 2007-07-14 19:00:16Z schaum $
//--------------------------------------------------------------

#ifndef FDLCODE_H
#define FDLCODE_H

#include "fdlsym.h"

// abstract code generator
class acodegen {
 public:
  acodegen();
  virtual ~acodegen();

  // CONTROLLER/ SYSTEM
  virtual void cghwire_ctl  (symid v, symid dp);
  virtual void cgseq_ctl    (symid v, symid dp);
  virtual void cgctlstep    (symid v, symid ctl, int n, symid condition);
  virtual void cgexec       (symid ctl, symid dp, symid sfg);
  virtual void cgtracer     (symid ctl);
  virtual void cgtracesig   (symid v, symid thesig, char *filename);
  virtual void cgfile       (symid v, char *filename);
  virtual void cgoption     (symid v, char *option);
  virtual void cgcon        (symid pin1, symid pin2);
  virtual void cgdisp       (symid disp, symid sfg, symid dp);
  virtual void cgfinish     (symid disp, symid sfg, symid dp);
  virtual void cgdispvar    (symid disp, symid var);
  virtual void cgdispdp     (symid disp);
  virtual void cgdispstr    (symid disp, char *str);
  virtual void cgdispcycle  (symid disp);
  virtual void cgdisptoggle (symid disp);
  virtual void cgdispones   (symid disp);
  virtual void cgdispzeroes (symid disp);
  virtual void cgdispbase   (symid disp, int v);
  virtual void cgfsm_ctl    (symid v, symid dp);
  virtual void cgfsm_state  (symid v, symid c, char *str);
  virtual void cgfsm_initial(symid v, symid c, char *str);
  virtual void cgfsm_trans  (symid v, symid c, symid from, symid to);
  virtual void cgctltrue    (symid v, symid c, symid e, symid p);
  virtual void cgctlfalse   (symid v, symid c, symid e, symid p);
  virtual void cgsysdp      (symid v);
  virtual void cgdpc_arg    (symid parentdp, symid parentpin, symid childdp, symid childpin);
  virtual void cgdpc_dp     (symid v, symid parentdp);
  virtual void cgdpc_ipblock(symid v, symid parentdp);

  // SFG/ SIGNAL
  virtual void cgdp         (symid v, char *);
  virtual void cgdpclone    (symid v, char *, symid p);
  virtual void cgipblock    (symid v, char * instancename, char *);
  virtual void cgipout      (symid v, int wlen, int sign, symid _dp, char *portname);
  virtual void cgipparm     (symid v, char *parmname, symid _dp);
  virtual void cgsfg        (symid v, char *);
  virtual void cginput      (symid v, int wlen, int sign, symid _dp, char *portname);
  virtual void cgoutput     (symid v, int wlen, int sign, symid _dp, char *portname);
  virtual void cgreg        (symid v, int wlen, int sign, symid _dp, char *);
  virtual void cgsig        (symid v, int wlen, int sign, symid _dp, char *);
  virtual void cgconst      (symid v, char *str);
  virtual void cgassign     (symid v, symid sfg, symid lhs, symid rhs);
  virtual void cgindex      (symid v, symid basevar, int idx, int len);
  virtual void cglutable    (symid v, int wlen, int sign, vector<char *> &content, symid _dp, char *);
  virtual void cgluop       (symid v, symid L, symid table);

  // OPERATIONS
  virtual void cgadd        (symid v, symid L, symid R);
  virtual void cgconcat     (symid v, symid L, symid R);
  virtual void cgsub        (symid v, symid L, symid R);
  virtual void cgior        (symid v, symid L, symid R);
  virtual void cgxor        (symid v, symid L, symid R);
  virtual void cgand        (symid v, symid L, symid R);
  virtual void cgshr        (symid v, symid L, symid R);
  virtual void cgshl        (symid v, symid L, symid R);
  virtual void cgmul        (symid v, symid L, symid R);
  virtual void cgmod        (symid v, symid L, symid R);
  virtual void cgeq         (symid v, symid L, symid R);
  virtual void cgne         (symid v, symid L, symid R);
  virtual void cggrt        (symid v, symid L, symid R);
  virtual void cgsmt        (symid v, symid L, symid R);
  virtual void cggrteq      (symid v, symid L, symid R);
  virtual void cgsmteq      (symid v, symid L, symid R);
  virtual void cgcast       (symid v, symid L, int wlen, int sign);
  virtual void cgnot        (symid v, symid L);
  virtual void cgneg        (symid v, symid L);
  virtual void cgsel        (symid v, symid L, symid M, symid R);

};

#endif
