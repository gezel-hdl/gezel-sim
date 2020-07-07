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

#include "xilinxitf.h"
#include <stdlib.h>

//--------------------------------------------------------
//
// Xilinx FSL Interface
//
// ipblock fsl(in  FSL_S_Clk     : ns(1);  // notused
//             in  FSL_S_Read    : ns(1);  // hshk for slave side
//             out FSL_S_Data    : ns(32); // data for slave side
//             out FSL_S_Control : ns(1);  // control for slave side
//             out FSL_S_Exists  : ns(1);  // hshk for slave side
//             in  FSL_M_Clk     : ns(1);  // notused
//             in  FSL_M_Write	 : ns(1);  // hshk for master side
//	       in  FSL_M_Data	 : ns(32); // data for master side
//	       in  FSL_M_Control : ns(1);  // control for master side
//	       out FSL_M_Full    : ns(1)) { // hshk for master side
//
//    iptype "xilinx_fsl";
//    ipparm "core=arm1";                // strongarm core
//    ipparm "slavewrite  = 0x80000000"; // address used to write slave data
//    ipparm "slavestatus = 0x80000008"; // address used to poll slave status
//    ipparm "masterread  = 0x80000004"; // address used to read master data
//    ipparm "masterstatus= 0x80000008"; // address used to poll master status
// }

xilinx_fsl::xilinx_fsl(char *name) : 
  aipblock(name) {
  hook     = 0;
  slave_write_deviceid   = 0;
  slave_status_deviceid  = 0;
  wakeup                 = false;
  slave_tokenavailable   = false;
  master_read_deviceid   = 0;
  master_status_deviceid = 0;
  master_tokenavailable  = false;
  master_lastdata        = 0;
}

xilinx_fsl::~xilinx_fsl() {}

void xilinx_fsl::setparm(char *name) {
  gval *v = make_gval(32,0);
  char *pname;
  
  if ((pname = matchparm(name, "core"))) {
    if ((hook = findcore(pname)))
      return;
    cerr << "xilinx_fsl: core " << pname << " not found\n";
    exit(0);
  } else if (matchparm(name, "slavewrite", *v)) {
    if (hook) {
      slave_write_deviceid = hook->mem->register_addr(v->toulong()); 
      register_ipblock(slave_write_deviceid, this);
    } else {
      cerr << "xilinx_fsl: set core parameter before selecting address\n";
      exit(0);
    }
    return;
  } else if (matchparm(name, "slavestatus", *v)) {
    if (hook) {
      slave_status_deviceid = hook->mem->register_addr(v->toulong()); 
      register_ipblock(slave_status_deviceid, this);
    } else {
      cerr << "xilinx_fsl: set core parameter before selecting address\n";
      exit(0);
    }
    return;
  } else if (matchparm(name, "masterread", *v)) {
    if (hook) {
      master_read_deviceid = hook->mem->register_addr(v->toulong()); 
      register_ipblock(master_read_deviceid, this);
    } else {
      cerr << "xilinx_fsl: set core parameter before selecting address\n";
      exit(0);
    }
    return;
  } else if (matchparm(name, "masterstatus", *v)) {
    if (hook) {
      master_status_deviceid = hook->mem->register_addr(v->toulong()); 
      register_ipblock(master_status_deviceid, this);
    } else {
      cerr << "xilinx_fsl: set core parameter before selecting address\n";
      exit(0);
    }
    return;
  }
  cerr << "xilinx_fsl: parameter " << name << " not understood\n";
}

void xilinx_fsl::run() {
  if (ioval[FSL_S_Read]->toulong() == 1) 
    slave_tokenavailable = 0;
  
  if (slave_tokenavailable)
    ioval[FSL_S_Exists]->assignulong(1); // exists <- 1
  else
    ioval[FSL_S_Exists]->assignulong(0); // exists <- 0

  if ((master_tokenavailable==false) && (ioval[FSL_M_Write]->toulong()==1)) {
    master_lastdata = ioval[FSL_M_Data]->toulong(); // current data
    master_tokenavailable = true;
  }
  if (master_tokenavailable) 
    ioval[FSL_M_Full]->assignulong(1); // full <- 1
  else
    ioval[FSL_M_Full]->assignulong(0); // full <- 0
}


