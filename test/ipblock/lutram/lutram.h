#include "ipblock.h"

#ifndef lutram_H
#define lutram_H

class lutram : public aipblock {
  vector<gval *> content;
  int wlen, alen;
  int ramset;
public:
  lutram(char *name);
  void setparm(char *_name);
  void run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool needsWakeupTest();
  bool cannotSleepTest();
  void generate_vhd_view();
};

#endif
