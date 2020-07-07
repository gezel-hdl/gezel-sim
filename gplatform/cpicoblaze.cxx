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

#include "cpicoblaze.h"

#include <iostream>

using namespace std ;

CProgramCounter::CProgramCounter() {
  pc = 0 ;
}

CProgramCounter::~CProgramCounter() {
  
}

void CProgramCounter::Next() {
  pc = ( pc + 1 ) % (MAX_ADDRESS);
}

void CProgramCounter::Set( uint16_t address ) {
  pc = address % (MAX_ADDRESS) ;
}

uint16_t CProgramCounter::Get() {
  return pc ;
}

CScratchPad::CScratchPad() {
  unsigned i ;
  for ( i = 0 ; i < sizeof( ram ) ; i++ )
    ram[ i ] = 0 ;
}

CScratchPad::~CScratchPad() {
}

CStack::CStack() {
  int i ;
  for ( i = 0 ; i < STACK_DEPTH ; i++ )
    stack[ i ] = 0 ;
  ptr = 0 ;
}

CStack::~CStack() {
}

void CStack::Push( uint16_t data ) {	
  data &= 0x3FF ;
  if ( ptr == STACK_DEPTH - 1 )
    cout << ">>>>Stack overflow!<<<<\r\n" ;
  
  stack[ ptr ] = data ;
  ptr = ( ptr + 1 ) % STACK_DEPTH ;
}

uint16_t CStack::Pop() {
  if ( ptr == 0 )
    cout << ">>>>Stack underflow!<<<<\r\n" ;
  
  ptr = ( ptr - 1 ) % STACK_DEPTH ;
  return stack[ ptr ] ;
}

void CStack::Reset() {
  ptr = 0 ;
}

uint8_t CScratchPad::Get( uint8_t address ) {
  return ram[ address % sizeof( ram ) ] ;
}

void CScratchPad::Set( uint8_t address, uint8_t data ) {
  ram[ address % sizeof( ram ) ] = data ;
}

//-----------------------------------------------------------

CCode::CCode( CPicoBlaze *cpu )  {
  m_cpu = cpu ;
  
  int i ;
  for ( i = 0 ; i < MAX_ADDRESS ; i++ )
    CodeMap[ i ] = 0 ;
}

CCode::~CCode() {
  ClearCode() ;
}

void CCode::ClearCode() {	
  int i ;
  for ( i = 0 ; i < MAX_ADDRESS ; i++ )
    if ( CodeMap[ i ] != 0 ) {
      delete CodeMap[ i ] ;
      CodeMap[ i ] = 0 ;
    }
}

