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

#include "nallatechfsb.h"

map<nalla_regio *, unsigned> nalla_regio_socket_map;
map<nalla_regio *, unsigned> nalla_regio_interface_map;

nalla_regio::nalla_regio(char *n) : 
  aipblock(n) {
  cstate = idle;
  read_complete = false;
  write_complete= false;
}

nalla_regio::~nalla_regio() {}

// ipblock nalla(out wdata : ns(16);   // write-data    
//               out addr  : ns(12);   // address       
//               out rden  : ns(1);    // read data enable
//               out wren  : ns(1);    // write data enable
//               in  rdata : ns(16);   // read-data
//               in  rdy   : ns(1))  {   // read signal
//   iptype = "nalla_regio";
//   ipparm = "socket=0";
//   ipparm = "interface=0";
// }

void nalla_regio::setparm(char *_name) {
  gval *v = make_gval(32, 0);
  if (matchparm(_name, "socket", *v)) {
    nalla_regio_socket_map[this] = v->toulong();
    return;
  } else if (matchparm(_name, "interface", *v)) {
    nalla_regio_interface_map[this] = v->toulong();
    return;
  }
  cerr << "nalla_regio: Parameter " << _name << " not understood\n";
}

bool nalla_regio::checkterminal(int n, char *tname, aipblock::iodir d) {
  switch (n) {
  case 0:
    return (isoutput(d) && isname(tname, "wdata"));
    break;
  case 1:
    return (isoutput(d) && isname(tname, "addr"));
    break;
  case 2:
    return (isoutput(d) && isname(tname, "rden"));
    break;
  case 3:
    return (isoutput(d) && isname(tname, "wren"));
    break;
  case 4:
    return (isinput(d) && isname(tname, "rdata"));
    break;
  case 5:
    return (isinput(d) && isname(tname, "rdy"));
    break;
  }
  return false;
}

// 0 wdata 
// 1 addr  
// 2 rden 
// 3 wren  
// 4 rdata 
// 5 rdy   

void nalla_regio::run() {
  ioval[0]->assignulong(access_wdata);
  ioval[1]->assignulong(access_address);
  access_rdata = ioval[4]->toulong();
  switch (cstate) {
  case idle:
    ioval[2]->assignulong(0);
    ioval[3]->assignulong(0);
    break;
  case writecycle:
    ioval[2]->assignulong(0);
    ioval[3]->assignulong(1);
    cstate = writewait;
    break;
  case readcycle:
    ioval[2]->assignulong(1);
    ioval[3]->assignulong(0);
    cstate = readwait;
    break;
  case writewait:
    ioval[2]->assignulong(0);
    ioval[3]->assignulong(0);
    if (ioval[5]->toulong()) {
      cstate = idle;
      write_complete = true;
    }
    break;
  case readwait:
    ioval[2]->assignulong(0);
    ioval[3]->assignulong(0);
    if (ioval[5]->toulong()) {
      cstate = idle;
      read_complete = true;
    }
    break;
  }
}

// This method can be repeatedly called:
//    WriteRegister(Socket, Address, Var) {
//       for each (O = find_nallatech_regio(Socket))
//         while (O->ext_write(Addres, Var) == false)
//           run gezel cycle

bool nalla_regio::ext_write(unsigned addr, unsigned data) {
  access_address = addr;
  access_wdata = data;
  if ((cstate == idle) && (write_complete == false))
    cstate = writecycle;
  if (write_complete) {
    write_complete = false;
    return true;
  } else 
    return false;
}

// This method can be repeatedly called:
//    ReadRegister(Socket, Address,*Var) {
//       for each (O = find_nallatech_regio(Socket))
//         while (O->ext_read(Addres, *Var) == false)
//           run gezel cycle

bool nalla_regio::ext_read(unsigned addr, unsigned *data) {
  access_address = addr;
  *data = access_rdata;
  if ((cstate == idle) && (read_complete == false))
    cstate = readcycle;
  if (read_complete) {
    read_complete = false;
    return true;
  } else
    return false;
}

