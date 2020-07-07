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
// $Id: ipuser.cxx,v 1.1.1.1 2005/07/14 17:32:44 schaum Exp $
//--------------------------------------------------------------

#include "ipblock.h"
#include "rterror.h"
#include "rtsleep.h"
#include "ipuser.h"
#include <cstdlib>

//  #     #  #####  ####### ######
//  #     # #     # #       #     #
//  #     # #       #       #     #
//  #     #  #####  #####   ######
//  #     #       # #       #   #
//  #     # #     # #       #    #
//   #####   #####  ####### #     #

// ---------- CTR32 ---------
ctr32::ctr32(char *inst) : aipblock(inst) { 
  value = make_gval(32,0);
  one = make_gval(32,0, "1");
}

void ctr32::run() {
  value->add_gval(*value,*one);
  *(ioval[0]) = *value;
}

bool ctr32::checkterminal(int n, char *tname, aipblock::iodir dir) {
  switch (n) {
  case 0:
    return (isoutput(dir) && isname(tname, "ctrvalue"));
    break;
  }
  return false;
}

// ---------- GCTR32 ---------
gctr32::gctr32(char *inst) : aipblock(inst) {
  value = make_gval(32,0);
  one = make_gval(32,0, "1");
}

void gctr32::run() {
  if (ioval[0]->toulong() == 1) 
    value->add_gval(*value,*one);
  *(ioval[1])  = *value;
}

bool gctr32::checkterminal(int n, char *tname, aipblock::iodir dir) {
  switch (n) {
  case 0:
    return (isinput(dir) && isname(tname, "enable"));
    break;
  case 1:
    return (isoutput(dir) && isname(tname, "ctrvalue"));
    break;
  }
  return false;
}

//----------- TRACER -----------
tracer::tracer(char *inst) : aipblock(inst) {
  td  = 0;
}

tracer::~tracer() {
  if (td)
    delete td;
}

void tracer::setparm(char *_name) {
  char *fname;
  gval *v = make_gval(32,0);

  if ((fname = matchparm(_name, "file"))) {
    td = new tracerdumper(fname);
  } else if (matchparm(_name, "wl", *v)) {
    if (td)
      td->setwl(v->toulong());
    else
      cout << "Error: Tracer ipblock must design target filename before wordlength\n";
  } else
    cout << "Error: Tracer ipblock does not recognize parameter " << _name << "\n";
}

bool tracer::checkterminal(int n, char *tname, aipblock::iodir dir) {
  switch (n) {
  case 0:
    return (isinput(dir) && isname(tname, "data"));
    break;
  default:
    return false;
    break;
  }
  return false;
}

bool tracer::cannotSleepTest() {
  return false;
}

void tracer::run() {
  td->addval(ioval[0]);
}

//----------- FILESOURCE -----------
filesource::filesource(char *inst) : aipblock(inst) {
  f  = 0;
  rv = 0;
  nfields = 1;
}

filesource::~filesource() {
  if (f) {
    f->close();
    delete f;
  }
  if (rv)
    delete rv;
}

void filesource::setparm(char *_name) {
  char *fname;
  gval *v = make_gval(32,0);

  if ((fname = matchparm(_name, "file"))) {
    f = new ifstream(fname);
    if (f->fail()) {
      cout << "Error: Filesource ipblock: Cannot open file " << fname << "\n";
      exit(0);
    }
    rv = make_gval(32, 0);
  } else if (matchparm(_name, "wl", *v)) {
    if (rv) {
      rv->setwordlength(v->toulong());
    } else {
      cout << "Error: Filesource ipblock must mention source filename before wordlength\n";
    }
  } else if (matchparm(_name, "base", *v)) {
    if (rv) {
      rv->setbase(v->toulong());
    } else {
      cout << "Error: Filesource ipblock must mention source filename before wordlength\n";
    }
  } else if (matchparm(_name, "fields", *v)) {
    nfields = v->toulong();
  } else
    cout << "Error: Filesource ipblock does not recognize parameter " << _name << "\n";
}

