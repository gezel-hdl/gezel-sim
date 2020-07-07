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

#include "fdlcode.h"

acodegen::acodegen() {}

acodegen::~acodegen() {}

// CONTROLLER/ SYSTEM
void acodegen::cghwire_ctl(symid v, symid dp) {}
void acodegen::cgseq_ctl  (symid v, symid dp) {}
void acodegen::cgctlstep  (symid v, symid c, int n, symid condition) {}
void acodegen::cgexec     (symid ctl, symid dp, symid sfg) {}
void acodegen::cgtracer   (symid ctl) {}
void acodegen::cgtracesig (symid v, symid thesig, char *filename) {}
void acodegen::cgfile     (symid v, char *filename) {}
void acodegen::cgoption   (symid v, char *option) {}
void acodegen::cgcon      (symid pin1, symid pin2) {}
void acodegen::cgdisp     (symid disp, symid sfg, symid dp) {}
void acodegen::cgfinish   (symid disp, symid sfg, symid dp) {}
void acodegen::cgdispvar  (symid disp, symid var) {}
void acodegen::cgdispstr  (symid disp, char * var) {}
void acodegen::cgdispcycle(symid disp) {}
void acodegen::cgdisptoggle(symid disp) {}
void acodegen::cgdispones (symid disp) {}
void acodegen::cgdispzeroes(symid disp) {}
void acodegen::cgdispdp   (symid disp) {}
void acodegen::cgfsm_ctl  (symid v, symid dp) {}
void acodegen::cgfsm_state(symid v, symid c, char *str) {}
void acodegen::cgfsm_initial(symid v, symid c, char *str) {}
void acodegen::cgctltrue  (symid v, symid c, symid e, symid p) {}
void acodegen::cgctlfalse (symid v, symid c, symid e, symid p) {}
void acodegen::cgfsm_trans(symid v, symid c, symid from, symid to) {}
void acodegen::cgsysdp    (symid v) {}
void acodegen::cgdpc_arg  (symid parentdp, symid parentpin, symid childdp, symid childpin) {}
void acodegen::cgdpc_dp   (symid v, symid parentdp) {}
void acodegen::cgdpc_ipblock(symid v, symid parentdp) {}
void acodegen::cgdispbase (symid disp, int v) {}

// SFG/ SIGNAL
void acodegen::cgdp       (symid v, char *) {}
void acodegen::cgdpclone  (symid v, char *, symid p) {}
void acodegen::cgipblock  (symid v, char *instancename, char *tname) {}
void acodegen::cgipparm   (symid v, char *parmname, symid _dp) {}
void acodegen::cgipout    (symid v, int wlen, int sign, symid _dp, char *portname) {}
void acodegen::cgsfg      (symid v, char *) {}
void acodegen::cginput    (symid v, int wlen, int sign, symid _dp, char *portname) {}
void acodegen::cgoutput   (symid v, int wlen, int sign, symid _dp, char *portname) {}
void acodegen::cgreg      (symid v, int wlen, int sign, symid _dp, char *) {}
void acodegen::cgsig      (symid v, int wlen, int sign, symid _dp, char *) {}
void acodegen::cglutable  (symid v, int wlen, int sign, vector<char *> &content, symid _dp, char *) {}
void acodegen::cgluop   (symid v, symid L, symid table) {}
void acodegen::cgconst  (symid v, char *str) {}
void acodegen::cgassign (symid v, symid sfg, symid lhs, symid rhs) {}
void acodegen::cgindex  (symid v, symid basevar, int idx, int len) {}

// OPERATIONS
void acodegen::cgadd    (symid v, symid L, symid R) {}
void acodegen::cgconcat (symid v, symid L, symid R) {}
void acodegen::cgsub    (symid v, symid L, symid R) {}
void acodegen::cgior    (symid v, symid L, symid R) {}
void acodegen::cgxor    (symid v, symid L, symid R) {}
void acodegen::cgand    (symid v, symid L, symid R) {}
void acodegen::cgshr    (symid v, symid L, symid R) {}
void acodegen::cgshl    (symid v, symid L, symid R) {}
void acodegen::cgmul    (symid v, symid L, symid R) {}
void acodegen::cgmod    (symid v, symid L, symid R) {}
void acodegen::cgeq     (symid v, symid L, symid R) {}
void acodegen::cgne     (symid v, symid L, symid R) {}
void acodegen::cggrt    (symid v, symid L, symid R) {}
void acodegen::cgsmt    (symid v, symid L, symid R) {}
void acodegen::cggrteq  (symid v, symid L, symid R) {}
void acodegen::cgsmteq  (symid v, symid L, symid R) {}
void acodegen::cgcast   (symid v, symid L, int wlen, int sign) {}
void acodegen::cgnot    (symid v, symid L) {}
void acodegen::cgneg    (symid v, symid L) {}
void acodegen::cgsel    (symid v, symid L, symid M, symid R) {}