bool nalla_regio::cannotSleepTest() {
  return false;
}

bool nalla_regio::needsWakeupTest() {
  return false;
}

//-------------------------------------------------------------------

map<nalla_spl *, unsigned> nalla_spl_socket_map;

// ipblock nalla2(out rx_data       : ns(256); // receive-data   // regOut
//                out rx_data_valid : ns(1);   // valid signal   // regOut
//                out rx_data_empty : ns(1);                     // regOut
//                in  rx_data_read  : ns(1);
//                in  tx_data       : ns(256);
//                in  tx_data_valid : ns(1);
//                out tx_data_almost_full : ns(1)) {             // regOut
//   iptype = "nalla_spl";
//   ipparm = "socket=0";
// }

nalla_spl::nalla_spl(char *_name) : aipblock(_name) {
  read_complete = false;
  write_complete= false;
  regOutput(0);
  regOutput(1);
  regOutput(2);
  regOutput(6);
}

nalla_spl::~nalla_spl() {}

void nalla_spl::setparm(char *_name) {
  gval *v = make_gval(32, 0);
  if (matchparm(_name, "socket", *v)) {
    nalla_spl_socket_map[this] = v->toulong();
    return;
  }
  cerr << "nalla_spl: Parameter " << _name << " not understood\n";
}

// 0 rx_data             
// 1 rx_data_valid       
// 2 rx_data_empty       
// 3 rx_data_read        
// 4 tx_data             
// 5 tx_data_valid       
// 6 tx_data_almost_full 

bool nalla_spl::checkterminal(int n, char *tname, aipblock::iodir d) {
  switch (n) {
  case 0:
    return (isoutput(d) && isname(tname, "rx_data"));
    break;
  case 1:
    return (isoutput(d) && isname(tname, "rx_data_valid"));
    break;
  case 2:
    return (isoutput(d) && isname(tname, "rx_data_empty"));
    break;
  case 3:
    return (isinput(d) && isname(tname, "rx_data_read"));
    break;
  case 4:
    return (isinput(d) && isname(tname, "tx_data"));
    break;
  case 5:
    return (isinput(d) && isname(tname, "tx_data_valid"));
    break;
  case 6:
    return (isoutput(d) && isname(tname, "tx_data_almost_full"));
    break;
  }
  return false;
}

void nalla_spl::out_run() {

  unsigned i, j, d[32/sizeof(unsigned)];

  ioval[1]->assignulong(0); // rx valid 
  ioval[2]->assignulong(1); // rx empty
  ioval[6]->assignulong(0); // tx full

  // this cycle writes an element, reads an element, or both
  ioval[0]->setwordlength(256); // 256 bit data - SPL to HW
  
  // write_spl operation: transfer data from SPL to HW
  if (buf_to_hardware.size()) {
    // elements are waiting to be send to HW
    // assign next element from buf_to_hardware to rx_data

    for (i = 0; i < 32/sizeof(unsigned); i++) 
      d[i] = buf_to_hardware[i];
    
    ioval[0]->assignuarray(d, 32/sizeof(unsigned));
    ioval[1]->assignulong(1); // rx valid
    ioval[2]->assignulong(0); // rx empty
  }    
  
  // read_spl operation: transfer data from HW to SPL
  ioval[6]->assignulong(0); // keep tx full low
  
}