CInstruction * CCode::Disassemble( uint32_t code ) {
  uint32_t code_17_0  = (code & 0x3ffff) ;
  uint32_t code_17_12 = (code & 0x3f000) ;
  uint32_t code_17_10 = (code & 0x3fC00) ;
  uint32_t code_7_0   = (code & 0x000ff) ;
  
  /* The picoBlaze-3 instruction set */
  if ( code_17_0 == instrRETURN )            return new RETURN( m_cpu, code ) ;
  if ( code_17_0 == instrRETURNC )           return new RETURNC( m_cpu, code ) ;
  if ( code_17_0 == instrRETURNNC )          return new RETURNNC( m_cpu, code ) ;
  if ( code_17_0 == instrRETURNNZ )          return new RETURNNZ( m_cpu, code ) ;
  if ( code_17_0 == instrRETURNZ )           return new RETURNZ( m_cpu, code ) ;
  if ( code_17_0 == instrRETURNI_DISABLE )   return new RETURNI_DISABLE( m_cpu, code ) ;
  if ( code_17_0 == instrRETURNI_ENABLE )    return new RETURNI_ENABLE( m_cpu, code ) ;
  if ( code_17_0 == instrDISABLE_INTERRUPT ) return new DISABLE_INTERRUPT( m_cpu, code ) ;
  if ( code_17_0 == instrENABLE_INTERRUPT )  return new ENABLE_INTERRUPT( m_cpu, code ) ;
  if ( code_17_10 == instrCALL )             return new CALL( m_cpu, code ) ;
  if ( code_17_10 == instrCALLC )            return new CALLC( m_cpu, code ) ;
  if ( code_17_10 == instrCALLNC )           return new CALLNC( m_cpu, code ) ;
  if ( code_17_10 == instrCALLNZ )           return new CALLNZ( m_cpu, code ) ;
  if ( code_17_10 == instrCALLZ )            return new CALLZ( m_cpu, code ) ;
  if ( code_17_10 == instrJUMP )             return new JUMP( m_cpu, code ) ;
  if ( code_17_10 == instrJUMPC )            return new JUMPC( m_cpu, code ) ;
  if ( code_17_10 == instrJUMPNC )           return new JUMPNC( m_cpu, code ) ;
  if ( code_17_10 == instrJUMPNZ )           return new JUMPNZ( m_cpu, code ) ;
  if ( code_17_10 == instrJUMPZ )            return new JUMPZ( m_cpu, code ) ;
  if ( code_17_12 == instrADD_SX_KK )        return new ADD_SX_KK( m_cpu, code ) ;
  if ( code_17_12 == instrADD_SX_SY )        return new ADD_SX_SY( m_cpu, code ) ;
  if ( code_17_12 == instrADDCY_SX_KK )      return new ADDCY_SX_KK( m_cpu, code ) ;
  if ( code_17_12 == instrADDCY_SX_SY )      return new ADDCY_SX_SY( m_cpu, code ) ;
  if ( code_17_12 == instrAND_SX_KK )        return new AND_SX_KK( m_cpu, code ) ;
  if ( code_17_12 == instrAND_SX_SY)         return new AND_SX_SY( m_cpu, code ) ;
  if ( code_17_12 == instrCOMPARE_SX_KK )    return new COMPARE_SX_KK( m_cpu, code ) ;
  if ( code_17_12 == instrCOMPARE_SX_SY )    return new COMPARE_SX_SY( m_cpu, code ) ;
  if ( code_17_12 == instrFETCH_SX_SS )      return new FETCH_SX_SS( m_cpu, code ) ;
  if ( code_17_12 == instrFETCH_SX_SY )      return new FETCH_SX_SY( m_cpu, code ) ;
  if ( code_17_12 == instrINPUT_SX_SY )      return new INPUT_SX_SY( m_cpu, code ) ;
  if ( code_17_12 == instrINPUT_SX_PP )      return new INPUT_SX_PP( m_cpu, code ) ;
  if ( code_17_12 == instrLOAD_SX_KK )       return new LOAD_SX_KK( m_cpu, code ) ;
  if ( code_17_12 == instrLOAD_SX_SY )       return new LOAD_SX_SY( m_cpu, code ) ;
  if ( code_17_12 == instrOR_SX_KK )         return new OR_SX_KK( m_cpu, code ) ;
  if ( code_17_12 == instrOR_SX_SY )         return new OR_SX_SY( m_cpu, code ) ;
  if ( code_17_12 == instrOUTPUT_SX_SY )     return new OUTPUT_SX_SY( m_cpu, code ) ;
  if ( code_17_12 == instrOUTPUT_SX_PP )     return new OUTPUT_SX_PP( m_cpu, code ) ;
  if ( code_17_12 == instrSTORE_SX_SS )      return new STORE_SX_SS( m_cpu, code ) ;	  
  if ( code_17_12 == instrSTORE_SX_SY )      return new STORE_SX_SY( m_cpu, code ) ;	  
  if ( code_17_12 == instrSUB_SX_KK )        return new SUB_SX_KK( m_cpu, code ) ;	  
  if ( code_17_12 == instrSUB_SX_SY )        return new SUB_SX_SY( m_cpu, code ) ;	  
  if ( code_17_12 == instrSUBCY_SX_KK )      return new SUBCY_SX_KK( m_cpu, code ) ;	  
  if ( code_17_12 == instrSUBCY_SX_SY )      return new SUBCY_SX_SY( m_cpu, code ) ;	  
  if ( code_17_12 == instrTEST_SX_KK )       return new TEST_SX_KK( m_cpu, code ) ;	  
  if ( code_17_12 == instrTEST_SX_SY )       return new TEST_SX_SY( m_cpu, code ) ;	  
  if ( code_17_12 == instrXOR_SX_KK )        return new XOR_SX_KK( m_cpu, code ) ;
  if ( code_17_12 == instrXOR_SX_SY )        return new XOR_SX_SY( m_cpu, code ) ;
  if ( code_7_0  == instrRL_SX )             return new RL_SX( m_cpu, code ) ;
  if ( code_7_0  == instrRR_SX )             return new RR_SX( m_cpu, code ) ;
  if ( code_7_0  == instrSL0_SX )            return new SL0_SX( m_cpu, code ) ;
  if ( code_7_0  == instrSL1_SX )            return new SL1_SX( m_cpu, code ) ;
  if ( code_7_0  == instrSLA_SX )            return new SLA_SX( m_cpu, code ) ;
  if ( code_7_0  == instrSLX_SX )            return new SLX_SX( m_cpu, code ) ;
  if ( code_7_0  == instrSR0_SX )            return new SR0_SX( m_cpu, code ) ;
  if ( code_7_0  == instrSR1_SX )            return new SR1_SX( m_cpu, code ) ;
  if ( code_7_0  == instrSRA_SX )            return new SRA_SX( m_cpu, code ) ;
  if ( code_7_0  == instrSRX_SX )            return new SRX_SX( m_cpu, code ) ;

  cout << "Invalid code (" << code << ")\r\n" ;
  
  return 0 ;
}

