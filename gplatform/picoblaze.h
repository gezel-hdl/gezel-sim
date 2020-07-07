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
  bool readflag();
  bool writeflag();

};

#endif
