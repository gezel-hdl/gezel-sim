//--------------------------------------------------------------
// Copyright (C) 2007 P. Schaumont
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
#ifndef XILINXITF
#define XILINXITF

#include "armsystem.h" // we are using the arm core as surrogate 'microblaze'

class xilinx_fsl_slave : public aipblock {
  int             write_deviceid;
  int             status_deviceid;
  arm_simulator  *hook; 
  bool            interfacewritten;
  bool            tokenavailable;
 public:
  xilinx_fsl_slave(char *name);
  ~xilinx_fsl_slave();
  void setparm(char *_name);
  void run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool needsWakeupTest();
  bool cannotSleepTest();
  void write_device(int dev, unsigned long n);
  unsigned long read_device(int dev);
};

class xilinx_fsl_master : public aipblock {
  int             read_deviceid;
  int             status_deviceid;
  arm_simulator  *hook; 
  bool            tokenavailable;
  long            lastdata;
public:
  xilinx_fsl_master(char *name);
  ~xilinx_fsl_master();
  void setparm(char *_name);
  void run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool cannotSleepTest(); 
  unsigned long read_device(int dev);
};

//--- combined FSL interface (master + slave)
//    for code generation

class xilinx_fsl : public aipblock {

  arm_simulator  *hook; 

  int             slave_write_deviceid;
  int             slave_status_deviceid;
  bool            slave_tokenavailable;

  int             master_read_deviceid;
  int             master_status_deviceid;
  bool            master_tokenavailable;
  long            master_lastdata;

  bool            wakeup;

  enum portid {
    FSL_S_Clk,
    FSL_S_Read,
    FSL_S_Data,
    FSL_S_Control,
    FSL_S_Exists,
    FSL_M_Clk,
    FSL_M_Write,
    FSL_M_Data,
    FSL_M_Control,
    FSL_M_Full};

 public:

  xilinx_fsl  (char *name);
  ~xilinx_fsl ();
  void setparm(char *_name);
  void run    ();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool needsWakeupTest();
  bool cannotSleepTest();
  void write_device(int dev, unsigned long n);
  unsigned long read_device(int dev);
};


// Simple Memory-mapped register(s) attached to OPB
//
// $option "generic user_logic C_DWIDTH integer 32"
// $option "generic user_logic C_NUM_CE integer 2"
//
// ipblock reg3(out Bus2IP_Data    : ns(32); // 0 to dwidth-1
//              out Bus2IP_BE      : ns(4);  // 0 to dwidth/8 - 1
//              out Bus2IP_RdCE    : ns(4);  // 0 to c_num_ce - 1
//              out Bus2IP_WrCR    : ns(4);  // 0 to c_num_ce - 1
//              in  IP2Bus_Data    : ns(32);
//              in  IP2Bus_Ack     : ns(1);
//              in  IP2Bus_Retry   : ns(1);
//              in  IP2Bus_Error   : ns(1);
//              in  IP2Bus_ToutSup : ns(1)) {
//   iptype "xilinx_ipif_reg";
//   ipparm "core=arm";
//   ipparm "regid=0x80000000"; // index for regs
//   ipparm "opid =0x80000004"; // operation id
//   ipparm "busy =0x80000008"; // busy flag
//   ipparm "data =0x8000000C"; // data r/w channel
//  }

class xilinx_ipif_reg : public aipblock {
  arm_simulator  *hook; 
  int regid_dev;
  int opid_dev;
  int data_dev;

  int  opid;
  enum opid {idle, writing, reading};

  int regid;

  int dataitem;

  enum portid {
    Bus2IP_Data   ,
    Bus2IP_BE     ,
    Bus2IP_RdCE   ,
    Bus2IP_WrCE   ,
    IP2Bus_Data   ,
    IP2Bus_Ack    ,
    IP2Bus_Retry  ,
    IP2Bus_Error  ,
    IP2Bus_ToutSup };

  unsigned long setmask(unsigned long, unsigned);
  unsigned long resetmask(unsigned long, unsigned);

 public:

