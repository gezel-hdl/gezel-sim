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
// $Id: ipdtu.cxx,v 1.1.1.1 2005/07/14 17:32:44 schaum Exp $
//--------------------------------------------------------------


#include "ipdtu.h"
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


rom::rom(char *inst) : aipblock(inst), wl(0), size(0), file(0), startbyte(4) {
  // printf("WL = %i",wl);
}

void rom::setparm(char *_name) {
  gval *v = make_gval(32,0);
  if (matchparm(_name, "size", *v)) {
    printf("rom: set size %li\n",v->toulong());
    size = v->toulong();
  } else if (matchparm(_name, "wl", *v)) {
    printf("rom: set wl %li\n",v->toulong());
    wl = v->toulong();
  } else if (matchparm(_name, "file", *v)) {
    printf("rom: set file %s\n",matchparm(_name, "file"));
    file = matchparm(_name, "file");
  } else if (matchparm(_name, "startbyte", *v)) {
    printf("rom: set startbyte %i\n",startbyte = v->toulong());
    return;
  } else
    printf("Error: Rom ipblock does not recognize parameter %s\n",_name);

  if (wl!=0 && size!=0 && file!=0 && startbyte != 0){
    initrom();
    cout << "---------------- (rom)\n";
  }
}

void rom::initrom(){
  int MAX = ((wl+7)/8)*size+startbyte;
  char *buffer = new char[MAX];

  int count = 0;

  FILE* src;
  int src_fd;

  while (file && *file && (*file == ' ')) file++;
  if (!(src = fopen(file,"r"))) {
    perror(file); 
    exit(1);
  }
  src_fd = fileno(src);

  content.clear();
  do {
    count += read(src_fd,&buffer[count],MAX-count);
    if (errno) { perror(file); exit(1); }
  } while (count < MAX);

  fclose(src);

  long int pos=startbyte,local;
  long int startpos=(wl+7)/8;
  union funny{ 
    char cshift[8];
    unsigned long long shift;
  } u;

  char number[30];
  local = 0;
  while(pos<MAX){
    for(int j=startpos, k=7; j>0 && pos<MAX; j--, k--){
      u.cshift[k] = buffer[pos++];
    }

    u.shift = u.shift >> (64-wl);
    sprintf(number,"0x%016llX",u.shift);
    // printf("rom[%03li] = %s (0x%016llx)\n",local++,number,u.shift);
    content.push_back(make_gval(number));
  }

  for (unsigned i=0; i<content.size(); ++i){ 
    content[i]->setwordlength(wl);
  }
  delete buffer;
}

#define VALUE(v,n) (v>>(31-n)) & 1
void rom::printbinary(unsigned long long *value){
  unsigned int *ivalue = (unsigned int *) value;
  printf("Hex value: %08X %08X \n",ivalue[1], ivalue[0]);
  printf("Bin value: ");
  for(int i=0; i<32; i++){
    printf("%i",VALUE(ivalue[1],i));
  }
  for(int i=0; i<32; i++){
    printf("%i",VALUE(ivalue[0],i));
  }
  printf("\n");    

}

bool rom::checkterminal(int n, char *tname, aipblock::iodir dir) {
  switch (n) {
  case 0:
    return (isinput(dir) && isname(tname, "address"));
    break;
  case 1:
    return (isinput(dir) && isname(tname, "rd"));
    break;
  case 2:
    return (isoutput(dir) && isname(tname, "odata"));
    break;
  }
  return false;
}

bool rom::cannotSleepTest() {
  return false; // rom can always cause 'sleep' mode
}

void rom::run() {
  // perform rom function using 
  //   io vector       #
  //   -----------------------------
  //   in address      0
  //   in rd           1
  //   out odata       2
  // cout << "rom::run operation " << ioval[1]->toulong() 
  //      << " at address " << ioval[0]->toulong() << "\n";
  if (ioval[1]->toulong() == 1) {
    if (ioval[0]->toulong() > content.size()-1) {
      cout << "Error: Address value for rom ip block out-of-range\n";
    } else {
      // is a read
      *ioval[2] = *content[ioval[0]->toulong()];
    }
  }
}

//--------------------------------------------------------------------------

ijvm::ijvm(char *inst) 
   :  aipblock(inst), wl(32), size(0), 
      sp(0x8000), lv(0x6000), cpp(0x4000), file(0) {}


