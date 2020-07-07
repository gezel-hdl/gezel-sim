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

#include "rterror.h"
#include <cstdlib>
#include <iostream>

void showerrortype(int n) {
  switch (n) {
  case ERROR_INTERNAL_UNKNOWN:
    cerr << "Internal Unkown Error";
    break;
  case ERROR_USER_MULTIPLE_ASSIGNMENT_SAME_SFG:
    cerr << "Multiple Assignment of a signal in current sfg: ";
    break;
  case ERROR_USER_MULTIPLE_ASSIGNMENT_MULTIPLE_SFG:
    cerr << "Multiple Assignment of a signal over multiple, concurrent sfg: ";
    break;
  case ERROR_USER_MISSING_ASSINGMENT:
    cerr << "Missing Assignment of signal ";
    break;
  case ERROR_INTERNAL_UNKNOWNCONTROLSTEP:
    cerr << "(Internal Error) Unknown Control Step";
    break;
  case ERROR_USER_MISSING_INITIAL_STATE:
    cerr << "Cannot find initial state for FSM ";
    break;
  case ERROR_USER_NO_SOURCESTATE_FSM:
    cerr << "Source state for FSM not found ";
    break;
  case ERROR_USER_NO_TARGETSTATE_FSM:
    cerr << "Target state for FSM not found ";
    break;
  case ERROR_INTERNAL_NO_CURRENT_CONDITION:
    cerr << "(Internal Error) Current Condition not found ";
    break;
  case ERROR_USER_NO_TRANSITIONS:
    cerr << "No transitions available out for current state ";
    break;
  case ERROR_USER_UNKNOWN_INSTRUCTION:
    cerr << "Unknown instruction on transition out of current state ";
    break;
  case ERROR_INTERNAL_NO_RTCODE:
    cerr << "(Internal Error) No RT Code available ";
    break;
  case ERROR_INTERNAL_DP_UNKNOWN:
    cerr << "(Internal Error) Unknown Datapath ";
    break;
  case ERROR_INTERNAL_IP_UNKNOWN:
    cerr << "(Internal Error) Unknown Library Block ";
    break;
  case ERROR_INTERNAL_DP_NOT_IN_HWIRE_CTL:
    cerr << "(Internal Error) Datapath not known to hardwired controller ";
    break;
  case ERROR_INTERNAL_DP_NOT_IN_SEQ_CTL:
    cerr << "(Internal Error) Datapath not known to sequencer controller ";
    break;
  case ERROR_INTERNAL_DP_NOT_IN_FSM_CTL:
    cerr << "(Internal Error) Datapath not known to FSM controller ";
    break;
  case ERROR_INTERNAL_CANNOT_ADD_STATE:
    cerr << "(Internal Error) Controller not found while adding state ";
    break;
  case ERROR_INTERNAL_CANNOT_ADD_CONDITION:
    cerr << "(Internal Error) Controller not found while adding condition ";
    break;
  case ERROR_INTERNAL_NO_CODE_FOR_CONDITION:
    cerr << "(Internal Error) No code found to execute under condition ";
    break;
  case ERROR_INTERNAL_CANNOT_ADD_TRANSITION:
    cerr << "(Internal Error) Control unknown while creating state transition ";
    break;
  case ERROR_INTERNAL_CANNOT_ADD_CTLSTEP:
    cerr << "(Internal Error) Fail to add control step to unknown controller id ";
    break;
  case ERROR_INTERNAL_CANNOT_FIND_CTL_CGEXEC:
    cerr << "(Internal Error) Controller id unknown at cgexec sfg id ";
    break;
  case ERROR_INTERNAL_PIN_IS_NOT_INPUT:
    cerr << "(Internal Error) Pin Id is not an input of current RT object ";
    break;
  case ERROR_USER_UNKNOWN_CHILD_DATAPATH:
    cerr << "Child Datapath unknown ";
    break;
  case ERROR_USER_UNKNOWN_CHILD_IPBLOCK:
    cerr << "Child Ipblock unknown ";
    break;
  case ERROR_USER_UNKNOWN_PARENT_DATAPATH:
    cerr << "Parent Datapath unknown ";
    break;
  case ERROR_INTERNAL_CANNOT_FIND_DATAPATH:
    cerr << "(Internal Error) Datapath not found ";
    break;
  case ERROR_INTERNAL_CANNOT_FIND_DATAPATH_PIN:
    cerr << "(Internal Error) Datapath pin not found ";
    break;
  case ERROR_INTERNAL_UNKNOWN_DISPLAY_ID:
    cerr << "(Internal Error) Unkown display id ";
    break;
  case ERROR_INTERNAL_UNKNOWN_DISPLAY_VAR_ID:
    cerr << "(Internal Error) Unkown display variable id ";
    break;
  case ERROR_INTERNAL_LOOKUP_TABLE_UNKOWN:
    cerr << "(Internal Error) Lookup Table not found ";
    break;
  case ERROR_USER_COULD_NOT_OPEN_FILE:
    cerr << "Could not open file ";
    break;
  case ERROR_USER_FAIL_ON_INPUT:
    cerr << "Fail condition on input file ";
    break;
  case ERROR_USER_SIGNAL_HAS_NO_DRIVER:
    cerr << "Signal has no driver ";
    break;
  case ERROR_USER_DRIVEN_SIGNAL_REASSIGNED:
    cerr << "A continously driven signal cannot be reassigned ";
    break;
  case ERROR_USER_COMBINATORIAL_LOOP:
    cerr << "Signal is in a combinatorial loop ";
    break;
  case ERROR_USER_IP_COMBINATORIAL_LOOP:
    cerr << "ipblock is in a combinatorial loop ";
    break;
  case ERROR_USER_PROXY_ASSIGN_NOT_SUPPORTED:
    cerr << "Assignment on subvectors is not supported ";
    break;
  case ERROR_INTERNAL_NOT_IMPLEMENTED:
    cerr << "(Internal Error) Not implemented feature ";
    break;
  case ERROR_USER_IPBLOCK_HAS_NO_TYPEDEF:
    cerr << "Library block has no type definition ";
    break;
  case ERROR_INTERNAL_UNKNOWN_SYMBOL:
    cerr << "(Internal Error) Unknown Symbol ";
    break;
  case ERROR_USER_NO_SOURCE_PROXY:
    cerr << "Cannot have indexed sourcing nets ";
    break;
  case ERROR_USER_NET_HAS_MULTIPLE_DRIVERS:
    cerr << "Net has multiple drivers ";
    break;
  case ERROR_USER_CANNOT_FIND_IP_OUTPUT:
    cerr << "Cannot found output pin on Library Block ";
    break;
  case ERROR_USER_IPBLOCK_HAS_NO_BEHAVIOR:
    cerr << "Library block has no defined behavior ";
    break;
  case ERROR_USER_IPBLOCK_HAS_NO_PARAMETER:
    cerr << "Cannot set parameter on Library Block ";
    break;
  case ERROR_USER_IPBLOCK_UNKNOWN:
    cerr << "Library Block Type unknown ";
    break;
  case ERROR_CANNOT_OPEN_VHD_FILE:
    cerr << "Cannot open VHDL file ";
    break;
  case ERROR_PORT_DIRECTION_UNKNOWN:
    cerr << "Port direction unknown for port ";
    break;
  case ERROR_INTERNAL_MISSING_INSLIST:
    cerr << "(Internal Error) Instruction List found empty for Controller ";
    break;
  case ERROR_INTERNAL_ABSTRACT_PORT:
    cerr << "(Internal Error) Abstract Port Found ";
    break;
  case ERROR_PORT_ENUM_TYPE_UNKNOWN:
    cerr << "(Internal Error) Enumerated Type for Port Unknown ";
    break;
  case ERROR_VENUMTYPE_IS_NULL:
    cerr << "(Internal Error) Venumtype is null ";
  case ERROR_USER_CLONE_WITHOUT_CTL:
    cerr << "Cannot perform clone operation. First need to have a controller for parent datapath ";
    break;
  case ERROR_INTERNAL_TRACE_SIGNAL_NOT_FOUND:
    cerr << "(Internal Error) Trace Signal not found";
    break;
  case ERROR_MULTIPLE_TRACES_NOT_SUPPORTED:
    cerr << "Traces of multiple-instantiated signals are not yet supported";
    break;
  case ERROR_UNDEFINED_LOCATION_IN_LOOKUP_TABLE:
    cerr << "Accessing lookup table at an undefined location ";
    break;
  case ERROR_INTERNAL_SYMBOL_NOT_FOUND:
    cerr << "(Internal Error) Symbol not found ";
    break;
  case ERROR_USER_DP_NOT_PRESENT:
    cerr << "Datapath not found\n";
    break;
  }

  //  cerr << "\n";
}


