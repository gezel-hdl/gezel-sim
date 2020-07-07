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
// $Id: i8051system.h 52 2006-05-28 19:33:23Z schaum $
//--------------------------------------------------------------

#ifndef I8051SYSTEM_H
#define I8051SYSTEM_H

#include "ipblock.h"
#include "i8051sim.h"
#include "platform.h"

class i8051system : public aipblock {
  int            period;
  int            period_cnt;
 public:
  i8051sim sim;
  i8051system(char *);
  void run();
  void setparm(char *_name);
  bool checkterminal(int n, char *tname, iodir dir);
  bool cannotSleepTest();
  bool needsWakeupTest();
  void blockincluded();
  void stop();
};

class i8051systemsource : public aipblock {
  int       address;
  bool      interfacewritten;
  i8051sim *hook; 

  int       portmapped;
  int       busmapped;
public:
  i8051systemsource(char *name);
  ~i8051systemsource();
  void setparm(char *_name);
  void run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool needsWakeupTest(); 
  bool cannotSleepTest(); 
  void touch();
};

class i8051systemsink : public aipblock {
  int       address;
  i8051sim *hook; 

  int       portmapped;
  int       busmapped;
public:
  i8051systemsink(char *name);
  ~i8051systemsink();
  void setparm(char *_name);
  void run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool cannotSleepTest(); 
};

class i8051buffer : public aipblock {
  unsigned  address;
  unsigned  range;
  i8051sim *hook; 
public:
  i8051buffer(char *name);
  ~i8051buffer();
  void setparm(char *_name);
  void run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool cannotSleepTest(); 
};

#endif