bool filesource::checkterminal(int n, char *tname, aipblock::iodir dir) {
  switch (n) {
  case 0:
    nfields = 1;
    return (isoutput(dir) && isname(tname, "d1"));
    break;
  case 1:
    nfields = 2;
    return (isoutput(dir) && isname(tname, "d2"));
    break;
  case 2:
    nfields = 3;
    return (isoutput(dir) && isname(tname, "d3"));
    break;
  case 3:
    nfields = 4;
    return (isoutput(dir) && isname(tname, "d4"));
    break;
  case 4:
    nfields = 5;
    return (isoutput(dir) && isname(tname, "d5"));
    break;
  case 5:
    nfields = 6;
    return (isoutput(dir) && isname(tname, "d6"));
    break;
  case 6:
    nfields = 7;
    return (isoutput(dir) && isname(tname, "d7"));
    break;
  case 7:
    nfields = 8;
    return (isoutput(dir) && isname(tname, "d8"));
    break;
  case 8:
    nfields = 9;
    return (isoutput(dir) && isname(tname, "d9"));
    break;
  case 9:
    nfields = 10;
    return (isoutput(dir) && isname(tname, "d10"));
    break;
  default:
    return false;
    break;
  }
  return false;
}

bool filesource::cannotSleepTest() {
  return false;
}

void filesource::run() {
  int k;
  for (k=0; k<nfields; k++) {
    if (! f->eof()) {
      *f >> *rv;
      // cerr << "file read value " << rv->toulong() << "\n";
      *ioval[k] = *rv;
    } else {
      cerr << "*** filesource " << name << ": end-of-file\n";
      break;
    }
  }
}

// ---------- RAM ---------
ram::ram(char *inst) : aipblock(inst), wl(0), f(0), rv(0), probestart(0), probestop(0) {
  regOutput(4);
}

void ram::setparm(char *_name) {
  gval *v = make_gval(32,0);
  char *fname;

  if (matchparm(_name, "size", *v)) {
    content.clear();
    cout << "ram: set size " << v->toulong() << "\n";
    for (unsigned i=0; i<v->toulong(); ++i)
      content.push_back(make_gval("0"));

    // output register to match xilinx block ram
    outputreg = make_gval("0");

  } else if ((fname = matchparm(_name, "file"))) {
    f = new ifstream(fname);
    if (f->fail()) {
      cout << "Error: ram ipblock: Cannot open file " << fname << "\n";
      exit(0);
    }
    rv = make_gval(32, 0);

  } else if (matchparm(_name, "wl", *v)) {
    cout << "ram: set wl " << v->toulong() << "\n";
    wl = v->toulong();
    if (rv) rv->setwordlength(v->toulong());

  } else if (matchparm(_name, "base", *v)) {
    if (rv) {
      rv->setbase(v->toulong());
    } else {
      cout << "Error: RAM ipblock must mention source filename before base\n";
    }

  } else
    cout << "Error: Ram ipblock does not recognize parameter " << _name << "\n";

  // check for intiialization
  if ((wl != 0) && (content.size() != 0)) {
    for (unsigned i=0; i<content.size(); ++i) 
      content[i]->setwordlength(wl);
    outputreg->setwordlength(wl);
  }

  if (rv) load_file();
}

void ram::load_file() {
  unsigned long addr;

  while (! f->eof()) {
    *f >> *rv;
    if (f->eof()) break;
    addr = rv->toulong();
    if (addr >= content.size()) {
      cerr << "ram file addr out-of-range  " << addr << "\n";
    }
    *f >> *rv;
    *(content[(unsigned)addr]) = *rv;
//cout << addr << "   " << *(content[(unsigned)addr]) << endl;
  }
}

bool ram::checkterminal(int n, char *tname, aipblock::iodir dir) {
  switch (n) {
  case 0:
    return (isinput(dir) && isname(tname, "address"));
    break;
  case 1:
    return (isinput(dir) && isname(tname, "wr"));
    break;
  case 2:
    return (isinput(dir) && isname(tname, "rd"));
    break;
  case 3:
    return (isinput(dir) && isname(tname, "idata"));
    break;
  case 4:
    return (isoutput(dir) && isname(tname, "odata"));
    break;
  }
  return false;
}

bool ram::cannotSleepTest() {
  return false; // ram can always cause 'sleep' mode
}

void ram::showcontents(unsigned from, unsigned to) {
  unsigned i;
  for (i = from; (i <= to) && (i < content.size()); i++) {
    cout << "ram " << name << ": [" << i << "]= " << hex << content[i]->toulong() << "\n";
  }
  if (from >= content.size()) {
    cerr << "ram: from/to probe: print out-of-range\n";
  }
  if (from > to) {
    cerr << "ram: from/to probe: from must be smaller than to\n";
  }
}

