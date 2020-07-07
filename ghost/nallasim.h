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

//--------------------------------------------------------
// User space object
//
// int main() {
//    nallasim machine("hardware.fdl");
//
//    -- write other software --
//
//    machine.ReadUserRegister(socket, interface, addr, &data);
//
//    -- write other software --
//
//    machine.WriteUserRegister(socket, interface, addr, data);

#ifndef NALLATECHSIM_H
#define NALLATECHSIM_H

#include "fdlparse.h"
#include "fdlsym.h"
#include "rtsim.h"
#include "rtopt.h"
#include "nallatechfsb.h"
#include "ipblock.h"   // glbIpblockcreate
#include "ipconfig.h"  // stdipblockcreate
#include <string>

class nallasim {
  rtsimgen RTLsim;
 public:
  nallasim(string file);
  void ReadUserRegister( unsigned sock, unsigned itf, 
			 unsigned addr, unsigned *data);
  void WriteUserRegister(unsigned sock, unsigned itf, 
			 unsigned addr, unsigned data);
  void MemCopy( unsigned sock,
                unsigned *source, unsigned sourcebytes,
                unsigned *dest,   unsigned destbytes);
};

class nallacode {
 public:
  enum sysedgetype {sysedge0rx0tx, sysedge2rx2tx};
 private:
  enum sysedgetype edge;
  map <unsigned, string> userchannel;
  string splcell;

  void writeucf0rx0tx (ostream &);
  void writeucf2rx2tx (ostream &);
  void writetop0rx0tx (ostream &, string);
  void writetop2rx2tx (ostream &, string);
  void writecomp0rx0tx(ostream &);
  void writecomp2rx2tx(ostream &);
  void writeinst0rx0tx(ostream &);
  void writeinst2rx2tx(ostream &);
  void writecompuser  (ostream &, string n);
  void writecompspl   (ostream &, string n);
  void writeinstuser  (ostream &F, string n, unsigned id);
  void writeinstspl   (ostream &, string n);

  bool clkdiv;
  
 public:
  nallacode(enum sysedgetype p);
  void adduser   (unsigned channel, string cell);
  void addspl    (string cell);
  void createtop_fpga0();
  void divideclock();

};


#endif
