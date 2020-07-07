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
// $Id$
//--------------------------------------------------------------

#include "ipblock.h"
#include "rterror.h"
#include "rtsleep.h"

//
// empty factory for ipblock (user must intialize this ptr to real factory
//
aipblock *nullipblockcreate(char *, char *) { return 0; }

// default init
glbIpblockcreate_t glbIpblockcreate = nullipblockcreate;


//   #      ###   ######  ######  #       #######  #####  #    #
//  # #      #    #     # #     # #       #     # #     # #   #
// #   #     #    #     # #     # #       #     # #       #  #
//#     #    #    ######  ######  #       #     # #       ###
//#######    #    #       #     # #       #     # #       #  #
//#     #    #    #       #     # #       #     # #     # #   #
//#     #   ###   #       ######  ####### #######  #####  #    #

vector<aipblock *> glbIpblocks;

aipblock::aipblock(char *_name) : name(_name) {
  generation = 0;
  ispresent  = false;
  evalloop   = false;
  glbIpblocks.push_back(this);
}

aipblock::~aipblock() {}

void aipblock::present() {
  ispresent = true;
  blockincluded();
}

void aipblock::blockincluded() {}

// probe is used to implement debug protocols
void aipblock::probe(char *) {}

void aipblock::stop() {}

void aipblock::addoutput(ipblockout *o, char *terminal) {
  outputs.insert(o);
  ioval.push_back(&(o->peekval()));  // gval of output is kept in ipblockout
  iomap[o] = (unsigned) ioval.size() - 1;
  if (! checkterminal((unsigned) ioval.size() - 1, terminal, output)) {
    cout << "Warning: ipblock " << name << " terminal mismatch for output ";
    cout << terminal << " (" << (unsigned) ioval.size() - 1 << ")\n";
  }
  invomap[iomap[o]] = o;  // maintain an inverted IO map as well

  if (indep_out_todo.find(iomap[o]) != indep_out_todo.end())
    regOutput(iomap[o]);

}

void aipblock::addinput(artsignal *i, char *terminal, int wlenreq, unsigned signreq) {
  inputs.insert(i);
  ioval.push_back(make_gval(wlenreq, signreq)); // gval of input is stored locally
  iomap[i] = (unsigned) ioval.size() - 1;
  if (! checkterminal((unsigned) ioval.size() - 1, terminal, input)) {
    cout << "Warning: ipblock " << name << " terminal name mismatch for input ";
    cout << terminal << " (" << (unsigned) ioval.size() - 1 << ")\n";
  }
}

bool aipblock::hasinput(artsignal *s) {
  for (set<artsignal *>::iterator i = inputs.begin();
       i != inputs.end();
       ++i)
    if (*i == s)
      return true;
  return false;  
}

void aipblock::eval(long g) {
  if (!ispresent)
    return;

  if (generation != g) {

    if (evalloop) {
      fdlerror(ERROR_USER_IP_COMBINATORIAL_LOOP, name);
    }

    evalloop = true;
    for (set<artsignal *>::iterator i = inputs.begin();
	 i != inputs.end();
	 ++i) {
      *(ioval[iomap[*i]]) = (*i)->eval(g);
    }

    run();
    generation = g;
    evalloop  = false;

    if (cannotSleepTest())
      glbRTSleep.cannotSleep();

  }
}

void aipblock::regOutput(unsigned i) {
  if (invomap.find(i) != invomap.end())
    indep_out.push_back(invomap[i]);
  else 
    indep_out_todo.insert(i);
}

void aipblock::eval_out(unsigned long g) {
  out_run();
  for (vector<ipblockout *>::iterator i = indep_out.begin();
       i != indep_out.end();
       i++)
    (*i)->updatedto(g);
}

bool aipblock::cannotSleepTest() {
  return true;
}

bool aipblock::needsWakeupTest() {
  return false;
}

// used by codesign interfaces
void aipblock::write_device(int dev, unsigned long n) {
}

// used by codesign interfaces
unsigned long aipblock::read_device(int dev) {
  return 0;
}

// used by codesign interfaces
void aipblock::touch() {
}

// DEPRECATED
//void aipblock::r_touch(int) {
  // used by codesign interfaces
//}

void aipblock::run() {
  fdlerror(ERROR_USER_IPBLOCK_HAS_NO_BEHAVIOR, name);
}

void aipblock::out_run() { }

void aipblock::setparm(char *_name) {
  fdlerror(ERROR_USER_IPBLOCK_HAS_NO_PARAMETER, _name);
}

bool aipblock::checkterminal(int n, char *tname, iodir d) {
  return false;
}

bool aipblock::isinput(iodir d) {
  return (d == input);
}

bool aipblock::isoutput(iodir d) {
  return (d == output);
}

bool aipblock::isname(char *n, char *n2) {
  return (!strcmp(n,n2));
}

bool aipblock::blockisname(char *n) {
  return (! strcmp(n,name));
}

char * aipblock::matchparm(char *orig, char *tgt) {
  // find first char of tgt in orig
  while (((unsigned char) (*tgt)) != 0) {
    while (*orig && (*orig != *tgt))
      orig++;
    if (*orig)
      tgt++;
    else
      return 0;
  }
  // complete match found - look for '='
  while (*orig && (*orig != '='))
    orig++;

  ++orig; // skip '='

  // strip additional ws
  while (*orig && ((*orig == ' ') || (*orig == '\t')))
    orig++;

  if (*orig == 0) {
    return 0;
  }


  return orig;
}

bool aipblock::matchparm(char *orig, char *tgt, gval &v) {
  char *val = matchparm(orig, tgt);
  if (val != 0) {
    gval *v2 = make_gval(val);
    v = *v2;
    return true;
  }
  return false;
}

//  ###   ######  ######  #       #######  #####  #    #  ####### #     # #######
//   #    #     # #     # #       #     # #     # #   #   #     # #     #    #
//   #    #     # #     # #       #     # #       #  #    #     # #     #    #
//   #    ######  ######  #       #     # #       ###     #     # #     #    #
//   #    #       #     # #       #     # #       #  #    #     # #     #    #
//   #    #       #     # #       #     # #     # #   #   #     # #     #    #
//  ###   #       ######  ####### #######  #####  #    #  #######  #####     #

ipblockout::ipblockout(symid _id, aipblock *_owner, int wlenreq, unsigned signreq) :
  artsignal(_id), 
  owner(_owner) {
  data = make_gval(wlenreq, signreq);
  if (glbRTProfile)
    glbRTProfile->makeop(_id);
}

void ipblockout::updatedto(unsigned long g) {
  generation = g;
}

gval &ipblockout::eval(unsigned long g) {
  if (generation != g) {
    owner->eval(g);
    generation = g;
  }
  if (glbRTProfile)
    glbRTProfile->evalop(id, data);
  return *data;
}

gval &ipblockout::peekval() {
  return *data;
}

int ipblockout::wlenreq() {
  return data->getwordlength();
}

unsigned ipblockout::signreq() {
  return data->getsign();
}


