//--------------------------------------------------------------
// Copyright (C) 2004 Jørgen Steensgaard-Madsen and Andreas Vad Lorenzen
// Danmark Technical University
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
// $Id: ipdtu.h,v 1.1.1.1 2005/07/14 17:32:44 schaum Exp $
//--------------------------------------------------------------
#ifndef IPDTU_H
#define IPDTU_H

#include "ipblock.h"


// ---------- ROM ---------

// Motivated by the microcontrol store of Andrew Tanenbaum's Mic-1 in his
// book Structured Computer Organization.  It offers the functionality of
// a read-only memory with an initial contents taken from a file with
// its name provided as an ipparm "file = ...".  This will typically be
// a binary file with its first four bytes forming a 'magic-number' that
// will be skipped.

// Author: Andreas Vad Lorenzen, modified by Jørgen Steensgaard-Madsen

class rom : public aipblock {
  vector<gval *> content;
  int wl;
  int size;
  char *file;
  int startbyte;
 public:
  rom(char *);
  void setparm(char *_name);
  void run();
  void printbinary(unsigned long long *value);
  void initrom();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool cannotSleepTest();
};

// ---------- IJVM memory ---------

// Like the type "rom" ipblock this is motivated by Tanenbaum's Mic-1.

// This describes a memory of 32 bit words to be initialised by the
// contents of a file with the following structure
//
//         int magic_number;
//         struct { int first_byte, length_in_bytes; unsigned char [length]; }
//           block1, block2;
//
// The format is that used by the mic1 simulator by Ray Ontko, as are the 
// default values for parameters sp, lv, and cpp.  Variation is needed to
// allow a model to be realised with an FPGA of modest size.  They are at
// the moment not used in the simulator.
//
// C++ does not support structures with a last member of non-fixed length, so
// it will be mimicked by use of dynamically allocated memory.

// Author: Jørgen Steensgaard-Madsen

class ijvm : public aipblock {
  vector<gval *> content;
  int wl;
  int size;
  unsigned int sp, lv, cpp;
  char *file;
 public:
  ijvm(char *);
  void setparm(char *_name);
  void run();
  void printbinary(unsigned long long *value);
  void initialise();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool cannotSleepTest();
 private:

  typedef struct {
    int start, length;
    unsigned char *block;
  } ijvm_block;
  
  int  initsection(int first,ijvm_block &block);
  void getblock(int src_fd,ijvm_block &block);
};

#endif

