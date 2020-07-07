/***************************************************************************
 *   Copyright (C) 2005 by Mark Six                                        *
 *   marksix@xs4all.nl                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef CPICOBLAZE
#define CPICOBLAZE

#include <iostream>
#include <list>

using namespace std ;

typedef unsigned char 	uint8_t ;
typedef unsigned short 	uint16_t ;
#ifndef __uint32_t_defined
#define __uint32_t_defined
typedef long unsigned int uint32_t ;
#endif

class CPicoBlaze ;
class CInstruction ;
#include "cpicoinstr.h"

#define instrADD_SX_KK 		0x18000
#define instrADD_SX_SY 		0x19000
#define instrADDCY_SX_KK 	0x1A000
#define instrADDCY_SX_SY 	0x1B000
#define instrAND_SX_KK		0x0A000
#define instrAND_SX_SY		0x0B000
#define instrCALL		0x30000
#define instrCALLC		0x31800
#define instrCALLNC		0x31C00
#define instrCALLNZ		0x31400
#define instrCALLZ		0x31000
#define instrCOMPARE_SX_KK	0x14000
#define instrCOMPARE_SX_SY	0x15000
#define instrDISABLE_INTERRUPT	0x3C000
#define instrENABLE_INTERRUPT	0x3C001
#define instrFETCH_SX_SS	0x06000
#define instrFETCH_SX_SY	0x07000
#define instrINPUT_SX_SY	0x05000
#define instrINPUT_SX_PP	0x04000
#define instrJUMP		0x34000
#define instrJUMPC		0x35800
#define instrJUMPNC		0x35C00
#define instrJUMPNZ		0x35400
#define instrJUMPZ		0x35000
#define instrLOAD_SX_KK		0x00000
#define instrLOAD_SX_SY		0x01000
#define instrOR_SX_KK		0x0C000
#define instrOR_SX_SY		0x0D000
#define instrOUTPUT_SX_SY	0x2D000
#define instrOUTPUT_SX_PP	0x2C000
#define instrRETURN		0x2A000
#define instrRETURNC		0x2B800
#define instrRETURNNC		0x2BC00
#define instrRETURNNZ		0x2B400
#define instrRETURNZ		0x2B000
#define instrRETURNI_DISABLE	0x38000
#define instrRETURNI_ENABLE	0x38001
#define instrROTATE		0x20000
#define instrRL_SX		0x00002
#define instrRR_SX		0x0000C
#define instrSL0_SX		0x00006
#define instrSL1_SX		0x00007
#define instrSLA_SX		0x00000
#define instrSLX_SX		0x00004
#define instrSR0_SX		0x0000E
#define instrSR1_SX		0x0000F
#define instrSRA_SX		0x00008
#define instrSRX_SX		0x0000A
#define instrSTORE_SX_SS	0x2E000
#define instrSTORE_SX_SY	0x2F000
#define instrSUB_SX_KK		0x1C000
#define instrSUB_SX_SY		0x1D000
#define instrSUBCY_SX_KK	0x1E000
#define instrSUBCY_SX_SY	0x1F000
#define instrTEST_SX_KK		0x12000
#define instrTEST_SX_SY		0x13000
#define instrXOR_SX_KK		0x0E000
#define instrXOR_SX_SY		0x0F000

#define TRUE	1
#define FALSE	0

#define MAX_ADDRESS		0x400
#define STACK_DEPTH		31
#define SCRATCHPAD_SIZE	64

class CProgramCounter {
 public:
  CProgramCounter() ;
  ~CProgramCounter() ;
  
  void     Next() ;
  void     Set( uint16_t address ) ;
  uint16_t Get() ;
  
 protected:
  uint16_t pc ;
} ;

class CScratchPad {
 public:
  CScratchPad() ;
  ~CScratchPad() ;
  
  uint8_t Get( uint8_t address ) ;
  void    Set( uint8_t address, uint8_t data ) ;
  
 protected:
  uint8_t ram[ SCRATCHPAD_SIZE ] ;
} ;

class CStack {
 public:
  CStack() ;
  ~CStack() ;
  
  void     Push( uint16_t value ) ;
  uint16_t Pop() ;
  void     Reset() ;
  
 protected:
  uint16_t stack[ STACK_DEPTH ] ;
  uint8_t ptr ;
} ;

class CPort {
 public:
  CPort() {}
  virtual ~CPort() {}
  
  void            PortID       ( uint8_t id  ) { portid = id ; } ;
  virtual void    PortIn       (             ) = 0;
  virtual uint8_t PortIn2ndHalf(             ) = 0;
  virtual void    PortOut      ( uint8_t data) = 0;
    
  uint16_t portid ;
};

class CCode {
 public:
  CCode( CPicoBlaze *cpu ) ;
  ~CCode() ;
  
  CInstruction *getInstruction( uint16_t address ) ;
  
  void ClearCode() ;
  void Print() ;	
  
  CInstruction * Disassemble( uint32_t code ) ;
  void loadCode(char *hexname);

 protected:
  CPicoBlaze *m_cpu ;
  CInstruction * CodeMap[ MAX_ADDRESS ] ;
};

class CPicoBlaze {
  bool twophase;
  
 public:		
  CPicoBlaze() ;
  ~CPicoBlaze() ;
  
  bool Next() ;
  void Next2ndHalf();
  void Need2ndHalf();
  void Reset() ;
  void Interrupt() ;
  void Print() ;
  
  void addPort   ( CPort * ioport ) ;
  
  uint8_t s[ 16 ] ;
  struct _flags {
    bool zero ;
    bool carry ;
    bool interrupt_enable ;
    
    bool preserved_zero ;
    bool preserved_carry ;
  } flags ;
  
  CProgramCounter *pc ;
  CScratchPad 	  *scratch ;
  CStack 	  *stack ;
  CPort		  *port ;
  CCode		  *code ;
} ;

#endif