void ram::probe(char *_name) {
  gval *v = make_gval(32,0);
  char *cmd;
  if (matchparm(_name, "start", *v)) {
    probestart = v->toulong();
  } else if (matchparm(_name, "stop", *v)) {
    probestop = v->toulong();
  } else if ((cmd = matchparm(_name, "cmd"))) {
    if (!strcmp(cmd, "print")) {
      showcontents(probestart, probestop);
    }
  }
}

void ram::out_run() {
  // the ram output has an extra register to make it identical
  // to Xilinx' BlockRAM (and the VHDL view create by the code generator)
  ioval[4]->assignulong(outputreg->toulong());
}

void ram::run() {
  // perform ram function using 
  //   io vector       #
  //   -----------------------------
  //   in address      0
  //   in wr           1
  //   in rd           2
  //   in idata        3
  //   out odata       4

  // WRITE_FIRST mode (default for Xilinx)

  if (ioval[1]->toulong() == 1) {
    if (ioval[0]->toulong() >= content.size()) {
      cout << "Error: Address value for ram ip block out-of-range\n";
    } else {
      // is a write
//       cout << hex <<"ram write. address = " << ioval[0]->toulong() << "  ";
//       cout << " data    = " << ioval[3]->toulong() << "\n";
//       cout << "ram size = " << content.size() << "\n";
//       cout << "ram write. address dnlchk = " << ioval[0]->toulong() << "\n";

      *content[ioval[0]->toulong()] = *ioval[3];

//       cout << "written data should be " << *ioval[3] << " and is " << hex << *content[ioval[0]->toulong()] << "\n";
    }
  }

  if (ioval[2]->toulong() == 1) {
    if (ioval[0]->toulong() >= content.size()) {
      cout << "Error: Address value for ram ip block out-of-range\n";
    } else {
      // is a read
//       cout << "ram read. address = " << ioval[0]->toulong() << "\n";
      
      *outputreg = *content[ioval[0]->toulong()];

//       cout << "ram read. data    = " << outputreg->toulong() << "\n";
//       cout << "ram read. output port = " << ioval[4]->toulong() << "\n";
    }
  }

}

// ---------- WIDERAM ---------
wideram::wideram(char *inst) : aipblock(inst), wl(1), width(1) {}

void wideram::setparm(char *_name) {
  gval *v = make_gval(32,0);
  if (matchparm(_name, "size", *v)) {
    content.clear();
    cout << "wideram: set size " << v->toulong() << "\n";
    for (unsigned i=0; i<v->toulong(); ++i)
      content.push_back(make_gval("0xdeadbeefdeadbeef"));
    for (unsigned i=0; i<content.size(); ++i) 
      content[i]->setwordlength(wl);
  } else if (matchparm(_name, "wl", *v)) {
    cout << "wideram: set wl " << v->toulong() << "\n";
    wl = v->toulong();
  } else if (matchparm(_name, "width", *v)) {
    cout << "wideram: set wide output " << v->toulong() << "\n";
    width =  v->toulong();
    if (!width) width = 1;
  } else
    cout << "Error: Wideram ipblock does not recognize parameter " << _name << "\n";
}

bool wideram::checkterminal(int n, char *tname, aipblock::iodir dir) {
  switch (n) {
  case 0:
    return (isinput(dir) && isname(tname, "address"));
    break;
  case 1:
    return (isinput(dir) && isname(tname, "wr"));
    break;
  case 2:
    return (isinput(dir) && isname(tname, "rd"));
    break;
  case 3:
    return (isinput(dir) && isname(tname, "idata"));
    break;
  case 4:
    return (isoutput(dir) && isname(tname, "odata"));
    break;
  case 5:
    return (isoutput(dir) && isname(tname, "wodata"));
    break;
  }
  return false;
}

bool wideram::cannotSleepTest() {
  return false; // wideram can always cause 'sleep' mode
}