bool xilinx_fsl::checkterminal(int n, char *tname, aipblock::iodir d) {
  switch (n) {
  case FSL_S_Clk:
    return (isinput(d) && isname(tname,  "FSL_S_Clk"));
    break;
  case FSL_S_Read:
    return (isinput(d) && isname(tname,  "FSL_S_Read"));
    break;
  case FSL_S_Data:
    return (isoutput(d) && isname(tname,  "FSL_S_Data"));
    break;
  case FSL_S_Control:
    return (isoutput(d) && isname(tname,  "FSL_S_Control"));
    break;
  case FSL_S_Exists:
    return (isoutput(d) && isname(tname,  "FSL_S_Exists"));
    break;
  case FSL_M_Clk:
    return (isinput(d) && isname(tname,  "FSL_M_Clk"));
    break;
  case FSL_M_Write:
    return (isinput(d) && isname(tname,  "FSL_M_Write"));
    break;
  case FSL_M_Data:
    return (isinput(d) && isname(tname,  "FSL_M_Data"));
    break;
  case FSL_M_Control:
    return (isinput(d) && isname(tname,  "FSL_M_Control"));
    break;
  case FSL_M_Full:
    return (isoutput(d) && isname(tname,  "FSL_M_Full"));
    break;
  }
  return false;
}

bool xilinx_fsl::needsWakeupTest() {
  bool v = wakeup;
  wakeup = false;
  return v;
}

bool xilinx_fsl::cannotSleepTest() {
  bool v = wakeup;
  wakeup = false;
  return v;
}

void xilinx_fsl::write_device(int dev, unsigned long n) {
  ioval[FSL_S_Data]->assignulong((long) n);
  ioval[FSL_S_Control]->assignulong((long) 0);
  wakeup = true;
  slave_tokenavailable = true;
}

unsigned long xilinx_fsl::read_device(int deviceid) {
  if (deviceid == slave_status_deviceid) {
    return ioval[FSL_S_Read]->toulong();
  } else if (deviceid == master_read_deviceid) {
    master_tokenavailable = false;
    wakeup = true;
    return master_lastdata;
  } else if (master_tokenavailable) 
    return 1;
  return 0;
}

//-----------------------------------------------------------------
// Simple Memory-mapped register(s) attached to OPB
//
// ipblock reg3(out Bus2IP_Data    : ns(32); // 0 to dwidth-1
//              out Bus2IP_BE      : ns(4);  // 0 to dwidth/8 - 1
//              out Bus2IP_RdCE    : ns(4);  // 0 to c_num_ce - 1
//              out Bus2IP_WrCE    : ns(4);  // 0 to c_num_ce - 1
//              in  IP2Bus_Data    : ns(32);
//              in  IP2Bus_RdAck   : ns(1);
//              in  IP2Bus_WrAck   : ns(1);
//              in  IP2Bus_Error   : ns(1)) {
//   iptype "xilinx_ipif_reg";
//   ipparm "core=arm";
//   ipparm "regid=0x80000000"; // index for regs
//   ipparm "opid =0x80000004"; // operation id
//   ipparm "busy =0x80000008"; // busy flag
//   ipparm "data =0x8000000C"; // data r/w channel
//  }

xilinx_ipif_reg::xilinx_ipif_reg(char *name) : aipblock(name) {
  hook = 0;
  regid_dev = 0;
  opid_dev  = 0;
  data_dev  = 0;

  opid      = idle;
  regid     = 0;
  dataitem  = 0;
}

xilinx_ipif_reg::~xilinx_ipif_reg () {}

void xilinx_ipif_reg::setparm(char *_name) {
  gval *v = make_gval(32,0);
  char *pname;
  
  if ((pname = matchparm(_name, "core"))) {
    if ((hook = findcore(pname)))
      return;
    cerr << "xilinx_ipif_reg: core " << pname << " not found\n";
    exit(0);

  } else if (matchparm(_name, "regid", *v)) {
    regid_dev = hook->mem->register_addr(v->toulong());
    register_ipblock(regid_dev, this);
    return;

  } else if (matchparm(_name, "opid", *v)) {
    opid_dev = hook->mem->register_addr(v->toulong());
    register_ipblock(opid_dev, this);
    return;

  } else if (matchparm(_name, "data", *v)) {
    data_dev = hook->mem->register_addr(v->toulong());
    register_ipblock(data_dev, this);
    return;
  }

  cerr << "xilinx_ipif_reg: parameter " << name << " not understood\n";
}

