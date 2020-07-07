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
// $Id: java_itf.h 21 2005-11-15 19:08:03Z schaum $
//--------------------------------------------------------------

#ifndef JAVA_ITF_H
#define JAVA_ITF_H

class javasource : public aipblock {
  bool interfacewritten;
public:
  javasource(char *name);
  void setparm(char *_name);
  void run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool needsWakeupTest();
  bool cannotSleepTest();
  void touch();
};

class javasink : public aipblock {
public:
  javasink(char *name);
  void setparm (char *_name);
  void run();
  bool checkterminal(int n, char *tname, aipblock::iodir d);
  bool cannotSleepTest();
};

#endif