void wideram::run() {
  // perform wideram function using 
  //   io vector       #
  //   -----------------------------
  //   in address      0
  //   in wr           1
  //   in rd           2
  //   in idata        3
  //   out odata       4
  //   out wodate      5
  if (ioval[2]->toulong() == 1) {
    if (ioval[0]->toulong() > content.size()) {
      cout << "Error: Address value for wideram ip block out-of-range\n";
    } else {
      // is a read
      //   cout << "wideram read. address = " << ioval[0]->toulong() << "\n";
      *ioval[4] = *content[ioval[0]->toulong()];
      // cout << "wideram read. data    = " << ioval[4]->toulong() << "\n";

      // do wide read: read 'width' consecutive words starting from address
      // and concatenate them
      *ioval[5] = *content[ioval[0]->toulong()];
      //      cout << "WO " << *ioval[5] << "\n";
      for (unsigned i=ioval[0]->toulong() + 1;
	   (i<(ioval[0]->toulong() + width)) && (i<content.size()); 
	   i++) {
	ioval[5]->concat_gval(*ioval[5], *content[i]);
	//	cout << "WO " << *ioval[5] << "\n";
      }
    }
  }
  if (ioval[1]->toulong() == 1) {
    if (ioval[0]->toulong() > content.size()) {
      cout << "Error: Address value for wideram ip block out-of-range\n";
    } else {
      // is a write
      //          cout << "wideram write. address = " << ioval[0]->toulong() << "\n";
      //       cout << "wideram write. data    = " << ioval[3]->toulong() << "\n";
      //      cout << "wideram size = " << content.size() << "\n";
      //    cout << "wideram write. address dnlchk = " << ioval[0]->toulong() << "\n";
      *content[ioval[0]->toulong()] = *ioval[3];
      // cout << "written data should be " << *ioval[3] << " and is " << *content[ioval[0]->toulong()] << "\n";
    }
  }
}

//-----------------------------------------
gfmul16::gfmul16(char *inst) : aipblock(inst), lasta(0), lastb(0) {}

void gfmul16::run() {
  unsigned i, r;

  if ( (lasta != ioval[0]->toulong())
       || (lastb != ioval[1]->toulong())) {

    lasta = ioval[0]->toulong();
    lastb = ioval[1]->toulong();

    // performs a gf(2^16) multiplication with irreducible
    // polynomial g(t) = t^16 + t^12 + t^3 + t + 1 

    /* sum */
    r = (lasta      & (-(lastb  &     1))) ^ 
      ((lasta <<  1) & (-(lastb &     2))) ^ 
      ((lasta <<  2) & (-(lastb &     4))) ^ 
      ((lasta <<  3) & (-(lastb &     8))) ^ 
      ((lasta <<  4) & (-(lastb &    16))) ^ 
      ((lasta <<  5) & (-(lastb &    32))) ^ 
      ((lasta <<  6) & (-(lastb &    64))) ^ 
      ((lasta <<  7) & (-(lastb &   128))) ^ 
      ((lasta <<  8) & (-(lastb &   256))) ^ 
      ((lasta <<  9) & (-(lastb &   512))) ^ 
      ((lasta << 10) & (-(lastb &  1024))) ^ 
      ((lasta << 11) & (-(lastb &  2048))) ^ 
      ((lasta << 12) & (-(lastb &  4096))) ^ 
      ((lasta << 13) & (-(lastb &  8192))) ^ 
      ((lasta << 14) & (-(lastb & 16384))) ^
      ((lasta << 15) & (-(lastb & 32768)));
    
    /* modulo reduction */
    for (i = 16; i > 0; i--) {
      if (r & (1 << (i+15))) 
	r ^= (0x1100B << (i-1));
    }

    ioval[2]->assignulong(r);
  }
}
       

bool gfmul16::checkterminal(int n, char *tname, aipblock::iodir dir) {
  switch (n) {
  case 0:
    return (isinput(dir) && isname(tname, "a"));
    break;
  case 1:
    return (isinput(dir) && isname(tname, "b"));
    break;
  case 2:
    return (isoutput(dir) && isname(tname, "o"));
    break;
  }
  return false;
}

//-----------------------------------------
rand16::rand16(char *inst) : aipblock(inst) {}

void rand16::run() {
  ioval[0]->assignulong(rand() & 0xFFFF);
}
       

bool rand16::checkterminal(int n, char *tname, aipblock::iodir dir) {
  switch (n) {
  case 0:
    return (isoutput(dir) && isname(tname, "o"));
    break;
  }
  return false;
}

