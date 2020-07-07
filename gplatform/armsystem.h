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
// $Id: armsystem.h 78 2007-07-14 19:00:16Z schaum $
//--------------------------------------------------------------

#ifndef ARMSYSTEM_H
#define ARMSYSTEM_H

#include "ipblock.h"

#define COSIM_STUB
#include "armsim.hpp"

using namespace simulator;

arm_simulator *findcore(char *pname);
void register_ipblock(int dev, aipblock *i);

class armsystem : public aipblock {
  char          *fpe; // name of FP emulator
 public:
  arm_simulator *sim;
 private:
  int            simargc;
  char          *simargv[32];
  int            period;
  int            period_cnt;
  aipblock *slave_exec;
public:
  armsystem(char *);
  void run();
  void setparm(char *_name);
  bool checkterminal(int n, char *tname, iodir dir);
  bool cannotSleepTest();
  bool needsWakeupTest();
  void blockincluded();
  void stop();

  // the armcore can only be run after SFU have been able to run
  // this is because there is an implicit data precedence between them
  // an aipblock can fill in slave_exec field to indicate this
  // the aipblock is then responsible to call slave_run
  // if slave_run is called with the correct aipblock * (itself)
  // it will then pass controll to the real run
  void setslave(aipblock *);
  void slaverun(aipblock *);
  void realrun();
};

class armsystemsource : public aipblock {
  int   deviceid;
  bool  interfacewritten;
  arm_simulator *hook; 
public:
  armsystemsource(char *name);
  void setparm(char *_name);
  void run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool needsWakeupTest();
  bool cannotSleepTest();
  void write_device(int dev, unsigned long n);

  //  void touch();
};

class armsystemsink : public aipblock {
  int   deviceid;
  arm_simulator *hook; 
public:
  armsystemsink(char *name);
  void setparm(char *_name);
  void run();
  unsigned long read_device(int dev);
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool cannotSleepTest();
};

class armbuffer : public aipblock {
  unsigned  address;
  unsigned  range;
  arm_simulator  *hook; 
public:
  armbuffer(char *name);
  ~armbuffer();
  void setparm(char *_name);
  void run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool cannotSleepTest(); 
};

//----------------

class armfslslave : public aipblock {
  int             write_deviceid;
  int             status_deviceid;
  arm_simulator  *hook; 
  bool            interfacewritten;
  bool            tokenavailable;
public:
  armfslslave (char *name);
  ~armfslslave();
  void setparm(char *_name);
  void run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool needsWakeupTest();
  bool cannotSleepTest();
  void write_device(int dev, unsigned long n);
  unsigned long read_device(int dev);
  //  void touch();      
  //  void r_touch(int );   // for status read
};

class armfslmaster : public aipblock {
  int             read_deviceid;
  int             status_deviceid;
  arm_simulator  *hook; 
  bool            tokenavailable;
  long            lastdata;
public:
  armfslmaster(char *name);
  ~armfslmaster();
  void setparm(char *_name);
  void run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool cannotSleepTest(); 
  unsigned long read_device(int dev);
  // void r_touch(int );
};

//----------------

#ifdef SUPPORTSIMITSFU

class armsfu2x2 : public aipblock {
  int            deviceid;
  arm_simulator *hook; 
  bool           interfacewritten;
  armsystem     *myarm;
public:
  armsfu2x2(char *name);
  void setparm(char *_name);
  void run();
  void out_run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool cannotSleepTest();
  bool needsWakeupTest();
  void touch();
};

class armsfu2x1 : public aipblock {
  int   deviceid;
  arm_simulator *hook; 
  bool           interfacewritten;
  armsystem     *myarm;
public:
  armsfu2x1(char *name);
  void setparm(char *_name);
  void run();
  void out_run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool cannotSleepTest();
  bool needsWakeupTest();
  void touch();
};

class armsfu3x1 : public aipblock {
  int   deviceid;
  arm_simulator *hook; 
  bool           interfacewritten;
  armsystem     *myarm;
public:
  armsfu3x1(char *name);
  void setparm(char *_name);
  void run();
  void out_run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool cannotSleepTest();
  bool needsWakeupTest();
  void touch();
};

#endif

//-- debug probe
class armsystemprobe : public aipblock {
  int   deviceid;
  bool runonce;
  bool  interfacewritten;
  simulator::arm_simulator *hook; 
  aipblock *probedblock;
 public:
  armsystemprobe(char *name);
  void setparm(char *_name);
  void run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool cannotSleepTest();
  bool needsWakeupTest();
  void write_device(int dev, unsigned long n);
  //  void touch();
};

#endif