#include <fstream>

void CCode::loadCode(char *decname) {
  ifstream F(decname);

  if (F.fail()) {
    cout << "Error: Picoblaze ISS: Cannot load file " << decname << "\n";
    exit(0);
  }

  uint32_t decfmt;
  for (int i = 0 ; (i < MAX_ADDRESS) && (!F.eof()) ; i++ ) {
    F >> decfmt;
    CodeMap[ i ] =  Disassemble(decfmt);
  }

  F.close();
}



CInstruction * CCode::getInstruction( uint16_t address ) {
  if ( address >= MAX_ADDRESS )
    return 0 ;
  else
    return CodeMap[ address ] ;
}

void CCode::Print() {
  int i ;
  
  cout << "----listing----\r\n" ;
  for ( i = 0 ; i < MAX_ADDRESS ;  i++ ) {
    if ( CodeMap[ i ] != 0 ) {
      cout << i << "  : " ;
      CodeMap[ i ]->Print() ;
      cout << "\r\n" ;
    }
  }
  cout << "----end listing----\r\n" ;
  
}

//-----------------------------------------------------------

CPicoBlaze::CPicoBlaze() {
  twophase = false;
  
  flags.zero = false ;
  flags.carry = false ;
  flags.interrupt_enable = false ;
  
  scratch = new CScratchPad ;
  pc = new CProgramCounter ;
  stack = new CStack ;
  port = 0;
  code = new CCode( this ) ;
}

CPicoBlaze::~CPicoBlaze() {
  delete scratch ;
  delete pc ;
  delete stack ;
  delete code ;
}

void CPicoBlaze::Reset() {
  RESET_EVENT resetEvent( this, 0 ) ;
  
  resetEvent.Print() ; cout << "\r\n" ;
  resetEvent.Execute() ;
}

void CPicoBlaze::Interrupt() {
  INTERRUPT_EVENT interruptEvent( this, 0 ) ;
  
  interruptEvent.Print() ; cout << "\r\n" ;
  interruptEvent.Execute() ;
}

void CPicoBlaze::Print() {
  int i ;
  
  cout << "----CPU----\r\n" ;
  cout << "regs|" ;
  for ( i = 0 ; i < 15 ; i++ ) 
    cout << "s" << i << "=" << (int) s[ i ]  << "|" ;
  cout << "\r\n" ;
  
  cout << "flags|";
  cout << "c=" << flags.carry ;
  cout << "|z=" << flags.zero ;
  cout << "|ie=" << flags.interrupt_enable << "|\r\n" ;
  cout << "----end CPU----\r\n" ;
}

bool CPicoBlaze::Next() {
  CInstruction *instr = code->getInstruction( pc->Get() ) ;
  if ( instr == 0 ) {
    cout << ">>>>Error in simulation (No code found at " << pc->Get() << ")<<<<\r\n" ;
    return FALSE ;
  }
  
  instr->Execute() ;
  return TRUE ;
}

void CPicoBlaze::Need2ndHalf() {
  twophase = true;
}

void CPicoBlaze::Next2ndHalf() {
  if (twophase) {
    twophase = false;
    CInstruction *instr = code->getInstruction( pc->Get() ) ;
    instr->Execute2ndHalf();
  }
}

void CPicoBlaze::addPort( CPort * ioport ) {
  port = ioport;
}



