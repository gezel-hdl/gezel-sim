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
// $Id: rterror.h 104 2009-05-19 14:35:17Z schaum $
//--------------------------------------------------------------

#ifndef RTERROR_H
#define RTERROR_H

#include "fdlsym.h"
#include <set>

enum {
  ERROR_INTERNAL_UNKNOWN,
  ERROR_USER_MULTIPLE_ASSIGNMENT_SAME_SFG,
  ERROR_USER_MULTIPLE_ASSIGNMENT_MULTIPLE_SFG,
  ERROR_USER_MISSING_ASSINGMENT,
  ERROR_INTERNAL_UNKNOWNCONTROLSTEP,
  ERROR_USER_MISSING_INITIAL_STATE,
  ERROR_USER_NO_SOURCESTATE_FSM,
  ERROR_USER_NO_TARGETSTATE_FSM,
  ERROR_INTERNAL_NO_CURRENT_CONDITION,
  ERROR_USER_NO_TRANSITIONS,
  ERROR_USER_UNKNOWN_INSTRUCTION,
  ERROR_INTERNAL_NO_RTCODE,
  ERROR_INTERNAL_DP_UNKNOWN,
  ERROR_INTERNAL_IP_UNKNOWN,
  ERROR_INTERNAL_DP_NOT_IN_HWIRE_CTL,
  ERROR_INTERNAL_DP_NOT_IN_SEQ_CTL,
  ERROR_INTERNAL_DP_NOT_IN_FSM_CTL,
  ERROR_INTERNAL_CANNOT_ADD_STATE,
  ERROR_INTERNAL_CANNOT_ADD_CONDITION,
  ERROR_INTERNAL_NO_CODE_FOR_CONDITION,
  ERROR_INTERNAL_CANNOT_ADD_TRANSITION,
  ERROR_INTERNAL_CANNOT_ADD_CTLSTEP,
  ERROR_INTERNAL_CANNOT_FIND_CTL_CGEXEC,
  ERROR_INTERNAL_PIN_IS_NOT_INPUT,
  ERROR_USER_UNKNOWN_CHILD_DATAPATH,
  ERROR_USER_UNKNOWN_CHILD_IPBLOCK,
  ERROR_USER_UNKNOWN_PARENT_DATAPATH,
  ERROR_INTERNAL_CANNOT_FIND_DATAPATH,
  ERROR_INTERNAL_CANNOT_FIND_DATAPATH_PIN,
  ERROR_INTERNAL_UNKNOWN_DISPLAY_ID,
  ERROR_INTERNAL_UNKNOWN_DISPLAY_VAR_ID,
  ERROR_INTERNAL_LOOKUP_TABLE_UNKOWN,
  ERROR_USER_COULD_NOT_OPEN_FILE,
  ERROR_USER_FAIL_ON_INPUT,
  ERROR_USER_SIGNAL_HAS_NO_DRIVER,
  ERROR_USER_DRIVEN_SIGNAL_REASSIGNED,
  ERROR_USER_COMBINATORIAL_LOOP,
  ERROR_USER_IP_COMBINATORIAL_LOOP,
  ERROR_USER_PROXY_ASSIGN_NOT_SUPPORTED,
  ERROR_INTERNAL_NOT_IMPLEMENTED,
  ERROR_USER_IPBLOCK_HAS_NO_TYPEDEF,
  ERROR_INTERNAL_UNKNOWN_SYMBOL,
  ERROR_USER_NO_SOURCE_PROXY,
  ERROR_USER_NET_HAS_MULTIPLE_DRIVERS,
  ERROR_USER_CANNOT_FIND_IP_OUTPUT,
  ERROR_USER_IPBLOCK_HAS_NO_BEHAVIOR,
  ERROR_USER_IPBLOCK_HAS_NO_PARAMETER,
  ERROR_USER_IPBLOCK_UNKNOWN,
  ERROR_CANNOT_OPEN_VHD_FILE,
  ERROR_PORT_DIRECTION_UNKNOWN,
  ERROR_INTERNAL_MISSING_INSLIST,
  ERROR_INTERNAL_ABSTRACT_PORT,
  ERROR_PORT_ENUM_TYPE_UNKNOWN,
  ERROR_VENUMTYPE_IS_NULL,
  ERROR_USER_CLONE_WITHOUT_CTL,
  ERROR_INTERNAL_CANNOT_FIND_CTL_CGTRACER,
  ERROR_INTERNAL_TRACE_SIGNAL_NOT_FOUND,
  ERROR_MULTIPLE_TRACES_NOT_SUPPORTED,
  ERROR_UNDEFINED_LOCATION_IN_LOOKUP_TABLE,
  ERROR_INTERNAL_SYMBOL_NOT_FOUND,
  ERROR_USER_DP_NOT_PRESENT
};
 
void fdlerror(int n);
void fdlerror(int n, symid v);
void fdlwarn (int n, symid v);
void fdlerror(int n, symid v, symid v2);
void fdlerror(int n, symid v, set<symid> &c);
void fdlerror(int n, const char *n2, int l);
void fdlerror(int n, const char *n2);

#endif
