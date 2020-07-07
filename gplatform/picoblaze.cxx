#include "picoblaze.h"

//---------------------------------------------------------
//  ipblock embedded_kcpsm3 (
//              out port_id : ns(8);
//              out write_strobe : ns(1);
//              out read_strobe : ns(1);
//              out out_port : ns(8);
//              in  in_port : ns(8);
//              in  interrupt : ns(1);
//              out interrupt_ack : ns(1)) {
//  iptype "picoblaze";
//  ipparm "exec=SMALL.DEC";  // DEC file from KCPSM3 assembler
//  ipparm "verbose=0";
//---------------------------------------------------------

void picoblaze::PortIn() {

  if (verbose)
    cout << glbRTCycle 
	 << ": PortIn[" 
	 << portid << "]" 
	 << endl;

  readoperation = true;
}

uint8_t picoblaze::PortIn2ndHalf() {

  if (verbose)
    cout << glbRTCycle 
	 << ": PortIn2ndHalf[" 
	 << portid << "]: " 
	 << ioval[PORT_IDATA]->toulong() 
	 << endl;

  return ioval[PORT_IDATA]->toulong();
}

void picoblaze::PortOut(uint8_t data) {

  if (verbose)
    cout << glbRTCycle 
	 << ": PortOut[" 
	 << portid 
	 << "]: " 
	 << data 
	 << endl;

  odata = data;
  writeoperation = true;
}

bool picoblaze::readflag() {

  bool retval;

  retval = readoperation;
  readoperation = false;

  return retval;
}

bool picoblaze::writeflag() {

  bool retval;

  retval = writeoperation;
  writeoperation = false;

  return retval;
}

picoblaze::~picoblaze() { }

picoblaze::picoblaze(char *n) : aipblock(n) { 
  verbose = 0;
  exec = TOP_HALF;
  pico.addPort(this);

  regOutput(PORT_INTERRUPT_ACK);
  regOutput(PORT_ID);
  regOutput(PORT_ODATA);
  regOutput(PORT_RS);
  regOutput(PORT_WS);
}

void picoblaze::out_run() {
  if (exec == TOP_HALF) {
    do_tophalf_out();
  } else if (exec == BOT_HALF) {
    do_bothalf_out();
  } else {
    assert((true == false) && "picoblaze: invalid exec type");
  }
}

void picoblaze::run() {
  if (exec == TOP_HALF) {
    do_tophalf();
    exec = BOT_HALF;
  } else if (exec == BOT_HALF) {
    do_bothalf();
    exec = TOP_HALF;
  } else {
    assert((true == false) && "picoblaze: invalid exec type");
  }
}

void picoblaze::do_tophalf_out() {
  pico.Next();

  // Interrupt acks always handled at tophalf
  ioval[PORT_INTERRUPT_ACK]->assignulong(should_ack_int);
  ioval[PORT_ID]->assignulong(portid);
  ioval[PORT_ODATA]->assignulong(odata);
  ioval[PORT_RS]->assignulong(0);
  ioval[PORT_WS]->assignulong(0);
}

void picoblaze::do_tophalf() {
  if (ioval[PORT_RESET]->toulong()) {
    pico.Reset();
  }
  else if (ioval[PORT_INTERRUPT]->toulong()) {
    pico.Interrupt();
  }

  // Figure out the value for the next instruction's
  // interrupt ack
  should_ack_int = ioval[PORT_INTERRUPT]->toulong();
}

void picoblaze::do_bothalf_out() {
  // Interrupt acks always handled at tophalf
  ioval[PORT_INTERRUPT_ACK]->assignulong(0);
  ioval[PORT_ID]->assignulong(portid);
  ioval[PORT_ODATA]->assignulong(odata);
  ioval[PORT_RS]->assignulong(readflag());
  ioval[PORT_WS]->assignulong(writeflag());
}

void picoblaze::do_bothalf() {
  pico.Next2ndHalf();
}

void picoblaze::setparm(char *_name) {

  gval *v = make_gval(32, 0);
  char *en;

  if ((en = matchparm(_name, "exec"))) {

    strncpy(execname, en, picoblaze::MAX_NAME_LEN);
    cerr << "picoblaze: executable [" << _name << "]\n";

    pico.code->loadCode(execname);
    pico.Reset();

  } else if (matchparm(_name, "verbose", *v)) {
    if (v->toulong() == 1)
      verbose = 1;
  } else {
    cerr << "picoblaze: Parameter not recognized: " << _name << "\n";
  }

}

bool picoblaze::checkterminal(int n, char *tname, iodir dir) {

//              out port_id : ns(8);
//              out write_strobe : ns(1);
//              out read_strobe : ns(1);
//              out out_port : ns(8);
//              in  in_port : ns(8);
//              in  interrupt : ns(1);
//              out interrupt_ack : ns(1);
//              in  reset : ns(1);
//              in  clk : ns(1)) {
  switch (n) {
    //************************************************ 
    // Input ports
    //************************************************ 
    case PORT_CLK:
      return (isinput(dir) && isname(tname, "clk"));
    case PORT_RESET:
      return (isinput(dir) && isname(tname, "reset"));
    case PORT_INTERRUPT:
      return (isinput(dir) && isname(tname, "interrupt"));
    case PORT_IDATA:
      return (isinput(dir) && isname(tname, "in_port"));

    //************************************************ 
    // Output ports
    //************************************************ 
    case PORT_ODATA:
      return (isoutput(dir) && isname(tname, "out_port"));
    case PORT_ID:
      return (isoutput(dir) && isname(tname, "port_id"));
    case PORT_RS:
      return (isoutput(dir) && isname(tname, "read_strobe"));
    case PORT_WS:
      return (isoutput(dir) && isname(tname, "write_strobe"));
    case PORT_INTERRUPT_ACK:
      return (isoutput(dir) && isname(tname, "interrupt_ack"));
  }

  return false;
}
bool picoblaze::cannotSleepTest() { return true; }
bool picoblaze::needsWakeupTest() { return false; }

