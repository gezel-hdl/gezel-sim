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

#ifndef XPICOBLAZE_H
#define XPICOBLAZE_H

#include "ipblock.h"
#include "cpicoblaze.h"
#include "platform.h"

class picoblaze : public aipblock, public CPort {

public:

  static const int MAX_NAME_LEN = 255;

  enum exec_half {
    TOP_HALF,
    BOT_HALF
  };

  enum port_ids {
    PORT_ID, 
    PORT_WS, 
    PORT_RS, 

    PORT_ODATA, 
    PORT_IDATA, 

    PORT_INTERRUPT, 
    PORT_INTERRUPT_ACK,

    PORT_RESET,
    PORT_CLK
  };

  picoblaze(char *name);
  ~picoblaze();

  void setparm(char *_name);
  void run();
  void out_run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool cannotSleepTest(); 
  bool needsWakeupTest();

  void    PortIn();
  uint8_t PortIn2ndHalf();
  void    PortOut(uint8_t data);

private:

  CPicoBlaze pico;

  enum exec_half exec;
  int verbose;
  char execname[MAX_NAME_LEN+1];
  int should_ack_int;

  bool readoperation;
  bool writeoperation;
  uint8_t odata;

  void do_tophalf();
  void do_bothalf();
  void do_tophalf_out();
  void do_bothalf_out();
  bool readflag();
  bool writeflag();

};

#endif