  xilinx_ipif_reg  (char *name);
  ~xilinx_ipif_reg ();
  void setparm(char *_name);
  void run    ();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool needsWakeupTest();
  bool cannotSleepTest();
  void write_device(int dev, unsigned long n);
  unsigned long read_device(int dev);
};

// Read FIFO on OPB
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
//    ipparm "depth=16";          // number of places in fifo
//    ipparm "read=0x80000010"; 
//    ipparm "status=0x80000014"; // request fifo to gezel
//  }
class xilinx_ipif_rdfifo : public aipblock {
  vector<unsigned>  rdfifo;
  unsigned          rdptr;
  unsigned          tokens;
  unsigned          maxtokens;

  arm_simulator    *hook;
  int             read_deviceid;
  int             status_deviceid;

  enum portid {
    IP2RFIFO_WrReq,	
    IP2RFIFO_Data,	
    RFIFO2IP_WrAck,	
    RFIFO2IP_AlmostFull,
    RFIFO2IP_Full       
  };

 public:

  xilinx_ipif_rdfifo  (char *name);
  ~xilinx_ipif_rdfifo ();
  void setparm        (char *name);
  void run            ();
  void showstatus     ();
  bool checkterminal  (int n, char *tname, aipblock::iodir d);
  bool cannotSleepTest();
  unsigned long read_device(int dev);

};

// Write FIFO on OPB
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
//    ipparm "depth=16";          // number of places in fifo
//    ipparm "write=0x80000010"; 
//    ipparm "status=0x80000014"; // request fifo to gezel
//  }

class xilinx_ipif_wrfifo : public aipblock {
  vector<unsigned>  wrfifo;
  unsigned          rdptr;
  unsigned          tokens;
  unsigned          maxtokens;

  arm_simulator    *hook;
  int               write_deviceid;
  int               status_deviceid;
  bool              interfacewritten;

  enum portid {
    IP2WFIFO_RdReq      ,
    WFIFO2IP_Data       ,
    WFIFO2IP_RdAck      ,
    WFIFO2IP_AlmostEmpty,
    WFIFO2IP_Empty      
  };

 public:

  xilinx_ipif_wrfifo  (char *name);
  ~xilinx_ipif_wrfifo ();
  void setparm        (char *name);
  void showstatus     ();
  void run            ();
  bool checkterminal  (int n, char *tname, aipblock::iodir d);
  bool cannotSleepTest();
  bool needsWakeupTest();
  void write_device(int dev, unsigned long d);
  unsigned long read_device(int dev);

};

// Memory-mapped interface attached to OPB
//
// $option "generic user_logic C_DWIDTH integer 32"
// $option "generic user_logic C_NUM_CE integer 2"
//
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
//   ipparm "busy =0x80000008"; // busy flag
//   ipparm "data =0x8000000C"; // data r/w channel
//  }

class xilinx_ipif_mem : public aipblock {
  arm_simulator  *hook; 
  int regid_dev;
  int opid_dev;
  int data_dev;

  int  opid;
  enum opid {idle, writing, reading};

  int regid;

  int dataitem;

  enum portid {
    Bus2IP_Data   ,
    Bus2IP_BE     ,
    Bus2IP_Burst  ,
    Bus2IP_RdCE   ,
    Bus2IP_WrCE   ,
    Bus2IP_RdReq  ,
    Bus2IP_WrReq  ,
    IP2Bus_Data   ,
    IP2Bus_RdAck  ,
    IP2Bus_WrAck  ,
    IP2Bus_Retry  ,
    IP2Bus_Error  ,
    IP2Bus_ToutSup};

  unsigned long setmask(unsigned long, unsigned);
  unsigned long resetmask(unsigned long, unsigned);

 public:

  xilinx_ipif_mem  (char *name);
  ~xilinx_ipif_mem ();
  void setparm(char *_name);
  void run    ();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool needsWakeupTest();
  bool cannotSleepTest();
  void write_device(int dev, unsigned long n);
  unsigned long read_device(int dev);
};


#endif

