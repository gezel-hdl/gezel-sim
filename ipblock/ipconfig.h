//--------------------------------------------------------------
// Copyright (C) 2005 Patrick Schaumont (schaum@ee.ucla.edu)                   
// University of California at Los Angeles
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
// $Id: ipconfig.h,v 1.1.1.1 2005/07/14 17:32:44 schaum Exp $
//--------------------------------------------------------------

#ifndef IPCONFIG_H
#define IPCONFIG_H

extern "C" {
  aipblock * dynamic_ipblockcreate(char *instname, char *tname);
  aipblock * static_ipblockcreate (char *instname, char *tname);
  aipblock * stdipblockcreate     (char *instname, char *tname);
}

#endif