void ijvm::setparm(char *_name) {
  gval *v = make_gval(32,0);
  if (matchparm(_name, "size", *v)) {
    size = v->toulong();
    printf("ijvm: set size %x\n",size);
  }
  else if (matchparm(_name, "file", *v)) {
    file = matchparm(_name, "file");
    printf("ijvm: set file %s\n",file);
  }
  else if (matchparm(_name, "sp", *v)) {
    sp = v->toulong(); printf("ijvm: set sp %i\n",sp);
  }
  else if (matchparm(_name, "lv", *v)) {
    lv = v->toulong(); printf("ijvm: set lv %i\n",lv);
  }
  else if (matchparm(_name, "cpp", *v)) {
    cpp = v->toulong(); printf("ijvm: set cpp %i\n",cpp);
  }
  else {
    printf("Error: ijvm ipblock does not recognize parameter %s\n",_name);
    return;
  }
  if(size != 0 && file != 0) {
    ijvm::initialise();
    cout << "---------------- (ijvm)\n";
  }
}


void ijvm::getblock(int src_fd,ijvm_block &block){
  int c_cnt = 0;
  union {
    unsigned char bytes[4];
    int word;
  } buf;

  for (int i=0; i<4; i++) 
    if (-1 == read(src_fd,&buf.bytes[3-i],1)) {
      perror(file); exit(1); 
    }
  block.start  = buf.word;
  for (int i=0; i<4; i++) 
    if (-1 == read(src_fd,&buf.bytes[3-i],1)) {
      perror(file); exit(1);
    }
  block.length = buf.word;

  if (block.start % 4) {
    printf("The start address must be a multiplum of 4\n");
    exit(1);
  }

  if (block.length) {
    block.block = (unsigned char*)malloc(block.length);
    c_cnt = read(src_fd,block.block,block.length);
    if (c_cnt != (int)block.length) {
      if (c_cnt == -1) { perror(file); exit(1); }
      printf("Problem: requested %i bytes, got %i\n",block.length,c_cnt);
    }
  } else block.block = 0;

  for (int i=0; i<block.length; i+=4) {
    for (int j=0;j<4;j++) buf.bytes[j] = block.block[i+j];
    // printf("ijvm[%x+%0x] : %04x\n",block.start,i/4,buf.word);
  }
  // printf("=====\n");

}

int ijvm::initsection(int first,ijvm_block &block) {
  int w_cnt = 0, byteno = 3;
  union {
    unsigned char bytes[4];
    int word;
  } buf;
  char number[30];

  for (int i=first+1; i<=block.start/4; i++) {
    content.push_back(make_gval("0xdeadbeef"));
    w_cnt++;
  }
  // printf("Default values from address %x to address %x\n",first+1,first+w_cnt-1);
  

  // printf("Section of %i bytes, starting at address %x\n",block.length,first+w_cnt);
  for (int i=0; i<block.length; i++) {
      buf.bytes[byteno] = block.block[i]; 
      if (byteno) byteno--; else byteno = 3; 
      if (byteno == 3) {
	sprintf(number,"0x%08X",buf.word);
        w_cnt++; buf.word = 0xdeadbeef;
        content.push_back(make_gval(number));
	// printf("ijvm[%8x] : %s\n",first+w_cnt,number); // !!
      }
  }
  if (byteno!=3) {
    sprintf(number,"0x%08X",buf.word);
    w_cnt++;
    content.push_back(make_gval(number));
    // printf("ijvm[%8x] : %s\n",w_cnt,number);   // !! 
  }
  return w_cnt;
}

void ijvm::initialise(){ 

  ijvm_block block1, block2, tmp;

  unsigned int magic_number;

  FILE* src;
  int src_fd, c_cnt = 0, w_cnt = 0;

  while (file && *file && (*file == ' ')) file++;
  if (!(src = fopen(file,"r"))) {
    perror(file); 
    exit(1);
  }
  src_fd = fileno(src);

  content.clear();

  c_cnt = read(src_fd,&magic_number,4);
  if (c_cnt != 4) {
    if (c_cnt == -1) { perror(file); exit(1); }
    printf("Problem: needed 4 bytes, got %i\n",c_cnt);
  }

  // if (magic_number != 0xaddfea1d) { report_error(); return; }

  // printf("Magic number is %08x\n",magic_number);

  ijvm::getblock(src_fd,block1);
  ijvm::getblock(src_fd,block2);

  fclose(src);

  if (block1.start > block2.start){
     tmp = block1; block1 = block2; block2 = tmp;
  }

  // printf("block2.start(was: %x) := %x\n",block2.start,cpp);
  block2.start = 4*cpp;         // an ugly hack, but hard to avoid

  if (block1.start + block1.length > block2.start)
    cout << "Inconsistent block information\n";

  if (size < (block2.start + block2.length + 3) / 4){
    cout << "*** insufficient memory for ijvm code\n";
    return;
  }

  // initialise
  w_cnt = 0;

  w_cnt += initsection(0,block1);
  w_cnt += initsection(w_cnt,block2);

  for (int i=(block2.start+block2.length)/4+1; i<size; i++) {
    content.push_back(make_gval("0xdeadbeef")); w_cnt++;
  }

  for (unsigned int i = 0; i<content.size(); ++i) content[i]->setwordlength(32);

  // for (unsigned int i = 0; i<content.size(); ++i)           
  //   printf("ijvm[%8x] = %8lx\n",i,content[i]->toulong());


  // --------------------- done --------------------------------------------
}