void xilinx_ipif_reg::run() {
  // write in progress

  if (opid == writing) {
    
    //    cerr << "writing cycle\n";
    
    ioval[Bus2IP_Data]->assignulong(dataitem);
    ioval[Bus2IP_WrCE]->assignulong(setmask(ioval[Bus2IP_WrCE]->toulong(), regid));
    ioval[Bus2IP_BE]->assignulong(15); // currently only support 32-bit writes

    if ( (ioval[IP2Bus_WrAck]->toulong() == 1) ||
	 (ioval[IP2Bus_Error]->toulong() == 1)
	 ) {
      // terminate write
      ioval[Bus2IP_WrCE]->assignulong(resetmask(ioval[Bus2IP_WrCE]->toulong(),regid));
      opid = idle;
      //      cerr << "writing cycle complete\n";
    }

  } else if (opid == reading) {

    dataitem = ioval[IP2Bus_Data]->toulong();
    ioval[Bus2IP_RdCE]->assignulong(setmask(ioval[Bus2IP_RdCE]->toulong(), regid));
    ioval[Bus2IP_BE]->assignulong(15); // currently only support 32-bit writes

    if ( (ioval[IP2Bus_RdAck]->toulong() == 1) ||
	 (ioval[IP2Bus_Error]->toulong() == 1)
	 ) {
      // terminate read
      ioval[Bus2IP_RdCE]->assignulong(resetmask(ioval[Bus2IP_RdCE]->toulong(),regid));
      opid = idle;
    }

  }

}

unsigned long xilinx_ipif_reg::setmask(unsigned long v, unsigned bit) {
  return v | (1 << (bit&31));
}

unsigned long xilinx_ipif_reg::resetmask(unsigned long v, unsigned bit) {
  return v & ~(1 << (bit&31));
}

void xilinx_ipif_reg::write_device(int dev, unsigned long n) {

  if (dev == regid_dev) {
    regid = n;
    //    cerr << "set regid " << regid << "\n";
  }

  if (dev == opid_dev) {
    if (opid != idle)
      cerr <<  "xilinx_ipif_reg: Warning - Operation in progress\n";
    else
      opid = n;
    //    cerr << "set opid " << opid << "\n";
  }

  if (dev == data_dev) {
    dataitem = n;
    //    cerr << "set dataitem " << dataitem << "\n";
  }
}

unsigned long xilinx_ipif_reg::read_device(int dev) {

  if (dev == regid_dev)
    return regid;

  if (dev == opid_dev)
    return opid;

  if (dev == data_dev)
    return dataitem;

  return 0;
}

bool xilinx_ipif_reg::checkterminal(int n, char *tname, aipblock::iodir d) {
  switch (n) {
  case Bus2IP_Data:  
    return (isoutput(d) && isname(tname,  "Bus2IP_Data"));   
    break;
  case Bus2IP_BE:  
    return (isoutput(d) && isname(tname,  "Bus2IP_BE"));   
    break;
  case Bus2IP_RdCE:  
    return (isoutput(d) && isname(tname,  "Bus2IP_RdCE"));   
    break;
  case Bus2IP_WrCE:  
    return (isoutput(d) && isname(tname,  "Bus2IP_WrCE"));   
    break;
  case IP2Bus_Data:  
    return (isinput(d)  && isname(tname,  "IP2Bus_Data"));   
    break;
  case IP2Bus_RdAck:  
    return (isinput(d)  && isname(tname,  "IP2Bus_RdAck"));   
    break;
  case IP2Bus_WrAck:  
    return (isinput(d)  && isname(tname,  "IP2Bus_WrAck"));   
    break;
  case IP2Bus_Error:  
    return (isinput(d)  && isname(tname,  "IP2Bus_Error"));   
    break;
  }
  return false;
}

bool xilinx_ipif_reg::needsWakeupTest() {
  if (opid != idle) return true;
  return false;
}

bool xilinx_ipif_reg::cannotSleepTest() {
  return false;
}

//------------------------------------------------------------------------
// Simple Memory-mapped register(s) attached to PLB
//
// $option "generic user_logic C_DWIDTH integer 32"
// $option "generic user_logic C_NUM_CE integer 2"
// $option "attribute_def SIGIS string"
// $option "attribute_use SIGIS Bus2IP_Clk   CLK"
// $option "attribute_use SIGIS Bus2IP_Reset RST"
//
// ipblock reg3(out Bus2IP_Data    : ns(32); // 0 to dwidth-1
//              out Bus2IP_BE      : ns(4);  // 0 to dwidth/8 - 1
//              out Bus2IP_RdCE    : ns(4);  // 0 to c_num_ce - 1
//              out Bus2IP_WrCE    : ns(4);  // 0 to c_num_ce - 1
//              in  IP2Bus_Data    : ns(32);
//              in  IP2Bus_RdAck   : ns(1);
//              in  IP2Bus_WrAck   : ns(1);
//              in  IP2Bus_Error   : ns(1)) {
//   iptype "xilinx_plb_ipif_reg";
//   ipparm "core=arm";
//   ipparm "regid=0x80000000"; // index for regs
//   ipparm "opid =0x80000004"; // operation id
//   ipparm "data =0x8000000C"; // data r/w channel
//  }

