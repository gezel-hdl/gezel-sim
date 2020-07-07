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

#ifndef NALLATECH_FSB_H
#define NALLATECH_FSB_H

#include "ipblock.h"
#include <map>

class nalla_regio : public aipblock {
  enum STATE {idle, writecycle, readcycle, writewait, readwait};
  enum STATE cstate;

  unsigned access_address;
  unsigned access_wdata;
  unsigned access_rdata;
  
  bool read_complete;
  bool write_complete;

 public:
  nalla_regio(char *);
  ~nalla_regio();
  void setparm(char *_name);
  void run();
  bool checkterminal(int n, char *tname, iodir dir);
  bool cannotSleepTest();
  bool needsWakeupTest();
  bool ext_write(unsigned addr, unsigned data);
  bool ext_read(unsigned addr, unsigned *data);
};


class nalla_spl : public aipblock {
  vector<unsigned> buf_to_hardware;
  vector<unsigned> buf_from_hardware;
  unsigned data_expected;

  bool write_complete;
  bool read_complete;

 public:
  nalla_spl(char *);
  ~nalla_spl();
  void setparm(char *_name);
  void out_run();
  void run();
  bool checkterminal(int n, char *tname, iodir dir);
  bool cannotSleepTest();
  bool needsWakeupTest();
  void write_spl_init(unsigned *data, unsigned size);
  void read_spl_init(unsigned *data, unsigned size);
  bool write_spl_check(unsigned *data, unsigned size);
  bool read_spl_check(unsigned *data, unsigned size);
};

extern map<nalla_regio *, unsigned> nalla_regio_socket_map;
extern map<nalla_regio *, unsigned> nalla_regio_interface_map;
extern map<nalla_spl *, unsigned>   nalla_spl_socket_map;

#endif
