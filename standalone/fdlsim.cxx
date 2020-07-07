//--------------------------------------------------------------
// Copyright (C) 2003 Patrick Schaumont (schaum@ee.ucla.edu)                   
// University of California at Los Angeles
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
// $Id: fdlsim.cxx,v 1.1.1.1 2005/07/14 17:32:44 schaum Exp $
//--------------------------------------------------------------

#include "fdlparse.h"

#include "fdlsym.h"
#include "rtsim.h"
#include "rtopt.h"
// #include "mpzcl.h" // for mpz_init

#include "module.h"
#include "ipblock.h"   // glbIpblockcreate
#include "ipconfig.h"  // stdipblockcreate

using namespace std;

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>

void usage(char *p) {
  cout << "Usage: " << p << " [-d] [<filename>] <cycles>\n";
  cout << "  -h          print this message\n";
  cout << "  -d          value-change trace mode\n";
  cout << "  [filename]  optional file name (default=stdin)\n";
  cout << "  cycles      number of cycles to simulate (-1=forever)\n";
}

int main(int argc, char **argv) {

  glbAllowSimulatorSleep = 0;
  glbUseGVALINT = 0;
  int floc = 1;
  long cycles = -1;
  char *filearg = (char *) "stdin";

  glbIpblockcreate = stdipblockcreate;

  if (argc < 2) {
    usage(argv[0]);
    exit(0);
  }
    
  if(strcmp(argv[1], "-h") == 0) {
    floc++;
    usage(argv[0]);
    exit(0);
  }
  
  if(strcmp(argv[1], "-d") == 0) {
    glbRTDebug = 1;
    glbVCDDebug = 1;
    floc++;
  }
  
  if ((floc+2) == argc) {
    cycles  = atoi(argv[floc+1]);
    filearg = argv[floc];
  } else if ((floc+1) == argc) {
    cycles  = atoi(argv[argc-1]);
    if (cycles == 0) {
      usage(argv[0]);
      exit(0);
    }
  } else {
    usage(argv[0]);
    exit(0);
  }

  if (call_parser(filearg)) {
    cout << "Failed to parse " << filearg << "\n";
    exit(0);
  }

  if (glbRTDebug) {
    ofstream F("fdl.symbols");
    glbSymboltable.show(F);
    F.close();
    //    glbModuleTable.show();
  }

  rtsimgen S;
  glbSymboltable.createcode(&S);
  S.reset();

  S.run(cycles); // clock cycles
  S.showstats(cout);

  return 0;
}