xilinx_plb_ipif_reg::xilinx_plb_ipif_reg(char *name) : aipblock(name) {
  hook      = 0;
  regid_dev = 0;
  opid_dev  = 0;
  data_dev  = 0;

  opid      = idle;
  regid     = 0;
  dataitem  = 0;
}

xilinx_plb_ipif_reg::~xilinx_plb_ipif_reg () {}

void xilinx_plb_ipif_reg::setparm(char *_name) {
  gval *v = make_gval(32,0);
  char *pname;
  
  if ((pname = matchparm(_name, "core"))) {
    if ((hook = findcore(pname)))
      return;
    cerr << "xilinx_plb_ipif_reg: core " << pname << " not found\n";
    exit(0);

  } else if (matchparm(_name, "regid", *v)) {
    regid_dev = hook->mem->register_addr(v->toulong());
    register_ipblock(regid_dev, this);
    return;

  } else if (matchparm(_name, "opid", *v)) {
    opid_dev = hook->mem->register_addr(v->toulong());
    register_ipblock(opid_dev, this);
    return;

  } else if (matchparm(_name, "data", *v)) {
    data_dev = hook->mem->register_addr(v->toulong());
    register_ipblock(data_dev, this);
    return;
  }

  cerr << "xilinx_plb_ipif_reg: parameter " << name << " not understood\n";
}

void xilinx_plb_ipif_reg::run() {
  // write in progress

  if (opid == writing) {


    ioval[Bus2IP_Data]->assignulong(dataitem);
    ioval[Bus2IP_WrCE]->assignulong(setmask(ioval[Bus2IP_WrCE]->toulong(), regid));

    ioval[Bus2IP_BE]->assignulong(15); // currently only support 32-bit writes

    if ( (ioval[IP2Bus_WrAck]->toulong() == 1) ||
	 (ioval[IP2Bus_Error]->toulong() == 1)
	 ) {
      // terminate write
      ioval[Bus2IP_WrCE]->assignulong(resetmask(ioval[Bus2IP_WrCE]->toulong(),regid));
      opid = idle;
    }

  } else if (opid == reading) {

    dataitem = ioval[IP2Bus_Data]->toulong();
    ioval[Bus2IP_RdCE]->assignulong(setmask(ioval[Bus2IP_RdCE]->toulong(), regid));
    ioval[Bus2IP_BE]->assignulong(15); // currently only support 32-bit writes

    if ( (ioval[IP2Bus_RdAck]->toulong() == 1) ||
	 (ioval[IP2Bus_Error]->toulong() == 1) 
	 ) {
      // terminate read
      ioval[Bus2IP_RdCE]->assignulong(resetmask(ioval[Bus2IP_RdCE]->toulong(),regid));
      opid = idle;
    }

  }

}

unsigned long xilinx_plb_ipif_reg::setmask(unsigned long v, unsigned bit) {
  return v | (1 << (bit&31));
}

unsigned long xilinx_plb_ipif_reg::resetmask(unsigned long v, unsigned bit) {
  return v & ~(1 << (bit&31));
}

void xilinx_plb_ipif_reg::write_device(int dev, unsigned long n) {

  if (dev == regid_dev) {
    regid = n;
    //    cerr << "set regid " << regid << "\n";
  }

  if (dev == opid_dev) {
    if (opid != idle)
      cerr <<  "xilinx_plb_ipif_reg: Warning - Operation in progress\n";
    else
      opid = n;
    //    cerr << "set opid " << opid << "\n";
  }

  if (dev == data_dev) {
    dataitem = n;
    //    cerr << "set dataitem " << dataitem << "\n";
  }
}

unsigned long xilinx_plb_ipif_reg::read_device(int dev) {

  if (dev == regid_dev)
    return regid;

  if (dev == opid_dev)
    return opid;

  if (dev == data_dev)
    return dataitem;

  return 0;
}

bool xilinx_plb_ipif_reg::checkterminal(int n, char *tname, aipblock::iodir d) {
  switch (n) {
  case Bus2IP_Data:  
    return (isoutput(d) && isname(tname,  "Bus2IP_Data"));   
    break;
  case Bus2IP_BE:  
    return (isoutput(d) && isname(tname,  "Bus2IP_BE"));   
    break;
  case Bus2IP_RdCE:  
    return (isoutput(d) && isname(tname,  "Bus2IP_RdCE"));   
    break;
  case Bus2IP_WrCE:  
    return (isoutput(d) && isname(tname,  "Bus2IP_WrCE"));   
    break;
  case IP2Bus_Data:  
    return (isinput(d)  && isname(tname,  "IP2Bus_Data"));   
    break;
  case IP2Bus_RdAck:  
    return (isinput(d)  && isname(tname,  "IP2Bus_RdAck"));   
    break;
  case IP2Bus_WrAck:  
    return (isinput(d)  && isname(tname,  "IP2Bus_WrAck"));   
    break;
  case IP2Bus_Error:  
    return (isinput(d)  && isname(tname,  "IP2Bus_Error"));   
    break;
  }
  return false;
}

