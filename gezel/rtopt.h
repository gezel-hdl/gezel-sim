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

#ifndef RTOPT_H
#define RTOPT_H

#include <iostream>

using namespace std;

// compile-time flags

// parser index tables
#define PARSERINDEXTABLES

// enable profiling of register activity during simulation
#define RTACTIVITY

// use operator return-by-ref instead of return-by-value
#define RTOPSBYREF
#define RTVALUECOPY

// use signal definer cache
#define RTSIGDEFCACHE

void gvalactivity_show(ostream &os);
void rtactivity_show(ostream &os);

// runtime flags
extern int glbAllowSimulatorSleep;
extern int glbMPZCustomAllocator;
extern int glbUseGVALINT;
extern int glbRTDebug;
extern int glbVCDDebug;

// counters
extern unsigned long glbRTDPEval;
extern unsigned long glbRTActivityEffUpdate;
extern unsigned long glbRTActivityUpdate;
extern unsigned long glbRTNumRegisters;

extern unsigned long glbmpz2gval;
extern unsigned long glbgval2mpz;

extern unsigned long glbRTCycle;

#endif