void fdlerror(int n) {
  cerr << "\n*** Error: ";
  showerrortype(n);
  exit(0);
}

void fdlerror(int n, symid v) {
  cerr << "\n*** Error: ";
  showerrortype(n);
  glbSymboltable.showlex(cerr, v);
  cerr << "\n\nContext:\n";
  glbSymboltable.showline(cerr, v);
 
  //  cerr << ">"; symbol::showheader(cerr); cerr << "\n";
  //  glbSymboltable.showsymbol(cerr, v);

  exit(0);
}

void fdlwarn(int n, symid v) {
  cerr << "\n*** Warning: ";
  showerrortype(n);
  glbSymboltable.showlex(cerr, v);
}

void fdlerror(int n, symid v, symid v2) {
  cerr << "\n*** Error: ";
  showerrortype(n);
  glbSymboltable.showlex(cerr, v);
  cerr << " in ";
  glbSymboltable.showlex(cerr, v2);
  cerr << "\n\nContext:\n";
  glbSymboltable.showline(cerr, v);

  //  cerr << ">"; symbol::showheader(cerr); cerr << "\n";
  //  glbSymboltable.showsymbol(cerr, v);
  //  cerr << "> Additional Context:\n";
  //  glbSymboltable.showsymbol(cerr, v2);

  exit(0);
}

void fdlerror(int n, symid v, set<symid> &xsect) {
  typedef set<symid> setsymid;
  typedef setsymid::iterator setsit;
  cerr << "\n*** Error: ";
  showerrortype(n);
  glbSymboltable.showlex(cerr, v);
  cerr << " in ";
  for (setsit i = xsect.begin(); i != xsect.end(); ) {
    glbSymboltable.showlex(cerr, *(i));
    if (++i != xsect.end())
      cerr << " and ";
  }
  cerr << "\n\nContext:\n";
  glbSymboltable.showline(cerr, v);  
  exit(0);
}

void fdlerror(int n, const char *f, int l) {
  cerr << "\n*** Error: ";
  showerrortype(n);
  cerr << "Context: (" << f << "," << l << ")\n";
  exit(0);
}

void fdlerror(int n, const char *f) {
  cerr << "\n*** Error: ";
  showerrortype(n);
  cerr << "Context: (" << f << ")\n";
  exit(0);
}