bool xilinx_plb_ipif_reg::needsWakeupTest() {
  if (opid != idle) return true;
  return false;
}

bool xilinx_plb_ipif_reg::cannotSleepTest() {
  return false;
}

//---------------------------------------------------------------
// read fifo (fifo channel for data from GEZEL to SW)
//
// This fifo MUST be used in combination with an ipreg module
// (the user_logic module outline generated by xilinx combines a rdfifo
//  interface with an ipreg interface)
//
// $option "generic user_logic C_RDFIFO_DWIDTH integer 32"
//
// ipblock user_logic(in   IP2RFIFO_WrReq      : ns(1);
//                    in   IP2RFIFO_Data       : ns(32);
//                    out  RFIFO2IP_WrAck      : ns(1);
//                    out  RFIFO2IP_AlmostFull : ns(1);
//                    out  RFIFO2IP_Full       : ns(1)) {
//    ipparm "xilinx_ipif_rdfifo";
//    ipparm "core=arm1";
//    ipparm "depth=16";           // number of places in fifo
//    ipparm "read=0x80000010"; 
//    ipparm "status=0x80000014";  // indicates data available
//  }

xilinx_ipif_rdfifo::xilinx_ipif_rdfifo(char *name) : aipblock(name) {
  rdptr     = 0;
  tokens    = 0;
  maxtokens = 0;
  hook      = 0;
}

xilinx_ipif_rdfifo::~xilinx_ipif_rdfifo() {}

void xilinx_ipif_rdfifo::setparm(char *_name) {
  gval *v = make_gval(32,0);
  char *pname;
  
  if ((pname = matchparm(_name, "core"))) {
    if ((hook = findcore(pname)))
      return;
    cerr << "xilinx_ipif_rdfifo: core " << pname << " not found\n";
    exit(0);

  } else if (matchparm(_name, "depth", *v)) {
    // create depth places in the fifo (32 bit unsigned)
    for (unsigned i=0; i< v->toulong(); i++)
      rdfifo.push_back(0);
    maxtokens = v->toulong();
    return;

  } else if (matchparm(_name, "read", *v)) {
    read_deviceid = hook->mem->register_addr(v->toulong());
    register_ipblock(read_deviceid, this);
    return;

  } else if (matchparm(_name, "status", *v)) {
    status_deviceid = hook->mem->register_addr(v->toulong());
    register_ipblock(status_deviceid, this);
    return;

  } 

  cerr << "xilinx_ipif_rdfifo: parameter " << _name << " not understood\n";
}

void xilinx_ipif_rdfifo::showstatus() {
  cerr << "Read Fifo " << name << "\n";
  cerr << "RdPtr " << rdptr << " Tokens " << tokens << " MaxTokens " << maxtokens << "\n";
  for (unsigned i=0; i<tokens; i++) {
    cerr << "@RdPtr+" << i << ": " << rdfifo[(rdptr + i) % maxtokens] <<"\n";
  }
}

void xilinx_ipif_rdfifo::run() {
  // see if we can accept a token
  if ((ioval[IP2RFIFO_WrReq]->toulong() == 1) && (tokens < maxtokens)) {
    // yes
    ioval[RFIFO2IP_WrAck]->assignulong(1);
    rdfifo[(rdptr + tokens) % maxtokens] = ioval[IP2RFIFO_Data]->toulong();
    tokens++;

  } else {
    ioval[RFIFO2IP_WrAck]->assignulong(0);
  }

  if (tokens == (maxtokens - 1))
    ioval[RFIFO2IP_AlmostFull]->assignulong(1);
  else
    ioval[RFIFO2IP_AlmostFull]->assignulong(0);

  if (tokens == maxtokens)
    ioval[RFIFO2IP_Full]->assignulong(1);
  else
    ioval[RFIFO2IP_Full]->assignulong(0);

}