void nalla_spl::run() {

  unsigned i, j, d[32/sizeof(unsigned)];

  ioval[4]->setwordlength(256); // 256 bit data - HW  to SPL
  
  // write_spl operation: transfer data from SPL to HW
  if (buf_to_hardware.size()) {
    // elements are waiting to be send to HW
    // assign next element from buf_to_hardware to rx_data

    if (ioval[3]->toulong()) 
      // data acknowledged, we can advance buffer
      buf_to_hardware.erase( buf_to_hardware.begin(),
			     buf_to_hardware.begin()+(32/sizeof(unsigned)));
    
    //    cout << "buffer now " << buf_to_hardware.size() << "\n";
    if (buf_to_hardware.size() == 0)
      write_complete = true;
  }    
  
  if (ioval[5]->toulong()) {

    ioval[4]->touarray(d, &j);

    for (i = 0; i < 32/sizeof(unsigned); i++) {
      buf_from_hardware.push_back(d[i]);
      if (data_expected)
	data_expected--;
    }

    //    cout << "data expected now " << data_expected << "\n";

    if (data_expected == 0)
      read_complete = true;
  }
  
}

/* ORIGINAL

void nalla_spl::run() {

  // WRITE
  // 0 rx_data             assign   to buf_to_hardware
  // 1 rx_data_valid       assign
  // 2 rx_data_empty       assign
  // 3 rx_data_read        read

  // READ
  // 4 tx_data             read     into buf_from_hardware
  // 5 tx_data_valid       read
  // 6 tx_data_almost_full assign                          

  unsigned i, j, d[32/sizeof(unsigned)];

  ioval[1]->assignulong(0); // rx valid 
  ioval[2]->assignulong(1); // rx empty
  ioval[6]->assignulong(0); // tx full

  // this cycle writes an element, reads an element, or both
  ioval[0]->setwordlength(256); // 256 bit data - SPL to HW
  ioval[4]->setwordlength(256); // 256 bit data - HW  to SPL
  
  // write_spl operation: transfer data from SPL to HW
  if (buf_to_hardware.size()) {
    // elements are waiting to be send to HW
    // assign next element from buf_to_hardware to rx_data

    for (i = 0; i < 32/sizeof(unsigned); i++) 
      d[i] = buf_to_hardware[i];
    
    ioval[0]->assignuarray(d, 32/sizeof(unsigned));
    ioval[1]->assignulong(1); // rx valid
    ioval[2]->assignulong(0); // rx empty
    
    if (ioval[3]->toulong()) 
      // data acknowledged, we can advance buffer
      buf_to_hardware.erase( buf_to_hardware.begin(),
			     buf_to_hardware.begin()+(32/sizeof(unsigned)));
    
    //    cout << "buffer now " << buf_to_hardware.size() << "\n";
    if (buf_to_hardware.size() == 0)
      write_complete = true;
  }    
  
  // read_spl operation: transfer data from HW to SPL
  ioval[6]->assignulong(0); // keep tx full low

  if (ioval[5]->toulong()) {

    ioval[4]->touarray(d, &j);

    for (i = 0; i < 32/sizeof(unsigned); i++) {
      buf_from_hardware.push_back(d[i]);
      if (data_expected)
	data_expected--;
    }

    //    cout << "data expected now " << data_expected << "\n";

    if (data_expected == 0)
      read_complete = true;
  }
  
}

*/

bool nalla_spl::cannotSleepTest() {
  return false;
}

bool nalla_spl::needsWakeupTest() {
  return false;
}

void nalla_spl::write_spl_init(unsigned *data, unsigned size) {
  unsigned i;

  buf_to_hardware.clear();
  for (i=0; i<size/sizeof(unsigned); i++)
    buf_to_hardware.push_back(data[i]);
}

bool nalla_spl::write_spl_check(unsigned *data, unsigned size) {
  if (write_complete) {
    write_complete = false;
    //    cout << "write complete\n";
    return true;
  } else  
    return false;
}

void nalla_spl::read_spl_init(unsigned *data, unsigned size) {
  data_expected = size/sizeof(unsigned);
}

bool nalla_spl::read_spl_check(unsigned *data, unsigned size) {
  unsigned i;
  if (read_complete) {
    read_complete = false;
    for (i=0; i<(size/sizeof(unsigned)); i++)
      data[i] = buf_from_hardware[i];
    //    cout << "read complete\n";
    return true;
  } else
    return false;
}
