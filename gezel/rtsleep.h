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


#ifndef RTSLEEP_H
#define RTSLEEP_H

//
//
// This module governs sleep mode for accelerated simulation
// in sleep mode, no evals are done until wake-up comes
// sleep mode only makes sense for cosimulation
// (in stand alone mode, it might as well terminate simulation).
//
// conditions for entering 'sleep' mode are as follows:
// 
// 1. no RT controller has changed state (X)
// 2. no RT register has changed state (X)
// 3. no IPblock has indicate change-of-state (X)
// 4. no stimuli files are used (X) (which can provide extra changing input data)
//
// The algorithm works by calculating, each clock cycle, a flag that
// indicates if the _next_ clock cycle should be simulated.
// At the start of each clock cycle, this flag is first checked to
// determine the simulation mode (sleep or active)
//
// in 'active' mode
//    - do full simulation
//    - calculate the sleep flag for the next clock cycle
//
// in 'sleep' mode
//    - check the blocks that can trigger wakeup mode (needsWakeUpTest)
//      these blocks are the cosimulation interfaces
//
// Transition from active->sleep->active is decided at the start of
// each clock cycle, before anything else is done.
//
//

class rtsleep {
  // true value indicates something has changed
  bool flag;

 public:
  rtsleep();
  void cannotSleep();
  bool testCannotSleep();
  void reset();
};

extern rtsleep       glbRTSleep;

#endif