bool xilinx_ipif_rdfifo::checkterminal(int n, char *tname, aipblock::iodir d) {
  switch (n) {
  case IP2RFIFO_WrReq     :  
    return (isinput(d) && isname(tname,  "IP2RFIFO_WrReq"));       
    break;
  case IP2RFIFO_Data      :  
    return (isinput(d) && isname(tname,  "IP2RFIFO_Data"));       
    break;
  case RFIFO2IP_WrAck     :  
    return (isoutput(d) && isname(tname,  "RFIFO2IP_WrAck"));       
    break;
  case RFIFO2IP_AlmostFull:  
    return (isoutput(d) && isname(tname,  "RFIFO2IP_AlmostFull"));       
    break;
  case RFIFO2IP_Full      :  
    return (isoutput(d) && isname(tname,  "RFIFO2IP_Full"));       
    break;
  }
  return false;

}

bool xilinx_ipif_rdfifo::cannotSleepTest() {
  return false;
}

unsigned long xilinx_ipif_rdfifo::read_device(int dev) {
  if (dev == read_deviceid) {

    if (tokens > 0) {
      unsigned retval = rdfifo[rdptr];
      rdptr = (rdptr + 1) % maxtokens;
      tokens = tokens - 1;
      return retval;
    }
  } else if (dev == status_deviceid) {
    return tokens;
  }
  return 0;
}

//---------------------------------------------------------------
// write fifo (fifo channel for data from SW to GEZEL)
//
// This fifo MUST be used in combination with an ipreg module
// (the user_logic module outline generated by xilinx combines a rdfifo
//  interface with an ipreg interface)
//
// $option "generic user_logic C_WRFIFO_DWIDTH integer 32"
//
// ipblock user_logic(in   IP2WFIFO_RdReq       : ns(1);
//                    out  WFIFO2IP_Data        : ns(32);
//                    out  WFIFO2IP_RdAck       : ns(1);
//                    out  WFIFO2IP_AlmostEmpty : ns(1);
//                    out  WFIFO2IP_Empty       : ns(1)) {
//    ipparm "xilinx_ipif_wrfifo";
//    ipparm "core=arm1";
//    ipparm "depth=16";           // number of places in fifo
//    ipparm "write=0x80000010"; 
//    ipparm "status=0x80000014";  // indicates data available
//  }

xilinx_ipif_wrfifo::xilinx_ipif_wrfifo(char *name) : aipblock(name) {
  rdptr            = 0;
  tokens           = 0;
  maxtokens        = 0;
  hook             = 0;
  interfacewritten = false;
}

xilinx_ipif_wrfifo::~xilinx_ipif_wrfifo() { }

void xilinx_ipif_wrfifo::showstatus() {
  cerr << "Write Fifo " << name << "\n";
  cerr << "RdPtr " << rdptr << " Tokens " << tokens << " MaxTokens " << maxtokens << "\n";
  for (unsigned i=0; i<tokens; i++) {
    cerr << "@RdPtr+" << i << ": " << wrfifo[(rdptr + i) % maxtokens] <<"\n";
  }
}

void xilinx_ipif_wrfifo::run() {
  // see if there is a token ready to pass along
  if (tokens == 1)
    ioval[WFIFO2IP_AlmostEmpty]->assignulong(1);
  else
    ioval[WFIFO2IP_AlmostEmpty]->assignulong(0);

  if (tokens == 0)
    ioval[WFIFO2IP_Empty]->assignulong(1);
  else
    ioval[WFIFO2IP_Empty]->assignulong(0);

  if ((ioval[IP2WFIFO_RdReq]->toulong() == 1) && (tokens > 0)) {
    ioval[WFIFO2IP_Data]->assignulong(wrfifo[rdptr]);
    ioval[WFIFO2IP_RdAck]->assignulong(1);
    rdptr = (rdptr + 1) % maxtokens;
    tokens = tokens - 1;
    //    showstatus();
  } else {
    ioval[WFIFO2IP_RdAck]->assignulong(0);
  }
}

void xilinx_ipif_wrfifo::setparm(char *_name) {
  gval *v = make_gval(32,0);
  char *pname;
  
  if ((pname = matchparm(_name, "core"))) {
    if ((hook = findcore(pname)))
      return;
    cerr << "xilinx_ipif_wrfifo: core " << pname << " not found\n";
    exit(0);

  } else if (matchparm(_name, "depth", *v)) {
    // create depth places in the fifo (32 bit unsigned)
    for (unsigned i=0; i< v->toulong(); i++)
      wrfifo.push_back(0);
    maxtokens = v->toulong();
    return;

  } else if (matchparm(_name, "write", *v)) {
    write_deviceid = hook->mem->register_addr(v->toulong());
    register_ipblock(write_deviceid, this);
    return;

  } else if (matchparm(_name, "status", *v)) {
    status_deviceid = hook->mem->register_addr(v->toulong());
    register_ipblock(status_deviceid, this);
    return;

  } 
  cerr << "xilinx_ipif_rwrfifo: parameter " << _name << " not understood\n";
}