bool ijvm::checkterminal(int n, char *tname, aipblock::iodir dir) {
  switch (n) {
  case 0:
    return (isinput(dir) && isname(tname, "address"));
    break;
  case 1:
    return (isinput(dir) && isname(tname, "wr"));
    break;
  case 2:
    return (isinput(dir) && isname(tname, "rd"));
    break;
  case 3:
    return (isinput(dir) && isname(tname, "idata"));
    break;
  case 4:
    return (isoutput(dir) && isname(tname, "odata"));
    break;
  case 5:   
    return (isinput(dir) && isname(tname, "bytes")); 
    break;
  case 6:   
    return (isinput(dir) && isname(tname, "fetch"));
    break;
  case 7:   
    return (isoutput(dir) && isname(tname, "byteval"));
    break;
  }
  return false;
}

bool ijvm::cannotSleepTest() {
  return false; // ram can always cause 'sleep' mode
}

void ijvm::run() {
  // perform ram function using 
  //   io vector       #
  //   -----------------------------
  //   in address      0
  //   in wr           1
  //   in rd           2
  //   in idata        3
  //   out odata       4
  //   in bytes        5
  //   in fetch        6
  //   out byteval     7

  // printf("ijvm::run called with address %x, (rd,wr,fetch) = (%li,%li,%li)\n",
  //	 ioval[0]->toulong(),
  //	 ioval[1]->toulong(),ioval[2]->toulong(),ioval[6]->toulong());

  if (ioval[2]->toulong() == 1) {
    if (ioval[0]->toulong() == 0xfffffffd) { // memory mapped I/O !
      char buf[15],data;
      // printf("Asking for input of one character: >\n");
      data = getchar();
      // printf("Got %c\n",data);
      sprintf(buf,"0x000000%02x",data);
      *ioval[4] = *make_gval(buf);
    } else 
    if (ioval[0]->toulong() > content.size()) {
      printf( "Error: Address value for ijvm ip block out-of-range %08lx (%li)\n",
	     ioval[0]->toulong(),ioval[0]->toslong());
    } else {
      // is a read
      //     cout << "ijvm read. address = " << ioval[0]->toulong() << "\n";
      *ioval[4] = *content[ioval[0]->toulong()];
      //     cout << "ijvm read. data    = " << ioval[4]->toulong() << "\n";
    }
  }
  if (ioval[1]->toulong() == 1) {
    if (ioval[0]->toulong() == 0xfffffffd) { // memory mapped I/O !
      putchar( (char)(0xFF & ioval[3]->toulong()));
      // printf("Writing '%c'\n",(char)(0xFF & ioval[3]->toulong()));
    } else
    if (ioval[0]->toulong() > content.size()) {
      printf("Error: Address value for ijvm ip block out-of-range %08lx (%li)\n",
	     ioval[0]->toulong(),ioval[0]->toslong());
    } else {
      // is a write
      //            cout << "ijvm write. address = " << ioval[0]->toslong() << "\n";
      //         cout << "ijvm write. data    = " << ioval[3]->toulong() << "\n";
      //      cout << "ijvm size = " << content.size() << "\n";
      //    cout << "ijvm write. address dnlchk = " << ioval[0]->toulong() << "\n";
      *content[ioval[0]->toulong()] = *ioval[3];
      // cout << "written data should be " << *ioval[3] 
      //      << " and is " << *content[ioval[0]->toulong()] << "\n";
    }
  }
  if (ioval[6]->toulong() == 1) {
    if (ioval[5]->toulong() > content.size()) {
      printf("Error: Address value for ijvm ip block out-of-range %08lx (%li) > %x\n",
	     ioval[6]->toulong(),ioval[6]->toslong(),content.size());
    } else {
      // is a read
      //   cout << "ijvm read. address = " << ioval[6]->toulong() << "\n";
      *ioval[7] = *content[ioval[5]->toulong()];
      //   cout << "ijvm read. data    = " << ioval[5]->toulong() << "\n";
    }
  }
  // printf("ijvm::run terminates\n");
}

