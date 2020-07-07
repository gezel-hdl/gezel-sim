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
// $Id: platform.cxx 83 2007-07-16 01:14:45Z schaum $
//--------------------------------------------------------------

#include "platform.h"

#ifndef _MSC_VER
#define COSIM_STUB
#include "armsim.hpp"
#endif

#include "ipblock.h"     // glbIpblockcreate
#include "ipconfig.h"    // static_ipblockcreate, dynamic_ipblockcreate

#ifndef _MSC_VER
#include "armsystem.h"   // ipblocks for arm
#include "xilinxitf.h"
#endif

#include "i8051system.h" // ipblocks for 8051
#include "picoblaze.h"  // ipblocks for picoblaze

// ------- ALLOCATOR --------
//
// decode typename and dynamically allocate 
// aipblock-inherited specific block

#define CREATE(S) if (!strcmp(tname, #S)) return new S(instname) 

aipblock * gplatform_ipblockcreate(char *instname, char *tname) {
  aipblock *r;

  r = static_ipblockcreate(instname, tname);
  if (r) return r;

#ifndef _MSC_VER
  CREATE(armsystem);
  CREATE(armsystemsource);
  CREATE(armsystemsink);
  CREATE(armsystemprobe);
  CREATE(armbuffer);
  CREATE(armfslslave);
  CREATE(armfslmaster);

  CREATE(xilinx_fsl_master);
  CREATE(xilinx_fsl_slave);
  CREATE(xilinx_fsl);
  CREATE(xilinx_ipif_reg);
  CREATE(xilinx_ipif_mem);
  CREATE(xilinx_ipif_wrfifo);
  CREATE(xilinx_ipif_rdfifo);

#ifdef SUPPORTSIMITSFU
  CREATE(armsfu2x2);
  CREATE(armsfu2x1);
  CREATE(armsfu3x1);
#endif
#endif
  CREATE(i8051system);
  CREATE(i8051systemsource);
  CREATE(i8051systemsink);
  CREATE(i8051buffer);

  // --- Eric's new picoblaze
  CREATE(picoblaze);

  r = dynamic_ipblockcreate(instname, tname);
  if (r) return r;

  cerr << "gplatform: Unknown ipblock type: " << tname << "\n";
  return new aipblock(instname);
}

//---------------------------------------------------------------

static void usage(char *fname) {
  cerr << "Usage : " << fname << " [-c max_cycles] [-d] gezel_description\n";
  cerr << "  -d : GEZEL value-change debug\n";
  cerr << "  -c : maximum number of clock cycles to simulate\n";
  cerr << "  -s : terminate when all ISS simulations are complete\n";
  cerr << "Supported cores:\n";
#ifndef _MSC_VER
  cerr <<      "  armsystem    Simit-ARM ISS + memory\n";
#endif
  cerr <<      "  i8051system  Intel 8051 ISS + memory\n";
  cerr <<      "  picosystem   Xilinx KCPSM3 Picoblaze ISS + memory\n";
}

/* ---------------------------------------------------- */

envptype *glbSimEnvp = 0;

unsigned int glbRunningISS;

int main(int argc, char *argv[], envptype envp) {
  unsigned long max_cnum  = (unsigned long long)-1;
  char * gezel_name = NULL;
  rtsimgen *RTsimgen = 0;
  unsigned cyclecount;
  unsigned ignoresleep = 0;

  glbSimEnvp = & envp;

  glbRunningISS = 0;

  glbIpblockcreate = gplatform_ipblockcreate;

  for(int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-d") == 0) {glbRTDebug = 1; glbVCDDebug = 1;} else
      if (strcmp(argv[i], "-s") == 0) {ignoresleep = 1; } else
	if(strcmp(argv[i], "-c") == 0) max_cnum = (unsigned long) atol(argv[++i]); else {
	  gezel_name = argv[i];
	  break;
	}
  }

  if (argc < 2) {
    usage(argv[0]);
    exit(0);
  }

  if (gezel_name == NULL) {
    cerr << "no gezel name\n";
    usage(argv[0]);
    return 0;
  }
  
  // load gezel program
  fdldebug = 0;
  if (call_parser(gezel_name)) {
    cerr << "Could not open " << gezel_name << "\n";
    exit(0);
  }
  
  if (glbRTDebug) {
    ofstream F("fdl.symbols");
    glbSymboltable.show(F);
    F.close();
  }
  
  glbAllowSimulatorSleep = 1;
  RTsimgen = new rtsimgen;
  glbSymboltable.createcode(RTsimgen);
  RTsimgen->reset();

  for (cyclecount=0; (cyclecount<max_cnum); cyclecount++) {    
    if (RTsimgen)
      RTsimgen->step();
    if ((ignoresleep || (glbRTSimMode == RTSimMode_sleep)) && (glbRunningISS == 0)) {
      break;
    }
  }

  for (vector<aipblock *>::iterator i = glbIpblocks.begin(); 
       i != glbIpblocks.end();
       i++)
    (*i)->stop();

  cerr << "Total Cycles: " << cyclecount << "\n";

  if (RTsimgen)
    delete RTsimgen;

  return 0;
}