bool xilinx_ipif_wrfifo::checkterminal(int n, char *tname, aipblock::iodir d) {
  switch (n) {
  case IP2WFIFO_RdReq      : 
    return (isinput(d) && isname(tname,  "IP2WFIFO_RdReq"));           
    break;
  case WFIFO2IP_Data       : 
    return (isoutput(d) && isname(tname,  "WFIFO2IP_Data"));           
    break;
  case WFIFO2IP_RdAck      : 
    return (isoutput(d) && isname(tname,  "WFIFO2IP_RdAck"));           
    break;
  case WFIFO2IP_AlmostEmpty: 
    return (isoutput(d) && isname(tname,  "WFIFO2IP_AlmostEmpty"));           
    break;
  case WFIFO2IP_Empty      : 
    return (isoutput(d) && isname(tname,  "WFIFO2IP_Empty"));           
    break;
  }
  return false; 
}

bool xilinx_ipif_wrfifo::needsWakeupTest() {
  bool v = interfacewritten;
  interfacewritten = false;
  return v;
}

bool xilinx_ipif_wrfifo::cannotSleepTest() {
  bool v = interfacewritten;
  interfacewritten = false;
  return v;
}

void xilinx_ipif_wrfifo::write_device(int dev, unsigned long d) {
  if (dev == write_deviceid) {
    // write value at (rdptr + tokens) % maxtokens
    if (tokens < maxtokens) {
      wrfifo[(rdptr + tokens) % maxtokens] = d;
      tokens++;
      //      showstatus();
    }
  }
  interfacewritten =true;
}

unsigned long xilinx_ipif_wrfifo::read_device(int dev) {
  if (dev == status_deviceid) {
    return tokens;
  }
  return 0;
}

//-----------------------------------------------------------------
// Memory-mapped interface attached to OPB
// ipblock reg3(out Bus2IP_Data    : ns(32); // 0 to dwidth-1
//              out Bus2IP_BE      : ns(4);  // 0 to dwidth/8 - 1
//              out Bus2IP_Burst   : ns(1);
//              out Bus2IP_RdCE    : ns(4);  // 0 to c_num_ce - 1
//              out Bus2IP_WrCE    : ns(4);  // 0 to c_num_ce - 1
//              out Bus2IP_RdReq   : ns(1);
//              out Bus2IP_WrReq   : ns(1);
//              in  IP2Bus_Data    : ns(32);
//              in  IP2Bus_RdAck   : ns(1);
//              in  IP2Bus_WrAck   : ns(1);
//              in  IP2Bus_Retry   : ns(1);
//              in  IP2Bus_Error   : ns(1);
//              in  IP2Bus_ToutSup : ns(1)) {
//   iptype "xilinx_ipif_reg";
//   ipparm "core=arm";
//   ipparm "regid=0x80000000"; // index for regs
//   ipparm "opid =0x80000004"; // operation id
//   ipparm "data =0x8000000C"; // data r/w channel
//  }
xilinx_ipif_mem::xilinx_ipif_mem(char *name) : aipblock(name) {
  hook = 0;
  regid_dev = 0;
  opid_dev  = 0;
  data_dev  = 0;

  opid      = idle;
  regid     = 0;
  dataitem  = 0;
}

xilinx_ipif_mem::~xilinx_ipif_mem () {}

void xilinx_ipif_mem::setparm(char *_name) {
  gval *v = make_gval(32,0);
  char *pname;
  
  if ((pname = matchparm(_name, "core"))) {
    if ((hook = findcore(pname)))
      return;
    cerr << "xilinx_ipif_mem: core " << pname << " not found\n";
    exit(0);

  } else if (matchparm(_name, "regid", *v)) {
    regid_dev = hook->mem->register_addr(v->toulong());
    register_ipblock(regid_dev, this);
    return;

  } else if (matchparm(_name, "opid", *v)) {
    opid_dev = hook->mem->register_addr(v->toulong());
    register_ipblock(opid_dev, this);
    return;

  } else if (matchparm(_name, "data", *v)) {
    data_dev = hook->mem->register_addr(v->toulong());
    register_ipblock(data_dev, this);
    return;
  }

  cerr << "xilinx_ipif_mem: parameter " << name << " not understood\n";
}

