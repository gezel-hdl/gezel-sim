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

// Integration of simulavr

#ifndef GEZELAVR_H
#define GEZELAVR_H

#include "simulavr/avrdevice.h"
#include "simulavr/avrfactory.h"
#include "simulavr/systemclock.h"
#include "simulavr/specialmem.h" // IO registers for read & write
#include "simulavr/pinnotify.h"
#include "simulavr/pin.h"
#include "ipblock.h"
#include "platform.h"

class atm128core : public aipblock {
  Pin *xtalpin;
  unsigned xtal;
  unsigned tracemode;
  fstream  tracefile;
 public:
  AvrDevice *sim;
  atm128core(char *);
  ~atm128core();
  void setparm(char *_name);
  void run();
  bool checkterminal(int n, char *tname, iodir dir);
  bool cannotSleepTest();
  bool needsWakeupTest();
  void blockincluded();
};

class atm128sourcepin : public aipblock, HasPinNotifyFunction {
  Pin       *avrpin;
  bool       interfacewritten;
  AvrDevice *hook;
 public:
  atm128sourcepin(char *);
  ~atm128sourcepin();
  void setparm(char *_name);
  void run();
  bool checkterminal(int n, char *tname, iodir dir);
  bool cannotSleepTest();
  bool needsWakeupTest();
  void PinStateHasChanged(Pin*);
};

class atm128sinkpin : public aipblock {
  Pin       *avrpin;
  Net        connection;
  AvrDevice *hook;
 public:
  atm128sinkpin(char *);
  ~atm128sinkpin();
  void setparm(char *_name);
  void run();
  bool checkterminal(int n, char *tname, iodir dir);
  bool cannotSleepTest();
  bool needsWakeupTest();
};

class atm128adpin : public aipblock, Pin {
  Net        connection;
  AvrDevice *hook;
 public:
  atm128adpin(char *);
  ~atm128adpin();
  void setparm(char *_name);
  void run();
  bool checkterminal(int n, char *tname, iodir dir);
  bool cannotSleepTest();
  bool needsWakeupTest();
};

#endif