void xilinx_ipif_mem::run() {
  // write in progress

  ioval[Bus2IP_Burst]->assignulong(0);

  if (opid == writing) {

    //    cerr << "writing cycle\n";

    ioval[Bus2IP_Data]->assignulong(dataitem);
    ioval[Bus2IP_WrCE]->assignulong(setmask(ioval[Bus2IP_WrCE]->toulong(), regid));
    ioval[Bus2IP_WrReq]->assignulong(1);
    ioval[Bus2IP_BE]->assignulong(15); // currently only support 32-bit writes

    if ( (ioval[IP2Bus_WrAck]->toulong() == 1) ||
	 (ioval[IP2Bus_Error]->toulong() == 1) ||
	 (ioval[IP2Bus_Retry]->toulong() == 1)
	 ) {
      // terminate write
      ioval[Bus2IP_WrCE]->assignulong(resetmask(ioval[Bus2IP_WrCE]->toulong(),regid));
      ioval[Bus2IP_WrReq]->assignulong(0);
      opid = idle;
      //      cerr << "writing cycle complete\n";
    }

  } else if (opid == reading) {

    dataitem = ioval[IP2Bus_Data]->toulong();
    ioval[Bus2IP_RdCE]->assignulong(setmask(ioval[Bus2IP_RdCE]->toulong(), regid));
    ioval[Bus2IP_RdReq]->assignulong(1);
    ioval[Bus2IP_BE]->assignulong(15); // currently only support 32-bit writes

    if ( (ioval[IP2Bus_RdAck]->toulong() == 1) ||
	 (ioval[IP2Bus_Error]->toulong() == 1) ||
	 (ioval[IP2Bus_Retry]->toulong() == 1)
	 ) {
      // terminate read
      ioval[Bus2IP_RdCE]->assignulong(resetmask(ioval[Bus2IP_RdCE]->toulong(),regid));
      ioval[Bus2IP_RdReq]->assignulong(0);
      opid = idle;
    }

  }

}

unsigned long xilinx_ipif_mem::setmask(unsigned long v, unsigned bit) {
  return v | (1 << (bit&31));
}

unsigned long xilinx_ipif_mem::resetmask(unsigned long v, unsigned bit) {
  return v & ~(1 << (bit&31));
}

void xilinx_ipif_mem::write_device(int dev, unsigned long n) {

  if (dev == regid_dev) {
    regid = n;
    //    cerr << "set regid " << regid << "\n";
  }

  if (dev == opid_dev) {
    if (opid != idle)
      cerr <<  "xilinx_ipif_mem: Warning - Operation in progress\n";
    else
      opid = n;
    //    cerr << "set opid " << opid << "\n";
  }

  if (dev == data_dev) {
    dataitem = n;
    //    cerr << "set dataitem " << dataitem << "\n";
  }
}

unsigned long xilinx_ipif_mem::read_device(int dev) {

  if (dev == regid_dev)
    return regid;

  if (dev == opid_dev)
    return opid;

  if (dev == data_dev)
    return dataitem;

  return 0;
}

bool xilinx_ipif_mem::checkterminal(int n, char *tname, aipblock::iodir d) {
  switch (n) {
  case Bus2IP_Data:  
    return (isoutput(d) && isname(tname,  "Bus2IP_Data"));   
    break;
  case Bus2IP_BE:  
    return (isoutput(d) && isname(tname,  "Bus2IP_BE"));   
    break;
  case Bus2IP_Burst:  
    return (isoutput(d) && isname(tname,  "Bus2IP_Burst"));   
    break;
  case Bus2IP_RdCE:  
    return (isoutput(d) && isname(tname,  "Bus2IP_RdCE"));   
    break;
  case Bus2IP_WrCE:  
    return (isoutput(d) && isname(tname,  "Bus2IP_WrCE"));   
    break;
  case Bus2IP_RdReq:  
    return (isoutput(d) && isname(tname,  "Bus2IP_RdReq"));   
    break;
  case Bus2IP_WrReq:  
    return (isoutput(d) && isname(tname,  "Bus2IP_WrReq"));   
    break;
  case IP2Bus_Data:  
    return (isinput(d)  && isname(tname,  "IP2Bus_Data"));   
    break;
  case IP2Bus_RdAck:  
    return (isinput(d)  && isname(tname,  "IP2Bus_RdAck"));   
    break;
  case IP2Bus_WrAck:  
    return (isinput(d)  && isname(tname,  "IP2Bus_WrAck"));   
    break;
  case IP2Bus_Retry:  
    return (isinput(d)  && isname(tname,  "IP2Bus_Retry"));   
    break;
  case IP2Bus_Error:  
    return (isinput(d)  && isname(tname,  "IP2Bus_Error"));   
    break;
  case IP2Bus_ToutSup:  
    return (isinput(d)  && isname(tname,  "IP2Bus_ToutSup"));   
    break;
  }
  return false;
}

bool xilinx_ipif_mem::needsWakeupTest() {
  if (opid != idle) return true;
  return false;
}

bool xilinx_ipif_mem::cannotSleepTest() {
  return false;
}
